/**
  ******************************************************************************
  * @file           : mx_usart2.c
  * @brief          : Peripheral initialization
  *                   File per peripheral instance that is handling the peripheral
  *                   initialization and the IRQ handlers. Init parameters provided
  *                   as a set of defines configurable/generated for a given target
  *                   through the  hal_usart_settings.json and the config tool.
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
#include "mx_usart2.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/

/* Exported variables by reference--------------------------------------------*/
static hal_uart_handle_t gh_usart2;

void *usart2_instance1_cfg1_init(void)
{
  hal_uart_config_t config;

  if (HAL_UART_Init(&gh_usart2, HAL_UART2) != HAL_OK)
  {
    return NULL;
  }
  /* Peripheral clock enable */
  HAL_RCC_USART2_EnableClock();

  if (HAL_RCC_USART2_SetKernelClkSource(HAL_RCC_USART2_CLK_SRC_PCLK1) != HAL_OK)
  {
    return NULL;
  }

  config.baud_rate = USART2_BAUDRATE;
  config.clock_prescaler = HAL_UART_PRESCALER_DIV1;
  config.word_length = HAL_UART_WORD_LENGTH_8_BIT;
  config.stop_bits = HAL_UART_STOP_BIT_1;
  config.parity = HAL_UART_PARITY_NONE;
  config.direction = HAL_UART_DIRECTION_TX;
  config.hw_flow_ctl = HAL_UART_HW_CONTROL_NONE;
  config.oversampling = HAL_UART_OVERSAMPLING_16;
  config.one_bit_sampling = HAL_UART_ONE_BIT_SAMPLE_DISABLE;

  if (HAL_UART_SetConfig(&gh_usart2, &config) != HAL_OK)
  {
    return NULL;
  }

  return (void *)&gh_usart2;
}

void usart2_deinit(void)
{
  (void)HAL_UART_DeInit(&gh_usart2);
  HAL_RCC_USART2_Reset();
  HAL_RCC_USART2_DisableClock();

}

void *usart2_gethandle(void)
{
  return (void *)&gh_usart2;
}
