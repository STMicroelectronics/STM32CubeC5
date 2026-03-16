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
/** Exported functions for RTC in LL layer (SW instance MyRTC_1)            **/
/******************************************************************************/

system_status_t mx_rtc_init(void)
{
  /* Disable RTC Domain Write Protection */
  LL_PWR_DisableRTCDomainWriteProtection();

  /* Clock configuration */
  LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);

  LL_APB3_GRP1_EnableClock(LL_APB3_GRP1_PERIPH_RTCAPB);
  LL_RCC_EnableRTC();

  /* Disable write protection */
  LL_RTC_DisableWriteProtection();

  /* Enter init mode */
  uint32_t timeout = 1000;
  if (LL_RTC_IsActiveFlag_INIT() == 0U)
  {
    LL_RTC_EnableInitMode();
    while (LL_RTC_IsActiveFlag_INIT() != 1)
    {
      LL_Delay_NoISR(1);
      timeout--;
      if (timeout == 0)
      {
        return SYSTEM_PERIPHERAL_ERROR;
      }
    }
  }

  /* Configure mode and RTC prescalers */
  uint32_t asynchronous_prescaler = 128 - 1;
  uint32_t synchronous_prescaler = 256 - 1;
  LL_RTC_SetPrescalers(asynchronous_prescaler, synchronous_prescaler);
  LL_RTC_SetBinaryMode(LL_RTC_BINARY_NONE);

  /* Exit init mode */
  timeout = 1000;
  LL_RTC_DisableInitMode();
  if (LL_RTC_IsEnabledBypassShadowReg() == 0U)
  {
    if (LL_RTC_IsActiveFlag_INIT() == 0U)
    {
      LL_RTC_ClearFlag_RS();
      while (LL_RTC_IsActiveFlag_RS() == 0U)
      {
        LL_Delay_NoISR(1) ;
        timeout--;
        if (timeout == 0)
        {
          return SYSTEM_PERIPHERAL_ERROR;
        }
      }
    }
  }

  /* Enable write protection */
  LL_RTC_EnableWriteProtection();

  return SYSTEM_OK;
}

void mx_rtc_deinit(void)
{
}
