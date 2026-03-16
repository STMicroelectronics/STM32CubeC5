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

/** brief:  Retrieves the MX_TIMx peripheral clock frequency.
  * retval: Kernel clock frequency.
  */
uint32_t RCC_TIM_GetKernelClkFreq(void);

/** brief: Start the MX_TIMx channel input capture in interrupt mode.
  */
void TIM_StartInputCaptureChannel_IT(void);

/** brief: Start the MX_TIMx in polling mode.
  */
void TIM_Start(void);

/** brief:  Gets the MX_TIMx peripheral max counter value.
  * retval: Max counter value.
  */
uint32_t TIM_GetCounterMaxValue(void);

/** brief: Read the captured value for an MX_TIMx input channel.
  */
uint32_t TIM_ReadChannelCapturedValue(void);

/** brief:  MX_TIMx input capture callback.
  */
void TIM_InputCaptureCallback(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_EXAMPLE_H */
