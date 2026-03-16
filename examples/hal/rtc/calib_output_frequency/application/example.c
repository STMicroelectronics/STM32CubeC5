/**
  ******************************************************************************
  * file           : example.c
  * brief          : RTC calibration output frequency
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
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the RTC peripheral,
    and calibrates the RTC frequency to generate a 1Hz signal.
  * The RTC continues to drive the 1 Hz output.
  */
app_status_t app_init(void)
{
  app_status_t return_status;

  if (mx_example_rtc_init() == SYSTEM_OK)
  {
    PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
    PRINTF("[INFO] 1 Hz calibration clock available on RTC Output pin (square wave).\n");
    return_status = EXEC_STATUS_INIT_OK;
  }
  else
  {
    PRINTF("[ERROR] Step 1: Device initialization ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  /* No further processing */

  return EXEC_STATUS_OK;
} /* end app_process */


/** Deinitializes the RTC instance.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_rtc_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
