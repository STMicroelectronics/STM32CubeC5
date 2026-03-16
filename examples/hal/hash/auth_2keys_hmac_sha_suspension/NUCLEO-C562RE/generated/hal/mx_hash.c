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
uint8_t input_key[40] = { 0xCF, 0xD4, 0xA4, 0x49, 0x10, 0xC9, 0xE5, 0x67, 0x50, 0x7A, 0xBB, 0x6C, 0xED, 0xE4, 0xFE, 0x60, 0x1A, 0x7A, 0x27, 0x65, 0xC9, 0x75, 0x5A, 0xA2, 0xCF, 0x6B, 0xA4, 0x81, 0x42, 0x23, 0x81, 0x1A, 0x26, 0xA8, 0xA1, 0xEF, 0x49, 0x9C, 0xEB, 0xD9 };

/******************************************************************************/
/* Exported functions for HASH in HAL layer (SW instance MyHASH_1) */
/******************************************************************************/
hal_hash_handle_t *mx_hash_init(void)
{
  hal_hash_hmac_config_t config;

  if (HAL_HASH_Init(&hHASH, HAL_HASH) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_HASH_EnableClock();

  config.data_swapping = HAL_HASH_DATA_SWAP_BYTE;
  config.algorithm     = HAL_HASH_ALGO_SHA256;
  config.p_hmac_key         = input_key;
  config.key_size_byte = 40;
  if (HAL_HASH_HMAC_SetConfig(&hHASH, &config) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for HASH */
  HAL_CORTEX_NVIC_SetPriority(HASH_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(HASH_IRQn);
  return &hHASH;
}

void mx_hash_deinit(void)
{
  /* Disable the interuption for HASH */
  HAL_CORTEX_NVIC_DisableIRQ(HASH_IRQn);

  (void)HAL_HASH_DeInit(&hHASH);

  HAL_RCC_HASH_Reset();

  HAL_RCC_HASH_DisableClock();
}

hal_hash_handle_t *mx_hash_gethandle(void)
{
  return &hHASH;
}

/******************************************************************************/
/*                               HASH interrupt                               */
/******************************************************************************/
void HASH_IRQHandler(void)
{
  HAL_HASH_IRQHandler(&hHASH);
}
