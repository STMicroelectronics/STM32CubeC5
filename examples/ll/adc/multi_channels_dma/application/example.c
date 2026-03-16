/**
  ******************************************************************************
  * file           : example.c
  * brief          : ADC multi channels conversion triggered by software in DMA mode with LL API
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
/* Variables for ADC conversion data computation to physical values */
/* Value of voltage calculated from ADC conversion data (unit: mV) */
volatile uint16_t GPIO_mV              = 0U;
/* Value of internal voltage reference VrefInt calculated from ADC conversion data (unit: mV) */
volatile uint16_t VrefInt_mV           = 0U;
/* Value of temperature calculated from ADC conversion data (unit: degree Celsius) */
volatile uint16_t Temperature_DegC     = 0U;
/* Set to 1 if the ADC conversion is correctly completed */
volatile uint8_t ADCConversionComplete = 0U;
/* Set to 1 if a transmission or a reception error is detected */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
volatile uint8_t ADCConversionError;
#endif /* USE_LL_APP_ERROR */

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

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_OK;

  ADCConversionComplete = 0U;

  /** ########## Step 3 ##########
    * Starts the MX_ADCx conversion in DMA mode.
    */
  ADC_StartConversion_DMA();

  /** ########## Step 4 ##########
    * Waits for one of these MX_ADCx interrupts: transfer complete or transfer error when USE_LL_APP_ERROR == 1.
    * - Retrieves GPIO voltage in mV from regular conversions.
    * - Retrieves Vref voltage in mV from regular conversions.
    * - Retrieves Temperature sensor in degC from regular conversions.
    */

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  while ((ADCConversionComplete == 0) && (ADCConversionError == 0))
#else
  while (ADCConversionComplete == 0U)
#endif /* USE_LL_APP_ERROR */
  {
    /** Put the CPU in Wait For Interrupt state. A MX_ADCx interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the DMA. It can be replaced by your own code.
      */
    __WFI();
  }

  GPIO_mV          = ADC_GetDataGPIOmV();
  VrefInt_mV       = ADC_GetDataVrefIntmV();
  Temperature_DegC = ADC_GetDataTemperatureDegC();

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


/** MX_ADCx conversion complete callback
  * user: This implementation of the MX_ADCx conversion complete callback can be customized.
  * This function is executed if the MX_ADCx conversion complete interrupt is generated.
  */
void ADC_ConversionCompleteCallback(void)
{
  /* Asynchronous processing related to step 3 */
  ADCConversionComplete = 1U;
}


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/** MX_ADCx conversion error callback
  * user: This implementation of the ADC conversion error callback can be customized.
  * This function is executed if the ADC conversion error interrupt is generated.
  */
void ADC_ConversionErrorCallback(void)
{
  /* Asynchronous processing related to step 3 */
  ADCConversionError = 1U;
}
#endif /* USE_LL_APP_ERROR */
