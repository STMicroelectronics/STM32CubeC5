/**
  ******************************************************************************
  * @file           : mx_i2c1.h
  * @brief          : Header for mx_i2c1.c file.
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
#ifndef MX_I2C1_H
#define MX_I2C1_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_ll.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USE_I2C1

/** Primary aliases for I2C1_SCL pin */
#define PB6_PORT                              GPIOB
#define PB6_PIN                               LL_GPIO_PIN_6

/** Primary aliases for I2C1_SDA pin */
#define PB7_PORT                              GPIOB
#define PB7_PIN                               LL_GPIO_PIN_7
/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for I2C in LL layer */
/******************************************************************************/
/**
  * @brief mx_i2c1_i2c init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
I2C_TypeDef *mx_i2c1_i2c_init(void);

/**
  * @brief  De-initialize i2c1 instance and return it.
  */
void mx_i2c1_i2c_deinit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_I2C1_H */
