/**
  ******************************************************************************
  * @file           : mx_eth1.h
  * @brief          : Header for mx_eth1.c file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_ETH1_H
#define MX_ETH1_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/**
  * brief eth init function
  * This function configures the hardware resources used in this example
  * retval pointer to handle or NULL in case of failure
  */
void *mx_eth1_hal_init(void);

/**
  * brief  De-initialize eth instance and return it.
  * retval None
  */
void mx_eth1_hal_deinit(void);

/**
  * @brief  Get the ETH1 object.
  * @retval Pointer on the ETH1 Handle
  */
void *mx_eth1_hal_gethandle(void);

/**
  * brief ETH IRQ handler
  * retval none
  */
void ETH1_IRQHandler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_ETH1_H */