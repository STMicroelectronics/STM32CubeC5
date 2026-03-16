/**
  ******************************************************************************
  * file           : example.c
  * brief          : example program body
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

hal_lptim_handle_t *pLPTIM;  /* pointer referencing the LPTIM handle from the generated code */
volatile uint32_t PulseCount = 0;
volatile uint32_t RepetitionCounterUnderflow = 0;

/* Private functions prototype -----------------------------------------------*/
static void UpdateCallback(hal_lptim_handle_t *hlptim);
static void CompareMatchCallback(hal_lptim_handle_t *hlptim, hal_lptim_channel_t channel);

/** ########## Step 1 ##########
  * The applicative code initializes the LPTIM instance, registers the callbacks for the update event and the
  * compare match event and starts both the LPTIM channel and the LPTIM in interrupt mode.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pLPTIM = mx_example_lptim_init();
  if (pLPTIM == NULL)
  {
    goto _app_init_exit;
  }

  if (HAL_LPTIM_RegisterUpdateCallback(pLPTIM, UpdateCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_LPTIM_RegisterCompareMatchCallback(pLPTIM, CompareMatchCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_LPTIM_OC_StartChannel_IT(pLPTIM, MX_LPTIM_CHANNEL_X) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_LPTIM_Start_IT(pLPTIM) != HAL_OK)
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

  uint32_t repetition_counter;
  uint32_t expected_pulse_count;

  /** ########## Step 2 ##########
    * Wait for the low-power timer's repetition counter underflow event, then
    * verify that the number of output pulses generated matches the expected
    * count, signaling an error if there is a mismatch.
    */
  while (RepetitionCounterUnderflow != 1)
  {
    /** Put the CPU in Wait For Interrupt state. An LPTIM interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupt mode.
      *        It can be replaced by your own code.
      */
    __WFI();
  }

  /* Get the current value of the repetition counter */
  repetition_counter = HAL_LPTIM_GetRepetitionCounter(pLPTIM);

  /* Calculate the expected number of pulses */
  expected_pulse_count = repetition_counter + 1;

  /* Verify that the expected number of pulses was generated */
  if (PulseCount == expected_pulse_count)
  {
    return_status = EXEC_STATUS_OK;
  }

  return return_status;
} /* end app_process */


/** De-initializes the LPTIM instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_lptim_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** Update Callback.
  * @user This callback can be customized.
  * This function is called when a repetition counter underflow event is detected.
  * The repetition counter underflow event is communicated to the software via the
  * update event.
  * NOTE: this callback is invoked in interrupt (IT) context.
  */
static void UpdateCallback(hal_lptim_handle_t *hlptim)
{
  RepetitionCounterUnderflow = 1;
}

/** Compare match Callback.
  * @user This callback can be customized.
  * This function is invoked when the counter value matches the configured compare value.
  * NOTE: this callback is invoked in interrupt (IT) context.
  */
static void CompareMatchCallback(hal_lptim_handle_t *hlptim,
                                 hal_lptim_channel_t channel)
{
  if (channel == MX_LPTIM_CHANNEL_X)
  {
    /* Increment the pulse count */
    PulseCount++;
  }
}

