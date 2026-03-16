/**
  ******************************************************************************
  * @file           : mx_gpio_default.h
  * @brief          : Header for mx_gpio_default.c file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_stm32c5xx_hal_drivers_license.md file
  * in the same directory as the generated code.
  * If no mx_stm32c5xx_hal_drivers_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_GPIO_DEFAULT_H
#define MX_GPIO_DEFAULT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"
#include "mx_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/******************************************************************************/
/* Exported defines for gpio_default in HAL layer (SW instance MyGPIO_1) */
/******************************************************************************/

/* Primary aliases for GPIO PA5 pin */
#define PA5_PORT                                        HAL_GPIOA
#define PA5_PIN                                         HAL_GPIO_PIN_5
#define PA5_INIT_STATE                                  HAL_GPIO_PIN_RESET
#define PA5_ACTIVE_STATE                                HAL_GPIO_PIN_SET
#define PA5_INACTIVE_STATE                              HAL_GPIO_PIN_RESET

/* Secondary aliases for GPIO PA5 pin */
#define MX_EXAMPLE_GPIO_PORT                            HAL_GPIOA
#define MX_EXAMPLE_GPIO_PIN                             HAL_GPIO_PIN_5
#define MX_EXAMPLE_GPIO_INIT_STATE                      HAL_GPIO_PIN_RESET
#define MX_EXAMPLE_GPIO_ACTIVE_STATE                    HAL_GPIO_PIN_SET
#define MX_EXAMPLE_GPIO_INACTIVE_STATE                  HAL_GPIO_PIN_RESET

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for gpio_default in HAL layer (SW instance MyGPIO_1) */
/******************************************************************************/
/**
  * @brief mx_gpio_default init function
  * This function configures the hardware resources used in this example
  * @retval 0  GPIO group correctly initialized
  * @retval -1 Issue detected during GPIO group initialization
  */
system_status_t mx_gpio_default_init(void);

/**
  * @brief  De-initialize gpio_default instance.
  */
system_status_t mx_gpio_default_deinit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_GPIO_DEFAULT_H */
