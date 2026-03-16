/**
  ******************************************************************************
  * @file           : mx_tim6.c
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
#include "mx_tim6.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/* Exported function definition ----------------------------------------------*/

/******************************************************************************/
/* Exported functions for TIM6 in LL layer (SW instance MyTIM_1) */
/******************************************************************************/
TIM_TypeDef *mx_tim6_init(void)
{
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);

  /* Timer configuration to reach the output frequency at 60000 Hz */
  /* LL_TIM_SetPrescaler(TIM6, 0); */ /* Configuration matches register reset state at startup. */
  /* LL_TIM_SetCounterMode(TIM6, LL_TIM_COUNTERMODE_UP); */ /* Configuration matches register reset state at startup. */
  LL_TIM_SetAutoReload(TIM6, 2399);
  /* LL_TIM_SetClockSource(TIM6, LL_TIM_CLK_INTERNAL); */ /* Configuration matches register reset state at startup. */

  /* Update Event Management */
  /* LL_TIM_SetUpdateSource(TIM6, LL_TIM_UPDATESOURCE_REGULAR); */ /* Configuration matches register reset state at startup. */
  /* LL_TIM_EnableUpdateEvent(TIM6); */ /* Configuration matches register reset state at startup. */

  /* Master Mode Configuration */
  LL_TIM_SetTriggerOutput(TIM6, LL_TIM_TRGO_UPDATE);

  return TIM6;
}

void mx_tim6_deinit(void)
{
  LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM6);

  LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM6);
  LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_TIM6);
}
