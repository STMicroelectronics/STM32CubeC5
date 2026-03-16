/**
  ******************************************************************************
  * file           : example.c
  * brief          : I3C IBI target example using interrupt-driven events with LL API
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
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
#define DEVICE_TARGET_DYNAMIC_ADDR (0x32U)
#endif /* USE_LL_APP_ERROR */

/** @user Payload data size to be transmitted to the controller. It can be 1, 2 or 3.
  * This value must be aligned with MX_I3Cx additional Mandatory Data Byte (MDB) configuration.
  */
#define PAYLOAD_DATA_SIZE          (0x03U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Set to 1 when the DAA (Dynamic Address Assignment) process is completed */
volatile uint8_t DAAProcessComplete = 0U;
/* Set to 1 when the IBI (In-Band Interrupt) process is completed */
volatile uint8_t IBIProcessComplete = 0U;
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/* Set to 1 when a transmission or reception error is detected */
volatile uint8_t TransferError = 0U;
#endif /* USE_LL_APP_ERROR */

/** IBI payload sent by the target.
  * @user: it possible to modify the payload content and length as needed.
  */
uint8_t IBIPayloadData[PAYLOAD_DATA_SIZE] = {0xAB, 0xCD, 0xEF};

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The MX_I3Cx peripheral is initialized by the application code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_i3cx_init() == NULL)
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


  /**
    * ########## Step 2 ##########
    * Perform DAA (Dynamic Address Assignment) process and verify dynamic address assignment.
    * This step activates the notification, waits for DAA completion, and checks the assigned address.
    */
  DAAProcessComplete = 0U;
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  TransferError = 0U;
#endif /* USE_LL_APP_ERROR */

  /* Activate notification for DAA process (Dynamic Address Assignment) */
  I3C_ActivateNotification();

  /* Wait for the DAA process to complete or for an error to occur when USE_LL_APP_ERROR == 1 */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  while ((DAAProcessComplete == 0U) && (TransferError == 0U))
#else
  while (DAAProcessComplete == 0U)
#endif /* USE_LL_APP_ERROR */
  {
    /** Put the CPU in Wait For Interrupt state. An MX_I3Cx interrupt or a SysTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
      */
    __WFI();
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  if ((TransferError == 1U) || (I3C_GetOwnDynamicAddress() != DEVICE_TARGET_DYNAMIC_ADDR))
  {
    goto _app_process_exit;
  }
#endif /* USE_LL_APP_ERROR */

  if (DAAProcessComplete == 1U)
  {
    IBIProcessComplete = 0U;


    /**
      * ########## Step 3 ##########
      * Send IBI (In-Band Interrupt) request to controller and wait for completion.
      */

    /* Send IBI request with payload (3 bytes) to the controller. */
    I3C_RequestIBI_IT(IBIPayloadData, PAYLOAD_DATA_SIZE);

    /* Wait for IBI process to complete or for an error to occur when USE_LL_APP_ERROR == 1 . */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    while ((IBIProcessComplete == 0U) && (TransferError == 0U))
#else
    while (IBIProcessComplete == 0U)
#endif /* USE_LL_APP_ERROR */
    {
      /** Put the CPU in Wait For Interrupt state. An MX_I3Cx interrupt or a SysTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
        */
      __WFI();
    }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    if (TransferError == 1U)
    {
      /* Handle transfer error after IBI process. */
      goto _app_process_exit;
    }
#endif /* USE_LL_APP_ERROR */

    return_status = EXEC_STATUS_OK;
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
_app_process_exit:
#endif /* USE_LL_APP_ERROR */
  return return_status;
} /* end app_process */


/** De-initializes the MX_I3Cx instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_i3cx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/**
  * Dynamic Address Assignment (DAA) complete callback
  * @user This implementation of the MX_I3Cx DAA complete callback can be customized.
  * This function is executed when the DAA process is completed.
  */
void I3C_DAACompleteCallback(void)
{
  /* Asynchronous processing related to DAA completion */
  DAAProcessComplete = 1U;
}

/**
  * MX_I3Cx target IBI complete callback.
  * @user This implementation of the MX_I3Cx target IBI complete callback can be customized.
  * This function is executed when an IBI completion event occurs.
  */
void I3C_IBICompleteCallback(void)
{
  /* IBI end event received:. */
  IBIProcessComplete = 1U;
}

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/**
  * MX_I3Cx target transfer error callback.
  * @user This implementation of the MX_I3Cx target transfer error callback can be customized.
  * This function is executed when a transfer error interrupt occurs.
  */
void I3C_ErrorCallback(void)
{
  TransferError = 1U;
}
#endif /* USE_LL_APP_ERROR */
