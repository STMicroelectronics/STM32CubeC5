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
/** VDDA must be computed not static.
  * @user: The Analog Voltage supply, VDDA, might be different from VDD and might require to be tuned.
  */
#define VDDA_APPLI_MILLIVOLT  3300UL

/** output voltage value of the DAC
  * @user: you can change the value to change the output voltage of the DAC.
  */
#define DAC_VALUE   ((3 * VDDA_APPLI_MILLIVOLT) / 4)

/* Private variables ---------------------------------------------------------*/
hal_dac_handle_t *pDAC;  /* pointer referencing the DAC handle from the generated code */

/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * The init of the DAC instance is triggered by the applicative code. The DAC is started.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  uint16_t dac_raw_value = LL_DAC_CALC_VOLTAGE_TO_DATA(VDDA_APPLI_MILLIVOLT, DAC_VALUE, LL_DAC_RESOLUTION_8B);
  pDAC = mx_example_dac_init();

  if (pDAC == NULL)
  {
    goto _app_init_exit;
  }

  if (HAL_DAC_SetChannelData(pDAC, MX_DAC_CHANNEL, dac_raw_value) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_DAC_StartChannel(pDAC, MX_DAC_CHANNEL) != HAL_OK)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * The device enters the STOP mode. The DAC still output the signal due to the sample and
  * hold feature that allows the DAC to still perform conversion even in low power mode like STOP mode.
  */
app_status_t app_process(void)
{
  /** Enter STOP Mode with WaitForInterrupt parameter, any interrupt enabled in NVIC would wake the system up.
    * The DAC still performs conversion in STOP Mode due to the sample and hold feature.
    * Note: systick will wake up the device from STOP mode regularly.
    */

  HAL_PWR_EnterStopMode(HAL_PWR_LOW_PWR_MODE_WFI, MX_STOP_MODE);

  /** When exiting from STOP mode, the clock might need to be reconfigured. See STOP mode examples. **/

  return EXEC_STATUS_OK;
} /* end app_process */


/** Deinitializes the DAC instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_dac_deinit();
  return EXEC_STATUS_OK;
} /* end app_deinit */

