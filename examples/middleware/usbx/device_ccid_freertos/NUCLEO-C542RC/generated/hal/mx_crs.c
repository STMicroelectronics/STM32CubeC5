/**
  ******************************************************************************
  * @file           : mx_crs.c
  * @brief          : CRS Peripheral initialization
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
#include "mx_crs.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_crs_handle_t hCRS;
/******************************************************************************/
/* Exported functions for CRS in HAL layer */
/******************************************************************************/
hal_crs_handle_t *mx_crs_init(void)
{
  if (HAL_CRS_Init(&hCRS, HAL_CRS1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_CRS_EnableClock();

  return &hCRS;
}

void mx_crs_deinit(void)
{
  (void)HAL_CRS_DeInit(&hCRS);

  HAL_RCC_CRS_Reset();

  HAL_RCC_CRS_DisableClock();
}

hal_crs_handle_t *mx_crs_gethandle(void)
{
  return &hCRS;
}
