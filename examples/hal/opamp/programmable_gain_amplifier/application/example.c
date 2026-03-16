/**
  ******************************************************************************
  * file           : example.c
  * brief          : OPAMP programmable gain amplifier based on STM32 HAL
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
#define ADC_VREFPLUS_VALUE (VDD_VALUE) /* Assumption: pin Vref+ connected to Vdd at board level */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_opamp_handle_t *pOPAMP; /* pointer referencing the OPAMP handle from the generated code */
hal_adc_handle_t   *pADC;   /* pointer referencing the ADC handle from the generated code   */

volatile uint16_t AmplifiedVoltageValue_mV; /* ADC GPIO channel amplified value in millivolt */
volatile uint8_t  ADCConversionComplete;    /* ADC conversion complete flag                  */

/* Private functions prototype -----------------------------------------------*/


app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1 ##########
    * Initializes the ADC instance.
    * - Configures the ADC instance.
    * - Calibrates the ADC instance.
    * - Activates the ADC instance.
    */
  pADC = mx_example_adc_init();
  if (pADC == NULL)
  {
    goto _app_init_exit;
  }

  if (HAL_ADC_Start(pADC) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_ADC_Calibrate(pADC) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: ADC ready.\n");

  /** ########## Step 2 ##########
    * Initializes the OPAMP instance.
    * - Configures the OPAMP instance.
    * - Starts the OPAMP instance.
    */
  pOPAMP = mx_example_opamp_init();

  if (pOPAMP == NULL)
  {
    goto _app_init_exit;
  }

  if (HAL_OPAMP_Start(pOPAMP) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 2: OPAMP ready.\n");

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  uint16_t adc_raw_data;
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 3 ##########
    * Start the MX_ADCx conversion of amplified signal.
    */
  ADCConversionComplete = 0U;

  if (HAL_ADC_REG_StartConv_IT(pADC) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3: Starts output voltage measurement.\n");

  /* Awaiting the measurement interrupt callback.*/
  while (ADCConversionComplete != 1U)
  {
    __WFI();
  };

  /* Retrieve the ADC raw conversion data and convert it to a physical value */
  adc_raw_data = HAL_ADC_REG_ReadConversionData(pADC);
  AmplifiedVoltageValue_mV = HAL_ADC_CALC_DATA_TO_VOLTAGE(ADC_VREFPLUS_VALUE, adc_raw_data, HAL_ADC_RESOLUTION_12_BIT);

  PRINTF("[INFO] Step 4: Output voltage = %" PRId16 " mV \n", AmplifiedVoltageValue_mV);

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** Deinitializes the OPAMP and ADC instances before leaving the scenario.
  * This function is never called and is provided here as reference.
  */
app_status_t app_deinit(void)
{
  mx_example_opamp_deinit();

  mx_example_adc_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** This function is executed when the ADC conversion complete interrupt is generated.
  * @user: the conversion complete callback can be customized according to the application needs.
  */
void HAL_ADC_REG_UnitaryConvCpltCallback(hal_adc_handle_t *hADC)
{
  ADCConversionComplete = 1U;
}
