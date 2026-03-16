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
  uint32_t asynchronous_prescaler = 127 - 1;
  uint32_t synchronous_prescaler = 255 - 1;
  LL_RTC_SetPrescalers(asynchronous_prescaler, synchronous_prescaler);
  LL_RTC_SetBinaryMode(LL_RTC_BINARY_NONE);

  /* Configure calendar */
  uint32_t calendar_date = 0x7U;
  uint32_t calendar_hours = 0x2U;
  uint32_t calendar_minutes = 0x20U;
  uint32_t calendar_seconds = 0x0U;

  LL_RTC_SetHourFormatAndShadowRegBypass(LL_RTC_HOUR_FORMAT_24HOUR, LL_RTC_SHADOW_REG_KEEP);
  LL_RTC_DATE_Config(LL_RTC_WEEKDAY_MONDAY, calendar_date, LL_RTC_MONTH_DECEMBER, 0x20);
  LL_RTC_TIME_Config(LL_RTC_TIME_FORMAT_AM_24H, calendar_hours, calendar_minutes, calendar_seconds);

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

  /* Configure alarm A */
  LL_RTC_ALM_SetBinAutoClr(LL_RTC_ALARM_A, LL_RTC_ALMA_SUBSECONDBIN_AUTOCLR_NO);
  LL_RTC_ALM_SetFlagAutoClr(LL_RTC_ALARM_A, LL_RTC_ALM_AUTOCLR_NO);

  uint32_t alarm_a_hours = 0x2U;
  uint32_t alarm_a_minutes = 0x20U;
  uint32_t alarm_a_seconds = 0x30U;
  uint32_t alarm_a_mask = LL_RTC_ALMA_MASK_NONE;

  uint32_t alarm_a_subseconds = 0U;
  uint32_t alarm_a_mask_subseconds = 0U;

  LL_RTC_ALM_SetConfigDateTime(LL_RTC_ALARM_A, alarm_a_mask, LL_RTC_ALMA_DATEWEEKDAYSEL_WEEKDAY, LL_RTC_WEEKDAY_MONDAY, LL_RTC_ALMA_TIME_FORMAT_AM_24H, alarm_a_hours, alarm_a_minutes, alarm_a_seconds);
  LL_RTC_ALM_SetConfigSubSecond(LL_RTC_ALARM_A, alarm_a_mask_subseconds, alarm_a_subseconds);

  /* Enable write protection */
  LL_RTC_EnableWriteProtection();

  NVIC_SetPriority(RTC_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(RTC_IRQn);

  return SYSTEM_OK;
}

void mx_rtc_deinit(void)
{
  NVIC_DisableIRQ(RTC_IRQn);
}

/******************************************************************************/
/*    RTC global non-secure interrupts is managed directly in user code.   */
/******************************************************************************/
/*
void RTC_IRQHandler(void)
{
}
  */
