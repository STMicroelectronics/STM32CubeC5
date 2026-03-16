/**
  ******************************************************************************
  * @file           : mx_usart3.c
  * @brief          : USART3 Peripheral initialization
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
#include "mx_usart3.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
/* Handle for USART */
static hal_usart_handle_t hUSART3;
/* Exported function definition ----------------------------------------------*/
/******************************************************************************/
/* Exported functions for USART in HAL layer */
/******************************************************************************/
hal_usart_handle_t *mx_usart3_usart_init(void)
{
  hal_usart_config_t usart_config;

  if (HAL_USART_Init(&hUSART3, HAL_USART3) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_USART3_EnableClock();

  if (HAL_RCC_USART3_SetKernelClkSource(HAL_RCC_USART3_CLK_SRC_PCLK1) != HAL_OK)
  {
    return NULL;
  }

  usart_config.clock_prescaler = HAL_USART_PRESCALER_DIV1;
  usart_config.word_length = HAL_USART_WORD_LENGTH_8_BIT;
  usart_config.stop_bits = HAL_USART_STOP_BIT_1;
  usart_config.parity = HAL_USART_PARITY_NONE;
  usart_config.direction = HAL_USART_DIRECTION_TX_RX;
  usart_config.clock_polarity = HAL_USART_CLOCK_POLARITY_LOW;
  usart_config.clock_phase = HAL_USART_CLOCK_PHASE_1_EDGE;
  usart_config.clock_last_bit = HAL_USART_CLOCK_LAST_BIT_ENABLED;
  usart_config.mode = HAL_USART_MODE_SLAVE;
  usart_config.baud_rate = 115200U;

  if (HAL_USART_SetConfig(&hUSART3, &usart_config) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_GPIOC_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    USART3 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PC12    ------>   USART3_CK
       PC11    ------>   USART3_RX
       PC10    ------>   USART3_TX
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_LOW;
  gpio_config.alternate   = HAL_GPIO_AF_7;
  HAL_GPIO_Init(HAL_GPIOC, HAL_GPIO_PIN_12 | HAL_GPIO_PIN_11 | HAL_GPIO_PIN_10, &gpio_config);

  /* Enable interrupt */
  HAL_CORTEX_NVIC_SetPriority(USART3_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(USART3_IRQn);

  return &hUSART3;
}

void mx_usart3_usart_deinit(void)
{
  /* Disable interrupt */
  HAL_CORTEX_NVIC_DisableIRQ(USART3_IRQn);
(void)HAL_USART_DeInit(&hUSART3);

  HAL_RCC_USART3_Reset();

  HAL_RCC_USART3_DisableClock();

  /* De-initialize all GPIO pins associated with USART3 */
  HAL_GPIO_DeInit(HAL_GPIOC, HAL_GPIO_PIN_10 | HAL_GPIO_PIN_11 | HAL_GPIO_PIN_12);
}

hal_usart_handle_t *mx_usart3_usart_gethandle(void)
{
  return &hUSART3;
}

/******************************************************************************/
/*                          USART3 global interrupt                           */
/******************************************************************************/
void USART3_IRQHandler(void)
{
  HAL_USART_IRQHandler(&hUSART3);
}
