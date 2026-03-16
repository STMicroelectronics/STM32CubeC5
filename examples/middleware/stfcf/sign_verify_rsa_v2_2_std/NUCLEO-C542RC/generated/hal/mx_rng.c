/**
  ******************************************************************************
  * @file           : mx_rng.c
  * @brief          : RNG Peripheral initialization
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
#include "mx_rng.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_rng_handle_t hRNG;
/******************************************************************************/
/* Exported functions for RNG in HAL layer (SW instance MyRNG_1) */
/******************************************************************************/
hal_rng_handle_t *mx_rng_init(void)
{
  if (HAL_RCC_CK48_SetKernelClkSource(HAL_RCC_CK48_CLK_SRC_HSIDIV3) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_RNG_EnableClock();

  if (HAL_RNG_Init(&hRNG, HAL_RNG) != HAL_OK)
  {
    return NULL;
  }

  /* Code HAL_RNG_Config....*/
  HAL_RNG_SetCandidateNISTConfig(&hRNG);

  return &hRNG;
}

void mx_rng_deinit(void)
{
  (void)HAL_RNG_DeInit(&hRNG);

  HAL_RCC_RNG_Reset();

  HAL_RCC_RNG_DisableClock();
}

hal_rng_handle_t *mx_rng_gethandle(void)
{
  return &hRNG;
}
