/**
  ******************************************************************************
  * file           : mx_led_pwm.h
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

/* Define to prevent recursive inclusion ----------------------------------- */
#ifndef MX_LED_PWM_H
#define MX_LED_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ---------------------------------------------------------------- */
#include "led_pwm.h"
#include "mx_hal_def.h"

/* Exported macros --------------------------------------------------------- */
/* Part Config ID */
#define LED_PWM_0                              0 /* Main label */

/* User-defined aliases for the part drivers instances IDs and getter functions */
#define MX_STATUS_LED                          LED_PWM_0
#define mx_status_led_getobject()    mx_led_pwm_0_getobject()

/* Resource bindings */
#define LED_PWM_0_TIM_GETHANDLE    mx_tim2_gethandle
#define LED_PWM_0_TIM_CHANNEL      HAL_TIM_CHANNEL_1







/* Exported functions ------------------------------------------------------ */
led_pwm_t *mx_led_pwm_0_getobject(void);



#ifdef __cplusplus
}
#endif

#endif /* MX_LED_PWM_H */
