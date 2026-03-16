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
volatile uint8_t CompareMatch;

/* Private functions prototype -----------------------------------------------*/
static void CompareMatchCallback(hal_lptim_handle_t *hlptim, hal_lptim_channel_t channel);

/** ########## Step 1 ##########
  * The applicative code initializes the LPTIM instance, registers the callback for the compare match event and starts
  * the LPTIM in interrupt mode.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pLPTIM = mx_example_lptim_init();
  if (pLPTIM == NULL)
  {
    goto _app_init_exit;
  }

  if (HAL_LPTIM_RegisterCompareMatchCallback(pLPTIM, CompareMatchCallback) != HAL_OK)
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
  CompareMatch = 0U;

  /** ########## Step 2 ##########
    * The device goes in stop mode and waits for an interrupt: ETR or Timeout.
    * The first ETR edge raises an interruption and starts the timeout. Any successive ETR resets the timeout.
    * If no trigger appears during the programmed timeout, CompareMatch interruption will occur.
    */

  while (CompareMatch != 1U)
  {
    /** In debug mode, it is necessary to suspend the Tick increment to prevent WakeUp by Systick interrupt.
      * Otherwise the Systick interrupt wakes the device up within 1ms (HAL time base).
      */
    HAL_SuspendTick();

    /* Enter STOP Mode with WaitForInterrupt parameter, any interrupt enabled in NVIC would wake the system up.*/
    HAL_PWR_EnterStopMode(HAL_PWR_LOW_PWR_MODE_WFI, MX_PWR_STOPy_MODE);

    /** Resume tick then configure the clock.
      * Clock configuration needs ticks to manage timeout.
      */
    HAL_ResumeTick();
    mx_example_rcc_init();

  }

  return_status = EXEC_STATUS_OK;

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
  * This function is executed if the autoreload register reaches the compare value to be detected.
  */
static void CompareMatchCallback(hal_lptim_handle_t *hlptim, hal_lptim_channel_t channel)
{
  CompareMatch = 1U;

  /* Restart timeout */
  HAL_LPTIM_ResetCounter(hlptim);
}

