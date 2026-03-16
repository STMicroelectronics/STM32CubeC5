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
/* @user: must be equal to the size (in bytes) of the buffer received from the controller */
#define BUFFER_SIZE         43U
#define SMBUS_PEC_SIZE      0x1U

hal_smbus_handle_t *pSMBUS;  /* Pointer referencing the SMBUS handle from the generated code */

volatile uint8_t TxTransferComplete; /* Set to 1 if the write transfer is correctly completed */
volatile uint8_t RxTransferComplete; /* Set to 1 if the read transfer is correctly completed */
volatile uint8_t TransferError;      /* Set to 1 if a transmission or a reception error is detected */
volatile uint8_t SlaveAddressMatch;  /* Set to 1 when the SMBUS Slave Address Match interrupt occurs */
volatile uint8_t ListenComplete;     /* Set to 1 when the slave listen process is completed */

/* Buffer used for reception (sent back to the controller) */
uint8_t RxBuffer[BUFFER_SIZE] = {0U};

/* Private functions prototype -----------------------------------------------*/
static app_status_t HandleTransferCplt(void);
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t smbus_error_code);

/* Functions allowing the user to configure dynamically the SMBUS slave callbacks instead of weak functions */
static void SlaveTxTransferCpltCallback(hal_smbus_handle_t *pSMBUS);
static void SlaveRxTransferCpltCallback(hal_smbus_handle_t *pSMBUS);
static void TransferErrorCallback(hal_smbus_handle_t *pSMBUS);
static void AddrMatchCallback(hal_smbus_handle_t *hsmbus, hal_smbus_slave_xfer_direction_t xfer_direction,
                              uint32_t addr_match_code);
static void ListenCpltCallback(hal_smbus_handle_t *hsmbus);

/** ########## Step 1 ##########
  * Configures and initializes the SMBUS instance and the NVIC.
  * Registers the user callbacks for SMBUS responder events: TX/RX transfer completed, transfer error, address match
  * and listen complete.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pSMBUS = mx_example_smbus_init();

  if (pSMBUS == NULL)
  {
    goto _app_init_exit;
  }

  if (HAL_SMBUS_SLAVE_RegisterTxCpltCallback(pSMBUS, SlaveTxTransferCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_SMBUS_SLAVE_RegisterRxCpltCallback(pSMBUS, SlaveRxTransferCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_SMBUS_RegisterErrorCallback(pSMBUS, TransferErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_SMBUS_SLAVE_RegisterAddrMatchCallback(pSMBUS, AddrMatchCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_SMBUS_SLAVE_RegisterListenCpltCallback(pSMBUS, ListenCpltCallback) != HAL_OK)
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

  /* Clear the Rx buffer before starting a new transfer */
  memset(RxBuffer, 0U, BUFFER_SIZE);

  /* Clear the slave address flag related to the AddrMatchCallback callback before enabling the related interrupt */
  SlaveAddressMatch = 0U;

  /* Enable the address listen mode interruption to detect the responder address sent by the controller */
  hal_status = HAL_SMBUS_SLAVE_EnableListen_IT(pSMBUS);

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Clear the SMBUS interrupts flags before starting a new transfer */
    TxTransferComplete = 0U;
    RxTransferComplete = 0U;
    TransferError = 0U;
    ListenComplete = 0U;

    /* Start a new communication attempt */
    com_attempts++;


    /** ########## Step 2 ##########
      * Initiates the receive sequence in interrupt mode. The responder expects a buffer containing a null-terminated
      * string using SMBUS.
      * The received message contains the PEC byte.
      */

    /* Wait to receive the frame containing the address and the direction of the communication from the controller */
    while (SlaveAddressMatch == 0U)
    {
      /** Put the CPU in Wait For Interrupt state. A SMBUS interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
        */
      __WFI();
    }
    SlaveAddressMatch = 0;

    /* Receive the frames containing the message */
    hal_status = HAL_SMBUS_SLAVE_SEQ_Receive_IT(pSMBUS,
                                                RxBuffer,
                                                BUFFER_SIZE + SMBUS_PEC_SIZE,
                                                HAL_SMBUS_XFER_FIRST_AND_LAST_FRAME_WITH_PEC);
    if (hal_status != HAL_OK)
    {
      /* An error occurs at the startup of the SMBUS read transfer. Report it, then restart the communication */
      smbus_error_code = HAL_SMBUS_GetLastErrorCodes(pSMBUS);
      return_status = HandleTransferError(hal_status, smbus_error_code);
      continue;
    }

    /** ########## Step 3 ##########
      * Waits for one of these SMBUS interrupts: read transfer complete or transfer error.
      */
    while ((RxTransferComplete == 0U) && (TransferError == 0U))
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

    /* Wait for the stop frame from the controller */
    while (ListenComplete == 0U)
    {
      /** Put the CPU in Wait For Interrupt state. A SMBUS interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
        */
      __WFI();
    }
    ListenComplete = 0;


    /** ########## Step 4 ##########
      * Initiates the transmit sequence in interrupt mode. The responder sends back the received buffer, containing a
      * null-terminated string using SMBUS.
      * The transmission uses the SMBUS PEC feature.
      */

    /* Wait to receive the frame containing the address and the direction of the communication from the controller */
    while (SlaveAddressMatch == 0U)
    {
      /** Put the CPU in Wait For Interrupt state. A SMBUS interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
        */
      __WFI();
    }
    SlaveAddressMatch = 0U;

    /* Send the message frames to the controller */
    hal_status = HAL_SMBUS_SLAVE_SEQ_Transmit_IT(pSMBUS,
                                                 RxBuffer,
                                                 BUFFER_SIZE + SMBUS_PEC_SIZE,
                                                 HAL_SMBUS_XFER_FIRST_AND_LAST_FRAME_WITH_PEC);
    if (hal_status != HAL_OK)
    {
      /* An error occurs at the startup of the SMBUS write transfer. Report it, then restart the communication */
      smbus_error_code = HAL_SMBUS_GetLastErrorCodes(pSMBUS);
      return_status = HandleTransferError(hal_status, smbus_error_code);
      continue;
    }

    /** ########## Step 5 ##########
      * Waits for one of these SMBUS interrupts: write transfer complete or transfer error.
      */
    while ((TxTransferComplete == 0U) && (TransferError == 0U))
    {
      /** Put the CPU in Wait For Interrupt state. An SMBUS interrupt or a SystTick interrupt can wake up the CPU.
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

    /* Wait for the stop frame from the controller */
    while (ListenComplete == 0U)
    {
      /** Put the CPU in Wait For Interrupt state. A SMBUS interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
        */
      __WFI();
    }

    /* Report the communication status: Data Transfer completed */
    return_status = HandleTransferCplt();

  }/* end while */

  hal_status = HAL_SMBUS_SLAVE_DisableListen_IT(pSMBUS);

  return return_status;
} /* end app_process */


/** SMBUS slave address match callback
  * @user This implementation of the SMBUS slave address match callback can be customized.
  * This function is executed if the address match callback interrupt is generated.
  */
static void AddrMatchCallback(hal_smbus_handle_t *hsmbus, hal_smbus_slave_xfer_direction_t xfer_direction,
                              uint32_t addr_match_code)
{
  /* Asynchronous processing related to step 2 */
  SlaveAddressMatch = 1U;
}

/** SMBUS listen complete callback
  * @user This implementation of the SMBUS slave listen complete callback can be customized.
  * This function is executed if the listen complete callback interrupt is generated.
  */
static void ListenCpltCallback(hal_smbus_handle_t *hsmbus)
{
  ListenComplete = 1U;

  /* Enable the address listen mode interruption again for the next communication step */
  HAL_SMBUS_SLAVE_EnableListen_IT(pSMBUS);
}


/** Slave transmission end of transfer callback
  * @user This implementation of the SMBUS slave Tx transfer complete callback can be customized.
  * This function is executed if the slave Tx transfer complete interrupt is generated.
  */
static void SlaveTxTransferCpltCallback(hal_smbus_handle_t *hsmbus)
{
  /* Asynchronous processing related to step 3 */
  TxTransferComplete = 1U;
}


/** Slave reception end of transfer callback
  * @user This implementation of the SMBUS slave Rx transfer complete callback can be customized.
  * This function is executed if the slave Rx transfer complete interrupt is generated.
  */
static void SlaveRxTransferCpltCallback(hal_smbus_handle_t *hsmbus)
{
  /* Asynchronous processing related to step 5 */
  RxTransferComplete = 1U;
}


/** SMBUS transfer error callback
  * @user This implementation of the SMBUS slave transfer error callback can be customized.
  * This function is executed if the slave transfer error interrupt is generated.
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


/** brief:  Checks the correctness of the data when the transfer is completed.
  * param:  none
  * retval: example status
  */
static app_status_t HandleTransferCplt(void)
{
  PRINTF("[INFO] Responder - Message received and sent back: %s \n", RxBuffer);

  return EXEC_STATUS_OK;
} /* end HandleTransferCplt */


/** brief:  This function is executed in case of a data transfer error.
  * param hal_status:  HAL status of the SMBUS TX/RX operations.
  * param smbus_error_code:  SMBUS Error Code.
  * retval: example status
  */
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t smbus_error_code)
{
  PRINTF("[ERROR] Responder - Communication ERROR: hal_status = %" PRIu32 ", \
  HAL_SMBUS_GetLastErrorCodes = %" PRIu32 ". TRYING AGAIN.\n", hal_status, smbus_error_code);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
