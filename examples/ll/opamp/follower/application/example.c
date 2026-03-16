/**
  ******************************************************************************
  * file           : example.c
  * brief          : OPAMP configured in follower mode using the ll.
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
/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * The applicative code initializes the OPAMP instance in follower mode.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Configures the opamp instance */
  if (mx_opampx_init() != NULL)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Start the OPAMP.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_OK;

  OPAMP_Start();

  return return_status;
} /* end app_process */


/** Deinitializes the OPAMP instance before leaving the scenario.
  * This function is never called and is provided here as reference.
  */
app_status_t app_deinit(void)
{
  mx_opampx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
