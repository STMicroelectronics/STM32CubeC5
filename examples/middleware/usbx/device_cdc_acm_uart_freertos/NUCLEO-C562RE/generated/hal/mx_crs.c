/**
  ******************************************************************************
  * file    mx_crs.c
  * brief   Initialisation of all the peripherals used in this project
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
#include "mx_crs.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported variables by reference -------------------------------------------*/
static hal_crs_handle_t gh_crs;

/* CRS initialization functions */
hal_status_t crs_init(void)
{
  hal_crs_config_t h_crs_config;

  HAL_CRS_Init(&gh_crs, HAL_CRS1);

  /** Configures CRS */
  h_crs_config.divider = HAL_CRS_SYNC_DIV1;
  h_crs_config.source = HAL_CRS_SYNC_SOURCE_USB;
  h_crs_config.polarity = HAL_CRS_SYNC_POLARITY_RISING;
  h_crs_config.reload = HAL_CRS_CALCULATE_RELOAD(48000000, 1000);
  h_crs_config.frequency_error_limit = 34;
  h_crs_config.trimming = 32;

  HAL_CRS_SetConfig(&gh_crs, &h_crs_config);

  return HAL_OK;
}


hal_status_t crs_deinit(void)
{

  HAL_CRS_DeInit(&gh_crs);

  return HAL_OK;
}
