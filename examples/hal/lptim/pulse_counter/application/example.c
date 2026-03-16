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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

hal_lptim_handle_t *pLPTIM;  /* pointer referencing the LPTIM handle from the generated code */
volatile uint8_t AutoReloadMatch;

/* Private functions prototype -----------------------------------------------*/
static void AutoReloadMatchCallback(hal_lptim_handle_t *hlptim);


/** ########## Step 1 ##########
  * The applicative code initializes the LPTIM instance and registers the user callback to detect when the number of
  * pulses is reached. Starts the LPTIM in interrupt mode.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pLPTIM = mx_example_lptim_init();
  if (pLPTIM == NULL)
  {
    goto _app_init_exit;
  }

  if (HAL_LPTIM_RegisterAutoReloadMatchCallback(pLPTIM, AutoReloadMatchCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_LPTIM_Start_IT(pLPTIM) != HAL_OK)
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
  AutoReloadMatch = 0U;

  /** ########## Step 2 ##########
    * The device goes in stop mode and waits for an interrupt.
    */

  /** In debug mode, it is necessary to suspend the Tick increment to prevent WakeUp by Systick interrupt.
    * Otherwise the Systick interrupt wakes the device up within 1ms (HAL time base).
    */
  HAL_SuspendTick();

  /* Enter STOP Mode with WaitForInterrupt parameter, any interrupt enabled in NVIC would wake the system up.*/
  HAL_PWR_EnterStopMode(HAL_PWR_LOW_PWR_MODE_WFI, MX_PWR_STOPy_MODE);

  /** ########## Step 3 ##########
    * When an interrupt occurs, the device is woken up. The LPTIM AutoReload interrupt is expected.
    */
  /* Restore the systick */
  HAL_ResumeTick();

  /* Reconfigure the clock.*/
  mx_rcc_init();

  if (AutoReloadMatch != 1U)
  {
    goto app_process_exit;
  }

  if (HAL_LPTIM_ResetCounter(pLPTIM) != HAL_OK)
  {
    goto app_process_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

app_process_exit:
  return return_status;
} /* end app_process */


/** De-initializes the LPTIM instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_lptim_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** AutoReload match callback.
  * @user This callback can be customized.
  * This function is executed if the autoreload register reaches the number of pulses to be detected.
  */
static void AutoReloadMatchCallback(hal_lptim_handle_t *hlptim)
{
  AutoReloadMatch = 1U;
}
