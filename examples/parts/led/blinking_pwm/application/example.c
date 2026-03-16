/**
  ******************************************************************************
  * file           : example.c
  * brief          : led blinking driven by pwm example with BSP part led
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
/* @user: configure the wait delay in milliseconds after the LED state is changed */
#define EXAMPLE_WAIT_DELAY_ON_MS (500U)
#define EXAMPLE_WAIT_DELAY_OFF_MS (500U)
#define EXAMPLE_WAIT_DELAY_TOGGLE_MS (200U)
/* @user: configure the number of loop where the LED state is changed */
#define EXAMPLE_NB_LOOP (10U)
/* @user: configure the speed, FAST, MEDIUM or SLOW, at which the led is blinked */
#define EXAMPLE_BLINKING_SPEED       LED_SPEED_FAST

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
led_pwm_t *pLEDObj;

/* Global variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
/** ########## Step 1 ##########
  * The applicative code initializes the LED PWM part driver.
  */
app_status_t app_init(void)
{
  app_status_t return_status;

  pLEDObj = mx_status_led_getobject();

  if (led_pwm_init(pLEDObj, MX_STATUS_LED) == LED_PWM_ERROR)
  {
    return_status = EXEC_STATUS_ERROR;
  }
  else
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  /** ########## Step 2 ##########
    * Turns on/off the MX_LED_PWM_EXAMPLE led, EXAMPLE_NB_LOOP times.
    */
  for (uint32_t loop_idx = 0 ; loop_idx < EXAMPLE_NB_LOOP ; loop_idx++)
  {
    led_pwm_on(pLEDObj);
    HAL_Delay(EXAMPLE_WAIT_DELAY_ON_MS);

    led_pwm_off(pLEDObj);
    HAL_Delay(EXAMPLE_WAIT_DELAY_OFF_MS);
  }

  /** ########## Step 3 ##########
    * Toggles, EXAMPLE_NB_LOOP times, the MX_LED_PWM_EXAMPLE led.
    */
  for (uint32_t loop_idx = 0 ; loop_idx < EXAMPLE_NB_LOOP ; loop_idx++)
  {
    led_pwm_toggle(pLEDObj);
    HAL_Delay(EXAMPLE_WAIT_DELAY_TOGGLE_MS);
    led_pwm_toggle(pLEDObj);
    HAL_Delay(EXAMPLE_WAIT_DELAY_TOGGLE_MS);
  }

  /** ########## Step 4 ##########
    * Starts blinking the MX_LED_PWM_EXAMPLE indefinitely
    */
  led_pwm_blink(pLEDObj, EXAMPLE_BLINKING_SPEED);

  return EXEC_STATUS_OK;
} /* end app_process */


/** Deinitializes the LED PWM part driver.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  app_status_t return_status;

  if (led_pwm_off(pLEDObj) == LED_PWM_ERROR)
  {
    return_status = EXEC_STATUS_ERROR;
  }
  else
  {
    return_status = EXEC_STATUS_OK;
  }

  return return_status;
} /* end app_deinit */
