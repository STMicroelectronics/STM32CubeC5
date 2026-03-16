/**
  ******************************************************************************
  * @file           : mx_crc.c
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
#include "mx_crc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_crc_handle_t gh_crc;

hal_crc_handle_t *crc_instance0_cfg1_init(void)
{
  hal_crc_config_t crc_config;

  HAL_RCC_CRC_EnableClock();

  if (HAL_CRC_Init(&gh_crc, HAL_CRC) != HAL_OK)
  {
    return NULL;
  }

  crc_config.polynomial_coefficient   = CRC_POLYNOMIAL_16B;
  crc_config.polynomial_size          = HAL_CRC_POLY_SIZE_16B;
  crc_config.crc_init_value           = 0xFFFFFFFF;
  crc_config.input_data_reverse_mode  = HAL_CRC_INDATA_REVERSE_NONE;
  crc_config.output_data_reverse_mode = HAL_CRC_OUTDATA_REVERSE_NONE;
  HAL_CRC_SetConfig(&gh_crc, &crc_config);

  return &gh_crc;
}

void crc_deinit(void)
{
  (void)HAL_CRC_DeInit(&gh_crc);

  HAL_RCC_CRC_DisableClock();
}

hal_crc_handle_t *mx_crc_hal_crc_gethandle(void)
{
  return &gh_crc;
}
