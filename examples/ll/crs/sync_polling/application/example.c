/**
  ******************************************************************************
  * file           : example.c
  * brief          : adjust the internal clock using the CRS polling method
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
  * The init of the MX_CRSx instance is triggered by the applicative code.
  * The HSI48 clock is configured to be output on the MCO pin
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Configures the MX_CRSx instance */
  if (mx_crsx_init() != NULL)
  {
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
  CRS_StartSynchronization();

  /** ########## Step 3 ##########
    * Poll for the end of synchronization
    */
  do
  {
    if (CRS_PollForSynchronization() != SYSTEM_OK)
    {
      goto _app_process_exit;
    }

    return_status = EXEC_STATUS_OK;

  } while (return_status != EXEC_STATUS_OK);

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 4 ##########
  * Deinitializes the MX_CRSx before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_crsx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
