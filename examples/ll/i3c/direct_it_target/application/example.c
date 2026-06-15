/**
  ******************************************************************************
  * file           : example.c
  * brief          : I3C direct IT target example with LL API
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
#define DEVICE_TARGET_DYNAMIC_ADDR     (0x32U)
#endif /* USE_LL_APP_ERROR */

/* @user Set this value to the expected Max Read Length (MRL) for the target. */
#define TARGET_CHECKED_MAX_READ_LENGTH  (0x04U)

/*  @user Set this value to the expected Max Write Length (MWL) for the target.*/
#define TARGET_CHECKED_MAX_WRITE_LENGTH (0x04U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t DAAProcessComplete; /* Set to 1 when the DAA (Dynamic Address Assignment) process is completed */
volatile uint8_t MRLProcessComplete; /* Set to 1 when the Max Read Length update process is completed           */
volatile uint8_t MWLProcessComplete; /* Set to 1 when the Max Write Length update process is completed          */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
volatile uint8_t TransferError;          /* Set to 1 when a transmission or reception error is detected         */
#endif /* USE_LL_APP_ERROR */

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

  /* Activate notification for (DAA process, MRL/MWL update)  */
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


    /** ########## Step 3 ##########
      * Checks and validates Max Read Length (MRLUPD) and Max Write Length (MWLUPD) flags.
      * Clears the update flags after validation.
      */
    if (MRLProcessComplete == 1U)
    {
      if (I3C_GetMaxReadLength() != TARGET_CHECKED_MAX_READ_LENGTH)
      {
        goto _app_process_exit;
      }
    }

    if (MWLProcessComplete == 1U)
    {
      if (I3C_GetMaxWriteLength() != TARGET_CHECKED_MAX_WRITE_LENGTH)
      {
        goto _app_process_exit;
      }
    }

    return_status = EXEC_STATUS_OK;
  }

_app_process_exit:
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
  * Max Read Length update complete callback
  * @user This implementation of the MX_I3Cx Max Read Length update callback can be customized.
  * This function is executed when the Max Read Length (MRL) update is received.
  */
void I3C_MRLCompleteCallback(void)
{
  MRLProcessComplete = 1U;
}

/**
  * Max Write Length update complete callback
  * @user This implementation of the MX_I3Cx Max Write Length update callback can be customized.
  * This function is executed when the Max Write Length (MWL) update is received.
  */
void I3C_MWLCompleteCallback(void)
{
  MWLProcessComplete = 1U;
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

