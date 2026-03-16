/**
  ******************************************************************************
  * file           : example.c
  * brief          : generate an infinite sequence of pulses with precise timing control, with LL API,
  *                  followed by a permanent reset of the output signal.
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
  * Initializes the MX_TIMx and the GPIO for PWM generation.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_timx_init() != NULL)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Starts the MX_TIMx PWM generation for both channels
  */
app_status_t app_process(void)
{
  /* Start the MX_TIMx channels */
  TIM_StartChannel(MX_TIM_1X_CHANNEL);
  TIM_StartChannel(MX_TIM_2X_CHANNEL);

  /* Start the MX_TIMx */
  TIM_Start();

  return EXEC_STATUS_OK;
} /* end app_process */


/** Deinitializes the MX_TIMx and GPIO and peripherals before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_timx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
