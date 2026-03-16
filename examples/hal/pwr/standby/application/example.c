/**
  ******************************************************************************
  * file           : example.c
  * brief          : Enter and exit the Standby mode through a wakeup pin using the HAL API
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
  * The application configures the system to reach typical product consumption.
  * The application configures the wake-up source to wake-up the MCU from Standby mode.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Configures the system to reach the typical product consumption */
  if (mx_rccx_init() != SYSTEM_OK)
  {
    goto _app_init_exit;
  }

  /* Configures the system to reach the typical product consumption and the wake-up source */
  if (mx_pwrx_init() != SYSTEM_OK)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * The application clears pending flags, preparing to enter Standby mode.
  * The application requests entry into Standby mode.
  */
app_status_t app_process(void)
{
  /* Clear pending flags: Wakeup pin and Standby flags */
  HAL_PWR_LP_CleanWakeupSource(PWR_WAKEUP_SOURCE);
  HAL_PWR_CleanPreviousSystemPowerMode();

  /* Enter Standby mode */
  HAL_PWR_EnterStandbyMode();

  /* This code is not expected to be reached. */
  return EXEC_STATUS_ERROR;
} /* end app_process */


/** This function is never called and is provided as reference only.
  */
app_status_t app_deinit(void)
{
  return EXEC_STATUS_OK;
} /* end app_deinit */
