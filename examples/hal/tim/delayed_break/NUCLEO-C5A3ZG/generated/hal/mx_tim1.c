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
/* Exported functions for TIM1 in HAL layer */
/******************************************************************************/
hal_tim_handle_t *mx_tim1_init(void)
{
  if (HAL_TIM_Init(&hTIM1, HAL_TIM1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_TIM1_EnableClock();

  /* Timer configuration to reach the output frequency at 10 kHz */
  hal_tim_config_t config;
  config.prescaler              = 143;
  config.counter_mode           = HAL_TIM_COUNTER_UP;
  config.period                 = 0x63;
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
  hal_tim_oc_compare_unit_config_t oc_compare_unit_config;

  oc_compare_unit_config.mode  = HAL_TIM_OC_PWM1;
  oc_compare_unit_config.pulse = 0x5A;
  if (HAL_TIM_OC_SetConfigCompareUnit(&hTIM1, HAL_TIM_OC_COMPARE_UNIT_1,
                                      &oc_compare_unit_config) != HAL_OK)
  {
    return NULL;
  }

  oc_config.polarity       = HAL_TIM_OC_HIGH;
  oc_config.idle_state     = HAL_TIM_OC_IDLE_STATE_RESET;
  oc_config.override_state = HAL_TIM_OC_OVERRIDE_RESET;
  oc_config.break_mode     = HAL_TIM_OC_BREAKMODE_DELAY1;
  if (HAL_TIM_OC_SetConfigChannel(&hTIM1, HAL_TIM_CHANNEL_2, &oc_config) != HAL_OK)
  {
    return NULL;
  }
  oc_compare_unit_config.mode  = HAL_TIM_OC_PWM1;
  oc_compare_unit_config.pulse = 0x5A;
  if (HAL_TIM_OC_SetConfigCompareUnit(&hTIM1, HAL_TIM_OC_COMPARE_UNIT_2,
                                      &oc_compare_unit_config) != HAL_OK)
  {
    return NULL;
  }

  oc_config.polarity       = HAL_TIM_OC_HIGH;
  oc_config.idle_state     = HAL_TIM_OC_IDLE_STATE_RESET;
  oc_config.override_state = HAL_TIM_OC_OVERRIDE_RESET;
  oc_config.break_mode     = HAL_TIM_OC_BREAKMODE_DELAY2;
  if (HAL_TIM_OC_SetConfigChannel(&hTIM1, HAL_TIM_CHANNEL_3, &oc_config) != HAL_OK)
  {
    return NULL;
  }
  oc_compare_unit_config.mode  = HAL_TIM_OC_PWM1;
  oc_compare_unit_config.pulse = 0x5A;
  if (HAL_TIM_OC_SetConfigCompareUnit(&hTIM1, HAL_TIM_OC_COMPARE_UNIT_3,
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
  /* Break Configuration */
  if (HAL_TIM_BREAK_EnableAutomaticOutput(&hTIM1) != HAL_OK)
  {
    return NULL;
  }

  hal_tim_off_states_config_t off_states;

  off_states.off_state_run  = HAL_TIM_OFF_STATE_RUN_ENABLE;
  off_states.off_state_idle = HAL_TIM_OFF_STATE_IDLE_ENABLE;
  if (HAL_TIM_BREAK_SetOutputOffStates(&hTIM1, &off_states) != HAL_OK)
  {
    return NULL;
  }

  /* Break 1 Configuration */
  hal_tim_break_input_config_t break_input;

  break_input.polarity  = HAL_TIM_BREAK_INPUT_LOW;
  break_input.filter    = HAL_TIM_FDIV1;
  break_input.mode      = HAL_TIM_BREAK_INPUT_MODE_INPUT;
  if (HAL_TIM_BREAK_SetConfigInput(&hTIM1, HAL_TIM_BREAK_INPUT_1, &break_input) != HAL_OK)
  {
    return NULL;
  }

  if (HAL_TIM_BREAK_SetBreakDelay(&hTIM1, HAL_TIM_BREAK_DELAY1, 60) != HAL_OK)
  {
    return NULL;
  }

  if (HAL_TIM_BREAK_SetBreakDelay(&hTIM1, HAL_TIM_BREAK_DELAY2, 120) != HAL_OK)
  {
    return NULL;
  }

  /* GPIO Break1 Input Source Configuration */
  if (HAL_TIM_BREAK_SetInputSourcePolarity(&hTIM1, HAL_TIM_BREAK_INPUT_1, HAL_TIM_BRK_TIM1_GPIO,
                                           HAL_TIM_BREAK_INPUT_SRC_NONINVERTED) != HAL_OK)
  {
    return NULL;
  }

  if (HAL_TIM_BREAK_EnableInputSource(&hTIM1, HAL_TIM_BREAK_INPUT_1, HAL_TIM_BRK_TIM1_GPIO) != HAL_OK)
  {
    return NULL;
  }

  if (HAL_TIM_BREAK_EnableInput(&hTIM1, HAL_TIM_BREAK_INPUT_1) != HAL_OK)
  {
    return NULL;
  }

  /* ### TIM1 GPIO Configuration ########################### */
  /* GPIO Clocks activation */
  HAL_RCC_GPIOA_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    [GPIO Pin] ------> [Signal Name] ------> [Labels]

       PA8     ------>   TIM1_CH1   ------>  PA8
       PA9     ------>   TIM1_CH2   ------>  PA9
       PA10    ------>   TIM1_CH3   ------>  PA10
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_LOW;
  gpio_config.alternate   = HAL_GPIO_AF_1;
  HAL_GPIO_Init(HAL_GPIOA, PA8_PIN | PA9_PIN | PA10_PIN, &gpio_config);

  /**
    [GPIO Pin] ------> [Signal Name] ------> [Labels]

       PA6     ------>   TIM1_BKIN   ------>  PA6
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_UP;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_LOW;
  gpio_config.alternate   = HAL_GPIO_AF_1;
  HAL_GPIO_Init(PA6_PORT, PA6_PIN, &gpio_config);

  return &hTIM1;
}

void mx_tim1_deinit(void)
{
  (void)HAL_TIM_DeInit(&hTIM1);

  HAL_RCC_TIM1_DisableClock();

  HAL_RCC_TIM1_Reset();

  /* De-initialize all GPIOA pins associated with TIM1 */
  HAL_GPIO_DeInit(HAL_GPIOA, PA6_PIN | PA8_PIN | PA9_PIN | PA10_PIN);
}

hal_tim_handle_t *mx_tim1_gethandle(void)
{
  return &hTIM1;
}
