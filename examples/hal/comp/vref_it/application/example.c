/**
  ******************************************************************************
  * file           : example.c
  * brief          : A comparator peripheral to compare a voltage level applied
  *                  on a GPIO pin to the internal voltage reference, in interrupt
  *                  mode.
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
/* Definition of the time spent in RUN mode.*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_comp_handle_t *pCOMP;  /* pointer referencing the COMP handle from the generated code */
volatile uint8_t CompCallback;

/* Private functions prototype -----------------------------------------------*/
static void CompCpltCallback(hal_comp_handle_t *CompCallback);

app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1 ##########
    * The init of the COMP instance is triggered by the applicative code.
    */

  pCOMP = mx_example_comp_init();

  if (pCOMP == NULL)
  {
    goto _app_init_exit;
  }

  if (HAL_COMP_RegisterOutputTriggerCallback(pCOMP, CompCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /** ########## Step 2 ##########
    * Starts the comparator in interrupt mode.
    */
  if (HAL_COMP_Start_IT(pCOMP) != HAL_OK)
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
  CompCallback = 0;

  /** ########## Step 3 ##########
    * The application switches to STOP mode.
    */
  /** In debug mode, it is necessary to suspend the Tick increment to prevent WakeUp by Systick interrupt.
     * Otherwise the Systick interrupt wakes the device up within 1ms (HAL time base).
     */
  HAL_SuspendTick();

  /* Enter STOP Mode with WaitForInterrupt parameter, after the system wake-up triggered by the comparator interrupt.*/

  HAL_PWR_EnterStopMode(MX_PWR_LOW_PWR_MODE_ENTRY, MX_PWR_STOP_MODE);

  /** ########## Step 4 ##########
     * The comparator generates an interrupt and system wakes-up from Stop mode. The CompCallback is expected.
     * then the application re-configures the clock.
     */
  /* Restore the systick */
  HAL_ResumeTick();

  /* Reconfigure the clock.*/
  mx_clock_config();

  if (CompCallback)
  {
    return_status = EXEC_STATUS_OK;
  }

  return  return_status;
} /* end app_process */


/* De-initializes the COMP instance. */
app_status_t app_deinit(void)
{
  mx_example_comp_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


static void CompCpltCallback(hal_comp_handle_t *hComp)
{
  CompCallback = 1U;
}

