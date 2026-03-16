/**
  ******************************************************************************
  * file           : example.c
  * brief          : TIM timebase with LL API
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
  * Initializes the MX_TIMx for time base and GPIO for status LED.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_timx_init() == NULL)
  {
    goto _app_init_exit;
  }

  if (mx_gpiox_init() != SYSTEM_OK)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Starts the MX_TIMx time base.
  */
app_status_t app_process(void)
{

  TIM_Start_IT();

  return EXEC_STATUS_OK;
} /* end app_process */


/** Deinitializes the MX_TIMx and GPIO and peripherals before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_gpiox_deinit() != SYSTEM_OK)
  {
    goto _app_deinit_exit;
  }

  mx_timx_deinit();

  return_status = EXEC_STATUS_OK;
_app_deinit_exit:
  return return_status;
} /* end app_deinit */


/** ########## Step 3 ##########
  * MX_TIMx time base callback
  * This implementation of the MX_TIMx time base callback can be customized.
  * This function is executed when the MX_TIMx time base interrupt is generated.
  */
void TimebaseCallback()
{
  /* Time base generation, LED toggled in accordance with the selected time base frequency. */
  GPIO_TogglePin();
}
