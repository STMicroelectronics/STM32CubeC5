/**
  ******************************************************************************
  * @file    stm32_hal_timebase.h
  * @brief   Include file used for stm32_hal_timebase.c
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
#ifndef STM32_HAL_TIMEBASE_H
#define STM32_HAL_TIMEBASE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mx_.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/******************************************************************************/
/*   TIMEBASE ALIASES   (private aliases)                                     */
/******************************************************************************/
#define timebase_gethandle   mx__gethandle

#define timebase_init        mx__init
#define timebase_deinit      mx__deinit

/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32_HAL_TIMEBASE_H */
