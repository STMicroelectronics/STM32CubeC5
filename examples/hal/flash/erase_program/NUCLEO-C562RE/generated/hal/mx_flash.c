/**
  ******************************************************************************
  * @file    mx_flash.c
  * @brief   Initialisation of all the peripherals used in this project
  *          File per peripheral instance that is handling the peripheral
  *          initialization and the IRQ handlers. Init parameters provided
  *          as a set of defines configurable/generated for a given target
  *          through STM32CubeMX2.
  *
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

static hal_flash_handle_t gh_flash;

hal_flash_handle_t *flash_instance0_cfg1_init(void)
{
  /* Init Flash */
  if (HAL_FLASH_Init(&gh_flash, HAL_FLASH) != HAL_OK)
  {
    return NULL;
  }
  else
  {
    return &gh_flash;
  }
}

void flash_deinit(void)
{
  (void)HAL_FLASH_DeInit(&gh_flash);

}

hal_flash_handle_t *flash_gethandle(void)
{
  return &gh_flash;
}
