/**
  ******************************************************************************
  * @file           : mx_fdcan1.h
  * @brief          : Header for mx_fdcan1.c file.
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
#ifndef MX_FDCAN1_H
#define MX_FDCAN1_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** Primary aliases for FDCAN1_TX pin */
#define PB9_PORT                              HAL_GPIOB
#define PB9_PIN                               HAL_GPIO_PIN_9

/** Primary aliases for FDCAN1_RX pin */
#define PB8_PORT                              HAL_GPIOB
#define PB8_PIN                               HAL_GPIO_PIN_8

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for FDCAN in HAL layer */
/******************************************************************************/
/**
  * @brief mx_fdcan1 init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
hal_fdcan_handle_t *mx_fdcan1_init(void);

/**
  * @brief  De-initialize fdcan1 instance and return it.
  */
void mx_fdcan1_deinit(void);

/**
  * @brief  Get the FDCAN1 object.
  * @retval Pointer on the FDCAN1 Handle
  */
hal_fdcan_handle_t *mx_fdcan1_gethandle(void);

/******************************************************************************/
/*                             FDCAN1 Interrupt 0                             */
/******************************************************************************/

void FDCAN1_IT0_IRQHandler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_FDCAN1_H */
