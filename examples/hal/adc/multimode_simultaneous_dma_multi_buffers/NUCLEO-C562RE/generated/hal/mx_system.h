/**
  ******************************************************************************
  * @file           : mx_system.h
  * @brief          : Exporting peripherals initialization
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
#ifndef STM32_SYSTEM_H
#define STM32_SYSTEM_H

/* Includes ------------------------------------------------------------------*/
#include "mx_hal_def.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/**
  * @brief  System initialization
  * @retval Status
  */
int32_t system_init(void);

void pre_system_init_hook(void);

void post_system_init_hook(void);

void SysTick_Handler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32_SYSTEM_H */
