/**
  ******************************************************************************
  * file           : example.c
  * brief          : Compare a voltage level applied on a GPIO pin
  *                  to the internal voltage reference, in interrupt mode.
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
volatile uint8_t COMP_OutputTrigger; /* Flag raised when the comparator detects a crossing. */
/* Private functions ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/


app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1 ##########
    * Initializes the COMP instance.
    */
  if (mx_compx_init() != NULL)
  {
    /** ########## Step 2 ##########
      * Starts the comparator in interrupt mode.
      */
    COMP_Activate_IT();

    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  COMP_OutputTrigger = 0U;

  /** ########## Step 3 ##########
    * The application switches to low power mode.
    */
  mx_pwr_enter_low_power();

  /** ########## Step 4 ##########
     * The comparator generates an interrupt
     * and the system wakes up from low power mode.
     */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  if (mx_pwr_system_was_low_power() == 0U)
  {
    /* An error occurs during transfer */
    goto _app_process_exit;
  }
#endif /* USE_LL_APP_ERROR */

  /* Reconfigure the system clock configuration for systick counting */
  mx_pwr_exit_low_power();

  /* Check that the system woke up for the expected reason. */
  if (COMP_OutputTrigger == 1U)
  {
    return_status = EXEC_STATUS_OK;
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
_app_process_exit:
#endif /* USE_LL_APP_ERROR */
  return return_status;
} /* end app_process */


/**
  * Deinitializes the MX_COMPx instance.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_compx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/* User callback */
void COMP_TriggerOutputCallback()
{
  /* The comparator input crossed (rising or falling edge) the internal reference voltage VrefInt. */
  COMP_OutputTrigger = 1U;
}
