/**
  ******************************************************************************
  * file           : example.c
  * brief          : I3C Target example with entry to Low power mode
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TARGET_DYNAMIC_ADDRESS   0x32U   /* Dynamic address expected */
#define VALID_ADDRESS            1U      /* Dynamic address valid */
/* @user: set the maximum number of attempts to complete a data transfer between the two boards */
#define MAX_COM_ATTEMPTS 10U

/* @user: must be equal to the size (in bytes) of the buffer received from the controller */
#define RX_BUFFER_SIZE 41U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

hal_i3c_handle_t *pI3C;   /* pointer referencing the I3C handle from the generated code */
hal_i3c_ccc_info_t CCCInfo; /* Holds Common Command Code (CCC) information */

volatile uint32_t DynamicAddressAssignmentReceived;  /* Dynamic address assignment notification received */
volatile uint32_t ReceiveTransferComplete;  /* RX transfer is completed */
volatile uint32_t TransmitTransferComplete; /* TX transfer is completed */
volatile uint32_t TransferError;            /* Transfer error detected */

uint8_t DynamicAddressAssignmentCompleted = 0U; /* Dynamic address assignment completed for this power cycle */

/* Buffer used for reception */
uint8_t RxBuffer[RX_BUFFER_SIZE] = {0U};

/* Private functions prototype -----------------------------------------------*/
/* Functions allowing the user to configure dynamically the I3C callbacks */
static void NotifyCallback(hal_i3c_handle_t *hi3c, uint32_t notifyId);
static void TgtRxCpltCallback(hal_i3c_handle_t *pI3C);
static void TgtTxCpltCallback(hal_i3c_handle_t *pI3C);
static void TransferErrorCallback(hal_i3c_handle_t *pI3C);


/** ########## Step 1 ##########
  * The initialization of the I3C instance is triggered by the application code.
  * The user callbacks for I3C target TX/RX transfer completed,
  * transfer error and notification callback are registered.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pI3C = mx_example_i3c_init();
  if (pI3C == NULL)
  {
    goto _app_init_exit;
  }

  /* Register callbacks */
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

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status;    /* Memorizes the HAL status of the I3C operations */
  uint32_t com_attempts = 0U; /* Memorizes the number of attempts */

  /**
    * ########## Step 2 ##########
    * Perform dynamic address assignment.
    * This step activates the notification, waits for dynamic address assignment completion,
    * and checks the assigned address.
    * The dynamic address assignment is executed only once per power-up in app_process.
    */
  if (!DynamicAddressAssignmentCompleted)
  {
    /* Activate notification for dynamic address assignment */
    hal_status = HAL_I3C_TGT_ActivateNotification(pI3C,
                                                  (uint8_t *)NULL,
                                                  0U,
                                                  (HAL_I3C_TGT_NOTIFICATION_DAU
                                                   | HAL_I3C_TGT_NOTIFICATION_WKP));
    if (hal_status != HAL_OK)
    {
      /* Error occurred while activating notification. Handle and retry. */
      return_status = EXEC_STATUS_ERROR;
    }

    /* Wait for dynamic address assignment completion or an error */
    while ((DynamicAddressAssignmentReceived == 0U) && (TransferError == 0U))
    {
      /** Put the CPU in Wait For Interrupt state.
        * An I3C interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the IT mode.
        * It can be replaced by your own code.
        */
      __WFI();
    }

    if (TransferError == 1U)
    {
      return_status = EXEC_STATUS_ERROR;
    }

    /* Retrieve Common Command Code info to check dynamic address assignment */
    hal_status = HAL_I3C_GetCCCInfo(pI3C, HAL_I3C_TGT_NOTIFICATION_DAU, &CCCInfo);
    if (hal_status != HAL_OK)
    {
      return_status  = EXEC_STATUS_ERROR;
    }

    /* Check the validity of the assigned dynamic address */
    if ((CCCInfo.dynamic_addr_valid != VALID_ADDRESS)
        || (CCCInfo.dynamic_addr != TARGET_DYNAMIC_ADDRESS))
    {
      return_status  = EXEC_STATUS_ERROR;
    }

    DynamicAddressAssignmentCompleted = 1U;
  }

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Clear I3C interrupt flags before starting a new transfer attempt */
    ReceiveTransferComplete = 0U;
    TransferError = 0U;

    /* Start a new communication attempt */
    com_attempts++;

    /** ########## Step 3 ##########
      * Start target receive in interrupt mode, then enter Low power mode.
      * The target wakes up when the controller starts the transfer.
      */

    hal_status = HAL_I3C_TGT_Receive_IT(pI3C, RxBuffer, RX_BUFFER_SIZE);
    if (hal_status != HAL_OK)
    {
      /* Error occurred during receive. Handle and retry. */
      return_status  = EXEC_STATUS_ERROR;
      continue;
    }
    else
    {
      /*
        * Enter Low power mode only after RX is armed.
        * If RX start returns an error, the code must not request low power; it retries instead.
        */
      HAL_SuspendTick();
      HAL_PWR_EnterStopMode(HAL_PWR_LOW_PWR_MODE_WFI, HAL_PWR_STOP_MODE);

      /* Set the sysclock source after waked-up */
      if (HAL_RCC_SetSYSCLKSource(HAL_RCC_SYSCLK_SRC_PSIS) != HAL_OK)
      {
        return EXEC_STATUS_ERROR;
      }

      HAL_ResumeTick();
    }

    /** ########## Step 4 ##########
      * After wakeup, wait for receive complete or error.
      */
    while ((ReceiveTransferComplete == 0) && (TransferError == 0));

    if (TransferError == 1U)
    {
      /** The write and read transfer is not correctly completed.
        * Report the error, then restart the communication.
        */
      return_status  = EXEC_STATUS_ERROR;
      continue;
    }

    /** ########## Step 5 ##########
      * Start target transmit in interrupt mode to send back the received buffer.
      */

    /* Clear I3C interrupt flags before starting a new transfer attempt */
    TransmitTransferComplete = 0U;
    TransferError            = 0U;

    hal_status = HAL_I3C_TGT_Transmit_IT(pI3C, RxBuffer, sizeof(RxBuffer));
    if (hal_status != HAL_OK)
    {
      /* The write and read transfer is not correctly completed.
        * Report the error, then restart the communication
        */
      return_status  = EXEC_STATUS_ERROR;
      continue;
    }

    /** ########## Step 6 ##########
      * Wait for transmit complete or error.
      * Use Wait For Interrupt state while waiting for completion.
      */
    while ((TransmitTransferComplete == 0) && (TransferError == 0))
    {
      /** Put the CPU in Wait For Interrupt state.
        * An I3C interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the IT mode.
        * It can be replaced by your own code.
        */
      __WFI();
    }

    if (TransferError == 1U)
    {
      /* TX transfer error occurred. Handle and retry. */
      return_status  = EXEC_STATUS_ERROR;
      continue;
    }

    return_status = EXEC_STATUS_OK;

  } /* end while */

  return return_status;
} /* end app_process */


/** I3C target notification callback (interrupt mode)
  * @user This implementation of the I3C callback can be customized.
  * This function is executed if the transfer complete interrupt is generated.
  */
static void NotifyCallback(hal_i3c_handle_t *hi3c, uint32_t notifyId)
{
  if ((notifyId & HAL_I3C_TGT_NOTIFICATION_DAU) == HAL_I3C_TGT_NOTIFICATION_DAU)
  {
    DynamicAddressAssignmentReceived = 1U;
  }
}


/**
  * I3C target RX transfer complete callback (interrupt mode).
  * @user This implementation can be customized.
  * This function is executed when the receive is completed by an interrupt.
  */
static void TgtRxCpltCallback(hal_i3c_handle_t *hi3c)
{
  /* Signal RX transfer completion for step 4 */
  ReceiveTransferComplete = 1U;
}


/**
  * I3C target TX transfer complete callback (interrupt mode).
  * @user This implementation can be customized.
  * This function is executed when the Transmit is completed by an interrupt.
  */
static void TgtTxCpltCallback(hal_i3c_handle_t *hi3c)
{
  /* Signal TX transfer completion for step 5 */
  TransmitTransferComplete = 1U;
}


/**
  * I3C target transfer error callback (interrupt mode).
  * @user This implementation can be customized.
  * This function is executed when a transfer error interrupt occurs.
  */
static void TransferErrorCallback(hal_i3c_handle_t *hi3c)
{
  /* Asynchronous processing for transfer error */
  TransferError = 1U;
}


/** De-initializes the I3C instances before leaving the scenario.
  * In this example, app_deinit is never called, it is provided as a reference.
  */
app_status_t app_deinit(void)
{
  mx_example_i3c_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
