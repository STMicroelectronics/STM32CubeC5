/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : TIM capturing external signals in interrupt mode with the LL API.
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
void mx_timx_cc_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/**
  *  Retrieves the MX_TIMx peripheral clock frequency.
  *  retval: Kernel clock frequency.
  */
inline uint32_t RCC_TIM_GetKernelClkFreq(void)
{
  /* PSI output frequency exactly 144 MHz without LSE as PSI clock source  */
  return (144000000U);
}


/**
  *  Start the MX_TIMx output channel.
  */
inline void TIM_StartInputCaptureChannel_IT(void)
{
  LL_TIM_EnableIT(MX_TIMx, LL_TIM_DIER_CC1IE);

  LL_TIM_CC_EnableChannel(MX_TIMx, MX_TIM_X_CHANNEL);
}


/**
  *  Start the MX_TIMx.
  */
inline void TIM_Start(void)
{
  LL_TIM_EnableCounter(MX_TIMx);
}


/**
  *  Gets the MX_TIMx peripheral max counter value.
  *  retval: Max counter value.
  */
uint32_t TIM_GetCounterMaxValue(void)
{
  uint32_t max_val = 0xFFFFU;

  if (IS_TIM_32B_COUNTER_INSTANCE(MX_TIMX))
  {
    max_val = 0xFFFFFFFFU;
  }

  return max_val;
}


/**
  *  Read the captured value for an MX_TIMx input channel.
  */
inline uint32_t TIM_ReadChannelCapturedValue(void)
{
  return LL_TIM_IC_GetCapturedValue(MX_TIMx, MX_TIM_X_CHANNEL);
}


/**
  *  Handles the MX_TIMx interrupt handler.
  */
void mx_timx_cc_irqhandler(void)
{
  const uint32_t flag_status        = LL_TIM_READ_REG(MX_TIMx, SR);
  const uint32_t flag_status_masked = flag_status & LL_TIM_READ_REG(MX_TIMx, DIER);

  if ((flag_status_masked & LL_TIM_SR_CC1IF) != 0U)
  {
    LL_TIM_ClearFlag_CC1(MX_TIMx);

    TIM_InputCaptureCallback();
  }
} /* mx_timx_cc_irqhandler() */
