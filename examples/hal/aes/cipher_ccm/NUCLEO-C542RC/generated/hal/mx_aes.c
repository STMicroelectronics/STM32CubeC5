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
    0x40414243, 0x44454647, 0x48494A4B, 0x4C4D4E4F
  };
uint32_t BlockB0[4] =
  {
    0x56101112, 0x13141516, 0x17000000, 0x00000010
  };
uint32_t BlockHeader[8] =
  {
    0x00100001, 0x02030405, 0x06070809, 0x0A0B0C0D,
    0x0E0F0000, 0x00000000, 0x00000000, 0x00000000
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

  hal_aes_ccm_config_t p_ccm_config;
  p_ccm_config.p_b0                 = BlockB0;
  p_ccm_config.p_header             = BlockHeader;
  p_ccm_config.header_size_byte     = 18;

  if (HAL_AES_CCM_SetConfig(&hAES, &p_ccm_config) != HAL_OK)
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
