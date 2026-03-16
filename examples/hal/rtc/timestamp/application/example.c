/**
  ******************************************************************************
  * file           : example.c
  * brief          : RTC timestamp with HAL API
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define PRINT_TIMESTAMP(h, m, s)                                 \
  do                                                             \
  {                                                              \
    PRINTF("[INFO] Step 3: Timestamp  hour=%2" PRIu32 " ", (h)); \
    PRINTF("min=%2" PRIu32 " ", (m));                            \
    PRINTF("sec=%2" PRIu32 "\n", (s));                           \
  } while (0)

#define PRINT_DATESTAMP(mon, mday)                               \
  do                                                             \
  {                                                              \
    PRINTF("[INFO] Step 3: Datestamp  mon=%2d ", (mon));         \
    PRINTF("mday=%2" PRIu32 " \n", (mday));                      \
  } while (0)
/* Private variables ---------------------------------------------------------*/
volatile hal_rtc_time_t RtcTimeStamp; /* Captured timestamp time */
volatile hal_rtc_date_t RtcDateStamp; /* Captured timestamp date */
volatile hal_rtc_timestamp_information_t TimeStampInfo; /* Timestamp event information */

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the RTC instance.
  * The applicative code enables the RTC timestamp IT feature.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_example_rtc_init() != SYSTEM_OK)
  {
    PRINTF("[ERROR] Step 1: Device initialization ERROR.\n");
    goto _app_process_exit;
  }

  /* Ensure timestamp info flag starts in a known state */
  TimeStampInfo.flag = HAL_RTC_TIMESTAMP_NO_EVENT;

  if (HAL_RTC_TIMESTAMP_EnableIT() != HAL_OK)
  {
    PRINTF("[ERROR] Step 1: Enable RTC timestamp IT ERROR.\n");
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_process_exit:

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_OK;
  hal_rtc_time_t rtc_time;
  hal_rtc_date_t rtc_date;

  /** ########## Step 2 ##########
    * The applicative code retrieves the current RTC date and time,
    * displays the current date and time, waits to get a one-second update
    */

  /* Get the RTC current Date and Time */
  if (HAL_RTC_CALENDAR_GetDateTime(&rtc_date, &rtc_time) != HAL_OK)
  {
    PRINTF("[ERROR] Step 2: Get RTC date/time ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
    goto _app_process_exit;
  }

  /* Display time Format : hh:mm:ss */
  PRINTF("[INFO] Step 2: Current Time : %2" PRIu32 ":%2" PRIu32 ":%2" PRIu32 " \n", rtc_time.hour, rtc_time.min,
         rtc_time.sec);

  /* Display date Format : mm-dd-yy */
  PRINTF("[INFO] Step 2: Current Date : %2d" "-%2" PRIu32 "-%2" PRIu32 " \n", rtc_date.mon, rtc_date.mday,
         2000 + rtc_date.year);

  /** ########## Step 3 ##########
    * The applicative code checks the RTC timestamp event to display timestamp/datestamp
    * information.
    */

  /* Check the timestamp information flag*/
  if (TimeStampInfo.flag != HAL_RTC_TIMESTAMP_NO_EVENT)
  {
    if (TimeStampInfo.flag == HAL_RTC_TIMESTAMP_EVENT)
    {
      /* Display time Format : hh:mm:ss */
      PRINT_TIMESTAMP(RtcTimeStamp.hour, RtcTimeStamp.min, RtcTimeStamp.sec);

      /* Display date Format : mm-dd-yy */
      PRINT_DATESTAMP(RtcDateStamp.mon, RtcDateStamp.mday);
    }
    else
    {
      return_status = EXEC_STATUS_ERROR;
      PRINTF("[ERROR] Step 3: Timestamp overflow event \n");
    }
  }

  /* Delay to get time updated each second */
  HAL_Delay(1000);

_app_process_exit:

  return return_status;
} /* end app_process */


/** Deinitializes the RTC instance.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_rtc_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** RTC timestamp event callback
  * This callback is invoked when the button is pressed.
  */
void HAL_RTC_TimestampEventCallback()
{
  HAL_RTC_TIMESTAMP_GetDateTime((hal_rtc_time_t *)&RtcTimeStamp, (hal_rtc_date_t *)&RtcDateStamp,
                                (hal_rtc_timestamp_information_t *)&TimeStampInfo);
}
