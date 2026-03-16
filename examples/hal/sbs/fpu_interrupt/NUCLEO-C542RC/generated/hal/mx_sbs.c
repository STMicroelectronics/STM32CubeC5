/**
  ******************************************************************************
  * @file           : mx_sbs.c
  * @brief          : SBS Peripheral initialization
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

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "mx_sbs.h"

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/* Private functions prototype----------------------------------------------------------------------------------------*/
/* Exported variables by reference------------------------------------------------------------------------------------*/
/* Exported function definition --------------------------------------------------------------------------------------*/
/******************************************************************************/
/* Exported functions for SBS in HAL layer */
/******************************************************************************/
system_status_t mx_sbs_init(void)
{
  HAL_RCC_SBS_EnableClock();

  return SYSTEM_OK;
}

system_status_t mx_sbs_deinit(void)
{
  HAL_RCC_SBS_Reset();

  HAL_RCC_SBS_DisableClock();

  return SYSTEM_OK;
}
