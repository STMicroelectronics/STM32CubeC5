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

  /* Timer configuration to reach the output frequency at 30303 Hz */
  hal_tim_config_t config;
  config.prescaler              = 143;
  config.counter_mode           = HAL_TIM_COUNTER_UP;
  config.period                 = 0x20;
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

  oc_compare_unit_config.mode  = HAL_TIM_OC_TOGGLE;
  oc_compare_unit_config.pulse = 0x10;
  if (HAL_TIM_OC_SetConfigCompareUnit(&hTIM1, hal_tim_oc_channel_to_compare_unit(HAL_TIM_CHANNEL_1),
                                      &oc_compare_unit_config) != HAL_OK)
  {
    return NULL;
  }

  oc_config.polarity       = HAL_TIM_OC_HIGH;
  oc_config.idle_state     = HAL_TIM_OC_IDLE_STATE_RESET;
  oc_config.override_state = HAL_TIM_OC_OVERRIDE_RESET;
  oc_config.break_mode     = HAL_TIM_OC_BREAKMODE_IMMEDIATE;
  if (HAL_TIM_OC_SetConfigChannel(&hTIM1, HAL_TIM_CHANNEL_2, &oc_config) != HAL_OK)
  {
    return NULL;
  }

  oc_config.polarity       = HAL_TIM_OC_HIGH;
  oc_config.idle_state     = HAL_TIM_OC_IDLE_STATE_RESET;
  oc_config.override_state = HAL_TIM_OC_OVERRIDE_RESET;
  oc_config.break_mode     = HAL_TIM_OC_BREAKMODE_IMMEDIATE;
  if (HAL_TIM_OC_SetConfigChannel(&hTIM1, HAL_TIM_CHANNEL_2N, &oc_config) != HAL_OK)
  {
    return NULL;
  }
  oc_compare_unit_config.mode  = HAL_TIM_OC_TOGGLE;
  oc_compare_unit_config.pulse = 0x8;
  if (HAL_TIM_OC_SetConfigCompareUnit(&hTIM1, hal_tim_oc_channel_to_compare_unit(HAL_TIM_CHANNEL_2),
                                      &oc_compare_unit_config) != HAL_OK)
  {
    return NULL;
  }

  oc_config.polarity       = HAL_TIM_OC_HIGH;
  oc_config.idle_state     = HAL_TIM_OC_IDLE_STATE_RESET;
  oc_config.override_state = HAL_TIM_OC_OVERRIDE_RESET;
  oc_config.break_mode     = HAL_TIM_OC_BREAKMODE_IMMEDIATE;
  if (HAL_TIM_OC_SetConfigChannel(&hTIM1, HAL_TIM_CHANNEL_3, &oc_config) != HAL_OK)
  {
    return NULL;
  }

  oc_config.polarity       = HAL_TIM_OC_HIGH;
  oc_config.idle_state     = HAL_TIM_OC_IDLE_STATE_RESET;
  oc_config.override_state = HAL_TIM_OC_OVERRIDE_RESET;
  oc_config.break_mode     = HAL_TIM_OC_BREAKMODE_IMMEDIATE;
  if (HAL_TIM_OC_SetConfigChannel(&hTIM1, HAL_TIM_CHANNEL_3N, &oc_config) != HAL_OK)
  {
    return NULL;
  }
  oc_compare_unit_config.mode  = HAL_TIM_OC_TOGGLE;
  oc_compare_unit_config.pulse = 0x4;
  if (HAL_TIM_OC_SetConfigCompareUnit(&hTIM1, hal_tim_oc_channel_to_compare_unit(HAL_TIM_CHANNEL_3),
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
  if (HAL_TIM_EnableCommutation(&hTIM1, HAL_TIM_COMMUTATION_SOFTWARE) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_GPIOA_EnableClock();

  HAL_RCC_GPIOB_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    TIM1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PA8     ------>   TIM1_CH1
       PA9     ------>   TIM1_CH2
       PA10    ------>   TIM1_CH3
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_DOWN;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_LOW;
  gpio_config.alternate   = HAL_GPIO_AF_1;
  HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_8 | HAL_GPIO_PIN_9 | HAL_GPIO_PIN_10, &gpio_config);

  /**
    TIM1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PB13    ------>   TIM1_CH1N
       PB14    ------>   TIM1_CH2N
       PB15    ------>   TIM1_CH3N
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_DOWN;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_LOW;
  gpio_config.alternate   = HAL_GPIO_AF_1;
  HAL_GPIO_Init(HAL_GPIOB, HAL_GPIO_PIN_13 | HAL_GPIO_PIN_14 | HAL_GPIO_PIN_15, &gpio_config);

  /* Enable Timer Trigger, Commutation, Direction change and Index interrupt */
  HAL_CORTEX_NVIC_SetPriority(TIM1_TRGI_COM_DIR_IDX_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(TIM1_TRGI_COM_DIR_IDX_IRQn);

  return &hTIM1;
}

void mx_tim1_deinit(void)
{
  (void)HAL_TIM_DeInit(&hTIM1);

  HAL_RCC_TIM1_DisableClock();

  HAL_RCC_TIM1_Reset();

  /* De-initialize all GPIO pins associated with TIM1 */
  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_8 | HAL_GPIO_PIN_9 | HAL_GPIO_PIN_10);

  HAL_GPIO_DeInit(HAL_GPIOB, HAL_GPIO_PIN_13 | HAL_GPIO_PIN_14 | HAL_GPIO_PIN_15);

  /* Disable Timer Trigger, Commutation, Direction change and Index interrupt */
  HAL_CORTEX_NVIC_DisableIRQ(TIM1_TRGI_COM_DIR_IDX_IRQn);
}

hal_tim_handle_t *mx_tim1_gethandle(void)
{
  return &hTIM1;
}

/******************************************************************************/
/*  TIM1 trigger and commutation/TIM1 Direction Change interrupt/TIM1 Index   */
/******************************************************************************/
void TIM1_TRGI_COM_DIR_IDX_IRQHandler(void)
{
  HAL_TIM_TRGI_COM_DIR_IDX_IRQHandler(&hTIM1);
}
