/**
  ******************************************************************************
  * @file           : mx_tim1.h
  * @brief          : Header for mx_tim1.c file.
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
#ifndef MX_TIM1_H
#define MX_TIM1_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_ll.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define USE_TIM1

/* Aliases of channels labels */
#define MX_TIM_X_CHANNEL  LL_TIM_CHANNEL_CH1

/** Primary aliases for TIM1_CH2 pin */
#define PA9_PORT                              GPIOA
#define PA9_PIN                               LL_GPIO_PIN_9

/** Primary aliases for TIM1_CH1 pin */
#define PA8_PORT                              GPIOA
#define PA8_PIN                               LL_GPIO_PIN_8
/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/******************************************************************************/
/* Exported functions for TIM in LL layer */
/******************************************************************************/
/**
  * @brief  mx_tim1 init function.
  *         This function configures the hardware resources used in this example.
  * @retval Pointer to handle
  * @retval NULL in case of failure
  */
TIM_TypeDef *mx_tim1_init(void);

/**
  * @brief  De-initialize mx_tim1 instance and return it.
  */
void mx_tim1_deinit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_TIM1_H */
