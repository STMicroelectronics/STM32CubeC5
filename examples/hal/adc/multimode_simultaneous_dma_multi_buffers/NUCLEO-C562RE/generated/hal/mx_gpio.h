/**
  ******************************************************************************
  * file           : mx_gpio.h
  * brief          : Header for mx_gpio.c file.
  ******************************************************************************
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
#ifndef STM32_GPIO_H
#define STM32_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define MX_STATUS_LED                  MX_STATUS_LED /* Input parameter to the LED  API */
#define MX_STATUS_LED_GPIO_PORT        HAL_GPIOA
#define MX_STATUS_LED_PIN              HAL_GPIO_PIN_5
#define MX_STATUS_LED_ACTIVE_STATE     HAL_GPIO_PIN_SET
#define MX_STATUS_LED_INACTIVE_STATE   HAL_GPIO_PIN_RESET
#define MX_STATUS_LED_INIT_STATE       HAL_GPIO_PIN_RESET
/* Private define ------------------------------------------------------------*/

int32_t gpio_cfg1_init(void);
int32_t gpio_cfg2_init(void);
int32_t gpio_cfg1_deinit(void);
int32_t gpio_cfg2_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* STM32_GPIO_H */
