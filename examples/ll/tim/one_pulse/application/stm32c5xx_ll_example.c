/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : generate a single pulse when a rising edge of an external signal is received
  *                  on the dedicated TIMER input pin with LL API.
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

/* Includes ------------------------------------------------------------------*/
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  *  Start the MX_TIMx output channel.
  */
inline void TIM_StartChannel(void)
{
  LL_TIM_CC_EnableChannel(MX_TIMx, MX_TIM_X_CHANNEL);

  LL_TIM_EnableAllOutputs(MX_TIMx);
}


/**
  *  Start the MX_TIMx counter.
  */
inline void TIM_Start(void)
{
  LL_TIM_EnableCounter(MX_TIMx);
}
