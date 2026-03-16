/**
  ******************************************************************************
  * file           : example.c
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
#include "example.h"
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define RTC_TIME_UPDATE_MS 1000U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile rtc_time_t                 RTCTimestamp;     /* MX_RTCx captured timestamp time     */
volatile rtc_date_t                 RTCDatestamp;     /* MX_RTCx captured timestamp date     */
volatile rtc_timestamp_event_info_t RTCTimestampInfo; /* MX_RTCx timestamp event information */
volatile rtc_time_t                 RTCTime;          /* MX_RTCX calendar time               */
volatile rtc_date_t                 RTCDate;          /* MX_RTCX calendar date               */

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the MX_RTCx instance.
  * The applicative code enables the MX_RTCx timestamp interrupt.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_rtcx_init() != SYSTEM_OK)
  {
    goto _app_process_exit;
  }

  RTCTimestampInfo = RTC_NO_EVENT;

  PWR_DisableRTCDomainWriteProtection();

  RTC_EnableTimestampIT();

  return_status = EXEC_STATUS_INIT_OK;

_app_process_exit:

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * The applicative code retrieves the current MX_RTCx time and date.
    */
  if (RTC_GetCalendarDatetime((rtc_date_t *)&RTCDate, (rtc_time_t *)&RTCTime) != SYSTEM_OK)
  {
    goto _app_process_exit;
  }

  /* Delay to get time updated each second */
  DelayMs(RTC_TIME_UPDATE_MS);


  /** ########## Step 3 ##########
    * The applicative code checks the MX_RTCx timestamp event to update timestamp and datestamp within RTCTimestamp and
    * RTCDatestamp variables.
    */
  if (RTCTimestampInfo != RTC_NO_EVENT)
  {
    if (RTCTimestampInfo == RTC_EVENT)
    {
      RTCTimestampInfo = RTC_NO_EVENT;
    }
    else
    {
      goto _app_process_exit;
    }
  }

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** Deinitializes the MX_RTCx instance.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_rtcx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** MX_RTCx timestamp event callback
  * This callback is invoked when the user button is pressed.
  */
void RTC_TimestampEventCallback(void)
{
  RTC_GetTimestampDateTime((rtc_time_t *)&RTCTimestamp,
                           (rtc_date_t *)&RTCDatestamp,
                           (rtc_timestamp_event_info_t *)&RTCTimestampInfo);
}
