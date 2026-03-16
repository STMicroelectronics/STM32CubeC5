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

/******************************************************************************/
/* Exported functions for WWDG in LL layer (SW instance MyWWDG_1) */
/******************************************************************************/
WWDG_TypeDef *mx_wwdg_init(void)
{
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_WWDG);

  return WWDG;
}

system_status_t mx_wwdg_start(void)
{
  /* These values are calculated from the ones inputted by the user in the WWDG panel. */
  uint32_t prescaler = 7UL;
  uint32_t reload = 127UL;
  uint32_t window = 127UL;
  uint32_t early_wakeup = 0U;

  if (early_wakeup != 0U)
  {
    LL_WWDG_EnableIT_EWKUP(WWDG);
  }

  /* Enable the WWDG driver and set the reload value to work with */
  LL_WWDG_SetControl(WWDG, reload);

  /* Write to WWDG CFR register the Prescaler and Window values to work with */
  LL_WWDG_SetConfig(WWDG, prescaler << WWDG_CFR_WDGTB_Pos, window);

  return SYSTEM_OK;
}
