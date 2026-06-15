/**
  ******************************************************************************
  * @file           : mx_i2c1.c
  * @brief          : I2C1 Peripheral initialization
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
#include "mx_i2c1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_i2c_handle_t hI2C1;

/******************************************************************************/
/* Exported functions for I2C1 in HAL layer */
/******************************************************************************/
hal_i2c_handle_t *mx_i2c1_i2c_init(void)
{
  hal_i2c_config_t i2c_config;

  if (HAL_I2C_Init(&hI2C1, HAL_I2C1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_I2C1_EnableClock();

  /*
    Timing automatically calculated with:
     - I2C1 input clock at 144000000 Hz
     - I2C clock speed at 100000 Hz
  */
  i2c_config.timing           = 0x70D25153;
  i2c_config.addressing_mode  = HAL_I2C_ADDRESSING_7BIT;
  i2c_config.own_address1     = 93 << 1U;
  if (HAL_I2C_SetConfig(&hI2C1, &i2c_config) != HAL_OK)
  {
    return NULL;
  }

  HAL_I2C_EnableAnalogFilter(&hI2C1);

  /* ### I2C1 GPIO Configuration ########################### */
  /* GPIO Clocks activation */
  HAL_RCC_GPIOB_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    [GPIO Pin] ------> [Signal Name] ------> [Labels]

       PB6     ------>   I2C1_SCL   ------>  PB6
       PB7     ------>   I2C1_SDA   ------>  PB7
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_OPENDRAIN;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_LOW;
  gpio_config.alternate   = HAL_GPIO_AF_4;
  HAL_GPIO_Init(HAL_GPIOB, PB6_PIN | PB7_PIN, &gpio_config);

  if (HAL_RCC_I2C1_SetKernelClkSource(HAL_RCC_I2C1_CLK_SRC_PCLK1) != HAL_OK)
  {
    return NULL;
  }

  return &hI2C1;
}

void mx_i2c1_i2c_deinit(void)
{
  (void)HAL_I2C_DeInit(&hI2C1);

  HAL_RCC_I2C1_Reset();

  HAL_RCC_I2C1_DisableClock();

  /* De-initialize all GPIOB pins associated with I2C1 */
  HAL_GPIO_DeInit(HAL_GPIOB, PB6_PIN | PB7_PIN);
}

hal_i2c_handle_t *mx_i2c1_i2c_gethandle(void)
{
  return &hI2C1;
}
