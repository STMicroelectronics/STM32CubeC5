/**
  ******************************************************************************
  * file           : example.c
  * brief          : OPAMP configured in standalone mode using the HAL.
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
  app_status_t return_status;

  /** ########## Step 1 ##########
    * Configure the OPAMP instance is triggered by the applicative code.
    */

  pOPAMP = mx_example_opamp_init();

  if (pOPAMP == NULL)
  {
    return_status = EXEC_STATUS_ERROR;
  }
  else
  {
    return_status = EXEC_STATUS_OK;
  }

  return_status = EXEC_STATUS_INIT_OK;

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * Start the OPAMP instance.
    */

  if (HAL_OPAMP_Start(pOPAMP) != HAL_OK)
  {
    goto _app_process_exit;
  }
  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


app_status_t app_deinit(void)
{

  mx_example_opamp_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
