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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define CRS_SYNC_TIMEOUT_MS 1000

/** @user: MCO divider,
  * you might have to tune it to output a frequency in coherence with your measurement tool.
  */
/* Private variables ---------------------------------------------------------*/

hal_crs_handle_t *pCRS;  /* pointer referencing the CRS handle from the generated code */

/* Private functions prototype -----------------------------------------------*/
/** ########## Step 1 ##########
  * The init of the CRS instance is triggered by the applicative code. The HSI48 clock is configured to be output on
  * the MCO pin.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pCRS = mx_example_crs_init();

  if (pCRS != NULL)
  {
    PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * Starts the CRS synchronization with LSE
    */
  if (HAL_CRS_StartSync(pCRS) != HAL_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2: CRS Synchronization started.\n");

  /** ########## Step 3 ##########
    * Poll for the end of synchronization
    */
  if (HAL_CRS_PollForSync(pCRS, CRS_SYNC_TIMEOUT_MS) != HAL_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 3: CRS synchronization finished.\n");

  return_status = EXEC_STATUS_OK;
_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 4 ##########
  * Deinitializes the CRS instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  PRINTF("[INFO] Step 4: de-init. \n");
  mx_example_crs_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */

