/**
  ******************************************************************************
  * file           : example.c
  * brief          : example program body
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
#include <string.h> /* importing memcmp and memset functions */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* @user: set the maximum number of attempts to establish a reliable data transfer between the two boards */
#define MAX_COM_ATTEMPTS 10U
/* @user: define the responder 7-bit address */
#define SMBUS_RESPONDER_ADDR_7BIT 0x3FU
/* The responder 7-bit address must be shifted to the left before calling SMBUS HAL APIs */
#define SMBUS_RESPONDER_ADDR_7BIT_SHIFTED (SMBUS_RESPONDER_ADDR_7BIT << 1U)
/* @user: set the payload size in bytes. The SMBUS message is a string made of this payload and terminated by '\0'. */
#define SMBUS_PAYLOAD_SIZE  42U
/**
  * Size of the TX and RX buffers in bytes.
  * +1 is used to store the null character as the transmitted SMBUS message is a null-terminated string.
  */
#define BUFFER_SIZE         (SMBUS_PAYLOAD_SIZE + 1U)
#define SMBUS_PEC_SIZE      0x1U

hal_smbus_handle_t *pSMBUS;  /* Pointer referencing the SMBUS handle from the generated code */

volatile uint8_t TxTransferComplete; /* Set to 1 if the write transfer is correctly completed */
volatile uint8_t RxTransferComplete; /* Set to 1 if the read transfer is correctly completed */
volatile uint8_t TransferError;      /* Set to 1 if a transmission or a reception error is detected */

const uint8_t TxBufferA[BUFFER_SIZE] = "SMBUS Two Boards Communication - Message A";
const uint8_t TxBufferB[BUFFER_SIZE] = "SMBUS Two Boards Communication - Message B";
/* Pointer to the buffer used for transmission */
const uint8_t *pTxData = NULL;
/* Buffer used for reception */
uint8_t RxBuffer[BUFFER_SIZE] = {0U};

/* Private functions prototype -----------------------------------------------*/
static inline void UpdateBuffers(void);
static app_status_t HandleTransferCplt(void);
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t smbus_error_code);

/* Functions allowing the user to configure dynamically the SMBUS master callbacks instead of weak functions */
static void MasterTxTransferCpltCallback(hal_smbus_handle_t *pSMBUS);
static void MasterRxTransferCpltCallback(hal_smbus_handle_t *pSMBUS);
static void TransferErrorCallback(hal_smbus_handle_t *pSMBUS);

/** ########## Step 1 ##########
  * Configures and initializes the SMBUS instance and the NVIC.
  * Registers the user callbacks for SMBUS controller events: TX/RX transfer completed and transfer error.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pSMBUS = mx_example_smbus_init();

  if (pSMBUS == NULL)
  {
    goto _app_init_exit;
  }

  if (HAL_SMBUS_MASTER_RegisterTxCpltCallback(pSMBUS, MasterTxTransferCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_SMBUS_MASTER_RegisterRxCpltCallback(pSMBUS, MasterRxTransferCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_SMBUS_RegisterErrorCallback(pSMBUS, TransferErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status;          /* Memorizes the HAL status of the SMBUS operations */
  uint32_t com_attempts = 0U;       /* Memorizes the number of attempts made to exchange a message between the boards */
  uint32_t smbus_error_code = 0U;   /* Memorizes the SMBUS error code limited to the last process */

  UpdateBuffers();

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Clear the SMBUS interrupts flags before starting a new transfer */
    TxTransferComplete = 0U;
    RxTransferComplete = 0U;
    TransferError = 0U;

    /* Start a new communication attempt */
    com_attempts ++;

    /** ########## Step 2 ##########
      * Transmits in interrupt mode the buffer pointed by pTxData, containing a null-terminated string, using SMBUS.
      * The transmission uses the SMBUS PEC feature.
      */
    hal_status = HAL_SMBUS_MASTER_SEQ_Transmit_IT(
                   pSMBUS,
                   SMBUS_RESPONDER_ADDR_7BIT_SHIFTED,
                   pTxData,
                   BUFFER_SIZE + SMBUS_PEC_SIZE,
                   HAL_SMBUS_XFER_FIRST_AND_LAST_FRAME_WITH_PEC /* This option appends the PEC byte to the message */
                 );
    if (hal_status != HAL_OK)
    {
      /* An error occurs at the startup of the SMBUS write transfer. Report it, then restart the communication */
      smbus_error_code = HAL_SMBUS_GetLastErrorCodes(pSMBUS);
      return_status = HandleTransferError(hal_status, smbus_error_code);
      continue;
    }

    /** ########## Step 3 ##########
      * Waits for one of these SMBUS interrupts: write transfer complete or transfer error.
      */
    while ((TxTransferComplete == 0U) && (TransferError == 0U))
    {
      /** Put the CPU in Wait For Interrupt state. A SMBUS interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
        */
      __WFI();
    }
    if (TransferError == 1U)
    {
      /* The write transfer is not correctly completed. Report the error, then restart the communication */
      smbus_error_code = HAL_SMBUS_GetLastErrorCodes(pSMBUS);
      return_status = HandleTransferError(hal_status, smbus_error_code);
      continue;
    }

    /** ########## Step 4 ##########
      * Receives in interrupt mode a buffer containing a null-terminated string using SMBUS.
      * The received message contains the PEC byte.
      */
    hal_status = HAL_SMBUS_MASTER_SEQ_Receive_IT(
                   pSMBUS,
                   SMBUS_RESPONDER_ADDR_7BIT_SHIFTED,
                   RxBuffer,
                   BUFFER_SIZE + SMBUS_PEC_SIZE,
                   HAL_SMBUS_XFER_FIRST_AND_LAST_FRAME_WITH_PEC /* This option enables the PEC check */
                 );
    if (hal_status != HAL_OK)
    {
      /* An error occurs at the startup of the SMBUS write transfer. Report it, then restart the communication */
      smbus_error_code = HAL_SMBUS_GetLastErrorCodes(pSMBUS);
      return_status = HandleTransferError(hal_status, smbus_error_code);
      continue;
    }

    /** ########## Step 5 ##########
      * Waits for one of these SMBUS interrupts: read transfer complete or transfer error.
      */
    while ((RxTransferComplete == 0U) && (TransferError == 0U))
    {
      /** Put the CPU in Wait For Interrupt state. An SMBUS interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
        */
      __WFI();
    }
    if (TransferError == 1U)
    {
      /* The receive transfer is not correctly completed. Report the error, then restart the communication */
      smbus_error_code = HAL_SMBUS_GetLastErrorCodes(pSMBUS);
      return_status = HandleTransferError(hal_status, smbus_error_code);
      continue;
    }

    /** ########## Step 6 ##########
      * Checks the correctness of the received buffer after the data transfer is completed.
      */
    return_status = HandleTransferCplt();

  }/* end while */

  return return_status;
} /* end app_process */


/** Master transmission end of transfer callback
  * @user This implementation of the SMBUS master Tx transfer complete callback can be customized.
  * This function is executed if the master Tx transfer complete interrupt is generated.
  */
static void MasterTxTransferCpltCallback(hal_smbus_handle_t *hsmbus)
{
  /* Asynchronous processing related to step 3 */
  TxTransferComplete = 1U;
}


/** Master reception end of transfer callback
  * @user This implementation of the SMBUS master Rx transfer complete callback can be customized.
  * This function is executed if the master Rx transfer complete interrupt is generated.
  */
static void MasterRxTransferCpltCallback(hal_smbus_handle_t *hsmbus)
{
  /* Asynchronous processing related to step 5 */
  RxTransferComplete = 1U;
}


/** SMBUS transfer error callback
  * @user This implementation of the SMBUS master transfer error callback can be customized.
  * This function is executed if the master transfer error interrupt is generated.
  */
static void TransferErrorCallback(hal_smbus_handle_t *hsmbus)
{
  /* Asynchronous processing related to step 3 and 5 */
  TransferError = 1U;
}

/** De-initializes the SMBUS instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_smbus_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** brief:  Updates the TX buffer pointer and clears the RX buffer before starting a new transfer.
  * param:  none
  * retval: none
  */
static inline void UpdateBuffers(void)
{
  /* Toggle between BufferA and BufferB to select the message to send */
  if (pTxData == TxBufferA)
  {
    pTxData = TxBufferB;
  }
  else
  {
    pTxData = TxBufferA;
  }

  memset(RxBuffer, 0U, BUFFER_SIZE);
} /* end UpdateBuffers */

/** brief:  Checks the correctness of the data when the transfer is completed.
  * param:  none
  * retval: example status
  */
static app_status_t HandleTransferCplt(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (memcmp(pTxData, RxBuffer, BUFFER_SIZE) == 0)
  {
    PRINTF("[INFO] Controller - Tx/Rx Buffers IDENTICAL. Transfer COMPLETED of %s \n", pTxData);

    return_status = EXEC_STATUS_OK;
  }
  else
  {
    PRINTF("[ERROR] Controller - Tx/Rx Buffers DIFFERENT. TRYING AGAIN.\n");
  }

  return return_status;
} /* end HandleTransferCplt */

/** brief:  This function is executed in case of a data transfer error.
  * param hal_status:  HAL status of the SMBUS TX/RX operations.
  * param smbus_error_code:  SMBUS Error Code.
  * retval: example status
  */
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t smbus_error_code)
{
  PRINTF("[ERROR] Controller - Communication ERROR: hal_status = %" PRIu32 ", \
  HAL_SMBUS_GetLastErrorCodes = %" PRIu32 ". TRYING AGAIN.\n", hal_status, smbus_error_code);

  /* Take the time to check the setup before restarting the communication */
  HAL_Delay(1000U);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
