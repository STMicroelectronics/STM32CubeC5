/**
  ******************************************************************************
  * file           : example.c
  * brief          : single delayed high pulse for low-power event signaling using the Low-Power Timer (LPTIM) through
  *                  the LL LPTIM API.
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
volatile uint8_t CompareMatch = 0U;  /* Variable to check the compare match */

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the MX_LPTIMx instance, starts the MX_LPTIMx channel in interrupt mode then
  * starts the MX_LPTIMx.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_lptimx_init() == NULL)
  {
    goto _app_init_exit;
  }

  if (LPTIM_StartCompareMatchChannel_IT() != SYSTEM_OK)
  {
    goto _app_init_exit;
  }

  LPTIM_Start();

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * The device enters the deepest low power mode allowing the MX_LPTIMx to operate without any impact and waits for
  * an interrupt triggered by MX_LPTIMx Compare Match.
  * The counter starts on a rising edge detected on the trigger input.
  * When the counter reaches the Compare 1 value, a compare match interrupt is generated, which wakes up the system
  * from low power mode.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Enter the deepest low power mode with WFI (Wait For Interrupt) instruction allowing the MX_LPTIMx to operate
   * without any impact.
   */
  mx_pwr_enter_low_power();

  /* Restore the initial system clock configuration after waking up */
  if (mx_pwr_system_was_low_power() == 1U)
  {
    mx_pwr_exit_low_power();

    return_status = EXEC_STATUS_OK;
  }

  return return_status;
} /* end app_process */


/** Deinitializes the MX_LPTIMx before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_lptimx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/**
  * MX_LPTIMx compare match callback
  * @user This implementation of the MX_LPTIMx compare match callback can be customized.
  * This function is executed when the MX_LPTIMx compare match interrupt is generated.
  */
void LPTIM_CompareMatchCallback(void)
{
  CompareMatch = 1U;
}
