/**
  ******************************************************************************
  * @file           : mx_comp1.c
  * @brief          : COMP1 Peripheral initialization
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
#include "mx_comp1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_comp_handle_t hCOMP1;
/******************************************************************************/
/* Exported functions for COMP1 in HAL layer */
/******************************************************************************/
hal_comp_handle_t *mx_comp1_init(void)
{
  if (HAL_COMP_Init(&hCOMP1, HAL_COMP1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_COMP12_EnableClock();

  hal_comp_config_t comp_config;
  comp_config.power_mode = HAL_COMP_POWER_MODE_HIGH_SPEED;
  comp_config.input_plus = HAL_COMP_INPUT_PLUS_IO2;
  comp_config.input_minus = HAL_COMP_INPUT_MINUS_VREFINT;
  comp_config.input_hysteresis = HAL_COMP_INPUT_HYSTERESIS_NONE;
  comp_config.output_polarity = HAL_COMP_OUTPUT_POLARITY_NONINVERTED;
  comp_config.output_trigger = HAL_COMP_OUTPUT_TRIG_RISING_FALLING;
  if (HAL_COMP_SetConfig(&hCOMP1, &comp_config) != HAL_OK)
  {
    return NULL;
  }

  /* ### COMP1 GPIO Configuration ########################### */
  /* GPIO Clocks activation */
  HAL_RCC_GPIOB_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    [GPIO Pin] ------> [Signal Name] ------> [Labels]

       PB0     ------>   COMP1_INP2   ------>  PB0
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ANALOG;
  HAL_GPIO_Init(PB0_PORT, PB0_PIN, &gpio_config);

  /* Enable comparator interruption through EXTI */
  HAL_CORTEX_NVIC_SetPriority(COMP1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(COMP1_IRQn);

  return &hCOMP1;
}

void mx_comp1_deinit(void)
{
  (void)HAL_COMP_DeInit(&hCOMP1);

  HAL_RCC_COMP12_DisableClock();

  HAL_RCC_COMP12_Reset();

  /* De-initialize all GPIOB pins associated with COMP1 */
  HAL_GPIO_DeInit(PB0_PORT, PB0_PIN);

  HAL_CORTEX_NVIC_DisableIRQ(COMP1_IRQn);
}

hal_comp_handle_t *mx_comp1_gethandle(void)
{
  return &hCOMP1;
}

/******************************************************************************/
/*                           COMP1 global interrupt                           */
/******************************************************************************/
void COMP1_IRQHandler(void)
{
  HAL_COMP_IRQHandler(&hCOMP1);
}
