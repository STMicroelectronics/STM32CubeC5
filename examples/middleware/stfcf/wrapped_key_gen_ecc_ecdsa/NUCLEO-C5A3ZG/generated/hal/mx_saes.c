/**
  ******************************************************************************
  * @file           : mx_saes.c
  * @brief          : SAES Peripheral initialization
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
#include "mx_saes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_aes_handle_t hSAES;

/******************************************************************************/
/* Exported functions for SAES in HAL layer (SW instance MyAES_1) */
/******************************************************************************/
hal_aes_handle_t *mx_saes_init(void)
{
  if (HAL_AES_Init(&hSAES, HAL_SAES) != HAL_OK)
  {
    return NULL;
  }

  if (HAL_RCC_CK48_SetKernelClkSource(HAL_RCC_CK48_CLK_SRC_HSIDIV3) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_RNG_EnableClock();

  HAL_RCC_SAES_EnableClock();

  return &hSAES;
}

void mx_saes_deinit(void)
{
  (void)HAL_AES_DeInit(&hSAES);
}

hal_aes_handle_t *mx_saes_gethandle(void)
{
  return &hSAES;
}
