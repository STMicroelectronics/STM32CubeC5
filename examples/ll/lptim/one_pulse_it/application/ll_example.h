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

/** brief: Start the MX_LPTIMx compare match channel in interrupt mode.
  */
void LPTIM_StartCompareMatchChannel_IT(void);

/** brief:  Start the MX_LPTIMx counter in interrupt mode.
  */
void LPTIM_Start_IT(void);

/** brief:  Get the MX_LPTIMx current value of the repetition counter.
  */
uint32_t LPTIM_GetRepetitionCounter(void);

/** brief:  MX_LPTIMx compare match callback.
  */
void LPTIM_CompareMatchCallback(void);

/** brief:  MX_LPTIMx update callback.
  */
void LPTIM_UpdateCallback(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_EXAMPLE_H */
