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
uint32_t InitVector[4] =
  {
    0x00000000, 0x00000000, 0x00000000, 0x00000002
  };
uint32_t HeaderMessage[1] =
  {
    0x00
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

  hal_aes_gcm_config_t p_gcm_config;
  p_gcm_config.p_init_vect          = InitVector;
  p_gcm_config.p_header             = HeaderMessage;
  p_gcm_config.header_size_byte     = 1;

  if (HAL_AES_GCM_GMAC_SetConfig(&hAES, &p_gcm_config) != HAL_OK)
  {
    return NULL;
  }
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
