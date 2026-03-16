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
#include "mx_eeprom_emul.h"   /* aliases to the eeprom_emul generated code       */
#include "example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t dataCounter = 0;
uint8_t dataWriteValue[EE_NB_OF_VARIABLES + 1] = {0};
/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the eeprom_emulation utility.
  */
app_status_t app_init(void)
{
  app_status_t return_status;

  if (mx_eeprom_emulation_init() == SYSTEM_OK)
  {
    PRINTF("[INFO] Step 1: EEPROM INITIALIZATION COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }
  else
  {
    PRINTF("[ERROR] Step 1: EEPROM INITIALIZATION ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
  }
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * The purpose of this step is to show how an application must manage eeprom data operation.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /**  Write all the eeprom variables
    * This operation involves iterating from 1 to EE_NB_OF_VARIABLES
    *   1 - store the value to write in dataWriteValue array
    *   2 - write the value
    */
  PRINTF("[INFO] Step 2: START THE WRITE PROCESS\n");
  for (uint16_t var_id = 1; var_id <= EE_NB_OF_VARIABLES; var_id++)
  {
    /* Increment the number of values written */
    dataCounter++;

    /* Set dataWriteValue */
    dataWriteValue[var_id] = (uint8_t)var_id;

    /* Write value at the virtual address var_id */
    ee_status ret_state =  EE_WriteVariable8bits(var_id, dataWriteValue[var_id]);
    switch (ret_state)
    {
      case EE_INFO_CLEANUP_REQUIRED:
      {
        PRINTF("[INFO] Step 2: CLEAN UP REQUIRED\n");
        EE_CleanUp();
        break;
      }
      case EE_OK :
      {
        /* Write operation is OK */
        break;
      }
      default:
      {
        PRINTF("[ERROR] Step 2: ERROR RETURNED BY EE_WRITE FUNCTION\n");
        goto error;
        break;
      }
    }
  }

  /** ########## Step 3 ##########
    * Read data from emulated eeprom memory.
    * This operation involves iterating from 1 to EE_NB_OF_VARIABLES
    *    1 - Read the variables
    *    2 - Compare the read variable with the written variable
    */
  PRINTF("[INFO] Step 3: START READ PROCESS\n");
  for (uint16_t var_id = 1; var_id <= EE_NB_OF_VARIABLES; var_id++)
  {
    uint8_t data_read = 0;
    /* Read the variable value from the virtual address var_id */
    if (EE_ReadVariable8bits(var_id, &data_read) != EE_OK)
    {
      PRINTF("[ERROR] Step 3: ERROR RETURNED BY EE_READ FUNCTION\n");
      goto error;
    }

    if (data_read != dataWriteValue[var_id])
    {
      PRINTF("[ERROR] Step 3: ERROR, READ and WRITTEN VARIABLES ARE DIFFERENT\n");
      goto error;
    }
  }

  return_status = EXEC_STATUS_OK;
  PRINTF("[INFO] Step 3: READ and WRITTEN VARIABLES ARE EQUAL\n");
error :
  return return_status;
} /* end app_process */


/** ########## Step 4 ##########
  * Deinitialize the PPP instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  PRINTF("[INFO] Step 4: DE-INIT\n");
  /** No action required
    */

  return EXEC_STATUS_OK;
} /* end app_deinit */
