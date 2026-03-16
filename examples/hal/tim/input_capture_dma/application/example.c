/**
  ******************************************************************************
  * file           : example.c
  * brief          : TIM input capture DMA with HAL API
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
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define IC_DMA_BUFFER_SIZE 10U
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_tim_handle_t *pTIM;  /* pointer referencing the TIM handle from the generated code */

/* Flag to indicate that a DMA transfer complete event occurred */
volatile uint32_t TransferCompleteDetected = 0U;
/* Flag to indicate that a DMA transfer error event occurred */
volatile uint32_t TransferErrorDetected = 0U;

/* Measured frequency values (Hz) */
volatile uint32_t FrequencyHz = 0U;
volatile uint32_t MaxFrequencyHz = 0U;
volatile uint32_t MinFrequencyHz = 0U;
volatile uint32_t AvgFrequencyHz = 0U;

/* Timer counter clock (Hz) */
uint32_t TimerCounterClock = 0U;

/* DMA capture buffer for input capture values */
uint32_t IcDmaBuffer[IC_DMA_BUFFER_SIZE];

/* Private functions prototype -----------------------------------------------*/
static void ErrorCallback(hal_tim_handle_t *htim);
static void InputCaptureCallback(hal_tim_handle_t *htim, hal_tim_channel_t channel);

app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1 ##########
    * Initializes the GPIO and the TIM for input capture measurement.
    */
  pTIM = mx_example_tim_init();

  if (pTIM == NULL)
  {
    PRINTF("[ERROR] Step 1: Device initialization ERROR.\n");
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");

  /** ########## Register TIM callbacks ##########
    * Register the Input Capture and Error callbacks.
    */
  if (HAL_TIM_RegisterInputCaptureCallback(pTIM, InputCaptureCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_TIM_RegisterErrorCallback(pTIM, ErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /* Pre-compute timer counter clock for frequency calculations */
  TimerCounterClock = HAL_RCC_TIM_GetKernelClkFreq(MX_TIMx) / (HAL_TIM_GetPrescaler(pTIM) + 1U);

  /** ########## Step 2 ##########
    * Starts the timer in Input Capture DMA mode for the channel.
    */
  /* Clear the capture buffer */
  memset(IcDmaBuffer, 0, sizeof(IcDmaBuffer));
  /* Start the Input Capture in DMA mode */
  if (HAL_TIM_IC_StartChannel_DMA(pTIM,
                                  MX_TIM_CHANNEL,
                                  (uint8_t *)IcDmaBuffer,
                                  (uint32_t)(IC_DMA_BUFFER_SIZE * sizeof(uint32_t))) != HAL_OK)
  {
    PRINTF("[ERROR] Step 2: The input Capture in DMA mode started ERROR.\n");
    goto _app_init_exit;
  }

  /* Ensure the timer counter is running */
  if (HAL_TIM_Start(pTIM) != HAL_OK)
  {
    PRINTF("[ERROR] Step 2: TIM start ERROR.\n");
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 2: Timer input capture started in DMA mode.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  uint32_t cnt_diff = 0U;
  uint32_t cnt_max = 0U;
  uint32_t cnt_min = 0U;
  uint32_t cnt_avg = 0U;
  uint32_t timer_arr = 0U;

  /* Nothing to do until a DMA completion or error is detected. */
  while ((TransferCompleteDetected == 0U) && (TransferErrorDetected == 0U));

  /** ########## Step 3 ##########
    * Processes a completed DMA capture window.
    * Calculates min/max/avg frequency values over the capture buffer.
    */
  if (TransferCompleteDetected == 1U)
  {
    /* Get the timer Autoreload value */
    timer_arr = (uint32_t)HAL_TIM_GetPeriod(pTIM);

    /* Calculation of max, min & avg period values of the input signal with the buffer */
    for (uint8_t i = 0U ; i < (IC_DMA_BUFFER_SIZE - 1U) ; i++)
    {
      /* In case of the next value is smaller than the current one,
         it means that the counter restarted after reaching the Autoreload value.
         So the difference is made with the ARR value */
      if (IcDmaBuffer[i + 1U] > IcDmaBuffer[i])
      {
        cnt_diff = IcDmaBuffer[i + 1U] - IcDmaBuffer[i];
      }
      else
      {
        cnt_diff = (timer_arr - IcDmaBuffer[i]) + IcDmaBuffer[i + 1U] + 1U;
      }

      /* Load the first value as it is the maximum, otherwise take the value if it is higher */
      if ((i == 0U) || (cnt_diff > cnt_max))
      {
        cnt_max = cnt_diff;
      }

      /* Load the first value as it is the minimum, otherwise take the value if it is lower */
      if ((i == 0U) || (cnt_diff < cnt_min))
      {
        cnt_min = cnt_diff;
      }

      /* Add the differences for average calculation */
      cnt_avg += cnt_diff;
    }
    /* Average calculation */
    cnt_avg = cnt_avg / (IC_DMA_BUFFER_SIZE - 1U);

    /* Reshaping the values in frequency from Timer clock */
    if ((cnt_min != 0U) && (cnt_max != 0U) && (cnt_avg != 0U))
    {
      MaxFrequencyHz = TimerCounterClock / cnt_min;
      MinFrequencyHz = TimerCounterClock / cnt_max;
      AvgFrequencyHz = TimerCounterClock / cnt_avg;
      FrequencyHz = AvgFrequencyHz;
    }
    else
    {
      MaxFrequencyHz = 0U;
      MinFrequencyHz = 0U;
      AvgFrequencyHz = 0U;
      FrequencyHz = 0U;
    }

    PRINTF("[INFO] Step 3: The measured frequency is: %lu Hz \n", (unsigned long)FrequencyHz);

    TransferCompleteDetected = 0U;
  }
  else
  {
    /* On error, clear flag and reset computed values to a safe state. */
    return_status = EXEC_STATUS_ERROR;
    goto _app_process_exit;
  }

  /* Restart a new measurement window */
  memset(IcDmaBuffer, 0, sizeof(IcDmaBuffer));
  TransferCompleteDetected = 0U;
  TransferErrorDetected = 0U;
  (void)HAL_TIM_IC_StopChannel_DMA(pTIM, MX_TIM_CHANNEL);
  if (HAL_TIM_IC_StartChannel_DMA(pTIM,
                                  MX_TIM_CHANNEL,
                                  (uint8_t *)IcDmaBuffer,
                                  (uint32_t)(IC_DMA_BUFFER_SIZE * sizeof(uint32_t))) != HAL_OK)
  {
    return_status = EXEC_STATUS_ERROR;
  }
  else
  {
    return_status = EXEC_STATUS_OK;
  }

_app_process_exit:

  return return_status;
} /* end app_process */


/** The app_deinit() function is provided as a reference only.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  (void)HAL_TIM_IC_StopChannel_DMA(pTIM, MX_TIM_CHANNEL);

  mx_example_tim_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** TIM input capture callback.
  * This callback is invoked by the HAL when an input capture DMA transfer window completes.
  * It sets a flag that is polled by the main loop in app_process().
  */
static void InputCaptureCallback(hal_tim_handle_t *htim, hal_tim_channel_t channel)
{
  if ((htim == pTIM) && (channel == MX_TIM_CHANNEL))
  {
    TransferCompleteDetected = 1U;
  }
}

/** TIM error callback.
  * This callback is invoked by the HAL when an error occurs on the TIM instance.
  * It sets a flag that is polled by the main loop in app_process().
  */
static void ErrorCallback(hal_tim_handle_t *htim)
{
  if (htim == pTIM)
  {
    TransferErrorDetected = 1U;
  }
}
