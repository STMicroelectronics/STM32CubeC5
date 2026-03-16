/**
  ******************************************************************************
  * file           : example.c
  * brief          : ADC continuous conversion by software in polling mode
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
#define VREFPLUS_APPLI    VDD_VALUE
/* @user: set ADC conversion time out value in milliseconds */
#define TIME_OUT          10U
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_adc_handle_t *pADC;  /* pointer referencing the ADC handle from the generated code */

/* Variable for the ADC conversion data (a count on the ADC scale) */
uint16_t AdcRawData = 0xFFFFU;

/* Value of voltage calculated from ADC conversion data (unit: mV) */
uint16_t AdcMilliVolt = 0U;

/* Private functions prototype -----------------------------------------------*/

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
    * Start the ADC conversion and waits for the end of conversions.
    * Start the ADC regular conversions continuously in silent mode (No interrupts)
    */
  if (HAL_ADC_REG_StartConv(pADC) != HAL_OK)
  {
    goto _app_init_exit;
  }
  PRINTF("[INFO] Step 3: ADC start conversion COMPLETED.\n");

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 4 ##########
    * Waits until the conversion is completed (or an error is raised).
    * Retrieves the raw ADC value, and computes the corresponding voltage.
  */
  if (HAL_ADC_REG_PollForConv(pADC, TIME_OUT) != HAL_OK)
  {
    PRINTF("[ERROR] Step 4: ADC Error detected \n");
    return_status = EXEC_STATUS_ERROR;
  }
  else
  {
    /* Retrieve the ADC conversion data */
    AdcRawData = HAL_ADC_REG_ReadConversionData(pADC);

    /* Compute the ADC conversion raw data to physical values using the proper helper macro. */
    AdcMilliVolt = HAL_ADC_CALC_DATA_TO_VOLTAGE(VREFPLUS_APPLI, AdcRawData, HAL_ADC_RESOLUTION_12_BIT);
    PRINTF("[INFO] Step 4: ADC conversion DONE.  Raw ADC value = 0x%" PRIx16 "   Voltage = %" PRId16 " mV  \n", \
           AdcRawData, AdcMilliVolt);
  }
  return_status = EXEC_STATUS_OK;

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
