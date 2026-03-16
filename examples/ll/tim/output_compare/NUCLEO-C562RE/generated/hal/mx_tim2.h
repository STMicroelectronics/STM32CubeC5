/**
  ******************************************************************************
  * @file           : mx_tim2.h
  * @brief          : Header for mx_tim2.c file.
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
#ifndef MX_TIM2_H
#define MX_TIM2_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_ll.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define USE_TIM2

/* Aliases of channels labels */
#define MX_TIM_1X_CHANNEL  LL_TIM_CHANNEL_CH2

#define MX_TIM_2X_CHANNEL  LL_TIM_CHANNEL_CH3

/** Primary aliases for TIM2_CH2 pin */
#define DBG_SWO_PORT                          GPIOB
#define DBG_SWO_PIN                           LL_GPIO_PIN_3

/** Primary aliases for TIM2_CH3 pin */
#define PB10_PORT                             GPIOB
#define PB10_PIN                              LL_GPIO_PIN_10
/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/******************************************************************************/
/* Exported functions for TIM in LL layer */
/******************************************************************************/
/**
  * @brief  mx_tim2 init function.
  *         This function configures the hardware resources used in this example.
  * @retval Pointer to handle
  * @retval NULL in case of failure
  */
TIM_TypeDef *mx_tim2_init(void);

/**
  * @brief  De-initialize mx_tim2 instance and return it.
  */
void mx_tim2_deinit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_TIM2_H */
