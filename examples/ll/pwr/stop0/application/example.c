/**
  ******************************************************************************
  * file           : example.c
  * brief          : Enter and exit the Stop0 mode through an EXTI interrupt using the LL API
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
/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * The application configures the system to reach typical product consumption.
  * The application configures the wake-up source to wake-up the MCU from Stop0 mode.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Configures the system to reach the typical product consumption */
  if (mx_rccx_init() != SYSTEM_OK)
  {
    goto _app_init_exit;
  }

  if (mx_pwrx_init() != SYSTEM_OK)
  {
    goto _app_init_exit;
  }

  RCC_DisableBuses();

  /* Configures the wake-up source */
  if (mx_extix_init() != SYSTEM_OK)
  {
    goto _app_init_exit;
  }

  EXTI_EnableLineMode_IT();

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * The application clears pending flags, preparing to enter Stop0 mode.
  * The application requests entry into Stop0 mode.
  * The application checks whether the system was in Stop0 mode.
  * The application reconfigures the system clock after exiting Stop0 mode.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Clear pending flags: Stop flag */
  PWR_ClearStopFlag();

  /* Enter Stop0 low-power mode */
  PWR_EnterStop0Mode();

  /* Execution resumes here when the MCU wakes up */
  if (PWR_CheckSystemStop0Mode() != SYSTEM_OK)
  {
    goto _app_process_exit;
  }

  /* Reconfigure the system clock configuration after exiting Stop0 mode */
  if (mx_rccx_init() != SYSTEM_OK)
  {
    goto _app_process_exit;
  }

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** This function is never called and is provided as reference only.
  * Deinitializes the wake-up source before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  return EXEC_STATUS_OK;
} /* end app_deinit */
