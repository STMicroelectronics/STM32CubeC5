/**
  ******************************************************************************
  * @file           : mx_crc.c
  * @brief          : CRC Peripheral initialization
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
#include "mx_crc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_crc_handle_t hCRC;

/******************************************************************************/
/* Exported functions for CRC (SW instance MyCRC_1 in HAL) layer */
/******************************************************************************/
hal_crc_handle_t *mx_crc_init(void)
{
  hal_crc_config_t crc_config;
  HAL_RCC_CRC_EnableClock();
  if (HAL_CRC_Init(&hCRC, HAL_CRC) != HAL_OK)
  {
    return NULL;
  }

  crc_config.polynomial_coefficient   = CRC_POLYNOMIAL_16B;
  crc_config.polynomial_size          = HAL_CRC_POLY_SIZE_16B;
  crc_config.crc_init_value           = 0x0;
  crc_config.input_data_reverse_mode  = HAL_CRC_INDATA_REVERSE_NONE;
  crc_config.output_data_reverse_mode = HAL_CRC_OUTDATA_REVERSE_NONE;
  HAL_CRC_SetConfig(&hCRC, &crc_config);

  return &hCRC;
}

void mx_crc_deinit(void)
{
  (void)HAL_CRC_DeInit(&hCRC);
  HAL_RCC_CRC_Reset();

  HAL_RCC_CRC_DisableClock();
}

hal_crc_handle_t *mx_crc_gethandle(void)
{
  return &hCRC;
}
