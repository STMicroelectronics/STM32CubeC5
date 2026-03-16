/**
  ******************************************************************************
  * File Name          : app_sntp_client.c
  * Description        : This file provides code for SNTP client example.
  ******************************************************************************
  * @attention
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
#include "app_sntp_client.h"

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#include "app_lwip.h"
#include "app_logger.h"

#include "app_config.h"

#include "lwip/apps/sntp.h"
#include "mx_lwip.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define APP_NAME "AppSntpClient"
#define UNIX_START_YEAR 1970
#define RTC_START_YEAR  2000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static volatile BaseType_t RunClient = pdTRUE;

static TaskHandle_t AppSntpClientTaskHandle;

extern EventGroupHandle_t AppLwipEvents;

/* Days in each month */
const uint8_t days_in_month[2][12] =
{
  {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, /* Normal years */
  {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31} /* Leap years */
};

/* Private functions prototype -----------------------------------------------*/

/**
  * brief:  SNTP client thread
  * param: arg: pointer on argument(not used here)
  * retval: None
  */
static void AppSntpClientThread(void *args);


/* Private functions ---------------------------------------------------------*/


/* Check for leap year */
static uint32_t AppSntpClientIsLeapYear(uint32_t year)
{
  return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

/* convert unix time (seconds) to RTC Date and Time */
static void AppSntpClientConvertUnixToRtcTime(uint32_t sec, hal_rtc_time_t *rtc_time,
                                              hal_rtc_date_t *rtc_date)
{
  uint32_t days = sec / 86400;
  uint32_t rem = sec % 86400;

  rtc_time->am_pm = HAL_RTC_TIME_FORMAT_AM_24H;

  /* Calculate hour, min, sec */
  rtc_time->hour = rem / 3600;
  rem %= 3600;
  rtc_time->min = rem / 60;
  rtc_time->sec = rem % 60;
  rtc_time->millisec = 0; /* Only used for alarm and not calendar */
  rtc_time->microsec = 0; /* Only used for alarm and not calendar */


  /* Calculate wday of week, 1970-01-01 was a Thursday*/
  rtc_date->wday = (hal_rtc_weekday_t)(((days + (HAL_RTC_WEEKDAY_THURSDAY - HAL_RTC_WEEKDAY_MONDAY)) \
                                        % HAL_RTC_WEEKDAY_SUNDAY) + HAL_RTC_WEEKDAY_MONDAY);

  /* Calculate year */
  uint32_t year = UNIX_START_YEAR;
  while (1)
  {
    uint32_t days_in_year = AppSntpClientIsLeapYear(year) ? 366 : 365;
    if (days >= days_in_year)
    {
      days -= days_in_year;
      year++;
    }
    else
    {
      break;
    }
  }
  rtc_date->year = year - RTC_START_YEAR;

  /* Calculate month and day */
  uint32_t leap = AppSntpClientIsLeapYear(year);
  uint32_t month = 0;
  while (days >= days_in_month[leap][month])
  {
    days -= days_in_month[leap][month];
    month++;
  }
  rtc_date->mon = (hal_rtc_month_t)LL_RTC_CONVERT_BIN2BCD(month + 1); /* 1-based */
  rtc_date->mday = days + 1; /* 1 - based */
}

/* convert RTC Date and Time to Unix time (seconds) to RTC Date and Time */
static void AppSntpClientConvertRtcToUnixTime(const hal_rtc_time_t *rtc_time, const hal_rtc_date_t *rtc_date,
                                              int32_t *sec)
{
  uint32_t year = RTC_START_YEAR + rtc_date->year;
  uint32_t month = LL_RTC_CONVERT_BCD2BIN(rtc_date->mon) - 1; /* 0-based */
  uint32_t day = rtc_date->mday - 1;  /* 0-based */
  uint32_t days = 0;

  /* Add days for all years since UNIX_START_YEAR */
  for (uint32_t y = UNIX_START_YEAR; y < year; y++)
  {
    days += AppSntpClientIsLeapYear(y) ? 366 : 365;
  }

  /* Add days for all months in current year */
  uint32_t leap = AppSntpClientIsLeapYear(year);
  for (uint32_t m = 0; m < month; m++)
  {
    days += days_in_month[leap][m];
  }

  /* Add days in current month */
  days += day;

  /* Calculate total seconds */
  *sec = days * 86400
         + rtc_time->hour * 3600
         + rtc_time->min * 60
         + rtc_time->sec;
}

/* Display RTC Date and Time */
static void AppSntpClientDisplayDateTime(const char *prefix, const hal_rtc_date_t *rtc_date,
                                         const hal_rtc_time_t *rtc_time)
{
  /* Display date-time Format : yyyy-mm-dd hh:mm:ss */
  APP_LOG_INFO(APP_NAME, "%s%02" PRIu32 "-%02" PRIu32 "-%02" PRIu32 " %02" PRIu32 ":%02" PRIu32 ":%02" PRIu32 " \n",
               prefix,
               RTC_START_YEAR + rtc_date->year, LL_RTC_CONVERT_BCD2BIN(rtc_date->mon), rtc_date->mday,
               rtc_time->hour, rtc_time->min, rtc_time->sec);

}

/* Display Current System RTC Date and Time */
static void AppSntpClientDisplayCurrentSystemDateTime(const char *prefix)
{
  hal_rtc_time_t RtcTime;
  hal_rtc_date_t RtcDate;
  hal_status_t hal_status;

  hal_status = HAL_RTC_CALENDAR_GetDateTime(&RtcDate, &RtcTime);
  if (hal_status != HAL_OK)
  {
    APP_LOG_ERROR(APP_NAME, "Failed to Get RTC Date and Time (%x)\n", hal_status);
  }
  else
  {
    AppSntpClientDisplayDateTime(prefix, &RtcDate, &RtcTime);
  }

}

/* Save RTC Date and Time */
static void AppSntpClientSaveDateTime(const hal_rtc_date_t *rtc_date, const hal_rtc_time_t *rtc_time)
{
  hal_status_t hal_status;

  /* Disable RTC Domain Write Protection */
  HAL_PWR_DisableRTCDomainWriteProtection();

  /* Disable write protection to configure the RTC, as it is part of the protected backup domain */
  hal_status = HAL_RTC_DisableWriteProtection();

  if (hal_status != HAL_OK)
  {
    APP_LOG_ERROR(APP_NAME, "Failed to disable RTC write protection (%x)\n", hal_status);
  }

  if (hal_status == HAL_OK)
  {
    /* Enter RTC initialization mode to be able to set the date and time */
    hal_status = HAL_RTC_EnterInitMode();
    if (hal_status != HAL_OK)
    {
      APP_LOG_ERROR(APP_NAME, "Failed to enter RTC init mode (%x)\n", hal_status);
    }
  }

  if (hal_status == HAL_OK)
  {
    hal_status = HAL_RTC_CALENDAR_SetDateTime(rtc_date, rtc_time);
    if (hal_status != HAL_OK)
    {
      APP_LOG_ERROR(APP_NAME, "Failed to set RTC date and time (%x)\n", hal_status);
    }
  }

  if (hal_status == HAL_OK)
  {
    hal_status = HAL_RTC_ExitInitMode();
    if (hal_status != HAL_OK)
    {
      APP_LOG_ERROR(APP_NAME, "Failed to exit RTC init mode (%x)\n", hal_status);
    }
  }

  if (hal_status == HAL_OK)
  {
    hal_status = HAL_RTC_EnableWriteProtection();
    if (hal_status != HAL_OK)
    {
      APP_LOG_ERROR(APP_NAME, "Failed to enable RTC write protection (%x)\n", hal_status);
    }
  }

  if (hal_status == HAL_OK)
  {
    /* Enable RTC Domain Write Protection */
    HAL_PWR_EnableRTCDomainWriteProtection();
  }
}

static void AppSntpClientRtcInit(void)
{
  HAL_RCC_RTCAPB_EnableClock();

  /* Check if the RTC calendar is configured to restore date and time from the backup domain. */
  if (HAL_RTC_CALENDAR_IsInitialized() != HAL_RTC_CALENDAR_INITIALIZED)
  {
    if (mx_example_rtc_init() != SYSTEM_OK)
    {
      APP_LOG_ERROR(APP_NAME, "RTC init failed\n");
      return;
    }

    APP_LOG_INFO(APP_NAME, "The RTC instance is initialized.\n");
  }
  else
  {
    APP_LOG_INFO(APP_NAME, "The RTC is restored from backup domain.\n");
  }

}

static void AppSntpSetSystemTime(u32_t sec, u32_t usec)
{
  hal_rtc_time_t RtcNetworkTime;
  hal_rtc_date_t RtcNetworkDate;
  hal_rtc_time_t RtcOldTime;
  hal_rtc_date_t RtcOldDate;
  hal_status_t hal_status;
  uint32_t old_sec;

  /** ########## Step 4.4 ##########
    * Convert, store to RTC and display old and new RTC time
  */
  hal_status = HAL_RTC_CALENDAR_GetDateTime(&RtcOldDate, &RtcOldTime);

  AppSntpClientConvertUnixToRtcTime(sec, &RtcNetworkTime, &RtcNetworkDate);
  AppSntpClientSaveDateTime(&RtcNetworkDate, &RtcNetworkTime);

  APP_LOG_INFO(APP_NAME, "SNTP callback - Network time   : %"PRIu32".%06"PRIu32" sec\n", sec, usec);
  AppSntpClientDisplayDateTime("SNTP callback - Network time   : ", &RtcNetworkDate, &RtcNetworkTime);

  if (hal_status != HAL_OK)
  {
    APP_LOG_ERROR(APP_NAME, "Failed to Get RTC Date and Time (%x)\n", hal_status);
  }
  else
  {
    AppSntpClientDisplayDateTime("SNTP callback - Old System time: ", &RtcOldDate, &RtcOldTime);
  }

  AppSntpClientDisplayCurrentSystemDateTime("SNTP callback - New System time: ");

  AppSntpClientConvertRtcToUnixTime(&RtcOldTime, &RtcOldDate, (int32_t *)&old_sec);

  APP_LOG_INFO(APP_NAME, "SNTP callback - Corrected time : %ld sec\n", (int32_t)sec - (int32_t)old_sec);

}

static void AppSntpClientThread(void *args)
{
  /** ########## Step 4.1 ##########
    * Initializes the RTC instance.
  */
  APP_LOG_INFO(APP_NAME, "Initializing RTC\n");
  AppSntpClientRtcInit();

  AppSntpClientDisplayCurrentSystemDateTime("RTC initialized - System time : ");

  APP_LOG_INFO(APP_NAME, "Waiting for IP address\n");
  xEventGroupWaitBits(AppLwipEvents,
                      EVT_IP_ADDR_SET,
                      pdTRUE,
                      pdFALSE,
                      portMAX_DELAY);

  /** ########## Step 4.2 ##########
    * Configure and start the SNTP client
    */
  /* Configure SNTP client */

  APP_LOG_INFO(APP_NAME, "SNTP server address: %s\n", APP_SNTP_SERVER_ADDR);
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, APP_SNTP_SERVER_ADDR);

  /* Register SNTP set system time function*/
  mx_lwip_sntp_register_set_system_time_fn(AppSntpSetSystemTime);

  /*/ Start SNTP client */
  sntp_init();

  while (RunClient == pdTRUE)
  {
    vTaskDelay(pdMS_TO_TICKS(APP_SNTP_CLIENT_SYSTEM_TIME_LOG_PERIOD_MS));

    /** ########## Step 4.3 ##########
      * Get and display the RTC Date and Time
      */
    AppSntpClientDisplayCurrentSystemDateTime("Main loop     - System time    : ");
  }

  /** ########## Step 4.5 ##########
    * Stop the SNTP client
    */
  sntp_stop();

  /* Inspect task stack high water mark on exit. */
  APP_LOG_INFO(APP_NAME, "Task High Water Mark: (%ld)\n", uxTaskGetStackHighWaterMark(NULL));

  /* Delete the task */
  APP_LOG_INFO(APP_NAME, "Delete Task\n");
  vTaskDelete(NULL);
}


/* Public functions ----------------------------------------------------------*/

void app_sntp_client_start()
{

  /** ########## Step 4 ##########
    * Creates and starts the SNTP client thread
    */
  BaseType_t xReturn;
  xReturn = xTaskCreate(AppSntpClientThread, APP_NAME, configMINIMAL_STACK_SIZE,
                        (void *) NULL, APP_SNTP_CLIENT_THREAD_PRIO, &AppSntpClientTaskHandle);
  if (xReturn != pdPASS)
  {
    APP_LOG_ERROR(APP_NAME, "Task Creation failed (%ld)\n", xReturn);
  }
}
