/**
  ******************************************************************************
  * file           : example.c
  * brief          : CRS sync to adjust the 48MHz internal clock using IT method
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t CRS_sync_ok = 0U;  /* variable to check the end of synchronisation */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
volatile uint8_t CRS_sync_error = 0U; /* variable to check the synchronisation error */
volatile uint8_t CRS_sync_warning = 0; /* variable to check the synchronisation warning */
#endif /* USE_LL_APP_ERROR */

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The init of the MX_CRSx instance is triggered by the applicative code.
  * The HSI48 clock is configured to be output on the MCO pin.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Configures the MX_CRSx instance */
  if (mx_crsx_init() != NULL)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  /** ########## Step 2 ##########
    * Starts the CRS synchronization with LSE in interrupt mode
    */
  CRS_StartSynchronization_IT();

  /** ########## Step 3 ##########
    * Wait for the end of synchronization or synchronization error/warning when USE_LL_APP_ERROR == 1
    */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  while ((CRS_sync_ok == 0U) && (CRS_sync_error == 0U) && (CRS_sync_warning == 0U))
#else
  while (CRS_sync_ok == 0U)
#endif /* USE_LL_APP_ERROR */
  {
    /** Put the CPU in Wait For Interrupt state. A CRS interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
      */
    __WFI();
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  if ((CRS_sync_error != 0U) || (CRS_sync_warning != 0U))
  {
    return EXEC_STATUS_ERROR;
  }
#endif /* USE_LL_APP_ERROR */

  return EXEC_STATUS_OK;
} /* end app_process */


/** ########## Step 4 ##########
  * Deinitializes the MX_CRSx before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_crsx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/**
  * MX_CRSx synchronization complete callback.
  */
void CRS_SynchronizationOKCallback(void)
{
  CRS_sync_ok = 1U;
  /* Disable CRS interrupt since synchronization succeeded */
  CRS_StopSynchronization_IT();
}

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/**
  * MX_CRSx synchronization error callback.
  */
void CRS_ErrorCallback(void)
{
  CRS_sync_error = 1U;
  /* Disable CRS interrupt since synchronization error */
  CRS_StopSynchronization_IT();
}


/**
  * MX_CRSx synchronization warning callback.
  */
void CRS_WarningCallback(void)
{
  CRS_sync_warning = 1U;
  /* Disable CRS interrupt since synchronization error */
  CRS_StopSynchronization_IT();
}
#endif /* USE_LL_APP_ERROR */
