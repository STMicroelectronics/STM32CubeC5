/**
  ******************************************************************************
  * @file           : mx_lpuart1.h
  * @brief          : Header for mx_lpuart1.c file.
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
#ifndef MX_LPUART1_H
#define MX_LPUART1_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_ll.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USE_LPUART1

/** Primary aliases for LPUART1_RX pin */
#define PB7_PORT                              GPIOB
#define PB7_PIN                               LL_GPIO_PIN_7

/** Primary aliases for LPUART1_TX pin */
#define PB6_PORT                              GPIOB
#define PB6_PIN                               LL_GPIO_PIN_6

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/******************************************************************************/
/* Exported functions for UART in LL layer */
/******************************************************************************/
/**
  * @brief mx_lpuart1_uart init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
USART_TypeDef *mx_lpuart1_uart_init(void);

/**
  * @brief  De-initialize mx_lpuart1_uart instance and return it.
  * @retval None
  */
void mx_lpuart1_uart_deinit(void);

/******************************************************************************/
/*                LPUART1 Rx interrupt OR LPUART1 Tx interrupt                */
/*                     is managed directly in user code.                      */
/******************************************************************************/

/* void LPUART1_IRQHandler(void); */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_LPUART1_H */
