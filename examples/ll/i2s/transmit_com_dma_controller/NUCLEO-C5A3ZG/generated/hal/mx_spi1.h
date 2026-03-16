/**
  ******************************************************************************
  * @file           : mx_spi1.h
  * @brief          : Header for mx_spi1.c file.
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
#ifndef MX_SPI1_H
#define MX_SPI1_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_ll.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USE_SPI1

/** Primary aliases for I2S1_CK pin */
#define PA5_PORT                              GPIOA
#define PA5_PIN                               LL_GPIO_PIN_5

/** Primary aliases for I2S1_WS pin */
#define NETR53_2_PORT                         GPIOA
#define NETR53_2_PIN                          LL_GPIO_PIN_15

/** Primary aliases for I2S1_SDI pin */
#define PA6_PORT                              GPIOA
#define PA6_PIN                               LL_GPIO_PIN_6

/** Primary aliases for I2S1_SDO pin */
#define PA7_PORT                              GPIOA
#define PA7_PIN                               LL_GPIO_PIN_7
/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for I2S in LL layer */
/******************************************************************************/
/**
  * @brief mx_spi1_i2s init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
SPI_TypeDef *mx_spi1_i2s_init(void);

/**
  * @brief  De-initialize spi1 instance and return it.
  */
void mx_spi1_i2s_deinit(void);

/******************************************************************************/
/*          SPI1 global interrupt is managed directly in user code.           */
/******************************************************************************/

/* void SPI1_IRQHandler(void); */

/******************************************************************************/
/*     LPDMA1 channel0 global interrupt is managed directly in user code.     */
/******************************************************************************/

/* void LPDMA1_CH0_IRQHandler(void); */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_SPI1_H */
