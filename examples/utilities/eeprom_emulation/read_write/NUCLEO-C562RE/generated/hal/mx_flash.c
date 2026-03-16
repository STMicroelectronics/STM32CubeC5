/**
  ******************************************************************************
  * @file           : mx_flash.c
  * @brief          : Flash initialisation
  ******************************************************************************
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

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables  -------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported variables by reference -------------------------------------------*/
/*flash handle*/

static hal_flash_handle_t hFLASH;

hal_flash_handle_t *flash_instance0_cfg1_init(void)
{
  /* Init Flash */
  if (HAL_FLASH_Init(&hFLASH, HAL_FLASH) != HAL_OK)
  {
    return NULL;
  }
  else
  {
    return &hFLASH;
  }
}

void flash_deinit(void)
{
  (void)HAL_FLASH_DeInit(&hFLASH);
}

hal_flash_handle_t *mx_hal_flash_gethandle(void)
{
  return &hFLASH;
}
