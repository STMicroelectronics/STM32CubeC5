/**
  ******************************************************************************
  * @file           : mx_tim1.c
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
#include "mx_tim1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_tim_handle_t hTIM1;

/* Exported function definition ----------------------------------------------*/

/******************************************************************************/
/* Exported functions for TIM1 in HAL layer (SW instance MyTIM_1) */
/******************************************************************************/
hal_tim_handle_t *mx_tim1_init(void)
{
  if (HAL_TIM_Init(&hTIM1, HAL_TIM1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_TIM1_EnableClock();

  /* Timer configuration to reach the output frequency at 8000 Hz */
  hal_tim_config_t config;
  config.prescaler              = 2249;
  config.counter_mode           = HAL_TIM_COUNTER_UP;
  config.period                 = 0x7;
  config.repetition_counter     = 0;
  config.clock_sel.clock_source = HAL_TIM_CLK_INTERNAL;
  if (HAL_TIM_SetConfig(&hTIM1, &config) != HAL_OK)
  {
    return NULL;
  }

  /* Sampling Clock */
  if (HAL_TIM_SetDTSPrescaler(&hTIM1, HAL_TIM_DTS_DIV1) != HAL_OK)
  {
    return NULL;
  }
  if (HAL_TIM_SetDTS2Prescaler(&hTIM1, HAL_TIM_DTS2_DIV1) != HAL_OK)
  {
    return NULL;
  }

  hal_tim_oc_channel_config_t oc_config;

  oc_config.polarity       = HAL_TIM_OC_HIGH;
  oc_config.idle_state     = HAL_TIM_OC_IDLE_STATE_RESET;
  oc_config.override_state = HAL_TIM_OC_OVERRIDE_RESET;
  oc_config.break_mode     = HAL_TIM_OC_BREAKMODE_IMMEDIATE;
  if (HAL_TIM_OC_SetConfigChannel(&hTIM1, HAL_TIM_CHANNEL_1, &oc_config) != HAL_OK)
  {
    return NULL;
  }

  oc_config.polarity       = HAL_TIM_OC_HIGH;
  oc_config.idle_state     = HAL_TIM_OC_IDLE_STATE_RESET;
  oc_config.override_state = HAL_TIM_OC_OVERRIDE_RESET;
  oc_config.break_mode     = HAL_TIM_OC_BREAKMODE_IMMEDIATE;
  if (HAL_TIM_OC_SetConfigChannel(&hTIM1, HAL_TIM_CHANNEL_1N, &oc_config) != HAL_OK)
  {
    return NULL;
  }
  hal_tim_oc_compare_unit_config_t oc_compare_unit_config;

  oc_compare_unit_config.mode  = HAL_TIM_OC_PWM1;
  oc_compare_unit_config.pulse = 0x4;
  if (HAL_TIM_OC_SetConfigCompareUnit(&hTIM1, hal_tim_oc_channel_to_compare_unit(HAL_TIM_CHANNEL_1),
                                      &oc_compare_unit_config) != HAL_OK)
  {
    return NULL;
  }

  /* Update Event Management */
  if (HAL_TIM_SetUpdateSource(&hTIM1, HAL_TIM_UPDATE_REGULAR) != HAL_OK)
  {
    return NULL;
  }
  if (HAL_TIM_EnableUpdateGeneration(&hTIM1) != HAL_OK)
  {
    return NULL;
  }
  /* Deadtime Configuration */
  if (HAL_TIM_SetDeadtime(&hTIM1, 226, 226) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_GPIOA_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    TIM1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PA8     ------>   TIM1_CH1
       PA7     ------>   TIM1_CH1N
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_LOW;
  gpio_config.alternate   = HAL_GPIO_AF_1;
  HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_8 | HAL_GPIO_PIN_7, &gpio_config);

  return &hTIM1;
}

void mx_tim1_deinit(void)
{
  (void)HAL_TIM_DeInit(&hTIM1);

  HAL_RCC_TIM1_DisableClock();

  HAL_RCC_TIM1_Reset();

  /* De-initialize all GPIO pins associated with TIM1 */
  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_7 | HAL_GPIO_PIN_8);
}

hal_tim_handle_t *mx_tim1_gethandle(void)
{
  return &hTIM1;
}
