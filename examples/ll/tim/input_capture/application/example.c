/**
  ******************************************************************************
  * file           : example.c
  * brief          : TIM capturing external signals in interrupt mode with the LL API.
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
volatile uint8_t InputCaptureState = 0U;
volatile uint32_t FrequencyHz;
uint32_t TimerCounterClock;

/* Private functions prototype -----------------------------------------------*/

app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1 ##########
    * Initializes the MX_TIMx for PWM input measurement.
    */
  if (mx_timx_init() != NULL)
  {
    TimerCounterClock = RCC_TIM_GetKernelClkFreq() / (MX_TIM_PRESCALER + 1);

    /** ########## Step 2 ##########
      * Starts the MX_TIMx in channel in Input Capture mode.
      * Starts the MX_TIMx peripheral.
      */
    TIM_StartInputCaptureChannel_IT();

    TIM_Start();

    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  static uint16_t capture_idx = 0U; /* Capture Index Value */
  static uint32_t ic_value1;        /* Captured Value 1    */
  static uint32_t ic_value2;        /* Captured Value 2    */

  uint32_t diff_Capture = 0U;
  uint32_t val_max      = TIM_GetCounterMaxValue();

  /** ########## Step 3 ##########
    * Measures the frequency by capturing the time between two successive rising edges of the external input signal.
    * (asynchronous Input Capture interrupt processing).
    */
  if (InputCaptureState == 1U)
  {
    if (capture_idx == 0U)
    {
      /* Get the 1st Input Capture value */
      ic_value1    = TIM_ReadChannelCapturedValue();
      capture_idx = 1U;
    }
    else
    {
      /* Get the 2nd Input Capture value */
      ic_value2 = TIM_ReadChannelCapturedValue();

      /* Time difference computation */
      if (ic_value2 > ic_value1)
      {
        diff_Capture = (ic_value2 - ic_value1);
      }
      else if (ic_value2 < ic_value1)
      {
        diff_Capture = ((val_max - ic_value1) + ic_value2) + 1U;
      }

      /* Frequency computation: for this example MX_TIMx is clocked by system frequency */
      FrequencyHz = TimerCounterClock / diff_Capture;

      capture_idx = 0U;
    }

    InputCaptureState = 0U;
  }

  return EXEC_STATUS_OK;
} /* end app_process */


/** De-initializes the MX_LPTIMx instance before leaving the scenario. */
app_status_t app_deinit(void)
{
  mx_timx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/**
  * MX_LPTIMx input capture callback
  * @user This implementation of the MX_LPTIMx controller input capture callback can be customized.
  * This function is executed when the MX_LPTIMx input capture interrupt is generated.
  */
void TIM_InputCaptureCallback(void)
{
  InputCaptureState = 1U;
}
