/**
  ******************************************************************************
  * file           : ll_example.h
  * brief          : Header for stm32ynxx_ll_example.c files.
  *                  This file contains example-specific declarations to interface with example.c functions.
  ******************************************************************************
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LL_EXAMPLE_H
#define LL_EXAMPLE_H

/* Includes ------------------------------------------------------------------*/
#include "mx_hal_def.h"       /* aliases to the target-specific generated code */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** brief: Disable RCC bus clocks.
  */
void RCC_DisableBuses(void);

/** brief: Enable the EXTI Line interrupt.
  */
void EXTI_EnableLineMode_IT(void);

/** brief: Clear Stop flag.
  */
void PWR_ClearStopFlag(void);

/** brief: Enter the Stop0 mode request.
  */
void PWR_EnterStop0Mode(void);

/** brief:  Enable EXTI in interrupt mode using LL PWR APIs.
  * retval: system status (see system_status_t)
  */
system_status_t PWR_CheckSystemStop0Mode(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_EXAMPLE_H */
