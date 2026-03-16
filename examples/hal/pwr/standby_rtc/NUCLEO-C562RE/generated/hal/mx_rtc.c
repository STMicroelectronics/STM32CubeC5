/**
  ******************************************************************************
  * @file           : mx_rtc.c
  * @brief          : RTC Peripheral initialization
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
#include "mx_rtc.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/******************************************************************************/
/** Exported functions for RTC in HAL layer (SW instance MyRTC_1)            **/
/******************************************************************************/
system_status_t mx_rtc_init(void)
{
  /* Disable RTC Domain Write Protection */
  HAL_PWR_DisableRTCDomainWriteProtection();

  /* Clock configuration */
  if (HAL_RCC_RTC_SetKernelClkSource(HAL_RCC_RTC_CLK_SRC_LSE) != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  HAL_RCC_RTCAPB_EnableClock();

  HAL_RCC_RTC_EnableKernelClock();

  /* Disable write protection */
  if (HAL_RTC_DisableWriteProtection() != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  /* Enter init mode */
  if (HAL_RTC_EnterInitMode() != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  /* Configure mode and RTC prescalers */
  hal_rtc_config_t rtc_config;
  rtc_config.mode = HAL_RTC_MODE_BCD;
  rtc_config.asynch_prediv = 7 - 1;
  rtc_config.synch_prediv = 255 - 1;
  if (HAL_RTC_SetConfig(&rtc_config) != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  /* Exit init mode */
  if (HAL_RTC_ExitInitMode() != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  /* Configure wake-up timer */
  hal_rtc_wakeup_config_t wakeup_config;
  wakeup_config.clock = HAL_RTC_WAKEUP_TIMER_CLOCK_RTCCLK_DIV16;
  if (HAL_RTC_WAKEUP_SetConfig(&wakeup_config) != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  uint32_t wakeup_reload_time = 10000U;
  uint32_t wakeup_clear_time = 0U;
  if (HAL_RTC_WAKEUP_SetAutoReloadAndAutoClear(wakeup_reload_time, wakeup_clear_time) != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  /* Enable write protection */
  if (HAL_RTC_EnableWriteProtection() != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  /* No GPIO configuration required for RTC */

  HAL_CORTEX_NVIC_SetPriority(RTC_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(RTC_IRQn);

  return SYSTEM_OK;
}

void mx_rtc_deinit(void)
{
  /* No GPIO de-initialization required for RTC */
  HAL_CORTEX_NVIC_DisableIRQ(RTC_IRQn);
}

 /******************************************************************************/
/**                     RTC global non-secure interrupts                     **/
/******************************************************************************/
void RTC_IRQHandler(void)
{
  HAL_RTC_IRQHandler();
}
