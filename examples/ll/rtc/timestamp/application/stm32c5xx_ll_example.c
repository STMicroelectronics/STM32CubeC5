/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : RTC timestamp in interrupt mode with LL API
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
#define RTC_TIMEOUT_VALUE_MS (1000U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
static system_status_t RTC_WaitSynchronizationRS(void);
void mx_rtcx_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * brief: Disable the RTC domain write protection (RCC RTC domain control register RCC_BDCR, RTC registers,
  *         TAMP registers, backup registers and backup SRAM).
  */
inline void PWR_DisableRTCDomainWriteProtection(void)
{
  LL_PWR_DisableRTCDomainWriteProtection();
}


/**
  * brief: Enable the MX_RTCx timestamp interrupt.
  */
inline void RTC_EnableTimestampIT(void)
{
  LL_RTC_EnableIT_TS();
}


/**
  * brief:  Retrieve the MX_RTCx calendar time and date.
  * param:  p_date pointer to a MX_RTCx timestamp date.
  * param:  p_time pointer to a MX_RTCx timestamp time.
  * retval: SYSTEM_OK When time and date are successfully synchronized
  * retval: SYSTEM_PERIPHERAL_ERROR When time and date are not successfully synchronized
  */
inline system_status_t RTC_GetCalendarDatetime(rtc_date_t *p_date, rtc_time_t *p_time)
{
  uint32_t        temp_time_format = LL_RTC_TIME_GetTimeAndFormat();
  uint32_t        temp_value_date  = LL_RTC_DATE_Get();
  system_status_t status           = SYSTEM_OK;

  status = RTC_WaitSynchronizationRS();
  if (status == SYSTEM_OK)
  {
    /* Retrieve the MX_RTCx current time */
    p_time->am_pm = (rtc_time_format_am_pm_t)(((uint32_t)RTC_TIME_PM) * LL_RTC_GET_FORMAT(temp_time_format));
    p_time->hour  = LL_RTC_CONVERT_BCD2BIN(LL_RTC_GET_HOUR(temp_time_format));
    p_time->min   = LL_RTC_CONVERT_BCD2BIN(LL_RTC_GET_MINUTE(temp_time_format));
    p_time->sec   = LL_RTC_CONVERT_BCD2BIN(LL_RTC_GET_SECOND(temp_time_format));

    /* Retrieve the MX_RTCx current date */
    p_date->year  = LL_RTC_CONVERT_BCD2BIN(LL_RTC_GET_YEAR(temp_value_date));
    p_date->month = (rtc_month_t)LL_RTC_GET_MONTH(temp_value_date);
    p_date->mday  = LL_RTC_CONVERT_BCD2BIN(LL_RTC_GET_DAY(temp_value_date));
    p_date->wday  = (rtc_weekday_t)LL_RTC_GET_WEEKDAY(temp_value_date);

    status = RTC_WaitSynchronizationRS();
  }

  return (status);
}


/**
  * brief:  Retrieve the MX_RTCx timestamp time and the source of the timestamp event.
  * param:  p_time pointer to a MX_RTCx timestamp time.
  * param:  p_date pointer to a MX_RTCx timestamp date.
  * param:  p_info pointer to a MX_RTCx information.
  * retval: SYSTEM_OK
  * retval: SYSTEM_PERIPHERAL_ERROR when an overflow occurred
  */
inline system_status_t RTC_GetTimestampDateTime(rtc_time_t *p_time,
                                                rtc_date_t *p_date,
                                                rtc_timestamp_event_info_t *p_info)
{
  uint32_t        temp_time = LL_RTC_TS_GetTimeAndFormat();
  uint32_t        temp_date = LL_RTC_TS_GetDate();
  system_status_t status    = SYSTEM_OK;

  /* Retrieve the MX_RTCx timestamp time */
  p_time->am_pm = ((rtc_time_format_am_pm_t)(LL_RTC_GET_FORMAT(temp_time)));
  p_time->hour  = LL_RTC_CONVERT_BCD2BIN(LL_RTC_GET_HOUR(temp_time));
  p_time->min   = LL_RTC_CONVERT_BCD2BIN(LL_RTC_GET_MINUTE(temp_time));
  p_time->sec   = LL_RTC_CONVERT_BCD2BIN(LL_RTC_GET_SECOND(temp_time));

  /* Retrieve the MX_RTCx timestamp date */
  p_date->year  = 0U;
  p_date->month = ((rtc_month_t)LL_RTC_GET_MONTH(temp_date));
  p_date->mday  = LL_RTC_CONVERT_BCD2BIN(LL_RTC_GET_DAY(temp_date));
  p_date->wday  = (rtc_weekday_t)LL_RTC_GET_WEEKDAY(temp_date);

  *p_info = ((rtc_timestamp_event_info_t)((uint32_t)(LL_RTC_READ_REG(SR) & (LL_RTC_SR_TSF | LL_RTC_SR_TSOVF))));

  if (((uint32_t)p_info & (uint32_t)RTC_EVENT) == (uint32_t)RTC_EVENT)
  {
    LL_RTC_ClearFlag_TS();

    if (LL_RTC_IsActiveFlag_TSOV() != 0U)
    {
      *p_info = (rtc_timestamp_event_info_t)((uint32_t)((uint32_t)p_info \
                                                        | (uint32_t)(RTC_OVERFLOW_EVENT)));
      status = SYSTEM_PERIPHERAL_ERROR;
    }
  }

  LL_RTC_WRITE_REG(SCR, (LL_RTC_SCR_TSF | LL_RTC_SCR_TSOVF));

  return status;
}


/**
  *  Handles the MX_RTC interrupts.
  */
void mx_rtcx_irqhandler(void)
{
  if (LL_RTC_IsActiveFlag_TS() != 0U)
  {
    RTC_TimestampEventCallback();
  }
}


/**
  * brief:  Wait until the MX_RTCx time and date registers (RTC_TR and RTC_DR) are synchronized with RTC APB clock.
  * retval: SYSTEM_OK When time and date are successfully synchronized
  * retval: SYSTEM_PERIPHERAL_ERROR When time and date are not successfully synchronized
  */
static inline system_status_t RTC_WaitSynchronizationRS(void)
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


/**
  * brief: This function provides delay (in milliseconds) based on SysTick counter flag.
  * param: delay_ms Number of milliseconds to delay.
  */
inline void DelayMs(uint32_t delay_ms)
{
  LL_Delay_NoISR(delay_ms);
}
