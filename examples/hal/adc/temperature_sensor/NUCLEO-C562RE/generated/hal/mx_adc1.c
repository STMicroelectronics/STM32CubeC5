/**
  ******************************************************************************
  * @file           : mx_adc1.c
  * @brief          : ADC1 Peripheral initialization
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
#include "mx_adc1.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_adc_handle_t hADC1;

/******************************************************************************/
/* Exported functions for ADC1 in HAL layer (SW instance MyADC_1) */
/******************************************************************************/
hal_adc_handle_t *mx_adc1_init(void)
{
  HAL_RCC_ADC12_EnableClock();

  if (HAL_RCC_ADCDAC_SetKernelClkSource(HAL_RCC_ADCDAC_CLK_SRC_PSIS) != HAL_OK)
  {
    return NULL;
  }

  if (HAL_ADC_Init(&hADC1, HAL_ADC1) != HAL_OK)
  {
    return NULL;
  }

  hal_adc_config_t adc_config;
  hal_adc_channel_config_t adc_channel_config;

  adc_config.resolution          = HAL_ADC_RESOLUTION_12_BIT;
  adc_config.sampling_mode       = HAL_ADC_SAMPLING_MODE_NORMAL;
  HAL_ADC_SetConfig(&hADC1, &adc_config);

  /****************************************************************************/
  /* Configuration of basic features (mandatory)                              */
  /****************************************************************************/

/* ==================== Group Regular ====================*/
  hal_adc_reg_config_t reg_config;
  reg_config.trigger_src        = HAL_ADC_REG_TRIG_SOFTWARE;
  reg_config.sequencer_length   = 1;
  reg_config.sequencer_discont  = HAL_ADC_REG_SEQ_DISCONT_DISABLE;
  reg_config.continuous         = HAL_ADC_REG_CONV_SINGLE;
  reg_config.overrun            = HAL_ADC_REG_OVR_DATA_OVERWRITTEN;
  HAL_ADC_REG_SetConfig(&hADC1, &reg_config);

  adc_channel_config.group           = HAL_ADC_GROUP_REGULAR;
  adc_channel_config.sequencer_rank  = 1;
  adc_channel_config.sampling_time   = HAL_ADC_SAMPLING_TIME_289CYCLES;
  adc_channel_config.input_mode      = HAL_ADC_IN_SINGLE_ENDED;
  HAL_ADC_SetConfigChannel(&hADC1, HAL_ADC_CHANNEL_TEMPSENSOR, &adc_channel_config);

  /****************************************************************************/
  /* Configuration of additional features (optional)                          */
  /****************************************************************************/

  /* No GPIO configuration required for ADC1 */

  return &hADC1;
}

void mx_adc1_deinit(void)
{
  (void)HAL_ADC_DeInit(&hADC1);

  /* No GPIO de-initialization required for ADC1 */
}

hal_adc_handle_t *mx_adc1_gethandle(void)
{
  return &hADC1;
}
