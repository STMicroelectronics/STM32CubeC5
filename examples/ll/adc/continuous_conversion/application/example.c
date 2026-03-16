/**
  ******************************************************************************
  * file           : example.c
  * brief          : ADC continuous conversion by software in polling mode with LL API
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
volatile uint32_t AdcMilliVolt;

/* Private functions ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * The init of the ADC instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Configures the MX_ADCx instance */
  if (mx_adcx_init() != NULL)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  /** ########## Step 2 ##########
    * Performs the ADC activation and self calibration to improve measurements accuracy.
    * The calibration is a prerequisite to any ADC operation. It removes the systematic errors that
    * might vary from chip to chip.
    */
  /* Calibrate the MX_ADCx instance */
  if (ADC_Calibrate() == SYSTEM_OK)
  {
    /* Activate the MX_ADCx instance */
    if (ADC_Activate() == SYSTEM_OK)
    {
      return_status = EXEC_STATUS_INIT_OK;
    }
  }
  /** ########## Step 3 ##########
    * Start the MX_ADCx conversion and waits for the end of conversions.
    * Start the MX_ADCx regular conversions continuously in silent mode (No interrupts)
    */
  ADC_StartConversion();

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_OK;


  /** ########## Step 4 ##########
    * - Waits until the conversion is completed (or an error is raised).
    * - Retrieves the MX_ADCx value in millivolt for GPIO conversions.
    */
  ADC_REG_PollForConv();
  AdcMilliVolt = ADC_GetDataGPIOmV();

  return return_status;
} /* end app_process */


/**
Deinitializes the MX_ADCx instance.
In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_adcx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
