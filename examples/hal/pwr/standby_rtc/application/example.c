/**
  ******************************************************************************
  * file           : example.c
  * brief          : example program body
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
/* Definition of the time spent in RUN mode.*/
#define RUN_MODE_ACTIVE_TIME_MS 2000U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * The application calls the init functions (GPIO, RTC and LED).
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");

  if (LL_PWR_IsActiveFlag_SB() != 0U)
  {
    PRINTF("[INFO] Restart from STANDBY mode.\n");
    return_status = EXEC_STATUS_OK;
  }
  else
  {
    PRINTF("[INFO] First start.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }

  PRINTF("[INFO] Start an RTC event to fire in 5s.\n");

  /* Start RTC wakeup timer with interrupt enabled */
  HAL_RTC_WAKEUP_Start(HAL_RTC_WAKEUP_IT_ENABLE);

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  /** ########## Step 2 ##########
    * The application stays in RUN mode for a few seconds.
    */
  PRINTF("[INFO] Step 2: RUN mode - Switch to STANDBY mode in %u milliseconds.\n", RUN_MODE_ACTIVE_TIME_MS);

  led_on(MX_STATUS_LED);

  /* Wait for a few seconds so the LED status can be noticed.*/
  HAL_Delay(RUN_MODE_ACTIVE_TIME_MS);

  /** ########## Step 3 ##########
    * The application switches to STANDBY mode.
    */
  led_off(MX_STATUS_LED);

  PRINTF("[INFO] Step 3: Enter STANDBY mode - After few seconds, RTC will wakeup the system.\n");

  /* Clear pending flags : Standby flag */
  LL_PWR_ClearFlag_SB();

  /* Enter STANDBY mode */
  HAL_PWR_EnterStandbyMode();

  /* This code can not be reached! */
  while (1)
  {
  }
} /* end app_process */


/** This function is never called and is provided as reference only.
  */
app_status_t app_deinit(void)
{
  return EXEC_STATUS_OK;
} /* end app_deinit */
