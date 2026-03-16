/**
  ******************************************************************************
  * file           : example.c
  * brief          : DAC generate signals without using any trigger signal with LL API.
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
#define BUFFER_SIZE     (4U)

/** MX_DACx output voltage values.
  * @user: you can change the values to change the output voltage of the DAC.
  */
#define DAC_SIGNAL_VALUE_0 (0U)
#define DAC_SIGNAL_VALUE_1 (DAC_VREFPLUS_VALUE / 3U)
#define DAC_SIGNAL_VALUE_2 ((2U * DAC_VREFPLUS_VALUE) / 3U)
#define DAC_SIGNAL_VALUE_3 (DAC_VREFPLUS_VALUE)

/* DAC output value array */
const uint16_t DACValueMilliVoltArray[BUFFER_SIZE] =
{
  DAC_SIGNAL_VALUE_0,
  DAC_SIGNAL_VALUE_1,
  DAC_SIGNAL_VALUE_2,
  DAC_SIGNAL_VALUE_3
};

uint8_t DACArrayIndex = 0U;

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The init of the MX_DACx instance and the conversion start is triggered by the applicative code.
  * - MX_DACx is configured.
  * - MX_DACx channel buffer is calibrated.
  * - MX_DACx channel is started.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_dacx_init() == NULL)
  {
    goto _app_init_exit;
  }

  /* The calibration allows a better output voltage precision */
  DAC_CalibrateChannelBuffer();

  /* Start the MX_DACx channel */
  DAC_StartChannel();

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * At each call, the MX_DACx output the following voltage stored in the DACValueMilliVoltArray variable.
  * The MX_DACx conversion is automatically done by setting a new value.
  */
app_status_t app_process(void)
{
  /* Configuration of the desire output voltage */
  uint8_t dac_raw_value = DAC_GetVoltageRawData(DACValueMilliVoltArray[DACArrayIndex]);

  DAC_SetChannelData(dac_raw_value);

  /* The DACArrayIndex is updated to change the next output voltage value */
  DACArrayIndex = (DACArrayIndex + 1) % BUFFER_SIZE;

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
