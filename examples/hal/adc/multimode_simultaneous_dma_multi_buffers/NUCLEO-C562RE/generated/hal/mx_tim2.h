/**
  ******************************************************************************
  * file           : mx_tim2.h
  * brief          : Header for mx_tim1.c file.
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
#ifndef STM32_TIM1_H
#define STM32_TIM1_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define PRESCALER_VALUE   8U

#define PERIOD_VALUE      2666U

/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/**
  * @brief tim1 init function
  * This function configures the hardware resources used in this example
  * @param None
  * @retval -1 in case of failure
  */
void *tim2_cfg1_init(void);


/**
  * @brief  Get the TIM1 object.
  * @param None
  * @retval Pointer on the TIM1 Handle
  */
void *tim2_gethandle(void);


/**
  * @brief Timer peripheral deinitialization function
  * @param None
  * @retval -1 in case of failure
  */
void tim2_deinit(void);


#ifdef __cplusplus
}
#endif

#endif /* STM32_TIM1_H */
