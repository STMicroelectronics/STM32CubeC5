/**
  ******************************************************************************
  * file           : example.c
  * brief : ADC internal sensor temperature reading in polling mode with LL API
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
/* Value of voltage calculated from ADC conversion data (unit: mV) */
volatile uint16_t Temperature_DegC;

/* Private functions ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/


app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1 ##########
    * The init of the MX_ADCx instance is triggered by the applicative code.
    */
  if (mx_adcx_init() == NULL)
  {
    goto _app_init_exit;
  }


  /** ########## Step 2 ##########
    * Performs the MX_ADCx  activation and self calibration to improve measurements accuracy.
    * The calibration is a prerequisite to any ADC operation. It removes the systematic errors that
    * might vary from chip to chip.
    */
  /* Calibrate the MX_ADCx instance */
  if (ADC_Activate() != SYSTEM_OK)
  {
    goto _app_init_exit;
  }

  if (ADC_Calibrate() != SYSTEM_OK)
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
  /** ########## Step 3 ##########
    * Starts the MX_ADCx  conversion in polling mode.
    */
  ADC_StartConversion();

  /** ########## Step 4 ##########
    * Waits until the conversion is completed (or an error is raised).
    * Retrieves Temperature value in degC from regular conversions.
    */
  if (ADC_PollForConversion() == SYSTEM_OK)
  {
    Temperature_DegC = ADC_GetDataTemperatureDegC();

    return_status = EXEC_STATUS_OK;
  }

  return return_status;
} /* end app_process */


/**
  * Deinitializes the MX_ADCx instance.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_adcx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
