/**
  ******************************************************************************
  * file           : example.c
  * brief          : Retrieves the Device ID and Revision ID using the HAL API
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

uint32_t DeviceID = 0x0000; /* Stores the device ID of the chip. */
uint32_t RevisionID = 0x0000; /* Stores the revision ID of the chip. */

/* Private functions prototype -----------------------------------------------*/

app_status_t app_init(void)
{
  /* The DBGMCU IP requires no specific initialization. System peripherals are initialized in `mx_system_init()`. */
  return EXEC_STATUS_INIT_OK;
} /* end app_init */


app_status_t app_process(void)
{
  /** ########## Step 1 ##########
    * Retrieves the Device ID and stores it in a global variable.
    *
  */
  DeviceID = HAL_DBGMCU_GetDeviceID();
  PRINTF("[INFO] Step 1: Device ID: 0x%.4"PRIX32"\n", DeviceID);

  /** ########## Step 2 ##########
    * Retrieves the Revision ID and stores it in a global variable.
  */
  RevisionID = HAL_DBGMCU_GetRevisionID();
  PRINTF("[INFO] Step 2: Revision ID: 0x%.4"PRIX32"\n", RevisionID);

  return EXEC_STATUS_OK;
} /* end app_process */


app_status_t app_deinit(void)
{
  /* This example requires no specific de-initialization. */
  return EXEC_STATUS_OK;
} /* end app_deinit */
