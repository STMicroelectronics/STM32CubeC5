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
volatile uint32_t CompareMatch = 0U;

/* Private functions prototype -----------------------------------------------*/
static void CompareMatchCallback(hal_lptim_handle_t *hlptim, hal_lptim_channel_t channel);

/** ########## Step 1 ##########
  * The applicative code initializes the LPTIM instance, registers the callback for the compare match event,s starts
  * the LPTIM channel in interrupt mode then, starts the LPTIM.
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

  if (HAL_LPTIM_OC_StartChannel_IT(pLPTIM, MX_LPTIM_CHANNEL_X) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_LPTIM_Start(pLPTIM) != HAL_OK)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * The device enters Stop mode and waits for an interrupt triggered by Compare Match 1.
  * The counter starts on a rising edge detected on the ETR input.
  * When the counter reaches the Compare 1 value, a compare match interrupt is generated,
  * which wakes up the system from Stop mode.
  */
app_status_t app_process(void)
{
  /**
    * In debug mode, suspend the SysTick timer to prevent it from waking the device prematurely.
    * Without this, the SysTick interrupt (HAL time base) would wake the device within 1 ms.
    */
  HAL_SuspendTick();

  /* Enter Stop mode with WFI (Wait For Interrupt) instruction.
    * Any enabled interrupt in NVIC can wake the system from this low-power state.
    */
  HAL_PWR_EnterStopMode(HAL_PWR_LOW_PWR_MODE_WFI, MX_PWR_STOPy_MODE);

  /**
    * Resume the SysTick timer and reconfigure the system clock.
    * The clock configuration requires the SysTick to handle timeouts correctly.
    */
  HAL_ResumeTick();

  /* Restore the initial system clock configuration after waking up */
  mx_example_rcc_init();

  return EXEC_STATUS_OK;
} /* end app_process */


/** De-initializes the LPTIM instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_lptim_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** Compare match Callback.
  * @user This callback can be customized.
  * This function is invoked when the counter value matches the configured compare value.
  * NOTE: this callback is invoked in interrupt (IT) context.
  */
static void CompareMatchCallback(hal_lptim_handle_t *hlptim,
                                 hal_lptim_channel_t channel)
{
  if (channel == MX_LPTIM_CHANNEL_X)
  {
    CompareMatch = 1U;
  }
}

