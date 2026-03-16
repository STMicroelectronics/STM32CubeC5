/**
  ******************************************************************************
  * @file           : mx_hash.c
  * @brief          : HASH Peripheral initialization
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
#include "mx_hash.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_hash_handle_t hHASH;

/******************************************************************************/
/* Exported functions for HASH in HAL layer (SW instance MyHASH_1) */
/******************************************************************************/
hal_hash_handle_t *mx_hash_init(void)
{
  hal_hash_config_t config;

  if (HAL_HASH_Init(&hHASH, HAL_HASH) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_HASH_EnableClock();
  config.data_swapping = HAL_HASH_DATA_SWAP_NO;
  config.algorithm     = HAL_HASH_ALGO_SHA1;
  if (HAL_HASH_SetConfig(&hHASH, &config) != HAL_OK)
  {
    return NULL;
  }

  return &hHASH;
}

void mx_hash_deinit(void)
{
  (void)HAL_HASH_DeInit(&hHASH);

  HAL_RCC_HASH_Reset();

  HAL_RCC_HASH_DisableClock();
}

hal_hash_handle_t *mx_hash_gethandle(void)
{
  return &hHASH;
}
