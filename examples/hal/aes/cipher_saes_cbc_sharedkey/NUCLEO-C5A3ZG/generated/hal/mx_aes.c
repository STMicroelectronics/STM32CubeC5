/**
  ******************************************************************************
  * @file           : mx_aes.c
  * @brief          : AES Peripheral initialization
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
#include "mx_aes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_aes_handle_t hAES;

const uint32_t AESKey[4] =
  {
    0x2B7E1516, 0x28AED2A6, 0xABF71588, 0x09CF4F3C
  };

uint32_t AESIV[4] =
  {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
  };

/******************************************************************************/
/* Exported functions for AES in HAL layer (SW instance MyAES_1) */
/******************************************************************************/
hal_aes_handle_t *mx_aes_init(void)
{
  if (HAL_AES_Init(&hAES, HAL_AES) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_AES_EnableClock();

  if (HAL_AES_CBC_SetConfig(&hAES, AESIV) != HAL_OK)
  {
    return NULL;
  }

  HAL_AES_SetNormalKey(&hAES, HAL_AES_KEY_SIZE_128BIT, AESKey);
  return &hAES;
}

void mx_aes_deinit(void)
{
  (void)HAL_AES_DeInit(&hAES);

  HAL_RCC_AES_Reset();

  HAL_RCC_AES_DisableClock();
}

hal_aes_handle_t *mx_aes_gethandle(void)
{
  return &hAES;
}
