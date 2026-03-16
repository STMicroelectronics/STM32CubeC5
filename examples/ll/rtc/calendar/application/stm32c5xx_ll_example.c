/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : RTC Calendar with LL API
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define RTC_TIMEOUT_VALUE_MS (1000U)

/* Private functions prototype -----------------------------------------------*/
static system_status_t WaitRegistersSynchronizationFlag(void);
/* Private functions ---------------------------------------------------------*/

/**
  *  Enable the MX_RTCx peripherals clock.
  */
inline void RTC_EnablePeripheralClock(void)
{
  LL_APB3_GRP1_EnableClock(LL_APB3_GRP1_PERIPH_RTCAPB);
}


/**
  *  Get Initialization status flag.
  */
inline uint32_t RTC_IsCalendarInitialized(void)
{
  return (LL_RTC_IsActiveFlag_INITS());
}


/**
  *  Enable the RTC domain write protection (RCC RTC domain control register RCC_BDCR, RTC registers,
  *  Tamper registers, backup registers and backup SRAM).
  */
inline void PWR_EnableRTCDomainWriteProtection(void)
{
  LL_PWR_EnableRTCDomainWriteProtection();
}


/**
  *  Retrieve the MX_RTCx time and date .
  *  param: p_date pointer to an MX_RTCx date.
  *  param: p_time pointer to an MX_RTCx time.
  * retval: SYSTEM_OK When time and date are successfully synchronized
  * retval: SYSTEM_PERIPHERAL_ERROR When time and date are not successfully synchronized
  */
inline system_status_t RTC_GetCalendarDateTime(rtc_date_t *p_date, rtc_time_t *p_time)
{
  uint32_t        temp_time_format = LL_RTC_TIME_GetTimeAndFormat();
  uint32_t        temp_value_date  = LL_RTC_DATE_Get();
  system_status_t status           = SYSTEM_OK;

  status = WaitRegistersSynchronizationFlag();
  if (status == SYSTEM_OK)
  {
    /* Retrieve the MX_RTCx current time */
    p_time->am_pm  = (rtc_time_format_am_pm_t)(((uint32_t)RTC_TIME_PM) * LL_RTC_GET_FORMAT(temp_time_format));
    p_time->hour   = LL_RTC_CONVERT_BCD2BIN(LL_RTC_GET_HOUR(temp_time_format));
    p_time->min    = LL_RTC_CONVERT_BCD2BIN(LL_RTC_GET_MINUTE(temp_time_format));
    p_time->sec    = LL_RTC_CONVERT_BCD2BIN(LL_RTC_GET_SECOND(temp_time_format));

    /* Retrieve the MX_RTCx current date */
    p_date->year  = LL_RTC_CONVERT_BCD2BIN(LL_RTC_GET_YEAR(temp_value_date));
    p_date->month = (rtc_month_t) LL_RTC_GET_MONTH(temp_value_date);
    p_date->mday  = LL_RTC_CONVERT_BCD2BIN(LL_RTC_GET_DAY(temp_value_date));
    p_date->wday  = (rtc_weekday_t)LL_RTC_GET_WEEKDAY(temp_value_date);

    status = WaitRegistersSynchronizationFlag();
  }

  return (status);
}


/**
  * brief:  Wait until the MX_RTCx time and date registers (RTC_TR and RTC_DR) are synchronized with RTC APB clock.
  * retval: SYSTEM_OK When time and date are successfully synchronized
  * retval: SYSTEM_PERIPHERAL_ERROR When time and date are not successfully synchronized
  */
static inline system_status_t WaitRegistersSynchronizationFlag(void)
{
  system_status_t return_status = SYSTEM_OK;

  LL_RTC_ClearFlag_RS();

#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
  uint32_t timeout_ms = RTC_TIMEOUT_VALUE_MS;
#endif /* USE_LL_APP_TIMEOUT */

  while (LL_RTC_IsActiveFlag_RS() == 0U)
  {
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
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
}

