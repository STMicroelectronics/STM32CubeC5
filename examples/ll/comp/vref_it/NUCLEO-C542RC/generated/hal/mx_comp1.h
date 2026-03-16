/**
  ******************************************************************************
  * @file           : mx_comp1.h
  * @brief          : Header for mx_comp1.c file.
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
#ifndef MX_COMP1_H
#define MX_COMP1_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_ll.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USE_COMP1

/** Primary aliases for COMP1_INP2 pin */
#define PB0_PORT                              GPIOB
#define PB0_PIN                               LL_GPIO_PIN_0

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for COMP in LL layer */
/******************************************************************************/
/**
  * @brief mx_comp1 init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
COMP_TypeDef *mx_comp1_init(void);

/**
  * @brief  De-initialize comp1 instance and return it.
  */
void mx_comp1_deinit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_COMP1_H */
