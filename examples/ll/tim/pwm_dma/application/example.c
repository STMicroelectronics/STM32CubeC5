/**
  ******************************************************************************
  * file           : example.c
  * brief          : TIM PWM DMA with LL API
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
#define BUFFER_SIZE 3U
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Compare buffer */
uint32_t CompareBuffer[BUFFER_SIZE] =
{
  MX_PERIOD_VALUE * 1 / 4,
  MX_PERIOD_VALUE * 2 / 4,
  MX_PERIOD_VALUE * 3 / 4
};

/* Private functions prototype -----------------------------------------------*/
/** ########## Step 1 ##########
  * Initializes the GPIO and the TIM for PWM generation.
  * The init of the TIM and the GPIO is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_timx_init() == NULL)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{

  /** ########## Step 2 ##########
    * Starts the Timer PWM generation with variable duty cycle using DMA after each update event
    * Data buffer size in byte
    */
  TIM_OC_StartChannel_DMA((uint8_t *)CompareBuffer, BUFFER_SIZE * sizeof(uint32_t));

  TIM_Start();

  return EXEC_STATUS_OK;
} /* end app_process */


/** Deinitializes the GPIO and TIM peripherals before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_timx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
