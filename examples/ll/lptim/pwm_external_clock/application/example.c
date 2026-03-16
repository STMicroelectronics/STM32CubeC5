/**
  ******************************************************************************
  * file           : example.c
  * brief          : Generate LPTIM PWM signal using external clock with LL API.
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
/* Private functions ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The init and the start of the MX_LPTIMx instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Configures the MX_LPTIMx instance */
  if (mx_lptimx_init() == NULL)
  {
    goto _app_init_exit;
  }

  LPTIM_LowPowerConfig();

  /* Start a MX_LPTIMx output channel in polling mode */
  LPTIM_EnableOutputCompareChannel();

  /* Start the MX_LPTIMx in polling mode */
  LPTIM_Start();

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * The device enters low power mode.
  * The PWM output signal using external clock is generated while the device is in low power mode.
  */
app_status_t app_process(void)
{
  mx_pwr_enter_low_power();

  /* Reconfigure the clock to be used later by the systick counting */
  mx_pwr_exit_low_power();

  /* In this example, the MCU will not wake up. */
  return EXEC_STATUS_ERROR;
} /* end app_process */


/**
  * Deinitializes the MX_LPTIMx instance.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_lptimx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
