/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : WWDG refresh and watchdog-based intentional MCU reset with LL API
  ******************************************************************************
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define WWDG_PRESCALER_VALUE 7UL
#define WWDG_RELOAD_VALUE    127UL
#define WWDG_WINDOW_VALUE    127UL
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  *  brief: Check if RCC MX_WWDGx reset flag is set or not.
  */
inline uint32_t WWDG_CheckWWDGReset(void)
{
  return (LL_RCC_IsActiveFlag_WWDGRST());
} /* WWDG_CheckWWDGReset */


/**
  *  brief: Clear the MX_WWDGx reset flags.
  */
inline void WWDG_ClearResetFlag(void)
{
  LL_RCC_ForceClearResetFlags();
  LL_RCC_ReleaseClearResetFlags();
} /* WWDG_ClearResetFlag */


/**
  *  brief: Start the MX_WWDGx
  */
inline void WWDG_Start(void)
{
  /* Set the MX_WWDGx reload value */
  LL_WWDG_SetControl(MX_WWDGx, WWDG_RELOAD_VALUE);

  /* Set the MX_WWDGx prescaler and window values */
  LL_WWDG_SetConfig(MX_WWDGx, WWDG_PRESCALER_VALUE, WWDG_WINDOW_VALUE);
} /* WWDG_Start */


/**
  *  Refresh the MX_WWDGx counter.
  *  param: counter value to count before reset.
  */
inline void WWDG_RefreshCounter(uint32_t counter)
{
  /* Set the MX_WWDGx counter value */
  LL_WWDG_SetCounter(MX_WWDGx, counter);

} /* WWDG_RefreshCounter */


/**
  * brief: This function provides delay (in milliseconds) based on SysTick counter flag.
  * param: delay_ms Number of milliseconds to delay.
  */
inline void DelayMs(uint32_t delay_ms)
{
  LL_Delay_NoISR(delay_ms);
} /* DelayMs */
