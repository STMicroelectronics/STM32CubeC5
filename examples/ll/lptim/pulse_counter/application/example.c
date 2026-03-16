/**
  ******************************************************************************
  * file           : example.c
  * brief          : Counting external pulses with the LPTIM (Low-Power Timer) using the LL API
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
volatile uint8_t AutoReloadMatch;

/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * Initializes the MX_LPTIMx instance to detect when the number of pulses is reached.
  * Starts the MX_LPTIMx instance in interrupt mode.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_lptimx_init() == NULL)
  {
    goto _app_init_exit;
  }

  LPTIM_Start_IT();

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  AutoReloadMatch = 0U;

  /** ########## Step 2 ##########
    * The device goes in stop mode and waits for an interrupt.
    */
  mx_pwr_enter_low_power();

  /** ########## Step 3 ##########
    * Checks the AutoReloadMatch flag value after the MX_LPTIMx interrupt generation.
    */
  if ((AutoReloadMatch == 1U) && (mx_pwr_system_was_low_power() == 1U))
  {
    /* Reconfigure the clock.*/
    mx_pwr_exit_low_power();

    LPTIM_ResetCounter();

    return_status = EXEC_STATUS_OK;
  }

  return return_status;
} /* end app_process */


/** Deinitializes the MX_LPTIMx peripheral before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_lptimx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/**
  * MX_LPTIMx auto reload match callback
  * @user This implementation of the MX_LPTIMx auto reload match callback can be customized.
  * This function is executed when the MX_LPTIMx auto reload match interrupt is generated.
  */
void LPTIM_AutoReloadMatchCallback(void)
{
  AutoReloadMatch = 1U;
}
