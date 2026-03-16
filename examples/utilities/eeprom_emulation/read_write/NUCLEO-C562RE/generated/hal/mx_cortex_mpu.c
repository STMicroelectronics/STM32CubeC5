/**
  ******************************************************************************
  * @file           : mx_cortex_mpu.c
  * @brief          : STM32 CORTEX MPU body
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
#include "mx_cortex_mpu.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
system_status_t mx_cortex_mpu_hal_init(void)
{
  hal_cortex_mpu_region_config_t config;

  HAL_CORTEX_MPU_Disable();

  HAL_CORTEX_MPU_SetCacheMemAttr(HAL_CORTEX_MPU_MEM_ATTR_0, HAL_CORTEX_MPU_NORMAL_MEM_NCACHEABLE);

  config.base_addr = 0x08FFF000;
  config.limit_addr = 0x08FFFFFF;
  config.access_attr = HAL_CORTEX_MPU_REGION_ALL_RO;
  config.exec_attr = HAL_CORTEX_MPU_EXECUTION_ATTR_DISABLE;
  config.attr_idx = HAL_CORTEX_MPU_MEM_ATTR_0;

  HAL_CORTEX_MPU_SetConfigRegion(HAL_CORTEX_MPU_REGION_0, &config);
  HAL_CORTEX_MPU_EnableRegion(HAL_CORTEX_MPU_REGION_0);

  config.base_addr = FLASH_EDATA_BASE;
  config.limit_addr = FLASH_EDATA_BASE + (2 * FLASH_EDATA_BANK_SIZE) - 1;
  config.access_attr = HAL_CORTEX_MPU_REGION_ALL_RW;

  HAL_CORTEX_MPU_SetConfigRegion(HAL_CORTEX_MPU_REGION_1, &config);
  HAL_CORTEX_MPU_EnableRegion(HAL_CORTEX_MPU_REGION_1);

  HAL_CORTEX_MPU_Enable(HAL_CORTEX_MPU_HARDFAULT_NMI_ENABLE, HAL_CORTEX_MPU_ACCESS_FAULT_ONLY_PRIV);

  return SYSTEM_OK;
}

