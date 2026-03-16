/**
  ******************************************************************************
  * file           : example.c
  * brief          : led fading driven by pwm example with BSP part led
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
/* @user: configure the speed, FAST, MEDIUM or SLOW, at which the led is blinked */
#define EXAMPLE_BLINKING_SPEED       LED_SPEED_MEDIUM

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
    * Starts blinking the MX_LED_PWM_EXAMPLE indefinitely
    */
  led_pwm_fade(pLEDObj, EXAMPLE_BLINKING_SPEED);

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
