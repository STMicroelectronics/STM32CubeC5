/**
  ******************************************************************************
  * file           : example.c
  * brief          : DAC perform a simple conversion in low power mode using
  *                  the sample and hold feature with LL API.
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
/** MX_DACx output voltage value.
  * @user: you can change the value to change the output voltage of the DAC.
  */
#define DAC_SIGNAL_VALUE ((3U * DAC_VREFPLUS_VALUE) / 4U)

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The init of the MX_DACx instance and the conversion start is triggered by the applicative code.
  * - MX_DACx is configured.
  */
app_status_t app_init(void)
{
  uint16_t     dac_raw_value = DAC_GetVoltageRawData(DAC_SIGNAL_VALUE);
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_dacx_init() == NULL)
  {
    goto _app_init_exit;
  }

  /* Start the MX_DACx channel */
  DAC_StartChannel();

  DAC_SetChannelData(dac_raw_value);

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * At each call, the MX_DACx outputs a voltage specified by DAC_SIGNAL_VALUE.
  * The device enters low power mode and the MX_DACx still output the signal.
  */
app_status_t app_process(void)
{
  /** Enter the system to low power Mode.
    */
  mx_pwr_enter_low_power();

  return EXEC_STATUS_OK;
} /* end app_process */


/**
  * Deinitializes the MX_DACx instance.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_dacx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
