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

/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * The applicative code initializes the LPTIM instance and start the LPTIM peripheral.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pLPTIM = mx_example_lptim_init();

  if (pLPTIM == NULL)
  {
    goto _app_init_exit;
  }

  if (HAL_LPTIM_OC_StartChannel(pLPTIM, MX_LPTIM_CHANNEL) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_LPTIM_Start(pLPTIM) != HAL_OK)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * The device goes in stop mode. The PWM output signal is generated while in STOP mode thanks to the external clock.
  */
app_status_t app_process(void)
{
  /** In debug mode, it is necessary to suspend the Tick increment to prevent WakeUp by Systick interrupt.
    * Otherwise the Systick interrupt wakes the device up within 1ms (HAL time base).
    */
  HAL_SuspendTick();

  /* Enter STOP Mode with WaitForInterrupt parameter, any interrupt enabled in NVIC would wake the system up. */
  HAL_PWR_EnterStopMode(HAL_PWR_LOW_PWR_MODE_WFI, MX_PWR_STOPy_MODE);

  /* Restore the systick */
  HAL_ResumeTick();

  /* Reconfigure the clock.*/
  mx_rcc_init();

  /* In this example, the MCU will not wake up. */
  return EXEC_STATUS_ERROR;
} /* end app_process */


/** De-initializes the LPTIM instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_lptim_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
