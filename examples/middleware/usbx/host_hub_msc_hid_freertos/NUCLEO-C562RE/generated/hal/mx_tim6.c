/**
  ******************************************************************************
  * @file           : mx_tim6.c
  * @brief          : Peripheral initialization
  *                   File per peripheral instance that is handling the peripheral
  *                   initialization and the IRQ handlers. Init parameters provided
  *                   as a set of defines configurable/generated for a given target
  *                   through the  hal_tim_settings.json and the config tool.
  ******************************************************************************
  * @attention
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
#include "mx_tim6.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define PRESCALER_VALUE  14399
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/

/* Exported variables by reference--------------------------------------------*/
static hal_tim_handle_t hTIM6;

/* Exported function definition ----------------------------------------------*/
/******************************************************************************/
/* Exported functions for SW instance0 in HAL layer */
/******************************************************************************/

hal_tim_handle_t *mx_tim6_cfg1_hal_init(void)
{
  hal_tim_config_t  config;

  if (HAL_TIM_Init(&hTIM6, HAL_TIM6) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_TIM6_EnableClock();

  /* Timer configuration to reach the output frequency at 144000000 Hz */
  config.prescaler              = PRESCALER_VALUE;
  config.counter_mode           = HAL_TIM_COUNTER_UP;
  config.period                 = 0x9;
  config.repetition_counter     = 0;
  config.clock_sel.clock_source = HAL_TIM_CLK_INTERNAL;
  if (HAL_TIM_SetConfig(&hTIM6, &config) != HAL_OK)
  {
    return NULL;
  }

  /* Enable Timer update interrupt */
  HAL_CORTEX_NVIC_SetPriority(TIM6_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(TIM6_IRQn);

  return &hTIM6;
}

void mx_tim6_cfg1_hal_deinit(void)
{
  /* Disable Timer update interrupt */
  HAL_CORTEX_NVIC_DisableIRQ(TIM6_IRQn);

  (void)HAL_TIM_DeInit(&hTIM6);

  HAL_RCC_TIM6_Reset();

  HAL_RCC_TIM6_DisableClock();

}


hal_tim_handle_t *mx_tim6_cfg2_hal_init(void)
{
  hal_tim_config_t  config;

  if (HAL_TIM_Init(&hTIM6, HAL_TIM6) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_TIM6_EnableClock();

  /* Timer configuration to reach the output frequency at 144000000 Hz */
  config.prescaler              = PRESCALER_VALUE;
  config.counter_mode           = HAL_TIM_COUNTER_UP;
  config.period                 = 0x63;
  config.repetition_counter     = 0;
  config.clock_sel.clock_source = HAL_TIM_CLK_INTERNAL;
  if (HAL_TIM_SetConfig(&hTIM6, &config) != HAL_OK)
  {
    return NULL;
  }

  /* Enable Timer update interrupt */
  HAL_CORTEX_NVIC_SetPriority(TIM6_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(TIM6_IRQn);

  return &hTIM6;
}

void mx_tim6_cfg2_hal_deinit(void)
{
  /* Disable Timer update interrupt */
  HAL_CORTEX_NVIC_DisableIRQ(TIM6_IRQn);

  (void)HAL_TIM_DeInit(&hTIM6);

  HAL_RCC_TIM6_Reset();

  HAL_RCC_TIM6_DisableClock();

}


hal_tim_handle_t *mx_tim6_cfg3_hal_init(void)
{
  hal_tim_config_t  config;

  if (HAL_TIM_Init(&hTIM6, HAL_TIM6) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_TIM6_EnableClock();

  /* Timer configuration to reach the output frequency at 144000000 Hz */
  config.prescaler              = PRESCALER_VALUE;
  config.counter_mode           = HAL_TIM_COUNTER_UP;
  config.period                 = 0x3E7;
  config.repetition_counter     = 0;
  config.clock_sel.clock_source = HAL_TIM_CLK_INTERNAL;
  if (HAL_TIM_SetConfig(&hTIM6, &config) != HAL_OK)
  {
    return NULL;
  }

  /* Enable Timer update interrupt */
  HAL_CORTEX_NVIC_SetPriority(TIM6_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(TIM6_IRQn);

  return &hTIM6;
}

void mx_tim6_cfg3_hal_deinit(void)
{
  /* Disable Timer update interrupt */
  HAL_CORTEX_NVIC_DisableIRQ(TIM6_IRQn);

  (void)HAL_TIM_DeInit(&hTIM6);

  HAL_RCC_TIM6_Reset();

  HAL_RCC_TIM6_DisableClock();

}

hal_tim_handle_t *mx_tim6_hal_gethandle(void)
{
  return &hTIM6;
}

/******************************************************************************/
/*                     Interruption and Exception Handlers                    */
/******************************************************************************/
void TIM6_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&hTIM6);
}

