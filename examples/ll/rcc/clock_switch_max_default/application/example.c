/**
  ******************************************************************************
  * file           : example.c
  * brief          : RCC clock switch max default
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
/* @user: configure the delay in ms between intermediate steps */
#define EXAMPLE_STEP_DELAY_MS 1000U

/* Private variables ---------------------------------------------------------*/

/* Informative-only: the following variables store SYSCLK frequency values
  read at runtime for user information purposes; they do not affect behavior. */
uint32_t DefaultSysclkFrequency = 0;
uint32_t MaxSysclkFrequency = 0;

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * In RCC examples, the init has already been triggered by mx_system_init() from main.
  * Generated SYSCLK signal is routed to the MCO pin.
  */
app_status_t app_init(void)
{
  /* Informative-only: this function performs no runtime initialization.
   * in main; app_init() is provided to document the flow and status only. */
  MaxSysclkFrequency = RCC_GetSYSCLKFreq();

  return EXEC_STATUS_INIT_OK;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * Switch the clock source from Max to Default then waits for 1s.
    */
  if (RCC_ResetSystemClock() != SYSTEM_OK)
  {
    goto _app_process_exit;
  }
  DefaultSysclkFrequency = RCC_GetSYSCLKFreq();
  DelayMs(EXAMPLE_STEP_DELAY_MS);

  /** ########## Step 3 ##########
    * Switch the clock source from Default to Max.
    */

  /* Disable Oscillator */
  RCC_DisableOscillator();

  if (mx_rccx_init() != SYSTEM_OK)
  {
    goto _app_process_exit;
  }
  MaxSysclkFrequency = RCC_GetSYSCLKFreq();

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */
