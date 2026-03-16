/**
  ******************************************************************************
  * file           : example.c
  * brief          : example program body
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
#include "mx_filex_app.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/** ########## Step 1 ##########
  * Initializes FileX.
  */
/* The init of the sd instance is triggered by the applicative code */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_INIT_OK;

  /* Initialize the filex system component */
  if (app_filex_init() != 0)
  {
    return_status = EXEC_STATUS_ERROR;
  }
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_OK;

  if (app_filex_process() != 0)
  {
    return_status = EXEC_STATUS_ERROR;
  }
  return return_status;
} /* end app_process */


/** ########## Step 6 ##########
  * Deinitializes the XSPI instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  return EXEC_STATUS_OK;

} /* end app_deinit */
