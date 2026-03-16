
/**
  ******************************************************************************
  * @file           : mx_ccb.c
  * @brief          : CCB Peripheral initialization
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
#include "mx_ccb.h"
#include "mx_rcc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_ccb_handle_t hCCB;

/******************************************************************************/
/* Exported functions for CCB in HAL layer (SW instance MyCCB_1) */
/******************************************************************************/
hal_ccb_handle_t *mx_ccb_init(void)
{
  if (HAL_CCB_Init(&hCCB, HAL_CCB) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_RNG_EnableClock();

  HAL_RCC_PKA_EnableClock();

  HAL_RCC_SAES_EnableClock();
  HAL_RCC_CCB_EnableClock();

  if (mx_rcc_ccb_clock_config() != SYSTEM_OK)
  {
    return NULL;
  }

  return &hCCB;
}

void mx_ccb_deinit(void)
{
  HAL_CCB_DeInit(&hCCB);

  HAL_RCC_CCB_Reset();

  HAL_RCC_CCB_DisableClock();
}

hal_ccb_handle_t *mx_ccb_gethandle(void)
{
  return &hCCB;
}
