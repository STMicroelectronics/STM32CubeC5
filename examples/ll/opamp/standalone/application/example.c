/**
  ******************************************************************************
  * file           : example.c
  * brief          : OPAMP configured in standalone mode using the LL.
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


/** ########## Step 1 ##########
  * Configure the OPAMP instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

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

  OPAMP_Start();

  return EXEC_STATUS_OK;
} /* end app_process */


/**
  * Deinitializes the OPAMP instance.
  * This function is never called and is provided here as reference.
  */
app_status_t app_deinit(void)
{
  mx_opampx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
