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
#include "mx_levelx_app.h"
#include "basic_stdio_core.h" /* logs */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_INIT_OK;

  BASIC_STDIO_Init(mx_basic_stdio_gethandle());

  /** ########## Step 1 ##########
    * Initializes and opens the NOR flash driver.
  */

  /* Initialize the levelx system component */
  if (app_levelx_init() != 0)
  {
    return_status = EXEC_STATUS_ERROR;
  }

  return return_status;
} /* end app_init */


app_status_t app_deinit(void)
{
  /** This API is not used in this example.
    * It is optimized out by the toolchain.
    */
  return EXEC_STATUS_OK;
} /* end app_deinit */
