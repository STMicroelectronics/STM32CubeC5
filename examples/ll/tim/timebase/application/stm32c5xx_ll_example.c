/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : TIM timebase with LL API
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
void mx_timx_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/**
  *  Start the MX_TIMx in interrupt mode.
  */
inline void TIM_Start_IT(void)
{
  LL_TIM_EnableIT(MX_TIMx, LL_TIM_DIER_UIE);

  LL_TIM_EnableCounter(MX_TIMx);
} /* TIM_Start_IT */


/**
  *  Toggle GPIO pin.
  */
inline void GPIO_TogglePin(void)
{
  LL_GPIO_TogglePin(MX_STATUS_LED_PORT, MX_STATUS_LED_PIN);
}


/**
  *  Handles the MX_TIMx interrupts.
  */
void mx_timx_irqhandler(void)
{
  const uint32_t flag_status        = LL_TIM_READ_REG(MX_TIMx, SR);
  const uint32_t flag_status_masked = flag_status & LL_TIM_READ_REG(MX_TIMx, DIER);

  /* Update interrupt flag */
  if ((flag_status_masked & LL_TIM_SR_UIF) != 0UL)
  {
    LL_TIM_ClearFlag_UPDATE(MX_TIMx);

    TimebaseCallback();
  }
} /* mx_timx_irqhandler() */
