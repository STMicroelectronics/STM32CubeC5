/**
  ******************************************************************************
  * file           : example.c
  * brief          : ADC calibration factors retrieval and reinjection through HAL
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_adc_handle_t *pADC;  /* pointer referencing the ADC handle from the generated code */
hal_adc_calib_t hal_adc_calib_backup; /* buffer holding calibration factors data */
const hal_adc_calib_t hal_adc_calib_reset = {0U}; /* buffer holding payload to reset calibration factors */

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

  PRINTF("[INFO] Step 2: ADC start COMPLETED.\n");

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 3 ##########
    * Performs the ADC self calibration to improve measurements accuracy.
    * The calibration is a prerequisite to any ADC operation. It removes the systematic errors that
    * might vary from chip to chip.
    */
  if (HAL_ADC_Calibrate(pADC) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3: ADC calibration COMPLETED.\n");

  memset(&hal_adc_calib_backup, 0, sizeof(hal_adc_calib_t));

  /** ########## Step 4 ##########
    * Retrieves the ADC calibration factors to reinject them in next loop.
    */
  if (HAL_ADC_GetCalibrationFactor(pADC, &hal_adc_calib_backup) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 4: ADC get calibration factor COMPLETED.\n");

  /* System could enter low-power mode and freeze up here */

  /** ########## Step 5 ##########
    * Resets the ADC calibration factors to emulate losing them at wake up from low-power mode.
    */
  if (HAL_ADC_SetCalibrationFactor(pADC, &hal_adc_calib_reset) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 5:ADC emulate calibration factor loss by loading a dummy calibration COMPLETED.\n");

  /* System could exit low-power mode and wake up here */

  /** ########## Step 6 ##########
    * Reinjects copy of the ADC calibration factors from previous loop.
    */
  if (HAL_ADC_SetCalibrationFactor(pADC, &hal_adc_calib_backup) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 6: ADC set calibration factor COMPLETED.\n");

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
