/**
  ******************************************************************************
  * @file           : mx_lptim1.c
  * @brief          : LPTIM1 Peripheral initialization
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
#include "mx_lptim1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/

/* Exported variables by reference--------------------------------------------*/
static hal_lptim_handle_t hLPTIM1;
/* Exported function definition ----------------------------------------------*/
/******************************************************************************/
/* Exported functions for SW instance in HAL layer */
/******************************************************************************/

hal_lptim_handle_t *mx_lptim1_init(void)
{
  hal_lptim_config_t  config;

  if (HAL_LPTIM_Init(&hLPTIM1, HAL_LPTIM1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_LPTIM1_EnableClock();

  if (HAL_RCC_LPTIM1_SetKernelClkSource(HAL_RCC_LPTIM1_CLK_SRC_LSI) != HAL_OK)
  {
    return NULL;
  }

  /* Timer configuration with external clock */
  config.mode               = HAL_LPTIM_CONTINUOUS;
  config.clock_source       = HAL_LPTIM_CLK_EXTERNAL_SYNCHRONOUS;
  config.period             = 0x3E8;
  config.prescaler          = HAL_LPTIM_CLK_SRC_DIV1;
  config.repetition_counter = 0;

  if (HAL_LPTIM_SetConfig(&hLPTIM1, &config) != HAL_OK)
  {
    return NULL;
  }

  hal_lptim_input1_config_t p_config_external;
  p_config_external.src = HAL_LPTIM_INPUT1_GPIO;
  p_config_external.polarity = HAL_LPTIM_INPUT1_RISING;
  p_config_external.filter = HAL_LPTIM_FDIV1;
  if (HAL_LPTIM_SetConfigInput1(&hLPTIM1, &p_config_external) != HAL_OK)
  {
    return NULL;
  }

  /* Init GPIO */
  HAL_RCC_GPIOA_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    LPTIM1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PA1     ------>   LPTIM1_IN1
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_LOW;
  gpio_config.alternate   = HAL_GPIO_AF_5;
  HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_1, &gpio_config);

  /* Enable the LPTIM global interruption */
  HAL_CORTEX_NVIC_SetPriority(LPTIM1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(LPTIM1_IRQn);

  return &hLPTIM1;
}

void mx_lptim1_deinit(void)
{
  (void)HAL_LPTIM_DeInit(&hLPTIM1);

  HAL_RCC_LPTIM1_Reset();

  HAL_RCC_LPTIM1_DisableClock();

  /* De-initialize all GPIO pins associated with LPTIM1 */
  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_1);

  /* Disable Timer global interrupt */
  HAL_CORTEX_NVIC_DisableIRQ(LPTIM1_IRQn);
}

hal_lptim_handle_t *mx_lptim1_gethandle(void)
{
  return &hLPTIM1;
}

/******************************************************************************/
/*         LPTIM1 global interrupt OR LPTimer1 AIT through EXTI line          */
/******************************************************************************/
void LPTIM1_IRQHandler(void)
{
  HAL_LPTIM_IRQHandler(&hLPTIM1);
}
