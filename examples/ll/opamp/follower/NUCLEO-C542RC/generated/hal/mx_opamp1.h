/**
  ******************************************************************************
  * @file           : mx_opamp1.h
  * @brief          : Header for mx_opamp1.c file.
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
#ifndef MX_OPAMP1_H
#define MX_OPAMP1_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_ll.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USE_OPAMP1

/** Primary aliases for OPAMP1_VINP0 pin */
#define PA1_PORT                              GPIOA
#define PA1_PIN                               LL_GPIO_PIN_1

/** Primary aliases for OPAMP1_VOUT pin */
#define PA6_PORT                              GPIOA
#define PA6_PIN                               LL_GPIO_PIN_6

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for OPAMP in LL layer */
/******************************************************************************/
/**
  * @brief mx_opamp1 init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */

OPAMP_TypeDef *mx_opamp1_init(void);

/**
  * @brief  De-initialize opamp1 instance and return it.
  */
void mx_opamp1_deinit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_OPAMP1_H */
