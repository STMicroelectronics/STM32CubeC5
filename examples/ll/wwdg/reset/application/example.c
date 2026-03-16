/**
  ******************************************************************************
  * file           : example.c
  * brief          : WWDG refresh and watchdog-based intentional MCU reset with LL API
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
/* @user: You can set the number of times that the MX_WWDGx will be refreshed before generating the reset */
#define PROCESS_LOOP_NUMBER 3

#define WWDG_AVOID_OVERLAP_MS 50    /* Time value to avoid MX_WWDGx overlap                     */
#define WWDG_MIN_TIME_VALUE   78    /* Minimum time value before MX_WWDGx refreshing is allowed */
#define WWDG_MAX_TIME_VALUE   209   /* Maximum time value before a MX_WWDGx reset               */

/* Delay corresponding to the mean of the MX_WWDGx timeout and window delays */
#define WWDG_VALID_REFRESH    ((2 * WWDG_MAX_TIME_VALUE - WWDG_MIN_TIME_VALUE) / 2)

/* Private variables ---------------------------------------------------------*/
uint8_t WWDG_RestartFromWWDGReset = 0;  /* Flag to know if the device restart due to MX_WWDGx reset */

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The init of the MX_WWDGx instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** If the device is restarted due to a MX_WWDGx reset, the MX_WWDGx initialization is bypassed.
    */
  if (WWDG_CheckWWDGReset() != 0U)
  {
    WWDG_ClearResetFlag();
    WWDG_RestartFromWWDGReset = 1;
    return_status = EXEC_STATUS_INIT_OK;
    goto _app_init_exit;
  }

  /** Initialization of the MX_WWDGx.
    * The MX_WWDGx is initialized with the parameters that can be configured by the user through the cube interface.
    */
  if (mx_wwdgx_init() == NULL)
  {
    goto _app_init_exit;
  }

  /* Start the MX_WWDGx */
  WWDG_Start();

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** Checking if the device restarted due to the MX_WWDGx. If not, the MX_WWDGx reset is triggered on purpose.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (WWDG_RestartFromWWDGReset == 0U)
  {
    /* The watchdog has not fired, so the next steps demonstrate how to reset the MX_WWDGx trigger. */

    /** ########## Step 2 ##########
      * The purpose is to refresh the watchdog PROCESS_LOOP_NUMBER times and to prevent watchdog reset.
      */
    for (uint8_t i = 0; i < PROCESS_LOOP_NUMBER; i++)
    {
      DelayMs(WWDG_VALID_REFRESH); /* MX_WWDGx refresh delay */

      WWDG_RefreshCounter(WWDG_MIN_TIME_VALUE);
    }

    /** ########## Step 3 ##########
      * The MX_WWDGx is refreshed outside the window to trigger the MX_WWDGx reset.
      */
    WWDG_RefreshCounter(WWDG_MAX_TIME_VALUE);

    /** When the MX_WWDGx is refreshed outside the window, the MX_WWDGx is triggered but it takes one period of the
      * MX_WWDGx prescaled clock to generate the interruption and the restart before completing this function execution.
      */
    DelayMs(WWDG_AVOID_OVERLAP_MS);
  }
  else
  {
    /* Watchdog has already fired, that means the previous steps (Step1,2,3) have already been executed */

    /** ########## Step 4 ##########
      * The device is restarted due to MX_WWDGx
      */
    return_status = EXEC_STATUS_OK;
  }

  return return_status;
} /* end app_process */


/** Nothing to do because the MX_WWDGx IP cannot be de-initialized.
  */
app_status_t app_deinit(void)
{
  return EXEC_STATUS_OK;
} /* end app_deinit */
