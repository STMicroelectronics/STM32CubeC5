/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : RCC clock switch max default
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
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define RCC_HSI_TIMEOUT_VALUE         2UL
#define RCC_CLOCKSWITCH_TIMEOUT_VALUE 5000UL
#define PSI_NOT_LSE_144 144000000U /* PSI output frequency exactly 144 MHz without LSE as PSI clock source  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  *  Reset the MX_RCCx instance clock configuration to the default system clock (HSIDIV3 at 48 MHz).
  *  SYSTEM_PERIPHERAL_ERROR MX_RCCx reset error
  *  SYSTEM_OK               MX_RCCx reset success
  */
system_status_t RCC_ResetSystemClock(void)
{
  system_status_t return_status = SYSTEM_OK;
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
  /* HSI timeout value: 2 ms (minimum Tick + 1) */
  uint32_t timeout_ms = RCC_HSI_TIMEOUT_VALUE;
#endif /* USE_LL_APP_TIMEOUT */

  LL_RCC_HSIDIV3_Enable();
  while (LL_RCC_HSIDIV3_IsReady() != 1)
  {
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
    if (SysTick_IsActiveCounterFlag() != 0)
    {
      if (timeout_ms-- == 0)
      {
        return_status = SYSTEM_PERIPHERAL_ERROR;
        break;
      }
    }
#endif /* USE_LL_APP_TIMEOUT */
  }
  /* Set the CPU bus clock source (SYSCLK). */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSIDIV3);

#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
  /* Clock switch timeout value: 5 s */
  timeout_ms = RCC_CLOCKSWITCH_TIMEOUT_VALUE;
#endif /* USE_LL_APP_TIMEOUT */
  /* Wait till clock switch is ready */
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSIDIV3)
  {
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
    if (SysTick_IsActiveCounterFlag() != 0)
    {
      if (timeout_ms-- == 0)
      {
        return_status = SYSTEM_PERIPHERAL_ERROR;
        break;
      }
    }
#endif /* USE_LL_APP_TIMEOUT */
  }

  return return_status;
} /* RCC_ResetSystemClock */


/**
  *  MX_RCCx disable oscillator.
  */
void RCC_DisableOscillator(void)
{
  /* Disable Oscillator */
  LL_RCC_PSIS_Disable();
}


/**
  * Wait for approximate delay in milliseconds.
  * @warning It is only an approximate delay and can not be used where precise timing is required.
  */
inline void DelayMs(uint32_t delay_ms)
{
  LL_Delay_NoISR(delay_ms);
}


/**
  *  Get current SYSCLK frequency based on the active system clock source.
  *  Informative-only: value is read for user information/logging and
  *  does not influence runtime behavior.
  *  @return Frequency in Hz
  */
uint32_t RCC_GetSYSCLKFreq(void)
{
  uint32_t sysclockfreq;
  uint32_t sysclk_source;

  sysclk_source = LL_RCC_GetSysClkSource();

  if (sysclk_source == LL_RCC_SYS_CLKSOURCE_STATUS_HSIS)
  {
    /* HSIS used as system clock source */
    sysclockfreq = HSI_VALUE;
  }
  else if (sysclk_source == LL_RCC_SYS_CLKSOURCE_STATUS_HSIDIV3)
  {
    /* HSIS used as system clock source */
    sysclockfreq = HSI_VALUE / 3U;
  }
  else
  {
    sysclockfreq = PSI_NOT_LSE_144;
  }
  return sysclockfreq;
}
