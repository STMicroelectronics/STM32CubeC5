/**
  ******************************************************************************
  * file           : ll_example.h
  * brief          : Header for stm32ynxx_ll_example.c files.
  *                  This file contains example-specific declarations to interface with example.c functions.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LL_EXAMPLE_H
#define LL_EXAMPLE_H

/* Includes ------------------------------------------------------------------*/
#include "mx_hal_def.h"       /* aliases to the target-specific generated code */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Exported types ------------------------------------------------------------*/

/**
  *  brief MX_RTCx time AM PM definitions.
  *  This enum provides descriptive names for time format (AM/PM),
  *  making the code more readable.
  */
typedef enum
{
  RTC_TIME_AM_24H = 0U,
  RTC_TIME_PM     = 0x400000U
} rtc_time_format_am_pm_t;

/**
  *  brief MX_RTCx month definitions.
  *  This enum provides descriptive names for month values,
  *  making the code more readable.
  */
typedef enum
{
  RTC_JANUARY   = 1U,
  RTC_FEBRUARY  = 2U,
  RTC_MARCH     = 3U,
  RTC_APRIL     = 4U,
  RTC_MAY       = 5U,
  RTC_JUNE      = 6U,
  RTC_JULY      = 7U,
  RTC_AUGUST    = 8U,
  RTC_SEPTEMBER = 9U,
  RTC_OCTOBER   = 10U,
  RTC_NOVEMBER  = 11U,
  RTC_DECEMBER  = 12U
} rtc_month_t;

/**
  *  brief MX_RTCx week day definitions.
  *  This enum provides descriptive names for weekday values,
  *  making the code more readable.
  */
typedef enum
{
  RTC_MONDAY    = 0x01U,
  RTC_TUESDAY   = 0x02U,
  RTC_WEDNESDAY = 0x03U,
  RTC_THURSDAY  = 0x04U,
  RTC_FRIDAY    = 0x05U,
  RTC_SATURDAY  = 0x06U,
  RTC_SUNDAY    = 0x07U
} rtc_weekday_t;

/**
  *  brief MX_RTCx timestamp event definitions.
  *  This enum provides descriptive names for timestamp event values,
  *  making the code more readable.
  */
typedef enum
{
  RTC_NO_EVENT       = 0U,
  RTC_EVENT          = 0x08U,
  RTC_OVERFLOW_EVENT = 0x10U
} rtc_timestamp_event_info_t;

/**
  *  brief: MX_RTCx time structure.
  */
typedef struct
{
  rtc_time_format_am_pm_t am_pm; /* Specifies that the time format */
  uint32_t                hour;  /* Specifies the time hours       */
  uint32_t                min;   /* Specifies the time minutes     */
  uint32_t                sec;   /* Specifies the time seconds     */

} rtc_time_t;

/**
  *  brief: MX_RTCx date structure.
  */
typedef struct
{
  uint32_t      year;  /* Specifies the year      */
  rtc_month_t   month; /* Specifies the month     */
  uint32_t      mday;  /* Specifies the month day */
  rtc_weekday_t wday;  /* Specifies the week day  */

} rtc_date_t;

/* Exported constants --------------------------------------------------------*/
#ifndef USE_LL_APP_TIMEOUT
#define USE_LL_APP_TIMEOUT 0U
#endif /* USE_LL_APP_TIMEOUT */

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/**
  * brief: Disable the RTC domain write protection (RCC RTC domain control register RCC_BDCR, RTC registers,
  *         TAMP registers, backup registers and backup SRAM).
  */
void PWR_DisableRTCDomainWriteProtection(void);

/**
  * brief: Enable the MX_RTCx timestamp interrupt.
  */
void RTC_EnableTimestampIT(void);

/**
  * brief:  Retrieve the MX_RTCx calendar time and date.
  * param:  p_date pointer to a MX_RTCx timestamp date.
  * param:  p_time pointer to a MX_RTCx timestamp time.
  * retval: system status (see system_status_t)
  */
system_status_t RTC_GetCalendarDatetime(rtc_date_t *p_date, rtc_time_t *p_time);

/**
  * brief:  Retrieve the MX_RTCx timestamp time and the source of the timestamp event.
  * param:  p_time pointer to a MX_RTCx timestamp time.
  * param:  p_date pointer to a MX_RTCx timestamp date.
  * param:  p_info pointer to a MX_RTCx information.
  * retval: system status (see system_status_t)
  */
system_status_t RTC_GetTimestampDateTime(rtc_time_t *p_time,
                                         rtc_date_t *p_date,
                                         rtc_timestamp_event_info_t *p_info);

/**
  * brief: MX_RTCx timestamp event callback.
  */
void RTC_TimestampEventCallback(void);

/**
  * brief: This function provides delay (in milliseconds) based on SysTick counter flag.
  * param: delay_ms Number of milliseconds to delay.
  */
void DelayMs(uint32_t delay_ms);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_EXAMPLE_H */
