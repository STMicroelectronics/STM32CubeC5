/**
  ******************************************************************************
  * file           : example.c
  * brief : ADC single conversion triggered by software in interrupt mode based on STM32 LL

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
volatile uint16_t GPIO_mV ;              /* MX_ADCx GPIO channel amplified value in millivolt */
volatile uint8_t  ADCConversionComplete; /* MX_ADCx conversion complete flag                  */

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
  /** ########## Step 3 ##########
    * Starts the MX_ADCx conversion in interrupt mode
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

  GPIO_mV  = ADC_GetDataGPIOmV();

  return EXEC_STATUS_OK;
} /* end app_process */


/** Deinitializes the MX_ADCx instance.
  * This function is never called and is provided here as reference.
  */
app_status_t app_deinit(void)
{
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
