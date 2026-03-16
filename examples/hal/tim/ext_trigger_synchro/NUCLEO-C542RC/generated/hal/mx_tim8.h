/**
  ******************************************************************************
  * @file           : mx_tim8.h
  * @brief          : Header for mx_tim8.c file.
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
#ifndef MX_TIM8_H
#define MX_TIM8_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** Primary aliases for TIM8_CH1 pin */
#define PB10_PORT                             HAL_GPIOB
#define PB10_PIN                              HAL_GPIO_PIN_10
/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/******************************************************************************/
/* Exported functions for TIM in HAL layer */
/******************************************************************************/
/**
  * @brief  mx_tim8 init function.
  *         This function configures the hardware resources used in this example.
  * @retval Pointer to handle
  * @retval NULL in case of failure
  */
hal_tim_handle_t *mx_tim8_init(void);

/**
  * @brief  De-initialize mx_tim8 instance and return it.
  */
void mx_tim8_deinit(void);

/**
  * @brief  Get the mx_tim8 object.
  * @return Pointer on the mx_tim8 Handle
  */
hal_tim_handle_t *mx_tim8_gethandle(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_TIM8_H */
