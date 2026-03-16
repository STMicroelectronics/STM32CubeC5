/**
  ******************************************************************************
  * file           : example.c
  * brief          : RTC Calendar with HAL API
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
hal_rtc_time_t RtcTime;
hal_rtc_date_t RtcDate;

/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/

app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1 ##########
    * The initialization of the RTC instance and setting the RTC calendar time and date.
    */

  HAL_RCC_RTCAPB_EnableClock();

  /* Check if the RTC calendar is configured to restore date and time from the backup domain. */
  if (HAL_RTC_CALENDAR_IsInitialized() != HAL_RTC_CALENDAR_INITIALIZED)
  {
    if (mx_example_rtc_init() != SYSTEM_OK)
    {
      return return_status;
    }
    PRINTF("[INFO] Step 1: The RTC instance is initialized and the calendar time and date are set \n");
  }

  /** ########## Step 3 ##########
    * Checks for RTC Reset Type
    */

  else
  {
    /* Check if the Power On Reset flag is set */
    if (HAL_RCC_GetResetSource() & HAL_RCC_RESET_FLAG_PWR)
    {
      PRINTF("[INFO] Step 3: Power On Reset occurred \n");
    }
    /* Check if Pin Reset flag is set */
    if (HAL_RCC_GetResetSource() & HAL_RCC_RESET_FLAG_PIN)
    {
      PRINTF("[INFO] Step 3: Reset occurred \n");
    }
  }

  /* Clear source Reset Flag */
  HAL_RCC_GetResetSource();

  return_status = EXEC_STATUS_INIT_OK;

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * Displays the updated time and date.
    */
  /* Get the RTC current Date and Time */
  if (HAL_RTC_CALENDAR_GetDateTime(&RtcDate, &RtcTime) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Display time Format : hh:mm:ss */
  PRINTF("[INFO] Step 2: Time:%2" PRIu32 ":%2" PRIu32 ":%2" PRIu32 " \n",
         RtcTime.hour,
         RtcTime.min,
         RtcTime.sec);

  /* Display date Format : mm-dd-yy */
  PRINTF("[INFO] Step 2: Date:%2" PRIu32 "-%2" PRIu32 "-%2" PRIu32 " \n",
         HAL_RTC_CONVERT_BCD2DEC(RtcDate.mon),
         RtcDate.mday,
         2000 + RtcDate.year);

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


app_status_t app_deinit(void)
{
  /** Nothing to deinitialize for RTC.
    */
  return EXEC_STATUS_OK;
} /* end app_deinit */
