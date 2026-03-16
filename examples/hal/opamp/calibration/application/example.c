/**
  ******************************************************************************
  * file           : example.c
  * brief          : OPAMP calibration trimming retrieval and reinjection through HAL
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
hal_opamp_handle_t *pOPAMP;  /* pointer referencing the OPAMP handle from the generated code */
hal_opamp_trimming_offset_pair_t default_trim_value;
hal_opamp_trimming_offset_pair_t user_trim_value;
/* Private functions prototype -----------------------------------------------*/

app_status_t app_init(void)
{
  app_status_t return_status;

  /** ########## Step 1 ##########
    * The init of the OPAMP instance is triggered by the applicative code.
    */
  pOPAMP = mx_example_opamp_init();
  if (pOPAMP == NULL)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{

  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * Retrieve the default trimming values and verify that the trimming mode is set to factory mode.
    */

  /* Retrieve default trimming values */
  HAL_OPAMP_GetConfigTrimming(pOPAMP, &default_trim_value, HAL_OPAMP_POWER_MODE_NORMAL);

  /* Verify if the trimming mode is by default to factory trimming mode */
  if (HAL_OPAMP_GetTrimmingMode(pOPAMP) != HAL_OPAMP_TRIMMING_MODE_FACTORY)
  {
    goto _app_process_exit;
  }

  /** ########## Step 3 ##########
    * Perform self-calibration of the OPAMP.
    */

  if (HAL_OPAMP_Calibrate(pOPAMP, HAL_OPAMP_POWER_MODE_NORMAL) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /** ########## Step 4 ##########
    * Retrieve the trimming values after calibration and confirm that the mode has switched to user mode.
    */

  /* Verify if the trimming mode is changed to user trimming mode */
  if (HAL_OPAMP_GetTrimmingMode(pOPAMP) != HAL_OPAMP_TRIMMING_MODE_USER)
  {
    goto _app_process_exit;
  }
  /* Retrieve trimming values after calibration */
  HAL_OPAMP_GetConfigTrimming(pOPAMP, &user_trim_value, HAL_OPAMP_POWER_MODE_NORMAL);

  /** ########## Step 5 ##########
    * Perform a complete reset of the OPAMP configuration.
    */
  HAL_OPAMP_ResetConfig(pOPAMP);

  /** ########## Step 6 ##########
    * Reinject copy of the OPAMP calibration trimming values stored from the previous calibration.
    */
  HAL_OPAMP_SetConfigTrimming(pOPAMP, &user_trim_value, HAL_OPAMP_POWER_MODE_NORMAL);

  /** ########## Step 7 ##########
    * Verify that the trimming mode is set to user mode.
    */

  if (HAL_OPAMP_GetTrimmingMode(pOPAMP) != HAL_OPAMP_TRIMMING_MODE_USER)
  {
    goto _app_process_exit;
  }

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/**
  * Deinitializes the OPAMP instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  /** It is important to indicate if the entire OPAMP HW instance is de-initialized
    * or if only a configuration is de-initialized (some registers are not reset).
    * We describe what the code must do: use the imperative.
    */
  mx_example_opamp_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
