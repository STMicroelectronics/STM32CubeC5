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
  */
typedef enum
{
  RTC_TIME_AM_24H = 0U,
  RTC_TIME_PM     = 0x400000
} rtc_time_format_am_pm_t;

/**
  *  brief MX_RTCx month definitions.
  */
typedef enum
{
  RTC_JANUARY   = 0x01U,
  RTC_FEBRUARY  = 0x02U,
  RTC_MARCH     = 0x03U,
  RTC_APRIL     = 0x04U,
  RTC_MAY       = 0x05U,
  RTC_JUNE      = 0x06U,
  RTC_JULY      = 0x07U,
  RTC_AUGUST    = 0x08U,
  RTC_SEPTEMBER = 0x09U,
  RTC_OCTOBER   = 0x10U,
  RTC_NOVEMBER  = 0x11U,
  RTC_DECEMBER  = 0x12U
} rtc_month_t;

/**
  *  brief MX_RTCx week day definitions.
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

/** brief:  Enable the MX_RTCx peripherals clock.
  */
void            RTC_EnablePeripheralClock(void);

/** brief:  Check if the calendar is initialized.
  * retval: Initialization status flag.
  */
uint32_t        RTC_IsCalendarInitialized(void);

/** brief:  Enable the MX_RTCx domain write protection.
  */
void PWR_EnableRTCDomainWriteProtection(void);

/** brief:  Retrieve the MX_RTCx date and time.
  * param:  p_date pointer to an MX_RTCx date.
  * param:  p_time pointer to an MX_RTCx time.
  * retval: system status (see system_status_t).
  */
system_status_t RTC_GetCalendarDateTime(rtc_date_t *p_date, rtc_time_t *p_time);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_EXAMPLE_H */
