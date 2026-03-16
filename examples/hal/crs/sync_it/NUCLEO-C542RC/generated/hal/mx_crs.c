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
/* Exported functions for CRS in HAL layer (SW instance MyCRS_1) */
/******************************************************************************/
hal_crs_handle_t *mx_crs_init(void)
{
  hal_crs_config_t crs_config;

  if (HAL_CRS_Init(&hCRS, HAL_CRS1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_CRS_EnableClock();

  crs_config.divider                = HAL_CRS_SYNC_DIV1;
  crs_config.source                 = HAL_CRS_SYNC_SOURCE_LSE;
  crs_config.polarity               = HAL_CRS_SYNC_POLARITY_FALLING;
  crs_config.reload                 = 0x5B7;
  crs_config.frequency_error_limit  = 0x22;
  crs_config.auto_trimming          = HAL_CRS_AUTO_TRIMMING_DISABLE;
  crs_config.trimming               = 0x30;

  HAL_CRS_SetConfig(&hCRS, &crs_config);

  /* No GPIO configuration required for CRS */
  /* Enable the interruption for CRS */
  HAL_CORTEX_NVIC_SetPriority(CRS_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(CRS_IRQn);
  return &hCRS;
}

void mx_crs_deinit(void)
{
  (void)HAL_CRS_DeInit(&hCRS);

  /* No GPIO de-initialization required for CRS */

  /* Disable the interruption for CRS */
  HAL_CORTEX_NVIC_DisableIRQ(CRS_IRQn);

  HAL_RCC_CRS_Reset();

  HAL_RCC_CRS_DisableClock();
}

hal_crs_handle_t *mx_crs_gethandle(void)
{
  return &hCRS;
}

/******************************************************************************/
/*                   Clock Recovery System global interrupt                   */
/******************************************************************************/
void CRS_IRQHandler(void)
{
  HAL_CRS_IRQHandler(&hCRS);
}
