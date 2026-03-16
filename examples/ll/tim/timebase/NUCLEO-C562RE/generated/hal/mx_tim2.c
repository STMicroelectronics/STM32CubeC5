/**
  ******************************************************************************
  * @file           : mx_tim2.c
  * @brief          : Peripheral initialization
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

/* Includes ------------------------------------------------------------------*/
#include "mx_tim2.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/* Exported function definition ----------------------------------------------*/

/******************************************************************************/
/* Exported functions for TIM2 in LL layer (SW instance MyTIM_1) */
/******************************************************************************/
TIM_TypeDef *mx_tim2_init(void)
{
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  /* Timer configuration to reach the output frequency at 1 Hz */
  LL_TIM_SetPrescaler(TIM2, 14399);
  /* LL_TIM_SetCounterMode(TIM2, LL_TIM_COUNTERMODE_UP); */ /* Configuration matches register reset state at startup. */
  LL_TIM_SetAutoReload(TIM2, 9999);
  /* LL_TIM_SetClockDivision(TIM2, LL_TIM_CLOCKDIVISION_DIV1); */ /* Configuration matches register reset state at startup. */
  /* LL_TIM_SetClockDivision2(TIM2, LL_TIM_CLOCKDIVISION2_DIV1); */ /* Configuration matches register reset state at startup. */
  /* LL_TIM_SetClockSource(TIM2, LL_TIM_CLK_INTERNAL); */ /* Configuration matches register reset state at startup. */

  /* Update Event Management */
  /* LL_TIM_SetUpdateSource(TIM2, LL_TIM_UPDATESOURCE_REGULAR); */ /* Configuration matches register reset state at startup. */
  /* LL_TIM_EnableUpdateEvent(TIM2); */ /* Configuration matches register reset state at startup. */

  /* Master Mode Configuration */

  /* Enable the Timer global interrupt */
  NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(TIM2_IRQn);

  return TIM2;
}

void mx_tim2_deinit(void)
{
  LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM2);

  LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM2);
  LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM2);

  /* Disable Timer global interrupt */
  NVIC_DisableIRQ(TIM2_IRQn);
}

/******************************************************************************/
/*          TIM2 global interrupt is managed directly in user code.           */
/******************************************************************************/
/* void TIM2_IRQHandler(void)
{
}
  */
