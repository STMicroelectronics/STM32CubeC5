/**
  ******************************************************************************
  * file           : example.c
  * brief          : DAC constant signal generation using software trigger
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

#include "example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/** VDDA must be computed not static.
  * @user: The Analog Voltage supply, VDDA, might be different from VDD and might require to be tuned.
  */
#define VREFPLUS_APPLI  3300U

#define DAC_ARRAY_SIZE   4

/* Private variables ---------------------------------------------------------*/

hal_dac_handle_t *pDAC;  /* pointer referencing the DAC handle from the generated code */

/** output voltage values of the DAC.
  * @user: you can change the values to change the output voltage of the DAC.
  */
const uint16_t DacMilliVoltArray[DAC_ARRAY_SIZE] =
{
  0,
  VREFPLUS_APPLI / 3,
  (2 * VREFPLUS_APPLI) / 3,
  VREFPLUS_APPLI
};

uint8_t DacTabIndex = 0;

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The init of the DAC instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pDAC = (hal_dac_handle_t *)mx_example_dac_init();

  if (pDAC == NULL)
  {
    goto _app_init_exit;
  }

  /* The calibration allows a better output voltage precision */
  if (HAL_DAC_CalibrateChannelBuffer(pDAC, EXAMPLE_DAC_CHANNEL) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_DAC_StartChannel(pDAC, EXAMPLE_DAC_CHANNEL) != HAL_OK)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  if (return_status == EXEC_STATUS_INIT_OK)
  {
    PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
  }
  else
  {
    PRINTF("[ERROR] Step 1: Device initialization ERROR.\n");
  }

  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * At each call, the DAC output the following voltage stored in the DacMilliVoltArray array.
  * The DAC conversion is triggered using the software trigger feature.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Configuration of the desired output voltage */
  uint8_t dac_raw_value = LL_DAC_CALC_VOLTAGE_TO_DATA(VREFPLUS_APPLI,
                                                      DacMilliVoltArray[DacTabIndex], LL_DAC_RESOLUTION_8B);
  if (HAL_DAC_SetChannelData(pDAC, EXAMPLE_DAC_CHANNEL, dac_raw_value) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_DAC_TrigSWConversionChannel(pDAC, EXAMPLE_DAC_CHANNEL) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2: DAC output voltage set to %" PRIu16 " mV.\n", DacMilliVoltArray[DacTabIndex]);

  return_status = EXEC_STATUS_OK;

  /* The DacTabIndex is updated to change the next output voltage value */
  DacTabIndex  = (DacTabIndex + 1) % DAC_ARRAY_SIZE;

_app_process_exit:
  return return_status;
} /* end app_process */


/** Deinitializes the DAC instance.
  */
app_status_t app_deinit(void)
{
  mx_example_dac_deinit();
  return EXEC_STATUS_OK;
} /* end app_deinit */
