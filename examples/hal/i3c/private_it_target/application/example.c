/**
  ******************************************************************************
  * file    example.c
  * brief   I3C target application handling private data transfers
  *          in non-blocking (interrupt-driven) mode using the HAL API.
  *          Demonstrates efficient, responsive communication with an I3C controller.
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
#define TARGET_CHECKED_DYNAMIC_ADDRESS   0x32U   /* Dynamic address expected from the controller */

/* @user: set the maximum number of attempts to complete a data transfer between the two boards */
#define MAX_COM_ATTEMPTS                10U

/* @user: must be equal to the size (in bytes) of the buffer received from the controller */
#define BUFFER_SIZE                     41U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

hal_i3c_handle_t *pI3C;   /* pointer referencing the I3C handle from the generated code */

hal_i3c_ccc_info_t CCCInfo; /* Holds Common Command Code (CCC) information, used to verify dynamic address assignment */

/* Set to 1 when the DAA (Dynamic Address Assignment) process is completed */
volatile uint8_t DAAProcessComplete = 0U;
volatile uint8_t RxTransferComplete = 0U;     /* Set to 1 when an RX transfer is successfully completed */
volatile uint8_t TxTransferComplete = 0U;     /* Set to 1 when a TX transfer is successfully completed */
volatile uint8_t TransferError = 0U;          /* Set to 1 when a transmission or reception error is detected */

uint8_t DaaCompleted = 0U; /* Tracks if DAA process completed for this power cycle */

/* Buffer used for reception (sent back to the controller) */
uint8_t RxBuffer[BUFFER_SIZE] = {0U};

/* Private functions prototype -----------------------------------------------*/
static app_status_t HandleTransferCplt(void);
static app_status_t HandleTransferError(hal_status_t hal_status, uint32_t i3c_error_code);

/* Functions allowing the user to configure dynamically the I3C callbacks instead of weak functions */
static void NotifyCallback(hal_i3c_handle_t *hi3c, uint32_t notifyId);
static void TgtRxCpltCallback(hal_i3c_handle_t *pI3C);
static void TgtTxCpltCallback(hal_i3c_handle_t *pI3C);
static void TransferErrorCallback(hal_i3c_handle_t *pI3C);


/** ########## Step 1 ##########
  * The initialization of the I3C instance is triggered by the application code.
  * The user callbacks for I3C target TX/RX transfer completed, transfer error and notification callback are registered.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pI3C = mx_example_i3c_init();
  if (pI3C == NULL)
  {
    goto _app_init_exit;
  }
  /* Register the I3C Controller callbacks */

  if (HAL_I3C_RegisterNotifyCallback(pI3C, NotifyCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_TGT_RegisterRxCpltCallback(pI3C, TgtRxCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_TGT_RegisterTxCpltCallback(pI3C, TgtTxCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_RegisterErrorCallback(pI3C, TransferErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /* If all checks pass, initialization is successful */
  return_status = EXEC_STATUS_INIT_OK;

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status;    /* Memorizes the HAL status of the I3C TX/RX operations */
  uint32_t com_attempts = 0U; /* Memorizes the number of attempts made to exchange a message between the boards */
  uint32_t i3c_error_code;    /* Memorizes the I3C error code limited to the last process */


  /**
    * ########## Step 2 ##########
    * Perform DAA (Dynamic Address Assignment) process and verify dynamic address assignment.
    * This step activates the notification, waits for DAA completion, and checks the assigned address.
    * The DAA process is executed only once per power-up in app_process.
    */
  if (!DaaCompleted)
  {
    /* Activate notification for DAA process (Dynamic Address Assignment) */
    hal_status = HAL_I3C_TGT_ActivateNotification(pI3C, (uint8_t *)NULL, 0U, HAL_I3C_TGT_NOTIFICATION_DAU);
    if (hal_status != HAL_OK)
    {
      /* Error occurred while activating notification. Handle and retry. */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status = HandleTransferError(hal_status, i3c_error_code);
    }

    /* Wait for the Dynamic Address Assignment (DAA) process to complete or for an error to occur */
    while ((DAAProcessComplete == 0U) && (TransferError == 0U))
    {
      /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
        */
      __WFI();
    }

    if (TransferError == 1U)
    {
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status = HandleTransferError(hal_status, i3c_error_code);
    }

    /* Retrieve CCC (Common Command Code) info to check dynamic address assignment */
    hal_status = HAL_I3C_GetCCCInfo(pI3C, HAL_I3C_TGT_NOTIFICATION_DAU, &CCCInfo);
    if (hal_status != HAL_OK)
    {
      /* Error occurred while retrieving CCC info. Handle and retry. */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status  = HandleTransferError(hal_status, i3c_error_code);
    }

    if (CCCInfo.dynamic_addr_valid != 1)
    {
      /* Dynamic address assignment is not valid. Handle error and retry. */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status  = HandleTransferError(hal_status, i3c_error_code);
    }

    /* Check if the assigned dynamic address matches the expected value */
    if (CCCInfo.dynamic_addr != TARGET_CHECKED_DYNAMIC_ADDRESS)
    {
      /* Dynamic address does not match. Handle error and retry. */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status  = HandleTransferError(hal_status, i3c_error_code);
    }

    DaaCompleted = 1U;
    PRINTF("[INFO] Step 2:DAA process and dynamic address verification COMPLETED.\n");
  }

  /* Clear the Rx buffer before starting a new transfer */
  memset(RxBuffer, 0U, BUFFER_SIZE);

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Clear I3C interrupt flags before starting a new transfer attempt */
    TxTransferComplete = 0U;
    RxTransferComplete = 0U;
    TransferError      = 0U;

    /* Start a new communication attempt */
    com_attempts++;

    /** ########## Step 3 ##########
      * Transmits in IT mode the buffer A or buffer B, which contains a null-terminated string,
      * and receives another null-terminated string buffer, both using I3C in IT (interrupt) mode.
      */

    hal_status = HAL_I3C_TGT_Receive_IT(pI3C, RxBuffer, BUFFER_SIZE);
    if (hal_status != HAL_OK)
    {
      /* Error occurred during receive. Handle and retry. */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status  = HandleTransferError(hal_status, i3c_error_code);
      continue;
    }

    /** ########## Step 4 ##########
      * Waits for one of these I3C interrupts: read (RX) transfer complete or transfer error.
      */
    while ((RxTransferComplete == 0) && (TransferError == 0))
    {
      /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
        */
      __WFI();
    }

    if (TransferError == 1U)
    {
      /* The write and read transfer is not correctly completed. Report the error, then restart the communication */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status  = HandleTransferError(hal_status, i3c_error_code);
      continue;
    }

    /** ########## Step 5 ##########
      * Transmits in IT mode the buffer A or buffer B, which contains a null-terminated string,
      * and receives another null-terminated string buffer, both using I3C in IT (interrupt) mode.
      */

    hal_status = HAL_I3C_TGT_Transmit_IT(pI3C, RxBuffer, sizeof(RxBuffer));
    if (hal_status != HAL_OK)
    {
      /* The write and read transfer is not correctly completed. Report the error, then restart the communication */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status  = HandleTransferError(hal_status, i3c_error_code);
      continue;
    }

    /** ########## Step 6 ##########
      * Waits for one of these I3C interrupts: write transfer complete or transfer error.
      */
    while ((TxTransferComplete == 0) && (TransferError == 0))
    {
      /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
        */
      __WFI();
    }

    if (TransferError == 1U)
    {
      /* TX transfer error occurred. Handle and retry. */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status  = HandleTransferError(hal_status, i3c_error_code);
      continue;
    }

    return_status = HandleTransferCplt();

  } /* end while */

  return return_status;
} /* end app_process */


/**
  * I3C target notification callback (interrupt mode).
  * @user This implementation of the I3C target notification callback can be customized.
  * This function is executed when a notification event occurs, such as DAA process completion (step 2).
  */
static void NotifyCallback(hal_i3c_handle_t *hi3c, uint32_t notifyId)
{
  if ((notifyId & HAL_I3C_TGT_NOTIFICATION_DAU) == HAL_I3C_TGT_NOTIFICATION_DAU)
  {
    DAAProcessComplete = 1U;
  }
}


/**
  * I3C target RX transfer complete callback (interrupt mode).
  * @user This implementation of the I3C target transfer complete callback can be customized.
  * This function is executed when the RX transfer is completed by an interrupt (step 4).
  */
static void TgtRxCpltCallback(hal_i3c_handle_t *hi3c)
{
  /* Signal RX transfer completion for step 4 */
  RxTransferComplete = 1U;
}


/**
  * I3C target TX transfer complete callback (interrupt mode).
  * @user This implementation of the I3C target transfer complete callback can be customized.
  * This function is executed when the TX transfer is completed by an interrupt (step 5).
  */
static void TgtTxCpltCallback(hal_i3c_handle_t *hi3c)
{
  /* Signal TX transfer completion for step 5 */
  TxTransferComplete = 1U;
}


/**
  * I3C target transfer error callback (interrupt mode).
  * @user This implementation of the I3C target transfer error callback can be customized.
  * This function is executed when a transfer error interrupt occurs (steps 4 or 6).
  */
static void TransferErrorCallback(hal_i3c_handle_t *hi3c)
{
  /* Signal transfer error for steps 4 and 6 */
  TransferError = 1U;
}


/** De-initializes the I3C and DMA instances before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_i3c_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** brief:  Checks the correctness of the data when the transfer is completed.
  * param:  none
  * retval: example status
  */
static app_status_t HandleTransferCplt(void)
{
  PRINTF("[INFO] Target - Message received and sent back: %s \n", RxBuffer);

  return EXEC_STATUS_OK;
} /* end HandleTransferCplt */


/** brief:  This function is executed in case of a data transfer error.
  * param hal_status:  HAL status of the I3C TX/RX operations.
  * param i3c_error_code:  I3C Error Code.
  * retval: example status
  */
static app_status_t HandleTransferError(hal_status_t hal_status, uint32_t i3c_error_code)
{
  PRINTF("[ERROR] Target - Communication ERROR: hal_status = %" PRIu8 ", \
  HAL_I3C_GetLastErrorCodes = %" PRIu32 ". TRYING AGAIN.\n", hal_status, i3c_error_code);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
