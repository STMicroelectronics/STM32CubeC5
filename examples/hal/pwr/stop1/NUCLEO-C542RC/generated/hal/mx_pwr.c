/**
  ******************************************************************************
  * @file           : mx_pwr.c
  * @brief          : PWR Peripheral initialization
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
#include "mx_pwr.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/

/******************************************************************************/
/* Exported functions for PWR in HAL layer (SW instance PWR) */
/******************************************************************************/

system_status_t mx_pwr_init(void)
{
  /****************************************************************************/
  /* Configuration of RAM retention                                           */
  /****************************************************************************/
  /* By default, all RAM retention is enabled */
  /* For SRAM2 in STOP mode, memory retention is disabled on all pages */
  HAL_PWR_LP_DisableMemoryPageRetention(HAL_PWR_MEMORY_RETENTION_SRAM2_STOP, 1, 2);

  /****************************************************************************/
  /* Configuration of FLASH low power mode                                    */
  /****************************************************************************/
  HAL_PWR_LP_EnableFlashLowPWRMode();

  return SYSTEM_OK;
}

system_status_t mx_pwr_deinit(void)
{
  return SYSTEM_OK;
}
