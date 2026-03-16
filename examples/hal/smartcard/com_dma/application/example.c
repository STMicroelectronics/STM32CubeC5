/**
  ******************************************************************************
  * file           : example.c
  * brief          : The SMARTCARD communication with DMA using HAL API.
  ******************************************************************************
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "example.h"
#include <inttypes.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SMARTCARD_GET_ATR          (0x00)  /* Answer to reset Commands */
#define SMARTCARD_CLA_GSM11        (0xA0)  /* SmartCard ADPU Command: Operation Code */
#define SMARTCARD_SELECT_FILE      (0xA4)  /* Data Area Management Commands */
#define T0_PROTOCOL                (0x00)  /* T0 protocol           */
#define SMARTCARD_RECEIVE_TIMEOUT  (1000U) /* Direction to reader   */
#define SMARTCARD_TRANSMIT_TIMEOUT (200U)  /* Direction to transmit */

#define SETUP_LENGTH (20U) /* Setup array size      */
#define HIST_LENGTH  (20U) /* Historical array size */

#define ADPU_SELECT_FILE_COMMAND_SIZE_BYTE (5U)
#define ACK_ANSWER_SIZE_BYTE (1U)
#define DATA_SIZE_BYTE       (2U)
#define ADPU_RESPONSE_SIZE_BYTE (2U)

#define EXPECTED_SW1_VALUE (0x9F)
#define EXPECTED_SW2_VALUE (0x1D)

/* Private typedef -------------------------------------------------------------*/
/* SmartCard power state enumeration */
typedef enum
{
  SMARTCARD_POWER_ON     = 0x00, /* SmartCard Power on state     */
  SMARTCARD_RESET_LOW    = 0x01, /* SmartCard reset low state    */
  SMARTCARD_RESET_HIGH   = 0x02, /* SmartCard reset high state   */
  SMARTCARD_ACTIVE       = 0x03, /* SmartCard active state       */
  SMARTCARD_ACTIVE_ON_T0 = 0x04, /* SmartCard active on T0 state */
  SMARTCARD_POWER_OFF    = 0x05  /* SmartCard power off state    */
} smartcard_state_t;

/* Answer To Reset structure ATR */
typedef struct
{
  uint8_t ts;                  /* Bit Convention */
  uint8_t t0;                  /* High nibble = Number of setup byte; low nibble = Number of historical byte */
  uint8_t setup[SETUP_LENGTH]; /* Setup array */
  uint8_t hist[HIST_LENGTH];   /* Historical array */
  uint8_t setup_length;        /* Setup array dimension */
  uint8_t hist_length;         /* Historical array dimension */
} smartcard_atr_t;

/* ADPU-Header command structure */
typedef struct
{
  uint8_t cla;  /* Command class    */
  uint8_t ins;  /* Operation code   */
  uint8_t p1;   /* Selection Mode   */
  uint8_t p2;   /* Selection Option */
} smartcard_header_t;

/* ADPU-Body command structure */
typedef struct
{
  uint8_t lc;         /* Data field length                      */
  uint8_t data[256];  /* Command parameters                     */
  uint8_t le;         /* Expected length of data to be returned */
} smartcard_body_t;

/* ADPU Command structure */
typedef struct
{
  smartcard_header_t header;
  smartcard_body_t body;
} smartcard_adpu_commands_t;

/* SmartCard response structure */
typedef struct
{
  uint8_t data[256];    /* Data returned from the card      */
  uint8_t sw1;          /* Command Processing status        */
  uint8_t sw2;          /* Command Processing qualification */
} smartcard_adpu_response_t;

/* Private variables ---------------------------------------------------------*/
hal_smartcard_handle_t *pUSARTx; /* Pointer referencing the USART handle from the generated code */
volatile uint8_t TxTransferComplete = 0U; /* Set to 1 if the write transfer is correctly completed       */
volatile uint8_t RxTransferComplete = 0U; /* Set to 1 if the read transfer is correctly completed        */
volatile uint8_t TransferError = 0U;      /* Set to 1 if a transmission or a reception error is detected */
volatile uint8_t CardInserted = 0;        /* Set to 1 if SmarCard is inserted succefully                 */
uint8_t MasterFile[2] = {0x3F, 0x00};     /* Set value of the Master File */
smartcard_atr_t SmartCardAtr;
uint8_t SmartCardAtrTable[40] = {0};

/* APDU Transport Structures */
smartcard_adpu_commands_t SmartCardAdpu;
smartcard_adpu_response_t SmartCardResponse;

uint8_t SmartCardCommand[5] = {0};
uint8_t SmartCardAnswer[50] = {0};
/* Private functions prototype -----------------------------------------------*/
static void SmartCardHandler(smartcard_state_t *smartcard_state,
                             smartcard_adpu_commands_t *smartcard_adpu);
static uint8_t SmartCardDecodeAnswer2Reset(uint8_t *card);
static void SmartCardAnswerReq(smartcard_state_t *smartcard_state, uint8_t *card, uint8_t length);  /* Ask ATR */
static void SmartCardReceiveDMA(uint8_t *p_data, uint16_t size);

/* Functions allowing the user to configure dynamically the SMARTCARD callbacks instead of weak functions */
static void TxTransferCpltCallback(hal_smartcard_handle_t *pUSARTx);
static void RxTransferCpltCallback(hal_smartcard_handle_t *pUSARTx);
static void TransferErrorCallback(hal_smartcard_handle_t *pUSARTx);

/** ########## Step 1 ##########
  * The init of the USART instance is triggered by the applicative code.
  * The user callbacks for USART TX/RX transfer completed and transfer error are registered.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pUSARTx = mx_example_smartcard_init();

  if (pUSARTx == NULL)
  {
    goto _app_init_exit;
  }

  /* Register the SMARTCARD callbacks */
  if (HAL_SMARTCARD_RegisterTxCpltCallback(pUSARTx, TxTransferCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_SMARTCARD_RegisterRxCpltCallback(pUSARTx, RxTransferCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_SMARTCARD_RegisterErrorCallback(pUSARTx, TransferErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1 : Device initialization COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  smartcard_state_t smartcard_state = SMARTCARD_POWER_ON;

  /** Application protocol data unit (APDU) is an application level protocol defined in the ISO 7816-4 standard.
    * APDU provide a set of APIs to manage file system (read, write, select ...).
    * It is exchanged between the reader application and the SmartCard application by the link level protocol.
    */
  SmartCardAdpu.header.cla = 0x00;
  SmartCardAdpu.header.ins = SMARTCARD_GET_ATR;
  SmartCardAdpu.header.p1 = 0x00;
  SmartCardAdpu.header.p2 = 0x00;
  SmartCardAdpu.body.lc = 0x00;

  /* Create a spectific ADPU command to select file from the SmartCard */
  SmartCardCommand[0] = SMARTCARD_CLA_GSM11;   /* CLA value */
  SmartCardCommand[1] = SMARTCARD_SELECT_FILE; /* INS value */
  SmartCardCommand[2] = 0x00;                  /* P1 value  */
  SmartCardCommand[3] = 0x00;                  /* P2 value  */
  SmartCardCommand[4] = 0x02;                  /* LC value  */

  /** ########## Step 2 ##########
    * Perform card power-up and reset procedure.
    * Transmits in DMA mode SmartCardAdpu command to the card.
    * Wait for answer to reset (ATR) message reception.
    */

  /* Configure Smartcard Interface GPIO pins */
  HAL_GPIO_WritePin(SC_CMDVCC_PIN_PORT, SC_CMDVCC_PIN_PIN, HAL_GPIO_PIN_SET);
  HAL_GPIO_WritePin(SC_RESET_PIN_PORT, SC_RESET_PIN_PIN, HAL_GPIO_PIN_SET);

  /** Retrieve the status of the SC_OFF_PIN_PIN.
    * loop while no Smartcard is detected.
    */
  while (HAL_GPIO_ReadPin(SC_OFF_PIN_PORT, SC_OFF_PIN_PIN) != HAL_GPIO_PIN_SET)
  {
    /** Put the CPU in Wait For Interrupt state. An SMARTCARD interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
      */
    __WFI();
  }

  /** SmartCard detected: update CardInserted flag.
    * This flag will evaluated later by SmartCardHandler().
    */
  CardInserted = 1;

  /* Select Smartcard 3V */
  HAL_GPIO_WritePin(SC_3_5V_PIN_PORT, SC_3_5V_PIN_PIN, HAL_GPIO_PIN_RESET);

  /* Disable the power to the smartcard */
  HAL_GPIO_WritePin(SC_CMDVCC_PIN_PORT, SC_CMDVCC_PIN_PIN, HAL_GPIO_PIN_SET);

  /* Power on card */
  while ((smartcard_state != SMARTCARD_ACTIVE_ON_T0) && (smartcard_state != SMARTCARD_POWER_OFF))
  {
    SmartCardHandler(&smartcard_state, &SmartCardAdpu);
  }

  /** Check if something wrong happen during this step.
    * If every thing completed OK, the SmartCard state must be in SMARTCARD_ACTIVE_ON_T0
    * and the TransferError flag still in reset state.
    */
  if ((smartcard_state == SMARTCARD_POWER_OFF) || (TransferError == 1))
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2 : Cold reset COMPLETED and ATR message RECEIVED.\n");

  RxTransferComplete = 0; /* Reset the RxTransferComplete value */

  /** ########## Step 3 ##########
    * Send a specific ADPU command to the SmartCard.
    * This command specify the communication protocol and the methods for accessing specific files and data on the card.
    */
  /* Start Tx DMA for Header transmission */
  if (HAL_SMARTCARD_Transmit_DMA(pUSARTx, (uint8_t *)SmartCardCommand, ADPU_SELECT_FILE_COMMAND_SIZE_BYTE) != HAL_OK)
  {
    /* An error occurs at the startup of the SMARTCARD transmission. Restart the communication */
    goto _app_process_exit;
  }

  /* Waits for one of these SMARTCARD interrupts: write transfer complete or transfer error.*/
  while ((TxTransferComplete == 0) && (TransferError == 0))
  {
    /** Put the CPU in Wait For Interrupt state. An SMARTCARD interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
      */
    __WFI();
  }

  if (TransferError == 1)
  {
    /* Error while smartcard transmission */
    PRINTF("[ERROR] Step 3 : SmartCard transmission ERROR.\n");
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3 : File select instruction SENT to the SmartCard.\n");

  /** ########## Step 4 ##########
    * Waits for acknowledgement byte from the smartcard.
    */
  /* Start Rx DMA for Ack Byte reception */
  if (HAL_SMARTCARD_Receive_DMA(pUSARTx, (uint8_t *)SmartCardAnswer, ACK_ANSWER_SIZE_BYTE) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /** Waits for one of these SMARTCARD interrupts: tx transfer complete or transfer error.
    */
  while ((RxTransferComplete == 0) && (TransferError == 0))
  {
    /** Put the CPU in Wait For Interrupt state. An SMARTCARD interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
      */
    __WFI();
  }

  if (TransferError == 1)
  {
    /* Error while smartcard transmission */
    PRINTF("[ERROR] Step 4 : SmartCard reception ERROR.\n");
    goto _app_process_exit;
  }

  if (((SmartCardAnswer[0] & 0xFE) != ((~(SMARTCARD_SELECT_FILE)) & 0xFE))
      && ((SmartCardAnswer[0] & 0xFE) != (SMARTCARD_SELECT_FILE & 0xFE)))
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 4 : ACK RECEIVED.\n");

  /** ########## Step 5 ##########
    * Send two bytes data value '0x3F00' referencing the Master File (MF).
    * Receive two bytes SW1 and SW2 from the smartcard and store them in a dedicated buffer.
    * Compare expected results to the received data.
    */
  TxTransferComplete = 0U; /* Reset the TxTransferComplete value */
  RxTransferComplete = 0U; /* Reset the RxTransferComplete value */

  /* Start Tx DMA for APDU Command parameters */
  if (HAL_SMARTCARD_Transmit_DMA(pUSARTx, (uint8_t *)MasterFile, DATA_SIZE_BYTE) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /** Waits for one of these SMARTCARD interrupts: tx transfer complete or transfer error.
    */
  while ((TxTransferComplete == 0) && (TransferError == 0))
  {
    /** Put the CPU in Wait For Interrupt state. An SMARTCARD interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
      */
    __WFI();
  }

  if (TransferError == 1)
  {
    /* Error while smartcard transmission */
    PRINTF("[ERROR] Step 5 : SmartCard transmission ERROR.\n");
    goto _app_process_exit;
  }

  /* Start Rx DMA for SW1/SW2 reception */
  if (HAL_SMARTCARD_Receive_DMA(pUSARTx, (uint8_t *)SmartCardAnswer, ADPU_RESPONSE_SIZE_BYTE) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /** Waits for one of these SMARTCARD interrupts: rx transfer complete or transfer error.
    */
  while ((RxTransferComplete == 0) && (TransferError == 0))
  {
    /** Put the CPU in Wait For Interrupt state. An SMARTCARD interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
      */
    __WFI();
  }

  if (TransferError == 1)
  {
    /* Error while smartcard transmission */
    PRINTF("[ERROR] Step 5 : SmartCard reception ERROR.\n");
    goto _app_process_exit;
  }

  SmartCardResponse.sw1 = SmartCardAnswer[0];
  SmartCardResponse.sw2 = SmartCardAnswer[1];

  if ((SmartCardResponse.sw1 != EXPECTED_SW1_VALUE) || (SmartCardResponse.sw2 != EXPECTED_SW2_VALUE))
  {
    PRINTF("[ERROR] Step 5 : Data received ERROR.\n");
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 5 : Data received as EXPECTED.\n");

  return_status = EXEC_STATUS_OK;
_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 6 ##########
  * De-initializes the SMARTCARD instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_smartcard_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** Transmission end of transfer completed callback
  * @user This implementation of the SMARTCARD Tx transfer complete callback can be customized.
  * This function is executed when the Tx transfer complete interrupt is generated.
  */
static void TxTransferCpltCallback(hal_smartcard_handle_t *pUSARTx)
{
  /** Tx Transfer completed.
    * Set the TxTransferComplete flag.
    * Back to the example execution flow and let application decide the action to take.
    */
  TxTransferComplete = 1U;
} /* end TxTransferCpltCallback */


/** Reception end of transfer completed callback
  * @user This implementation of the SMARTCARD Rx transfer complete callback can be customized.
  * This function is executed when the Rx transfer complete interrupt is generated.
  */
static void RxTransferCpltCallback(hal_smartcard_handle_t *pUSARTx)
{
  /** Rx Transfer completed.
    * Set the RxTransferComplete flag.
    * Back to the example execution flow and let application decide the action to take.
    */
  RxTransferComplete = 1U;
} /* end RxTransferCpltCallback */

/** SMARTCARD transfer error callback
  * @user This implementation of the SMARTCARD transfer error callback can be customized.
  * This function is executed when the transfer error interrupt is generated.
  */
static void TransferErrorCallback(hal_smartcard_handle_t *pUSARTx)
{
  /** Transfer error is occurred.
    * Set the TransferError flag.
    * Back to the example execution flow and let application decide the action to take.
    */
  TransferError = 1U;
} /* end TransferErrorCallback */


/**
  * Handles all Smartcard states and serves to send and receive all
  * communication data between Smartcard and reader.
  */
void SmartCardHandler(smartcard_state_t *smartcard_state, smartcard_adpu_commands_t *smartcard_adpu)
{
  uint32_t i = 0;
  switch (*smartcard_state)
  {
    case SMARTCARD_POWER_ON:
      if (smartcard_adpu->header.ins == SMARTCARD_GET_ATR)
      {
        /* Set the RSTIN HIGH */
        HAL_GPIO_WritePin(SC_RESET_PIN_PORT, SC_RESET_PIN_PIN, HAL_GPIO_PIN_SET);

        /* Select 5V */
        HAL_GPIO_WritePin(SC_3_5V_PIN_PORT, SC_3_5V_PIN_PIN, HAL_GPIO_PIN_SET);
        HAL_Delay(1);

        /*Enable CMDVCC */
        HAL_GPIO_WritePin(SC_CMDVCC_PIN_PORT, SC_CMDVCC_PIN_PIN, HAL_GPIO_PIN_RESET);
        HAL_Delay(1);

        /* Set RSTIN LOW */
        HAL_GPIO_WritePin(SC_RESET_PIN_PORT, SC_RESET_PIN_PIN, HAL_GPIO_PIN_RESET);

        /* Reset Data from SC buffer -------------------------------------------*/
        for (i = 0; i < 40; i++)
        {
          SmartCardAtrTable[i] = 0;
        }

        /* Reset SC_A2R Structure ----------------------------------------------*/
        SmartCardAtr.ts = 0;
        SmartCardAtr.t0 = 0;
        for (i = 0; i < SETUP_LENGTH; i++)
        {
          SmartCardAtr.setup[i] = 0;
        }
        for (i = 0; i < HIST_LENGTH; i++)
        {
          SmartCardAtr.hist[i] = 0;
        }
        SmartCardAtr.setup_length = 0;
        SmartCardAtr.hist_length = 0;

        /* Next State ----------------------------------------------------------*/
        *smartcard_state = SMARTCARD_RESET_LOW;
      }
      break;

    case SMARTCARD_RESET_LOW:
      if (smartcard_adpu->header.ins == SMARTCARD_GET_ATR)
      {
        /* If card is detected then Power ON, Card Reset and wait for an answer) */
        if (CardInserted == 1)
        {
          while (((*smartcard_state) != SMARTCARD_POWER_OFF) && ((*smartcard_state) != SMARTCARD_ACTIVE))
          {
            SmartCardAnswerReq(smartcard_state, (uint8_t *)&SmartCardAtrTable, 40); /* Check for answer to reset */
          }
        }
        else
        {
          (*smartcard_state) = SMARTCARD_POWER_OFF;
        }
      }
      break;

    case SMARTCARD_ACTIVE:
      if (smartcard_adpu->header.ins == SMARTCARD_GET_ATR)
      {
        if (SmartCardDecodeAnswer2Reset(&SmartCardAtrTable[0]) == T0_PROTOCOL)
        {
          (*smartcard_state) = SMARTCARD_ACTIVE_ON_T0;
        }
        else
        {
          (*smartcard_state) = SMARTCARD_POWER_OFF;
        }
      }
      break;

    case SMARTCARD_POWER_OFF:
      break;

    default:
      (*smartcard_state) = SMARTCARD_POWER_OFF;
  }
} /* end SmartCardHandler */


/**
  * Decodes the Answer to reset received from card.
  */
static uint8_t SmartCardDecodeAnswer2Reset(uint8_t *card)
{
  uint8_t i = 0;
  uint8_t flag = 0;
  uint8_t buf = 0;
  uint8_t protocol = 0;

  SmartCardAtr.ts = card[0];  /* Initial character */
  SmartCardAtr.t0 = card[1];  /* Format character */

  SmartCardAtr.hist_length = SmartCardAtr.t0 & 0x0F;

  if ((SmartCardAtr.t0 & 0x80) == 0x80)
  {
    flag = 1;
  }

  for (i = 0; i < 4; i++)
  {
    SmartCardAtr.setup_length = SmartCardAtr.setup_length + (((SmartCardAtr.t0 & 0xF0) >> (4 + i)) & 0x01);
  }

  for (i = 0; i < SmartCardAtr.setup_length; i++)
  {
    SmartCardAtr.setup[i] = card[i + 2];
  }

  protocol = SmartCardAtr.setup[SmartCardAtr.setup_length - 1] & 0x0F;

  while (flag)
  {
    if ((SmartCardAtr.setup[SmartCardAtr.setup_length - 1] & 0x80) == 0x80)
    {
      flag = 1;
    }
    else
    {
      flag = 0;
    }

    buf = SmartCardAtr.setup_length;
    SmartCardAtr.setup_length = 0;

    for (i = 0; i < 4; i++)
    {
      SmartCardAtr.setup_length = (SmartCardAtr.setup_length + \
                                   (((SmartCardAtr.setup[buf - 1] & 0xF0) >> (4 + i)) & 0x01));
    }

    for (i = 0; i < SmartCardAtr.setup_length; i++)
    {
      SmartCardAtr.setup[buf + i] = card[i + 2 + buf];
    }

    SmartCardAtr.setup_length += buf;
  }

  for (i = 0; i < SmartCardAtr.hist_length; i++)
  {
    SmartCardAtr.hist[i] = card[i + 2 + SmartCardAtr.setup_length];
  }

  return protocol;
} /* end SmartCardDecodeAnswer2Reset */


/**
  * Requests the reset answer from card.
  */
static void SmartCardAnswerReq(smartcard_state_t *smartcard_state, uint8_t *card, uint8_t length)
{
  switch (*smartcard_state)
  {
    case SMARTCARD_RESET_LOW:
      /* Check response with reset low -----------------------------------------*/
      SmartCardReceiveDMA(card, length);
      if (card[0] != 0x00)
      {
        (*smartcard_state) = SMARTCARD_ACTIVE;
      }
      else
      {
        (*smartcard_state) = SMARTCARD_RESET_HIGH;
      }
      break;

    case SMARTCARD_RESET_HIGH:
      /* Check response with reset high ----------------------------------------*/
      /*Set the reset pin High */
      HAL_GPIO_WritePin(SC_RESET_PIN_PORT, SC_RESET_PIN_PIN, HAL_GPIO_PIN_SET);
      SmartCardReceiveDMA(card, length);
      if (card[0])
      {
        (*smartcard_state) = SMARTCARD_ACTIVE;
      }
      else
      {
        (*smartcard_state) = SMARTCARD_POWER_OFF;
      }
      break;

    case SMARTCARD_ACTIVE:
      break;

    case SMARTCARD_POWER_OFF:
      /* Close Connection if no answer received --------------------------------*/
      /* Reset pin high - a bit is used as level shifter from 3.3 to 5 V */
      HAL_GPIO_WritePin(SC_RESET_PIN_PORT, SC_RESET_PIN_PIN, HAL_GPIO_PIN_SET);

      /* Disable the power to the smartcard */
      HAL_GPIO_WritePin(SC_CMDVCC_PIN_PORT, SC_CMDVCC_PIN_PIN, HAL_GPIO_PIN_SET);
      break;

    default:
      (*smartcard_state) = SMARTCARD_RESET_LOW;
      break;
  }
} /* end SmartCardAnswerReq */


/**
  * Receive an amount of data from Smartcard, according to selected method (Polling, IT or DMA).
  */
static void SmartCardReceiveDMA(uint8_t *p_data, uint16_t size)
{
  uint32_t tickstart = 0;
  /* Init tickstart for timeout management*/
  tickstart = HAL_GetTick();

  if (HAL_SMARTCARD_Receive_DMA(pUSARTx, p_data, size) != HAL_OK)
  {
    return;
  }
  while (HAL_SMARTCARD_GetState(pUSARTx) != HAL_SMARTCARD_STATE_IDLE)
  {
    /* In case of timeout (less bytes received than expected, return Error result */
    if ((HAL_GetTick() - tickstart) > SMARTCARD_RECEIVE_TIMEOUT)
    {
      /* Clean DMA handle used for Rx operation */
      HAL_SMARTCARD_Abort(pUSARTx);
      return;
    }
  }
  while ((RxTransferComplete == 0) && (TransferError == 0))
  {
    /** Put the CPU in Wait For Interrupt state. An SMARTCARD interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
      */
    __WFI();
  }
} /* end SmartCardReceiveDMA */
