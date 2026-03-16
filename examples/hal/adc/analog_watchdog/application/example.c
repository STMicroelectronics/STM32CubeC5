/**
  ******************************************************************************
  * file           : example.c
  * brief          : ADC single conversion triggered by software in interrupt mode
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

/* Assumption: pin Vref+ connected to Vdd at board level */
#define VREFPLUS_APPLI    (VDD_VALUE)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_adc_handle_t *pADC;  /* pointer referencing the ADC handle from the generated code */

/* Variable for the ADC conversion data (a count on the ADC scale) */
uint16_t AdcRawData = 0xFFFFU;

/* Value of voltage calculated from ADC conversion data (unit: mV) */
uint16_t AdcMilliVolt = 0U;
/* Variable to report status of ADC analog watchdog:                        */
/*  0: ADC conversion data into AWD window                                    */
/*  1: ADC conversion data out of AWD window                                  */
volatile uint8_t AnalogWatchdogStatus = 0U;
volatile uint8_t AdcError = 0U;       /* Set to 1 if an ADC error is detected */

/* Private functions prototype -----------------------------------------------*/
/* Functions allowing the user to configure dynamically the ADC callbacks instead of weak functions */
static void AdcAWDCallback(hal_adc_handle_t *hADC, hal_adc_awd_instance_t awd_instance);
static void AdcErrorCallback(hal_adc_handle_t *pADC);

app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1 ##########
    * The init of the ADC instance is triggered by the applicative code.
    * The user callbacks for ADC group regular end of unitary conversion and error are registered.
    */
  pADC = mx_example_adc_init();
  if (pADC == NULL)
  {
    goto _app_init_exit;
  }

  /* Register the ADC callbacks */
  if (HAL_ADC_RegisterAwdOutOfWindowCallback(pADC, AdcAWDCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_ADC_RegisterErrorCallback(pADC, AdcErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: ADC initialization COMPLETED.\n");

  /** ########## Step 2 ##########
    * Performs the ADC activation and self calibration to improve measurements accuracy.
    * The calibration is a prerequisite to any ADC operation. It removes the systematic errors that
    * might vary from chip to chip.
    */
  if (HAL_ADC_Start(pADC) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_ADC_Calibrate(pADC) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 2: ADC start and calibration COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;


  /** ########## Step 3 ##########
    * Starts the analog watchdog conversion in interrupt mode
  */

  if (HAL_ADC_REG_StartConv_IT_Opt(pADC, HAL_ADC_OPT_IT_AWD_1) != HAL_OK)
  {
    goto _app_init_exit;
  }
  PRINTF("[INFO] Step 3:ADC start conversion COMPLETED.\n");

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  AnalogWatchdogStatus = 0U;

  PRINTF("[INFO] Step 4: Conversion data is in analog watchdog window.\n");

  /** Awaiting the Step 4 ADC completion (or error): asynchronous ADC interrupt callback.
    * Print the results: the raw ADC value and the computed corresponding voltage.
    * For this example purpose, we poll for the callback flag
    * but thanks to interrupt mode, application can do other things instead of polling.
    */
  while ((AnalogWatchdogStatus != 1U) && (AdcError != 1U))
  {
    /** Put the CPU in Wait For Interrupt state. An ADC interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupt mode.
      *        It can be replaced by your own code.
      */

    __WFI();
  }

  if (AdcError == 1U)
  {
    PRINTF("[ERROR] Step 4: ADC Error detected \n");

    return_status = EXEC_STATUS_ERROR;
  }
  else
  {
    PRINTF("[INFO] Step 4: Conversion data is out of analog watchdog window. \
    Raw ADC value = 0x%" PRIx16 "   Voltage = %" PRId16 " mV  \n", AdcRawData, AdcMilliVolt);

    return_status = EXEC_STATUS_OK;
  }

  return return_status;
} /* end app_process */


/** Deinitializes the ADC instance.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_adc_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** ########## Step 4 ##########
  * Waits until the analog watchdog is out of window (or an error is raised).
  * Retrieves the raw ADC value, and computes the corresponding voltage.
  * This function is executed when the ADC conversion complete interrupt is generated.
  * @user: the conversion complete callback can be customized according to the application needs.
  */
static void AdcAWDCallback(hal_adc_handle_t *hADC, hal_adc_awd_instance_t awd_instance)
{
  /* Retrieve the ADC conversion data */
  AdcRawData = HAL_ADC_REG_ReadConversionData(pADC);

  /* Compute the ADC conversion raw data to physical values using the proper helper macro. */
  AdcMilliVolt = HAL_ADC_CALC_DATA_TO_VOLTAGE(VREFPLUS_APPLI, AdcRawData, HAL_ADC_RESOLUTION_12_BIT);

  /* Update status variable of ADC analog watchdog */
  AnalogWatchdogStatus = 1U;
}


/** ADC error interruption callback
  */
static void AdcErrorCallback(hal_adc_handle_t *hADC)
{
  /* report the error (this terminates the execution loop) */
  AdcError = 1U;
}
