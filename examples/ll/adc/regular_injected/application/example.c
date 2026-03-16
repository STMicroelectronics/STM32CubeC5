/**
  ******************************************************************************
  * file           : example.c
  * brief          : ADC convert multiple channels from groups regular (in polling mode) and injected
  *                  (in interrupt mode) with LL API
  ******************************************************************************
  *
  * Copyright (c) 2025 STMicroelectronics.
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
/* Variables for ADC conversion data computation to physical values */
/* Value of voltage calculated from ADC conversion data (unit: mV) */
volatile uint16_t GPIO_mV = 0U;
/* Value of internal voltage reference VrefInt calculated from ADC conversion data (unit: mV) */
volatile uint16_t VrefInt_mV = 0U;
/* Value of temperature calculated from ADC conversion data (unit: degree Celsius) */
volatile uint16_t Temperature_DegC = 0U;
/* Set to 1 if the ADC transfer is correctly completed */
volatile uint8_t ConversionComplete = 0U;

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
    * Performs the MX_ADCx activation and self calibration to improve measurements accuracy.
    * The calibration is a prerequisite to any ADC operation. It removes the systematic errors that
    * might vary from chip to chip.
    */
  if (ADC_Activate() != SYSTEM_OK)
  {
    goto _app_init_exit;
  }

  if (ADC_Calibrate() != SYSTEM_OK)
  {
    goto _app_init_exit;
  }


  /** ########## Step 3 ##########
    * Starts the MX_ADCx regular conversion in polling mode and using continuous conversion mode.
    * Continuous mode enables automatic, uninterrupted ADC conversions.
    */
  if (ADC_REG_StartConversion() != SYSTEM_OK)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_OK;

  /** ########## Step 4 ##########
    * The MX_ADCx conversion is started in it mode for injected channels. (one conversion after each MX_ADCx start)
    */
  ADC_INJ_StartConversion_IT();

  /** ########## Step 5 ##########
    * Waits until the transfer is completed.
    * - Retrieves GPIO voltage in mV from regular conversions.
    * - Retrieves Vref voltage in mV from injected conversions.
    * - Retrieves Temperature sensor in degC from injected conversions.
    */

  /** Awaiting the Step 4 ADC completion (or error): asynchronous ADC interrupt callback.
    * Print the results: the raw ADC value and the computed corresponding voltage.
    * For this example purpose, we poll for the callback flag
    * but thanks to interrupt mode, application can do other things instead of polling.
  */
  while (ConversionComplete == 0U)
  {
    __WFI();
  }

  /* Reset the MX_ADCx conversion complete flag */
  ConversionComplete = 0U;

  GPIO_mV          = ADC_GetDataGPIOmV();
  VrefInt_mV       = ADC_GetDataVrefIntmV();
  Temperature_DegC = ADC_GetDataTemperatureDegC();

  return return_status;
} /* end app_process */


/**
  * Deinitializes the MX_ADCx instance.
  * In this example, app_deinit is never called and it is provided as a reference only
  */
app_status_t app_deinit(void)
{
  mx_adcx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** brief: MX_ADCx conversion complete callback.
  */
void ADC_ConversionCompleteCallback(void)
{
  ConversionComplete = 1U;
}
