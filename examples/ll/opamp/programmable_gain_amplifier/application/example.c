/**
  ******************************************************************************
  * file           : example.c
  * brief          : OPAMP programmable gain amplifier based on STM32 LL
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
volatile uint16_t AmplifiedVoltageValue; /* MX_ADCx GPIO channel amplified value in millivolt */
volatile uint8_t  ADCConversionComplete; /* MX_ADCx conversion complete flag                  */

/* Private functions ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/


app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1 ##########
    * Initializes the MX_OPAMPx instance.
    * - Configures the MX_OPAMPx instance.
    * - Starts the MX_OPAMPx instance.
    */
  if (mx_opampx_init() == NULL)
  {
    goto _app_init_exit;
  }

  OPAMP_Start();

  /** ########## Step 2 ##########
    * Initializes the MX_ADCx instance.
    * - Configures the MX_ADCx instance.
    * - Activates the MX_ADCx instance.
    * - Calibrates the MX_ADCx instance.
    */
  if (mx_adcx_init() == NULL)
  {
    goto _app_init_exit;
  }

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
  /** ########## Step 3 ##########
    * Start the ADC peripheral conversion of amplified signal from OPAMP output.
    */
  ADCConversionComplete = 0U;

  ADC_StartConversion_IT();

  /** ########## Step 4 ##########
    * Waits until the voltage measure is completed.
    * Retrieves converted values for GPIO in millivolt.
    */
  while (ADCConversionComplete == 0U)
  {
    __WFI();
  }

  AmplifiedVoltageValue = ADC_GetDataGPIOmV();

  return EXEC_STATUS_OK;
} /* end app_process */


/** Deinitializes the MX_OPAMPx and MX_ADCx instances before leaving the scenario.
  * This function is never called and is provided here as reference.
  */
app_status_t app_deinit(void)
{
  mx_opampx_deinit();

  mx_adcx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** This function is executed when the MX_ADCx conversion complete interrupt is generated.
  * @user: the conversion complete callback can be customized according to the application needs.
  */
void ADC_ConversionCompleteCallback(void)
{
  ADCConversionComplete = 1U;
}