/**
  ******************************************************************************
  * @file           : mx_dac1.c
  * @brief          : DAC1 Peripheral initialization
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
#include "mx_dac1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_dac_handle_t hDAC1;

/******************************************************************************/
/* Exported functions for DAC1 in HAL layer (SW instance MyDAC_1) */
/******************************************************************************/
hal_dac_handle_t *mx_dac1_init(void)
{
  HAL_RCC_DAC1_EnableClock();

  if (HAL_RCC_ADCDAC_SetKernelClkSource(HAL_RCC_ADCDAC_CLK_SRC_PSIS) != HAL_OK)
  {
    return NULL;
  }

  /****************************************************************************/
  /* Initialization of DAC instance                                           */
  /****************************************************************************/

  if (HAL_DAC_Init(&hDAC1, HAL_DAC1) != HAL_OK)
  {
    return NULL;
  }

  hal_dac_config_t dac_config;
  dac_config.high_frequency_mode = HAL_DAC_HIGH_FREQ_MODE_ABOVE_80MHZ;
  if (HAL_DAC_SetConfig(&hDAC1, &dac_config) != HAL_OK)
  {
    return NULL;
  }

  /****************************************************************************/
  /* Configuration of DAC channel                                             */
  /****************************************************************************/

  hal_dac_channel_config_t dac_channel_config;

  /* ========== Channel 1 ========== */
  dac_channel_config.alignment         = HAL_DAC_DATA_ALIGN_8_BITS_RIGHT;
  dac_channel_config.trigger           = HAL_DAC_TRIGGER_SOFTWARE;
  dac_channel_config.output_buffer     = HAL_DAC_OUTPUT_BUFFER_ENABLED;
  dac_channel_config.output_connection = HAL_DAC_OUTPUT_CONNECTION_EXTERNAL;
  dac_channel_config.data_sign_format  = HAL_DAC_SIGN_FORMAT_UNSIGNED;
  if (HAL_DAC_SetConfigChannel(&hDAC1, HAL_DAC_CHANNEL_1, &dac_channel_config) != HAL_OK)
  {
    return NULL;
  }

  /****************************************************************************/
  /* Configuration of GPIO                                                    */
  /****************************************************************************/

  HAL_RCC_GPIOA_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    DAC1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PA4     ------>   DAC1_OUT1
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ANALOG;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_4, &gpio_config);

  return &hDAC1;
}

void mx_dac1_deinit(void)
{
  (void)HAL_DAC_DeInit(&hDAC1);

  HAL_RCC_DAC1_DisableClock();

  HAL_RCC_DAC1_Reset();

  /* De-initialize all GPIO pins associated with DAC1 */
  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_4);
}

hal_dac_handle_t *mx_dac1_gethandle(void)
{
  return &hDAC1;
}
