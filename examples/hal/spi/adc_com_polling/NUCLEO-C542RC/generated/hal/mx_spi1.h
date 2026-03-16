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
#include "stm32_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** Primary aliases for SPI1_SCK pin */
#define PA5_PORT                              HAL_GPIOA
#define PA5_PIN                               HAL_GPIO_PIN_5

/** Primary aliases for SPI1_MISO pin */
#define PA6_PORT                              HAL_GPIOA
#define PA6_PIN                               HAL_GPIO_PIN_6

/** Primary aliases for SPI1_MOSI pin */
#define PA7_PORT                              HAL_GPIOA
#define PA7_PIN                               HAL_GPIO_PIN_7

/** Primary aliases for SPI1_NSS pin */
#define PA4_PORT                              HAL_GPIOA
#define PA4_PIN                               HAL_GPIO_PIN_4

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for SPI in HAL layer */
/******************************************************************************/
/**
  * @brief mx_spi1 init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
hal_spi_handle_t *mx_spi1_init(void);

/**
  * @brief  De-initialize spi1 instance and return it.
  */
void mx_spi1_deinit(void);

/**
  * @brief  Get the SPI1 object.
  * @retval Pointer on the SPI1 Handle
  */
hal_spi_handle_t *mx_spi1_gethandle(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* MX_SPI1_H */
