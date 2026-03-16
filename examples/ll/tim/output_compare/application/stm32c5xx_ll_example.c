/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : TIM output compare toggle mode with LL API
  ******************************************************************************
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

/* Includes ------------------------------------------------------------------*/
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  *  Start the MX_TIMx output compare toggle channel.
  *  param:  channel Specifies the MX_TIMx channel.
  */
inline void TIM_StartOutputCompareToggleChannel(uint32_t channel)
{
  LL_TIM_CC_EnableChannel(MX_TIMx, channel);

  LL_TIM_EnableAllOutputs(MX_TIMx);
}

/**
  *  Start the MX_TIMx counter.
  */
inline void TIM_Start(void)
{
  LL_TIM_EnableCounter(MX_TIMx);
}
