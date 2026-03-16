
/**
  ******************************************************************************
  * @file           : mx_pka.c
  * @brief          : PKA Peripheral initialization
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
#include "mx_pka.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
hal_pka_handle_t hPKA;

/******************************************************************************/
/* Exported functions for PKA in HAL layer (SW instance MyPKA_1) */
/******************************************************************************/
hal_pka_handle_t *mx_pka_init(void)
{
  if (HAL_PKA_Init(&hPKA, HAL_PKA1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_RNG_EnableClock();

  HAL_RCC_RNG_EnableClock();

  HAL_RCC_PKA_EnableClock();

  if (HAL_RCC_CK48_SetKernelClkSource(HAL_RCC_CK48_CLK_SRC_HSIDIV3) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for RNG */
  HAL_CORTEX_NVIC_SetPriority(PKA_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(PKA_IRQn);
  return &hPKA;
}

void mx_pka_deinit(void)
{
  (void)HAL_PKA_DeInit(&hPKA);
}

hal_pka_handle_t *mx_pka_gethandle(void)
{
  return &hPKA;
}

/******************************************************************************/
/*                            PKA global interrupt                            */
/******************************************************************************/
void PKA_IRQHandler(void)
{
  HAL_PKA_IRQHandler(&hPKA);
}
