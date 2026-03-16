/**
  ******************************************************************************
  * file           : example.c
  * brief          : LPTIM Encoder interface with HAL API
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
hal_lptim_handle_t *pLPTIM_EncoderInterface;
/* Configuration macros -----------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * Initializes the LPTIM for encoder interface.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pLPTIM_EncoderInterface = mx_example_lptim_init();
  if (pLPTIM_EncoderInterface == NULL)
  {
    PRINTF("[ERROR] Step 1: Device initialization ERROR.\n");
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Start the LPTIM in continuous mode for encoder counting.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (HAL_LPTIM_Start(pLPTIM_EncoderInterface) != HAL_OK)
  {
    PRINTF("[ERROR] Step 2: Start LPTIM encoder interface ERROR.\n");
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2: LPTIM encoder interface started.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Deinitializes the LPTIM instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  PRINTF("[INFO] Step 3: de-init \n");

  mx_example_lptim_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */

