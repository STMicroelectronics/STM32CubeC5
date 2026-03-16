/**
  ******************************************************************************
  * file           : example.c
  * brief          : generate a finite sequence of pulses with precise timing control followed by a permanent reset of
  *                  the output signal using LL API.
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
volatile uint32_t PulseCount               = 0U; /* Variable to check the counted pulses           */
volatile uint32_t RepetitionCountUnderflow = 0U; /* Variable to check repetition counter underflow */

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the MX_LPTIMx instance, starts the MX_LPTIMx channel in interrupt mode then
  * starts the MX_LPTIMx.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_lptimx_init() == NULL)
  {
    goto _app_init_exit;
  }

  LPTIM_StartCompareMatchChannel_IT();

  LPTIM_Start_IT();

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Wait for the low-power timer's repetition counter underflow event, then verify that the number of output pulses
  * generated matches the expected count, signaling an error if there is a mismatch.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  while (RepetitionCountUnderflow != 1U)
  {
    /** Put the CPU in Wait For Interrupt state. An LPTIM interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupt mode.
      *        It can be replaced by your own code.
      */
    __WFI();
  }

  if (PulseCount == LPTIM_GetRepetitionCounter() + 1U)
  {
    return_status = EXEC_STATUS_OK;
  }

  return return_status;
} /* end app_process */


/** Deinitializes the MX_LPTIMx before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_lptimx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/**
  * MX_LPTIMx update callback
  * @user This implementation of the MX_LPTIMx update callback can be customized.
  * This function is executed when the MX_LPTIMx update interrupt is generated.
  */
void LPTIM_UpdateCallback(void)
{
  RepetitionCountUnderflow = 1U;
}

/**
  * MX_LPTIMx compare match callback
  * @user This implementation of the MX_LPTIMx compare match callback can be customized.
  * This function is executed when the MX_LPTIMx compare match interrupt is generated.
  */
void LPTIM_CompareMatchCallback(void)
{
  /* Increment the pulse count */
  PulseCount++;
}
