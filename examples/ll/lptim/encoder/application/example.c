/**
  ******************************************************************************
  * file           : example.c
  * brief          : LPTIM Encoder interface with LL API
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
/* Configuration macros -----------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the MX_LPTIMx instance for encoder interface.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_lptimx_init() == NULL)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Start the MX_LPTIMx in continuous mode for encoder counting.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_OK;

  LPTIM_Start();

  return return_status;
} /* end app_process */


/** Deinitializes the MX_LPTIMx peripheral before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_lptimx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
