/**
  ******************************************************************************
  * file           : example.c
  * brief          : ADC multimode simultaneous conversion by timer in dma mode
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

/** VREF+ Analog reference voltage in millivolt.
  * It must be computed not static.
  * @user: The analog voltage reference, VREF+, might be different from VDD and might require to be tuned.
  */
#define VREFPLUS_APPLI    3300UL
#define BUFFER_LENGTH        3UL

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Variable for the ADC conversion data (a count on the ADC scale) */
uint32_t BufferAdcPrimary[BUFFER_LENGTH];
uint32_t BufferAdcSecondary[BUFFER_LENGTH];

/* Value of voltage calculated from ADC conversion data (unit: mV) */
uint32_t BufferAdcPrimaryVoltage[BUFFER_LENGTH];
uint32_t BufferAdcSecondaryVoltage[BUFFER_LENGTH];

hal_adc_handle_t *pADC_Master; /* pointer referencing the ADC handle from the generated code */
hal_adc_handle_t *pADC_Slave; /* pointer referencing the ADC handle from the generated code */
hal_tim_handle_t *pTIM; /* pointer referencing the TIM handle from the generated code */

volatile uint8_t NewDataAdcPrimary = 0U;           /* Set to 1 if the ADC Master transfer is correctly completed */
volatile uint8_t NewDataAdcSecondary = 0U;            /* Set to 1 if the ADC Slave transfer is correctly completed */
volatile uint8_t ADC_MasterError = 0U;               /* Set to 1 if an ADC Master error is detected */
volatile uint8_t ADC_SlaveError = 0U;                /* Set to 1 if an ADC Slave error is detected */

/* Private functions prototype -----------------------------------------------*/
static void TransferCompleteCallback(hal_adc_handle_t *pADC);
static void AdcErrorCallback(hal_adc_handle_t *pADC);

app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1 ##########
    * The init of the ADC instance is triggered by the applicative code.
    * The init of the TIM instance is triggered by the applicative code.
    * The user callbacks for DataTransfer are registered.
    * Init ADC_Slave before ADC_Master. ADC_Master is master and configure multimode.
    */
  pADC_Slave = mx_example_adc_slave_init();
  if (pADC_Slave == NULL)
  {
    goto _app_init_exit;
  }

  pADC_Master = mx_example_adc_master_init();
  if (pADC_Master == NULL)
  {
    goto _app_init_exit;
  }

  pTIM = mx_example_tim_init();
  if (pTIM == NULL)
  {
    goto _app_init_exit;
  }

  /* Register the ADC callbacks */
  if (HAL_ADC_RegisterDataTransferCpltCallback(pADC_Master, TransferCompleteCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_ADC_RegisterDataTransferCpltCallback(pADC_Slave, TransferCompleteCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_ADC_RegisterErrorCallback(pADC_Master, AdcErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_ADC_RegisterErrorCallback(pADC_Slave, AdcErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  PRINTF("[INFO] Step 1: ADC initialization COMPLETED.\n");

  /** ########## Step 2 ##########
    * Performs the ADC activation and self calibration to improve measurements accuracy.
    * The calibration is a prerequisite to any ADC operation. It removes the systematic errors that
    * might vary from chip to chip.
    */
  if (HAL_ADC_MM_Start(pADC_Master) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_ADC_MM_Calibrate(pADC_Master) != HAL_OK)
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

  NewDataAdcPrimary = 0U;
  NewDataAdcSecondary = 0U;

  /** ########## Step 3 ##########
    * Configure one data buffer or each ADC instance. Start the ADC conversion in multimode DMA mode, multi buffers
    * mode.
    */
  HAL_ADC_MM_REG_SetMultiDMA(pADC_Master, (uint8_t *)&BufferAdcPrimary,
                             BUFFER_LENGTH * sizeof(BufferAdcPrimary[0]));
  HAL_ADC_MM_REG_SetMultiDMA(pADC_Slave, (uint8_t *)&BufferAdcSecondary,
                             BUFFER_LENGTH * sizeof(BufferAdcSecondary[0]));
  if (HAL_ADC_MM_REG_StartConvM_DMA(pADC_Master) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_TIM_Start(pTIM) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3: ADC conversion STARTED.\n");

  /** ########## Step 4 ##########
    * Wait for the interrupt service routine to be called indicating that the data buffer is filled. Then stop the ADC,
    * DMA and Timer.
    */
  while (NewDataAdcPrimary != 1UL || NewDataAdcSecondary != 1UL) ;

  if (HAL_TIM_Stop(pTIM) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_ADC_MM_REG_StopConv_DMA(pADC_Master) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 4: DMA transfer COMPLETED for both ADCs.\n");

  /** ########## Step 5 ##########
    * Computes the corresponding voltage for both ADCs and prints the ADC raw value and the computed voltage.
    */
  for (uint32_t index = 0; index < BUFFER_LENGTH; index++)
  {
    BufferAdcPrimaryVoltage[index] = HAL_ADC_CALC_DATA_TO_VOLTAGE(VREFPLUS_APPLI,
                                                                  BufferAdcPrimary[index], HAL_ADC_RESOLUTION_12_BIT);
    BufferAdcSecondaryVoltage[index] = HAL_ADC_CALC_DATA_TO_VOLTAGE(VREFPLUS_APPLI,
                                                                    BufferAdcSecondary[index],
                                                                    HAL_ADC_RESOLUTION_12_BIT);
  }

  PRINTF("[INFO] Step 5: Print voltage buffer for both ADCs.\n");
  for (uint32_t index = 0; index < BUFFER_LENGTH; index++)
  {
    PRINTF("[INFO] ADC_Master[%" PRIu32 "] = %" PRIu32 "mV (raw value: 0x%03" PRIx32 "), "
           "ADC_Slave[%" PRIu32 "] = %" PRIu32 "mV (raw value: 0x%03" PRIx32 ")\n",
           index, BufferAdcPrimaryVoltage[index], BufferAdcPrimary[index],
           index, BufferAdcSecondaryVoltage[index], BufferAdcSecondary[index]);
  }
  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** Deinitialize the ADC instance.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_adc_master_deinit();
  mx_example_adc_slave_deinit();
  mx_example_tim_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** Conversion complete callback in non-blocking mode.
  * @user: the transfer complete callback can be customized according to the application needs.
  */
static void TransferCompleteCallback(hal_adc_handle_t *hADC)
{
  if (hADC == pADC_Master)
  {
    NewDataAdcPrimary = 1UL;
  }
  else /* pADC_Slave */
  {
    NewDataAdcSecondary = 1UL;
  }
}

/** ADC error interruption callback
  */
static void AdcErrorCallback(hal_adc_handle_t *hADC)
{
  /* report the error (this terminates the execution loop) */
  if (hADC == pADC_Master)
  {
    ADC_MasterError = 1U;
  }
  else /* pADC_Slave */
  {
    ADC_SlaveError = 1U;
  }
}
