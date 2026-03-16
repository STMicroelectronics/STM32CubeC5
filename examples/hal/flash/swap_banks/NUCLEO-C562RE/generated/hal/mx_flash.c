/**
  ******************************************************************************
  * @file : mx_flash.c
  * @brief : FLASH Peripheral initialization
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
/* Exported functions for FLASH in HAL layer (SW instance MyFLASH_1) */
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

