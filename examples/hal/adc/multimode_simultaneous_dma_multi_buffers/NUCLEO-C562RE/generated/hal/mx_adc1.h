/**
  ******************************************************************************
  * @file           : mx_adc1.h
  * @brief          : Header for mx_adc1.c file.
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
#ifndef STM32_ADC1_H
#define STM32_ADC1_H

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
/* Exported functions for ADC in HAL layer */
/******************************************************************************/
/**
  * @brief mx_adc1_cfg1_hal init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
hal_adc_handle_t *mx_adc1_cfg1_hal_init(void);

/**
  * @brief  De-initialize adc1 instance and return it.
  */
void mx_adc1_cfg1_hal_deinit(void);

/**
  * @brief  Get the ADC1 object.
  * @retval Pointer on the ADC1 Handle
  */
hal_adc_handle_t *mx_adc1_hal_adc_gethandle(void);


/******************************************************************************/
/*    ADC1 global interrupt is managed directly in user code.   */
/******************************************************************************/
void ADC1_IRQHandler(void);

void LPDMA1_CH0_IRQHandler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32_ADC1_H */
