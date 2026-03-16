/**
  ******************************************************************************
  * @file           : mx_usart2.h
  * @brief          : Header for mx_usart2.c file.
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
#ifndef MX_USART2_H
#define MX_USART2_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_ll.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USE_USART2

/** Primary aliases for USART2_CK pin */
#define PC12_PORT                             GPIOC
#define PC12_PIN                              LL_GPIO_PIN_12

/** Primary aliases for USART2_RX pin */
#define PC11_PORT                             GPIOC
#define PC11_PIN                              LL_GPIO_PIN_11

/** Primary aliases for USART2_TX pin */
#define PC10_PORT                             GPIOC
#define PC10_PIN                              LL_GPIO_PIN_10

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/******************************************************************************/
/* Exported functions for USART in LL layer */
/******************************************************************************/
/**
  * @brief mx_usart2_usart init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
USART_TypeDef *mx_usart2_usart_init(void);

/**
  * @brief  De-initialize mx_usart2_usart instance and return it.
  * @retval None
  */
void mx_usart2_usart_deinit(void);

/******************************************************************************/
/*         USART2 global interrupt is managed directly in user code.          */
/******************************************************************************/

/* void USART2_IRQHandler(void); */

/******************************************************************************/
/*     LPDMA1 channel0 global interrupt is managed directly in user code.     */
/******************************************************************************/

/* void LPDMA1_CH0_IRQHandler(void); */

/******************************************************************************/
/*     LPDMA1 channel1 global interrupt is managed directly in user code.     */
/******************************************************************************/

/* void LPDMA1_CH1_IRQHandler(void); */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_USART2_H */
