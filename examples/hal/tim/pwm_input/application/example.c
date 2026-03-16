/**
  ******************************************************************************
  * file           : example.c
  * brief          : TIM PWM input with HAL API
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
hal_tim_handle_t *pTim;  /* pointer referencing the TIM handle from the generated code */

/* Timer counter clock */
uint32_t TimerCounterClock;
/* Duty Cycle Value */
volatile uint32_t            DutyCyclePercent = 0;
/* Frequency Value */
volatile uint32_t            FrequencyHz = 0;

/* Private functions prototype -----------------------------------------------*/
static void InputCaptureCallback(hal_tim_handle_t *hTIM, hal_tim_channel_t);

app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1 ##########
    * Initializes the GPIO and the TIM for PWM input measurement.
    */

  pTim = mx_example_tim_init();
  if (pTim != NULL)
  {
    /* Register the callback for input capture interrupt */
    if (HAL_TIM_RegisterInputCaptureCallback(pTim, InputCaptureCallback) != HAL_OK)
    {
      goto _app_init_exit;
    }

    /** ########## Step 2 ##########
      * Starts the timer in Input Capture mode for the two channels.
      */
    if (HAL_TIM_IC_StartChannel_IT(pTim, MX_TIM_CHANNEL_Y) != HAL_OK)
    {
      goto _app_init_exit;
    }

    if (HAL_TIM_IC_StartChannel_IT(pTim, MX_TIM_CHANNEL_Z) != HAL_OK)
    {
      goto _app_init_exit;
    }

    TimerCounterClock = HAL_RCC_TIM_GetKernelClkFreq(MX_TIMX) / (MX_TIM_PRESCALER + 1);

    return_status = EXEC_STATUS_INIT_OK;
  }

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_OK;

  /** ########## Step 4 ##########
    * Logs the value of the frequency and the duty cycle of the external PWM signal (if USE_TRACE is enabled).
    * The displayed value is always the last measured value.
    */
  PRINTF("[INFO] PWM frequency %" PRIu32 " Hz", FrequencyHz);
  PRINTF(" - duty cycle %" PRIu32 " %%\n", DutyCyclePercent);

  return return_status;
} /* end app_process */


/** The app_deinit() function is provided as a reference only.
  * This can be used to de-initialize GPIO and TIM peripherals in case of error.
  */
app_status_t app_deinit(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (HAL_TIM_IC_StopChannel_IT(pTim, MX_TIM_CHANNEL_Y) != HAL_OK)
  {
    goto _app_deinit_exit;
  }
  if (HAL_TIM_IC_StopChannel_IT(pTim, MX_TIM_CHANNEL_Z) != HAL_OK)
  {
    goto _app_deinit_exit;
  }

  mx_example_tim_deinit();

  return_status = EXEC_STATUS_OK;

_app_deinit_exit:
  return return_status;
} /* end app_deinit */


/** brief  Input Capture callback in non blocking mode
  * param  hTIM : TIM IC handle
  * retval None
  */
static void InputCaptureCallback(hal_tim_handle_t *hTIM, hal_tim_channel_t channel)
{
  /* Captured Value */
  uint32_t ic_rising_value;
  uint32_t ic_falling_value;

  if (channel == MX_TIM_CHANNEL_Y)
  {
    /** ########## Step 3 ##########
      * Measures of the frequency and the duty cycle on each rising edge of the external input signal.
      * (asynchronous Input Capture interrupt processing).
      */
    ic_rising_value = HAL_TIM_IC_ReadChannelCapturedValue(hTIM, MX_TIM_CHANNEL_Y);
    if (ic_rising_value != 0)
    {
      ic_falling_value = HAL_TIM_IC_ReadChannelCapturedValue(hTIM, MX_TIM_CHANNEL_Z);
      if (ic_falling_value != 0)
      {
        DutyCyclePercent = (ic_falling_value * 100) / ic_rising_value;

        FrequencyHz = TimerCounterClock / ic_rising_value;
      }
    }
    else
    {
      DutyCyclePercent = 0;
      FrequencyHz = 0;
    }
  }
}
