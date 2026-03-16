/**
  ******************************************************************************
  * @file           : mx_usart3.h
  * @brief          : Header for mx_usart3.c file.
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
#ifndef MX_USART3_H
#define MX_USART3_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** Primary aliases for USART3_CK pin */
#define PC12_PORT                             HAL_GPIOC
#define PC12_PIN                              HAL_GPIO_PIN_12

/** Primary aliases for USART3_RX pin */
#define PC11_PORT                             HAL_GPIOC
#define PC11_PIN                              HAL_GPIO_PIN_11

/** Primary aliases for USART3_TX pin */
#define PC10_PORT                             HAL_GPIOC
#define PC10_PIN                              HAL_GPIO_PIN_10

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/******************************************************************************/
/* Exported functions for USART in HAL layer */
/******************************************************************************/
/**
  * @brief mx_usart3_usart init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
hal_usart_handle_t *mx_usart3_usart_init(void);

/**
  * @brief  De-initialize mx_usart3_usart instance and return it.
  * @retval None
  */
void mx_usart3_usart_deinit(void);

/**
  * @brief  Get the USART3_usart object.
  * @retval Pointer on the USART3 Handle
  */
hal_usart_handle_t *mx_usart3_usart_gethandle(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_USART3_H */
