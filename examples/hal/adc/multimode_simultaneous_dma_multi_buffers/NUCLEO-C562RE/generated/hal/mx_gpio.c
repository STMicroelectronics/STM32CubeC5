/**
  ******************************************************************************
  * file    mx_gpio.c
  * brief   Initialisation of all the peripherals used in this project
  *          File per peripheral instance that is handling the peripheral
  *          initialization and the IRQ handlers. Init parameters provided
  *          as a set of defines configurable/generated for a given target
  *          through STM32CubeMX2.
  *
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
#include "mx_gpio.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported variables by reference -------------------------------------------*/

int32_t gpio_cfg1_init(void)
{
  /* PA5 <= MX_STATUS_LED */
  hal_gpio_config_t  config_gpio;

  HAL_RCC_GPIOA_EnableClock();

  HAL_GPIO_WritePin(MX_STATUS_LED_GPIO_PORT, MX_STATUS_LED_PIN, MX_STATUS_LED_INIT_STATE);

  config_gpio.mode         = HAL_GPIO_MODE_OUTPUT;
  config_gpio.pull         = HAL_GPIO_PULL_NO;
  config_gpio.output_type  = HAL_GPIO_OUTPUT_PUSHPULL;
  config_gpio.speed        = HAL_GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MX_STATUS_LED_GPIO_PORT, MX_STATUS_LED_PIN, &config_gpio);

  return 0;
}

int32_t gpio_cfg2_init(void)
{
  /* PA2  <= USART1_TX */

  HAL_RCC_GPIOA_EnableClock();

  hal_gpio_config_t  config_gpio;

  config_gpio.mode        = HAL_GPIO_MODE_ALTERNATE;
  config_gpio.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  config_gpio.pull        = HAL_GPIO_PULL_NO;
  config_gpio.speed       = HAL_GPIO_SPEED_FREQ_LOW;
  config_gpio.alternate   = HAL_GPIO_AF_7;
  HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_2, &config_gpio);

  return 0;
}

int32_t gpio_cfg1_deinit(void)
{
  /* PA5 <= MX_STATUS_LED */
  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_5);

  return 0;
}

int32_t gpio_cfg2_deinit(void)
{
  /* PA2  <= USART1_TX */
  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_2);

  return 0;
}
