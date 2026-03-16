/**
  ******************************************************************************
  * @file           : mx_tim6.h
  * @brief          : Header for mx_tim6.c file.
  ******************************************************************************
  * @attention
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
#ifndef STM32_TIM6_H
#define STM32_TIM6_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/******************************************************************************/
/* Exported functions for TIM in HAL layer */
/******************************************************************************/
/**
  * @brief  mx_tim6 init function.
  *         This function configures the hardware resources used in this example.
  * @retval Pointer to handle
  * @retval NULL in case of failure
  */
hal_tim_handle_t *mx_tim6_init(void);

/**
  * @brief  De-initialize mx_tim6 instance and return it.
  */
void mx_tim6_deinit(void);

/**
  * @brief  Get the mx_tim6 object.
  * @return Pointer on the mx_tim6 Handle
  */
hal_tim_handle_t *mx_tim6_gethandle(void);

/******************************************************************************/
/*                     TIM6 global interrupt                    */
/******************************************************************************/
void TIM6_IRQHandler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32_TIM6_H */