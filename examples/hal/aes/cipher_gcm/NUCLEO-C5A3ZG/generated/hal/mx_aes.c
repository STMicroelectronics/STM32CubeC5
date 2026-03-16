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

const uint32_t AESKey[8] =
  {
    0x463B4129, 0x11767D57, 0xA0B33969, 0xE674FFE7,
    0x845D313B, 0x88C6FE31, 0x2F3D724B, 0xE68E1FCA
  };
uint32_t InitVector[4] =
  {
    0x611CE6F9, 0xA6880750, 0xDE7DA6CB, 0x00000002
  };
uint32_t HeaderMessage[12] =
  {
    0x0A682FBC, 0x6192E1B4, 0x7A5E0868, 0x787FFDAF,
    0xE5A50CEA, 0xD3575849, 0x990CDD2E, 0xA9B35977,
    0x49403EFB, 0x4A56684F, 0x0C6BDE35, 0x2D4AEEC5
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
  p_gcm_config.header_size_byte     = 48;

  if (HAL_AES_GCM_GMAC_SetConfig(&hAES, &p_gcm_config) != HAL_OK)
  {
    return NULL;
  }

  HAL_AES_SetNormalKey(&hAES, HAL_AES_KEY_SIZE_256BIT, AESKey);
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
