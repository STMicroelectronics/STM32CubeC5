/**
  ******************************************************************************
  * @file    stm32_hal_timebase.h
  * @author  GPAM Application Team
  * @brief   Include file used for stm32_hal_timebase.c
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
#ifndef STM32_HAL_TIMEBASE_H
#define STM32_HAL_TIMEBASE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mx_tim6.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/******************************************************************************/
/*   TIMEBASE ALIASES   (private aliases)                                     */
/******************************************************************************/
#define timebase_gethandle   mx_tim6_gethandle

#define timebase_init        mx_tim6_init
#define timebase_deinit      mx_tim6_deinit

#define TIMEBASE_IRQ         TIM6_IRQn

/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32_HAL_TIMEBASE_H */
