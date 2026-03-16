/**
  ******************************************************************************
  * file           : example.c
  * brief          : Capturing external signals with the LPTIM using the LL API.
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
#define CAPTURE_MAX_TOLERANCE      (1.10) /* 110 % of the frequency */
#define CAPTURE_MIN_TOLERANCE      (0.90) /* 90 % of the frequency  */

/* @user MX_LPTIMx input signal frequency:
   update the input signal information according your external input signal frequency */
#define INPUT_SIGNAL_FREQ          (100)  /* 100Hz */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t MeasuredInputSignalCounter;    /* Counter measured from the input signal */
uint16_t ExpectedInputSignalCounter;    /* Expected counter value for the input signal */

/* Buffer to store captured values from the input signal using DMA */
uint16_t DMACapturedValue[CAPTURED_VALUE_BUFFER_SIZE] = {0};

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the MX_LPTIMx instance in Input Capture Mode with an external signal connected to
  * MX_LPTIMx Channel.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_lptimx_init() == NULL)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;


  /** ########## Step 2 ##########
    * Starts the MX_LPTIMx channel input capture in DMA mode.
    * The device goes in the deepest low power mode without impacting the MX_LPTIMx functional.
    * Upon completion of the transfer, the DMA generates an interrupt to wake up the MCU.
    */
  LPTIM_StartInputCaptureChannel_DMA(DMACapturedValue);

  LPTIM_Start();

  /* Enter in low power mode */
  mx_pwr_enter_low_power();


  /** ########## Step 3 ##########
    * Checks that the system resumes from low power mode.
    * Calculate the expected and measured input signal counter.
    */
  if (mx_pwr_system_was_low_power() != 0U)
  {
    /* Compute expected tick value */
    ExpectedInputSignalCounter = LSE_VALUE / INPUT_SIGNAL_FREQ;

    /* Get the Input Capture value */
    if (DMACapturedValue[1U] > DMACapturedValue[0U])
    {
      MeasuredInputSignalCounter = DMACapturedValue[1U] - DMACapturedValue[0U];
    }
    else if (DMACapturedValue[1U] < DMACapturedValue[0U])
    {
      /* 0xFFFF is the max MX_LPTIMx count value */
      MeasuredInputSignalCounter = ((0xFFFFU - DMACapturedValue[0U]) + DMACapturedValue[1U]) + 1U;
    }
    else
    {
      /* If capture values are equal, we have reached the limit of frequency measures */
      goto _app_process_exit;
    }

    if ((MeasuredInputSignalCounter  < (ExpectedInputSignalCounter * CAPTURE_MAX_TOLERANCE))
        && (MeasuredInputSignalCounter  > (ExpectedInputSignalCounter * CAPTURE_MIN_TOLERANCE)))
    {
      return_status = EXEC_STATUS_OK;
    }
  }

_app_process_exit:
  return return_status;
} /* end app_process */


/** De-initializes the MX_LPTIMx instance before leaving the scenario. */
app_status_t app_deinit(void)
{
  mx_lptimx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
