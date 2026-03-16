/**
  ******************************************************************************
  * file           : example.c
  * brief          : RTC wakeup timer from standby mode with HAL API
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
#define APP_TARGET_WAKEUP_COUNT      (5U)   /* Number of wakeups required to conclude the example */
#define APP_TARGET_SECONDS_AT_COUNT  (15U)  /* Expected RTC seconds value when wakeup count reached */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint32_t WakeupNumber = 0U; /* Number of wakeups from standby mode */

/* Private functions prototype -----------------------------------------------*/

app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1.1 ##########
    * The applicative code detects whether the device resumed from standby.
    */

  /* Check whether we resumed from standby or it's the first start */
  if (LL_PWR_IsActiveFlag_SB() == 0U)
  {
    PRINTF("[INFO] Step 1.1: First start.\n");

    /** ########## Step 1.2 ##########
      * The applicative code initializes the RTC on the first start (Step 1.2).
      */

    /* RTC init resets the RTC domain; perform it only once on first start */
    if (mx_example_rtc_init() != SYSTEM_OK)
    {
      PRINTF("[ERROR] Step 1.2: Device initialization ERROR.\n");
      goto _app_process_exit;
    }
    PRINTF("[INFO] Step 1.2: Device initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }
  else
  {
    /** ########## Step 2.2 ##########
      * The applicative code updates the wakeup number after each restart.
      */

    /* Re-enable the RTC clock after a restart from Standby mode; no full reconfiguration is required. */
    HAL_RCC_RTCAPB_EnableClock();

    PRINTF("[INFO] Step 2.2: Restart from STANDBY mode.\n");

    /* Get the number of wakeups from standby mode and update it */
    WakeupNumber = HAL_TAMP_ReadBackupRegisterValue(HAL_TAMP_BACKUP_REG_0) + 1;

    /* Save the number of wakeups from standby mode */
    HAL_TAMP_WriteBackupRegisterValue(HAL_TAMP_BACKUP_REG_0, WakeupNumber);

    return_status = EXEC_STATUS_INIT_OK;
  }

  /** ########## Step 1.3 ##########
    * The applicative code starts the RTC wakeup interrupt.
    */

  /* Start RTC wakeup timer with interrupt enabled */
  if (HAL_RTC_WAKEUP_Start(HAL_RTC_WAKEUP_IT_ENABLE) != HAL_OK)
  {
    PRINTF("[ERROR] Step 1.3: Enable RTC wakeup interrupt ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 1.3: Enable RTC wakeup interrupt.\n");

_app_process_exit:

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_rtc_time_t rtc_time;
  hal_rtc_date_t rtc_date;

  /** ########## Step 3 ##########
    * Demonstrates wakeup count handling and elapsed RTC time.
    * Counts the number of wakeups elapsed (WakeupNumber).
    * Checks whether the elapsed time is equal to 3 s * APP_TARGET_WAKEUP_COUNT.
    */

  /* Check the number of wakeups */
  if (WakeupNumber == APP_TARGET_WAKEUP_COUNT)
  {
    HAL_RTC_CALENDAR_GetDateTime(&rtc_date, &rtc_time);
    if (rtc_time.sec == APP_TARGET_SECONDS_AT_COUNT)
    {
      PRINTF("[INFO] Step 3: Target wakeup count reached.\n");
      return_status = EXEC_STATUS_OK;
      goto _app_process_exit;
    }
    else
    {
      PRINTF("[ERROR] Step 3: Elapsed timing not as expected.\n");
      goto _app_process_exit;
    }
  }
  else if (WakeupNumber > APP_TARGET_WAKEUP_COUNT)
  {
    PRINTF("[ERROR] Step 3: Wakeup count exceeded expected limit.\n");
    goto _app_process_exit;
  }

  /** ########## Step 2.1 ##########
    * Prepare and re-enter Standby to allow the RTC to wake the device again.
    */

  /* Clear any pending Standby flag before entering Standby mode */
  LL_PWR_ClearFlag_SB();

  PRINTF("[INFO] Step 2.1: Enter STANDBY mode - After 3s RTC will generate a wakeup interrupt.\n ");

  /* Enter standby mode */
  HAL_PWR_EnterStandbyMode();

  /* This code cannot be reached: after resuming from standby, the system resets. */
  return_status = EXEC_STATUS_ERROR;

_app_process_exit:

  return return_status;
} /* end app_process */


/** ########## Step 4 ##########
  * Deinitializes the RTC peripheral before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  PRINTF("[INFO] Step 4: RTC deinitialization COMPLETED.\n");
  mx_example_rtc_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
