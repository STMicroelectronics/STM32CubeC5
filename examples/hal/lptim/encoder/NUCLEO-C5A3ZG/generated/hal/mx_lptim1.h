/**
  ******************************************************************************
  * @file           : mx_lptim1.h
  * @brief          : Header for mx_lptim1.c file.
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
#ifndef MX_LPTIM1_H
#define MX_LPTIM1_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** Primary aliases for LPTIM1_IN1 pin */
#define PA1_PORT                              HAL_GPIOA
#define PA1_PIN                               HAL_GPIO_PIN_1

/** Primary aliases for LPTIM1_IN2 pin */
#define PD11_PORT                             HAL_GPIOD
#define PD11_PIN                              HAL_GPIO_PIN_11
/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/******************************************************************************/
/* Exported functions for SW instance in HAL layer */
/******************************************************************************/
/**
  * @brief mx_lptim1 init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
hal_lptim_handle_t *mx_lptim1_init(void);

/**
  * @brief  De-initialize mx_lptim1 instance and return it.
  */
void mx_lptim1_deinit(void);

/**
  * @brief  Get the mx_lptim1 object.
  * @retval Pointer on the mx_lptim1 Handle
  */
hal_lptim_handle_t *mx_lptim1_gethandle(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_LPTIM1_H */
