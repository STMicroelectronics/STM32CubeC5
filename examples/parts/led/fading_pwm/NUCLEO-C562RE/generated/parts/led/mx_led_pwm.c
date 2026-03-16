/**
  ******************************************************************************
  * file           : mx_led_pwm.c
  * brief          : Code generation for the LED PWM part driver.
  ******************************************************************************
  * attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_led_license.md file
  * in the same directory as the generated code.
  * If no mx_led_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ---------------------------------------------------------------- */
#include "mx_led_pwm.h"

static led_pwm_t  led_pwm_0_obj;

/**
  * @brief initialize the IO layer.
  * @param  pio      pointer to the part IO structure.
  * @retval error status.
  */
int32_t led_pwm_io_init(led_pwm_t *pio)
{
  /* Part Status */
  int32_t ret = 0;

  switch (pio->id)
  {
case LED_PWM_0:
      pio->htim         = LED_PWM_0_TIM_GETHANDLE();
      pio->pwm_channel  = LED_PWM_0_TIM_CHANNEL;
      break;



    default:
      /* Error -- Unknown ID */
      ret = -1;
      break;
  }

  return ret;
}

/**
  * @brief Function to retrieve the Part object LED_PWM_0.
  * @retval part object.
  */
led_pwm_t *mx_led_pwm_0_getobject(void)
{
  return &led_pwm_0_obj;
}



