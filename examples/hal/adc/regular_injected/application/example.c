/**
  ******************************************************************************
  * file           : example.c
  * brief          : ADC convert multiple channels from groups regular (in polling mode) and injected
  *                  (in interrupt mode) with HAL API
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
/* ADC group injected sequencer rank 1*/
#define SEQUENCER_RANK_1  1U
/* ADC group injected sequencer rank 2*/
#define SEQUENCER_RANK_2  2U
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_adc_handle_t *pADC;  /* pointer referencing the ADC handle from the generated code */

uint16_t adc_conv_data_gpio_raw;       /* ADC conversion data of channel to a GPIO (unit: digital value
                                          corresponding to ADC resolution) */
uint16_t adc_conv_data_vrefint_raw;    /* ADC conversion data of channel VrefInt (unit: digital value
                                          corresponding to ADC resolution) */
uint16_t adc_conv_data_tempsensor_raw; /* ADC conversion data of channel temperature sensor (unit: digital value
                                       corresponding to ADC resolution) */

/* Variables for ADC conversion data computation to physical values */
/* Value of voltage calculated from ADC conversion data (unit: mV) */
uint16_t VoltageGpioVoltage = 0U;
/* Value of internal voltage reference VrefInt calculated from ADC conversion data (unit: mV) */
uint16_t VrefIntVoltage = 0U;
/* Value of temperature calculated from ADC conversion data (unit: degree Celsius) */
uint16_t TempDegreeCelsius = 0U;

volatile uint8_t AdcTransferComplete = 0U;     /* Set to 1 if the ADC transfer is correctly completed */
volatile uint8_t AdcError = 0U;               /* Set to 1 if an ADC error is detected */

/* Private functions prototype -----------------------------------------------*/
/* Functions allowing the user to configure dynamically the ADC callbacks instead of weak functions */
static void AdcTransfCallback(hal_adc_handle_t *pADC);
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
  if (HAL_ADC_RegisterInjUnitaryConvCpltCallback(pADC, AdcTransfCallback) != HAL_OK)
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
    * Starts the ADC regular conversion in polling mode and using continuous conversion mode.
    * Continuous mode enables automatic, uninterrupted ADC conversions.
    */
  if (HAL_ADC_REG_StartConv(pADC) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 3: ADC start regular conversion COMPLETED.\n");
_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  AdcTransferComplete = 0;

  /** ########## Step 4 ##########
    * The ADC conversion is started in it mode for injected channels
    */

  /* Start the the ADC injected conversions (one conversion after each MX_ADCx start) in interrupt mode*/
  if (HAL_ADC_INJ_StartConv_IT(pADC) != HAL_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 4: ADC start injected conversion COMPLETED.\n");
  /* ADC conversion data of channel to a GPIO. */
  adc_conv_data_gpio_raw = HAL_ADC_REG_ReadConversionData(pADC);
  /* ADC conversion data of channel VrefInt . */
  adc_conv_data_vrefint_raw = HAL_ADC_INJ_ReadConversionDataRank(pADC, SEQUENCER_RANK_1);
  /* ADC conversion data of channel temperature sensor   . */
  adc_conv_data_tempsensor_raw = HAL_ADC_INJ_ReadConversionDataRank(pADC, SEQUENCER_RANK_2);

  PRINTF("[INFO] Step 5: ADC conversion DONE.  Raw ADC value gpio = 0x%" PRIx16 "  Raw ADC value vref = 0x%"
         PRIx16 "  Raw ADC value temperature = 0x%" PRIx16 "  \n", \
         adc_conv_data_gpio_raw, adc_conv_data_vrefint_raw, adc_conv_data_tempsensor_raw);

  /** Awaiting the Step 4 ADC completion (or error): asynchronous ADC interrupt callback.
    * Print the results: the raw ADC value and the computed corresponding voltage.
    * For this example purpose, we poll for the callback flag
    * but thanks to interrupt mode, application can do other things instead of polling.
  */
  while ((AdcTransferComplete != 1U) && (AdcError != 1U))
  {
    /** Put the CPU in Wait For Interrupt state. An ADC interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupt mode.
      *        It can be replaced by your own code.
      */
    __WFI();
  }

  if (AdcError == 1U)
  {
    PRINTF("[ERROR] Step 5: ADC Error detected \n");
    goto _app_process_exit;
  }

  /* Computation of ADC conversions raw data to physical values*/
  /* - Retrieves GPIO voltage in mV from regular conversions.*/
  VoltageGpioVoltage = HAL_ADC_CALC_DATA_TO_VOLTAGE(VREFPLUS_APPLI, adc_conv_data_gpio_raw, HAL_ADC_RESOLUTION_12_BIT);
  /* - Retrieves Vref voltage in mV from injected conversions.*/
  VrefIntVoltage = HAL_ADC_CALC_DATA_TO_VOLTAGE(VREFPLUS_APPLI, adc_conv_data_vrefint_raw, HAL_ADC_RESOLUTION_12_BIT);
  /* - Retrieves temperature in Degree Celsius from injected conversions.    */
  TempDegreeCelsius = HAL_ADC_CALC_TEMPERATURE(VREFPLUS_APPLI, adc_conv_data_tempsensor_raw, HAL_ADC_RESOLUTION_12_BIT);

  PRINTF("[INFO] Step 5: ADC conversion DONE.  Voltage  gpio value = %" PRId16 " mV Voltage vref value = %"
         PRId16 " Temperature value = %" PRId16 " degree Celsius  \n", \
         VoltageGpioVoltage, VrefIntVoltage, TempDegreeCelsius);

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


/** ########## Step 5 ##########
  * Waits until the transfer is completed (or an error is raised).
  * Computes the corresponding voltage.
  * Transfer complete callback in non-blocking mode.
  * @user: the transfer complete callback can be customized according to the application needs.
  */
static void AdcTransfCallback(hal_adc_handle_t *hADC)
{
  AdcTransferComplete = 1U;
}


/** ADC error interruption callback
  */
static void AdcErrorCallback(hal_adc_handle_t *hADC)
{
  /* report the error (this terminates the execution loop) */
  AdcError = 1U;
}
