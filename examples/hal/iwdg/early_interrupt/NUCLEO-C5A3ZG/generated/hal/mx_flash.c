/**
  ******************************************************************************
  * @file           : mx_flash.c
  * @brief          : FLASH Peripheral initialization
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
#include "mx_flash.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_flash_handle_t hFLASH;

/******************************************************************************/
/*     Exported functions for FLASH in HAL layer (SW instance MyFLASH_1)      */
/******************************************************************************/
hal_flash_handle_t *mx_flash_init(void)
{
  if (HAL_FLASH_Init(&hFLASH, HAL_FLASH) != HAL_OK)
  {
   return NULL;
  }

  return &hFLASH;
}

void mx_flash_deinit(void)
{
  HAL_FLASH_DeInit(&hFLASH);
}

hal_flash_handle_t *mx_flash_gethandle(void)
{
  return &hFLASH;
}

/******************************************************************************/
/*           FLASH NMI interrupt is managed directly in user code.            */
/******************************************************************************/
__WEAK system_status_t FLASH_NMI_IRQHandler(void)
{
  /* NOTE : This function must not be modified in this file, when the IRQHandler is needed,
            the function must preferably be implemented in the user file
   */
  return SYSTEM_PERIPHERAL_ERROR;
}
