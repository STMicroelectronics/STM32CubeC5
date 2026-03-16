/**
  ******************************************************************************
  * @file           : mx_wwdg.c
  * @brief          : WWDG Peripheral initialization
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
#include "mx_wwdg.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_wwdg_handle_t hWWDG;

/******************************************************************************/
/* Exported functions for WWDG in HAL layer (SW instance MyWWDG_1) */
/******************************************************************************/
hal_wwdg_handle_t *mx_wwdg_init(void)
{
  if (HAL_WWDG_Init(&hWWDG, HAL_WWDG1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_WWDG_EnableClock();

  return &hWWDG;
}

hal_status_t mx_wwdg_start(void)
{
  return HAL_WWDG_Start(&hWWDG, 0UL, 233UL, 0UL);
}

hal_wwdg_handle_t *mx_wwdg_wwdg_gethandle(void)
{
  return &hWWDG;
}
