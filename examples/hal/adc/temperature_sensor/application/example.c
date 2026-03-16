/**
  ******************************************************************************
  * file           : example.c
  * brief          : ADC internal sensor temperature reading
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

/* Value of temperature calculated from ADC conversion data (unit: degree Celsius) */
uint16_t TempDegreeCelsius = 0U;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_adc_handle_t *pADC;  /* pointer referencing the ADC handle from the generated code */

/* Private functions prototype -----------------------------------------------*/

app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1 ##########
    * The init of the ADC instance is triggered by the applicative code.
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


_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  /* ADC conversion data of channel temperature sensor (unit: digital value corresponding to ADC resolution) */
  uint16_t adc_conv_data_tempsensor_raw;
  /** ########## Step 3 ##########
    * Starts the ADC conversion in polling mode.
    */
  if (HAL_ADC_REG_StartConv(pADC) != HAL_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 3: ADC start conversion COMPLETED.\n");
  /** ########## Step 4 ##########
    * Waits until the conversion is completed (or an error is raised).
    * Retrieves the raw ADC value, and computes the corresponding temperature.
  */
  if (HAL_ADC_REG_PollForConv(pADC, TIME_OUT) != HAL_OK)
  {
    PRINTF("[ERROR] Step 4: ADC Error detected \n");
    return_status = EXEC_STATUS_ERROR;
  }
  else
  {
    /* ADC conversion data obtained from the internal temperature sensor. */
    adc_conv_data_tempsensor_raw = HAL_ADC_REG_ReadConversionData(pADC);

    /* Compute the temperature sensor data temperature in DegC. */
    TempDegreeCelsius = HAL_ADC_CALC_TEMPERATURE(
                          VREFPLUS_APPLI,
                          adc_conv_data_tempsensor_raw,
                          HAL_ADC_RESOLUTION_12_BIT
                        );
    PRINTF("[INFO] Step 4: ADC conversion DONE.  Raw ADC value temperature = 0x%" PRIx16
           "   Temperature value = %" PRId16 " degree Celsius  \n",
           adc_conv_data_tempsensor_raw, TempDegreeCelsius);
  }
  return_status = EXEC_STATUS_OK;
_app_process_exit:
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
