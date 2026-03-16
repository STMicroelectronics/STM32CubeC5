/**
  ******************************************************************************
  * file           : mx_led.h
  * brief          : Exporting led part resources
  ******************************************************************************
  * attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_LED_H
#define MX_LED_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "led.h"
#include "mx_hal_def.h"

/* Exported macros -----------------------------------------------------------*/
/* Configuration macros for GPIO LED: MX_STATUS_LED */
#define MX_STATUS_LED                  MX_STATUS_LED /* Input parameter to the LED  API */
#define MX_STATUS_LED_GPIO_PORT        HAL_GPIOA
#define MX_STATUS_LED_PIN              HAL_GPIO_PIN_5
#define MX_STATUS_LED_ACTIVE_STATE     HAL_GPIO_PIN_SET
#define MX_STATUS_LED_INACTIVE_STATE   HAL_GPIO_PIN_RESET
#define MX_STATUS_LED_INIT_STATE       HAL_GPIO_PIN_RESET

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_LED_H */
