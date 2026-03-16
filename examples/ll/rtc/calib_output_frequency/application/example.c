/**
  ******************************************************************************
  * file        : example.c
  * brief       : Real-Time Clock calibration output frequency using the LL API
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
  * The applicative code initializes the MX_RTCx peripheral,
    and calibrates the frequency to generate a 1Hz signal.
  * The applicative code continues to drive the 1 Hz output.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_rtcx_init() == SYSTEM_OK)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  /* No further processing */

  return EXEC_STATUS_OK;
} /* end app_process */


/** Deinitializes the MX_RTCx instance.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_rtcx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
