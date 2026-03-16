/**
  ******************************************************************************
  * file           : example.c
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
#include "example.h"
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
rtc_time_t RTCTime;
rtc_date_t RTCDate;
/* Private functions prototype -----------------------------------------------*/

app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1 ##########
    * The initialization of the MX_RTCx instance and setting the MX_RTCx calendar time and date.
    */

  RTC_EnablePeripheralClock();

  /* Check if the MX_RTCx calendar is configured to restore date and time from the backup domain. */
  if (RTC_IsCalendarInitialized() == 0)
  {
    if (mx_rtcx_init() != SYSTEM_OK)
    {
      return return_status;
    }
  }

  /* Enable MX_RTCx Domain Write Protection */
  PWR_EnableRTCDomainWriteProtection();

  return_status = EXEC_STATUS_INIT_OK;

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * Retrieve the updated time and date.
    */
  /* Get the MX_RTCx current Time and Date */
  RTC_GetCalendarDateTime(&RTCDate, &RTCTime);

  return_status = EXEC_STATUS_OK;

  return return_status;
} /* end app_process */


app_status_t app_deinit(void)
{
  mx_rtcx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
