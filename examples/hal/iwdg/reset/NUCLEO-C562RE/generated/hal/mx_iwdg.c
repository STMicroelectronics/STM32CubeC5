/**
  ******************************************************************************
  * @file           : mx_iwdg.c
  * @brief          : IWDG Peripheral initialization
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
#include "mx_iwdg.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_iwdg_handle_t hIWDG;

/******************************************************************************/
/* Exported functions for IWDG in HAL layer (SW instance MyIWDG_1) */
/******************************************************************************/
hal_iwdg_handle_t *mx_iwdg_init(void)
{
  if (HAL_IWDG_Init(&hIWDG, HAL_IWDG1) != HAL_OK)
  {
    return NULL;
  }

  return &hIWDG;
}

/*
  Start the Independent Watchdog (IWDG) with the following parameters:
  - Maximum time before watchdog reset   : 131072 milliseconds
  - Minimum time before refresh (window) :  0 milliseconds
  - Early Wakeup Interrupt (EWI) time    :  0 milliseconds
*/
hal_status_t mx_iwdg_start(void)
{
  return HAL_IWDG_Start(&hIWDG, 0UL, 131072UL, 0UL);
}

hal_iwdg_handle_t *mx_iwdg_iwdg_gethandle(void)
{
  return &hIWDG;
}
