/**
  ******************************************************************************
  * file           : example.c
  * brief          : timebase example
  *                   Called by the main after system initialization
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
/* @user: configure the delay in milliseconds for led toggling */
#define EXAMPLE_DELAY_MS 500U
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code reconfigures the timebase with user tick frequency.
  * the frequency set by HAL_InitTick() directly influences the granularity and accuracy of the HAL_Delay() function.
  * @user: Set the tick frequency for timebase here either:
  * HAL_TICK_FREQ_10HZ, HAL_TICK_FREQ_100HZ or HAL_TICK_FREQ_1KHZ.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (HAL_InitTick(HAL_TICK_FREQ_100HZ, USE_HAL_TICK_INT_PRIORITY) != HAL_OK)
  {
    goto _app_init_tick;
  }
  return_status = EXEC_STATUS_INIT_OK;

_app_init_tick:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Toggles the LED state with a 500ms delay.
  * The Hal_Delay() uses the global tick counter that is incremented by the tick ISR to count the passage of time.
  */
app_status_t app_process(void)
{
  HAL_GPIO_TogglePin(MX_STATUS_LED_GPIO_PORT, MX_STATUS_LED_PIN);
  HAL_Delay(EXAMPLE_DELAY_MS);
  return EXEC_STATUS_OK;
} /* end app_process */


/** In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  /** This API is not used in this example (infinite loop).
    * It is optimized out by the toolchain.
    */
  return EXEC_STATUS_OK;
} /* end app_deinit */
