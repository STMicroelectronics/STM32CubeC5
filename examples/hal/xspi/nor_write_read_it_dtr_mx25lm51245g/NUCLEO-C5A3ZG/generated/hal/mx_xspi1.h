/**
  ******************************************************************************
  * @file           : mx_xspi1.h
  * @brief          : Header for mx_xspi1.c file.
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
#ifndef MX_XSPI1_H
#define MX_XSPI1_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** Primary aliases for XSPI1_CLK pin */
#define PF10_PORT                             HAL_GPIOF
#define PF10_PIN                              HAL_GPIO_PIN_10

/** Primary aliases for XSPI1_DQS pin */
#define PB2_PORT                              HAL_GPIOB
#define PB2_PIN                               HAL_GPIO_PIN_2

/** Primary aliases for XSPI1_IO0 pin */
#define PF9_PORT                              HAL_GPIOF
#define PF9_PIN                               HAL_GPIO_PIN_9

/** Primary aliases for XSPI1_IO1 pin */
#define PF8_PORT                              HAL_GPIOF
#define PF8_PIN                               HAL_GPIO_PIN_8

/** Primary aliases for XSPI1_IO2 pin */
#define PF7_PORT                              HAL_GPIOF
#define PF7_PIN                               HAL_GPIO_PIN_7

/** Primary aliases for XSPI1_IO3 pin */
#define PF6_PORT                              HAL_GPIOF
#define PF6_PIN                               HAL_GPIO_PIN_6

/** Primary aliases for XSPI1_IO4 pin */
#define PE7_PORT                              HAL_GPIOE
#define PE7_PIN                               HAL_GPIO_PIN_7

/** Primary aliases for XSPI1_IO5 pin */
#define PE8_PORT                              HAL_GPIOE
#define PE8_PIN                               HAL_GPIO_PIN_8

/** Primary aliases for XSPI1_IO6 pin */
#define PE9_PORT                              HAL_GPIOE
#define PE9_PIN                               HAL_GPIO_PIN_9

/** Primary aliases for XSPI1_IO7 pin */
#define PE10_PORT                             HAL_GPIOE
#define PE10_PIN                              HAL_GPIO_PIN_10

/** Primary aliases for XSPI1_NCLK pin */
#define PB12_PORT                             HAL_GPIOB
#define PB12_PIN                              HAL_GPIO_PIN_12

/** Primary aliases for XSPI1_NCS1 pin */
#define PE11_PORT                             HAL_GPIOE
#define PE11_PIN                              HAL_GPIO_PIN_11

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for XSPI in HAL layer */
/******************************************************************************/
/**
  * @brief mx_xspi1 init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
hal_xspi_handle_t *mx_xspi1_init(void);

/**
  * @brief  De-initialize xspi1 instance and return it.
  */
void mx_xspi1_deinit(void);

/**
  * @brief  Get the XSPI1 object.
  * @retval Pointer on the XSPI1 Handle
  */
hal_xspi_handle_t *mx_xspi1_gethandle(void);

/******************************************************************************/
/*                         OCTOSPI1 global interrupt                          */
/******************************************************************************/

void XSPI1_IRQHandler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_XSPI1_H */
