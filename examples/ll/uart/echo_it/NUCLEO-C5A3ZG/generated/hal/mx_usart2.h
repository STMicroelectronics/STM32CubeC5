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

/** Primary aliases for USART2_RX pin */
#define DBGIN_VCP_RX_PORT                     GPIOA
#define DBGIN_VCP_RX_PIN                      LL_GPIO_PIN_3

/** Primary aliases for USART2_TX pin */
#define DBGIN_VCP_TX_PORT                     GPIOA
#define DBGIN_VCP_TX_PIN                      LL_GPIO_PIN_2

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/******************************************************************************/
/* Exported functions for UART in LL layer */
/******************************************************************************/
/**
  * @brief mx_usart2_uart init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
USART_TypeDef *mx_usart2_uart_init(void);

/**
  * @brief  De-initialize mx_usart2_uart instance and return it.
  * @retval None
  */
void mx_usart2_uart_deinit(void);

/******************************************************************************/
/*         USART2 global interrupt is managed directly in user code.          */
/******************************************************************************/

/* void USART2_IRQHandler(void); */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_USART2_H */
