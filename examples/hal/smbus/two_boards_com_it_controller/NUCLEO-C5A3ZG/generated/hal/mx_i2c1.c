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
static hal_smbus_handle_t hI2C1;

/******************************************************************************/
/* Exported functions for I2C1 in HAL layer (SW instance MySMBUS_1) */
/******************************************************************************/
hal_smbus_handle_t *mx_i2c1_smbus_init(void)
{
  hal_smbus_config_t smbus_config;

  if (HAL_SMBUS_Init(&hI2C1, HAL_SMBUS1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_I2C1_EnableClock();

  /*
    Timing automatically calculated with:
     - I2C1 input clock at 144000000 Hz
     - I2C clock speed at 1000000 Hz
  */
  smbus_config.timing           = 0x20500C16;
  smbus_config.own_address1     = 93 << 1U;
  smbus_config.device_mode      = HAL_SMBUS_PERIPHERAL_MODE_HOST;
  if (HAL_SMBUS_SetConfig(&hI2C1, &smbus_config) != HAL_OK)
  {
    return NULL;
  }

  /* Default value set for digital filter */
  /* HAL_SMBUS_SetDigitalFilter(&hI2C1, 0); */

  HAL_SMBUS_EnableAnalogFilter(&hI2C1);
  HAL_SMBUS_EnablePacketErrorCheck(&hI2C1);

  HAL_RCC_GPIOB_EnableClock();

  HAL_RCC_GPIOA_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    I2C1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PB6     ------>   I2C1_SCL
       PB7     ------>   I2C1_SDA
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_OPENDRAIN;
  gpio_config.pull        = HAL_GPIO_PULL_UP;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_HIGH;
  gpio_config.alternate   = HAL_GPIO_AF_4;
  HAL_GPIO_Init(HAL_GPIOB, HAL_GPIO_PIN_6 | HAL_GPIO_PIN_7, &gpio_config);

  /**
    I2C1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PA9     ------>   I2C1_SMBA
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_OPENDRAIN;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_LOW;
  gpio_config.alternate   = HAL_GPIO_AF_9;
  HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_9, &gpio_config);

  if (HAL_RCC_I2C1_SetKernelClkSource(HAL_RCC_I2C1_CLK_SRC_PCLK1) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the Event interruption for I2C */
  HAL_CORTEX_NVIC_SetPriority(I2C1_EV_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(I2C1_EV_IRQn);

  /* Enable the Error interruption for I2C */
  HAL_CORTEX_NVIC_SetPriority(I2C1_ERR_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(I2C1_ERR_IRQn);

  return &hI2C1;
}

void mx_i2c1_smbus_deinit(void)
{
  /* Disable the Event interruption for I2C */
  HAL_CORTEX_NVIC_DisableIRQ(I2C1_EV_IRQn);

  /* Disable the Error interruption for I2C */
  HAL_CORTEX_NVIC_DisableIRQ(I2C1_ERR_IRQn);

  (void)HAL_SMBUS_DeInit(&hI2C1);

  HAL_RCC_I2C1_Reset();

  HAL_RCC_I2C1_DisableClock();

  /* De-initialize all GPIO pins associated with I2C1 */
  HAL_GPIO_DeInit(HAL_GPIOB, HAL_GPIO_PIN_6 | HAL_GPIO_PIN_7);

  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_9);
}

hal_smbus_handle_t *mx_i2c1_smbus_gethandle(void)
{
  return &hI2C1;
}

/******************************************************************************/
/*                            I2C1 event interrupt                            */
/******************************************************************************/
void I2C1_EV_IRQHandler(void)
{
  HAL_SMBUS_EV_IRQHandler(&hI2C1);
}

/******************************************************************************/
/*                            I2C1 error interrupt                            */
/******************************************************************************/
void I2C1_ERR_IRQHandler(void)
{
  HAL_SMBUS_ERR_IRQHandler(&hI2C1);
}
