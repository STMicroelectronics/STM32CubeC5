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
static hal_tim_handle_t hTIM6;

/* Exported function definition ----------------------------------------------*/

/******************************************************************************/
/* Exported functions for TIM6 in HAL layer (SW instance MyTIM_1) */
/******************************************************************************/
hal_tim_handle_t *mx_tim6_init(void)
{
  if (HAL_TIM_Init(&hTIM6, HAL_TIM6) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_TIM6_EnableClock();

  /* No GPIO configuration required for TIM6 */
  /* Enable the Timer global interrupt */
  HAL_CORTEX_NVIC_SetPriority(TIM6_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_15, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(TIM6_IRQn);

  return &hTIM6;
}

void mx_tim6_deinit(void)
{
  (void)HAL_TIM_DeInit(&hTIM6);

  HAL_RCC_TIM6_DisableClock();

  HAL_RCC_TIM6_Reset();

  /* No GPIO de-initialization required for TIM6 */
  /* Disable Timer global interrupt */
  HAL_CORTEX_NVIC_DisableIRQ(TIM6_IRQn);
}

hal_tim_handle_t *mx_tim6_gethandle(void)
{
  return &hTIM6;
}

/******************************************************************************/
/*                           TIM6 global interrupt                            */
/******************************************************************************/
void TIM6_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&hTIM6);
}
