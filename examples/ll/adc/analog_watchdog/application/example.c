/**
  ******************************************************************************
  * file           : example.c
  * brief          : ADC Analog Wotchdog to monitor a channel in interrupt mode with LL API
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
volatile uint16_t GPIO_mV;

/* Variable to report status of ADC analog watchdog:                          */
/*  0: ADC conversion data into AWD window                                    */
/*  1: ADC conversion data out of AWD window                                  */
volatile uint8_t AnalogWatchdogStatus;

/* Private functions prototype -----------------------------------------------*/

app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1 ##########
    * The init of the MX_ADCx instance is triggered by the applicative code.
    */
  /* Configures the MX_ADCx instance */
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


  /** ########## Step 3 ##########
    * Starts the MX_ADCx analog watchdog conversion in interrupt mode
  */

  ADC_StartConversion_IT();

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  AnalogWatchdogStatus = 0U;

  /** ########## Step 4 ##########
    * Waits until the analog watchdog is out of window (or an error is raised).
    * Retrieves the MX_ADCx value in millivolt for GPIO conversions.
    */
  GPIO_mV = ADC_GetDataGPIOmV();

  while (AnalogWatchdogStatus != 1U)
  {
    /** Put the CPU in Wait For Interrupt state. An ADC interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupt mode.
      *        It can be replaced by your own code.
      */

    __WFI();
  }

  GPIO_mV = ADC_GetDataGPIOmV();

  return_status = EXEC_STATUS_OK;

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


/** This function is executed when the analog watchdog interrupt is generated.
  * @user: the analog watchdog out of window callback can be customized according to the application needs.
  */
void AnalogWatchdogCallback(void)
{
  AnalogWatchdogStatus = 1U;
}
