/**
  ******************************************************************************
  * @file           : mx_opamp1.c
  * @brief          : OPAMP1 Peripheral initialization
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
#include "mx_opamp1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_opamp_handle_t hOPAMP1;

/******************************************************************************/
/* Exported functions for OPAMP1 in HAL layer (SW instance MyOPAMP_1) */
/******************************************************************************/
hal_opamp_handle_t *mx_opamp1_init(void)
{
  HAL_RCC_OPAMP1_EnableClock();

  /****************************************************************************/
  /* Configuration of basic features (mandatory)                              */
  /****************************************************************************/

  hal_opamp_config_t opamp_config;
  hal_opamp_config_input_connection_t config_input_connection;

  if (HAL_OPAMP_Init(&hOPAMP1, HAL_OPAMP1) != HAL_OK)
  {
    return NULL;
  }

  opamp_config.speed_mode          = HAL_OPAMP_SPEED_MODE_HIGH;
  opamp_config.configuration_mode  = HAL_OPAMP_MODE_STANDALONE;
  opamp_config.opamp_output        = HAL_OPAMP_OUTPUT_CONNECTION_EXTERNAL;
  HAL_OPAMP_SetConfig(&hOPAMP1, &opamp_config);

  config_input_connection.inverting_input = HAL_OPAMP_INVERTING_INPUT_GPIO_0;

  config_input_connection.non_inverting_input = HAL_OPAMP_NON_INVERTING_INPUT_GPIO_0;
  HAL_OPAMP_SetConfigInputConnection(&hOPAMP1, &config_input_connection);

  HAL_OPAMP_SetPGAExternalMode(&hOPAMP1, HAL_OPAMP_PGA_EXT_NONE);

  HAL_RCC_GPIOA_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    OPAMP1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PA1     ------>   OPAMP1_VINP0
       PA6     ------>   OPAMP1_VOUT
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ANALOG;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_1 | HAL_GPIO_PIN_6, &gpio_config);

  return &hOPAMP1;
}

void mx_opamp1_deinit(void)
{
  (void)HAL_OPAMP_DeInit(&hOPAMP1);

  HAL_RCC_OPAMP1_Reset();

  HAL_RCC_OPAMP1_DisableClock();

  /* De-initialize all GPIO pins associated with OPAMP1 */
  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_1 | HAL_GPIO_PIN_6);
}
hal_opamp_handle_t *mx_opamp1_gethandle(void)
{
  return &hOPAMP1;
}
