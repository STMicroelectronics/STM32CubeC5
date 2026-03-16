/**
  ******************************************************************************
  * file           : example.c
  * brief          : OPAMP configured in follower mode using the HAL.
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

/* Private functions prototype -----------------------------------------------*/


app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;


  /** ########## Step 1 ##########
    * The applicative code initializes the OPAMP instance in follower mode.
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
    * Reads output voltage measurement.
    */

  if (HAL_OPAMP_Start(pOPAMP) != HAL_OK)
  {
    goto _app_process_exit;
  }
  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** Deinitializes the OPAMP instance before leaving the scenario.
  * This function is never called and is provided here as reference.
  */
app_status_t app_deinit(void)
{
  mx_example_opamp_deinit();
  return EXEC_STATUS_OK;
} /* end app_deinit */

