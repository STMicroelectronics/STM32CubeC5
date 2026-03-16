/**
  ******************************************************************************
  * file           : example.c
  * brief          : TIM input capture with HAL API
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

/** Frequency Value
  * The 'volatile' keyword is used here because 'FrequencyHz' is accessed in two separate contexts:
  * 1. It is updated in the interrupt context (InputCaptureCallback).
  * 2. It is read in the main loop to print the frequency when USE_TRACE is activated.
  *
  * The 'volatile' keyword ensures that the compiler does not optimize out accesses to this variable,
  * guaranteeing that the main loop always reads the most recent value updated by the interrupt.
  */
volatile uint32_t            FrequencyHz = 0;
/* Captured Value */
uint32_t ic_value1     = 0;
uint32_t ic_value2     = 0;

/* Volatile Capture Index Value */
volatile uint16_t CaptureIndex = 0;

/* Timer counter clock */
uint32_t TimerCounterClock;

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
      * Starts the timer in Input Capture mode for the channel.
      */
    if (HAL_TIM_IC_StartChannel_IT(pTim, MX_TIM_CHANNEL) != HAL_OK)
    {
      goto _app_init_exit;
    }

    if (HAL_TIM_Start(pTim) != HAL_OK)
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
    * Logs the value of the frequency of the external PWM signal (if USE_TRACE is enabled).
    * The displayed value is always the last measured value.
    */
  PRINTF("[INFO] Frequency %" PRIu32 " Hz\n", FrequencyHz);

  return return_status;
} /* end app_process */


/** The app_deinit() function is provided as a reference only.
  * This can be used to de-initialize GPIO and TIM peripherals in case of error.
  */
app_status_t app_deinit(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_gpio_status_led_deinit() != 0)
  {
    goto _app_deinit_exit;
  }

  if (HAL_TIM_IC_StopChannel_IT(pTim, MX_TIM_CHANNEL) != HAL_OK)
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

  uint32_t diff_Capture = 0;
  uint32_t val_max;

  if (channel == MX_TIM_CHANNEL)
  {
    /** ########## Step 3 ##########
      * Measures the frequency by measuring the time between two successive rising edges of the external input signal.
      * (asynchronous Input Capture interrupt processing).
      */
    if (CaptureIndex == 0)
    {
      /* Get the 1st Input Capture value */
      ic_value1 = HAL_TIM_IC_ReadChannelCapturedValue(hTIM, MX_TIM_CHANNEL);
      CaptureIndex = 1;
    }
    else if (CaptureIndex == 1)
    {
      /* Get the 2nd Input Capture value */
      ic_value2 = HAL_TIM_IC_ReadChannelCapturedValue(hTIM, MX_TIM_CHANNEL);

      /* Time difference computation */
      if (ic_value2 > ic_value1)
      {
        diff_Capture = (ic_value2 - ic_value1);
      }
      else if (ic_value2 < ic_value1)
      {
        if (IS_TIM_32B_COUNTER_INSTANCE(MX_TIMX))
        {
          /* Maximum value for 32-bit timers */
          val_max = 0xFFFFFFFF;
        }
        else
        {
          /* Maximum value for 16-bit timers */
          val_max = 0xFFFF;
        }
        diff_Capture = ((val_max - ic_value1) + ic_value2) + 1;
      }

      /* Frequency computation: for this example TIMx is clocked by system frequency */
      FrequencyHz = TimerCounterClock / diff_Capture;
      CaptureIndex = 0;
    }
  }
}
