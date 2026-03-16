/**
  ******************************************************************************
  * @file           : mx_dac1.h
  * @brief          : Header for mx_dac1.c file.
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
#ifndef MX_DAC1_H
#define MX_DAC1_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_ll.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USE_DAC1

/* Aliases of channels labels */
#define MX_DAC_CHANNEL LL_DAC_CHANNEL_1

/** Primary aliases for DAC1_OUT1 pin */
#define PA4_PORT                              GPIOA
#define PA4_PIN                               LL_GPIO_PIN_4

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for DAC in LL layer */
/******************************************************************************/
/**
  * @brief mx_dac1 init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
DAC_TypeDef *mx_dac1_init(void);

/**
  * @brief  De-initialize dac1 instance and return it.
  */
void mx_dac1_deinit(void);

/**
  * @brief  Get pointer to DAC1_CH1 CH1 DMA linked-list registers array.
  * @return Pointer to array holding LL DMA node register values for DAC1_CH1 CH1
  */
uint32_t *mx_dac1_ch1_dma_get_lli_reg(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_DAC1_H */
