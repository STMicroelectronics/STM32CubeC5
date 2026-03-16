/**
  ******************************************************************************
  * file           : example.c
  * brief          : CRC UserDefinedPolynomial with HAL API
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
#define BUFFER_SIZE 1U /* Size in uint32_t */

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
hal_crc_handle_t *pCRC; /* pointer referencing the CRC handle from the generated code */

/** Buffer containing the data on which the CRC is calculated (one-word buffer in this example)
  * @user: Update ExpectedValue in case you modify DataBuffer
  */
const uint32_t DataBuffer = 0x00001234;
/* Expected CRC Value, corresponding to this default example configuration and DataBuffer content */
const uint32_t ExpectedValue = 0x98;
/* computed CRC value */
uint32_t CrcValue = 0;

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the CRC.
  * @user: The polynomial value can be modified with STM32CubeMX2.
  *        In this case, ExpectedValue needs to be updated accordingly also.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pCRC = mx_example_crc_init();

  if (pCRC != NULL)
  {
    PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Computes the CRC of DataBuffer and checks it against ExpectedValue.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  hal_status_t hal_status = HAL_CRC_Calculate(pCRC, &DataBuffer, sizeof(uint32_t) * BUFFER_SIZE, &CrcValue);

  if (hal_status == HAL_OK)
  {
    /* Compare the CRC value to the Expected one */
    if (CrcValue == ExpectedValue)
    {
      PRINTF("[INFO] Step 2: Correct CRC value (0x%" PRIx32 ")\n", CrcValue);
      return_status = EXEC_STATUS_OK;
    }
    else
    {
      PRINTF("[ERROR] Step 2: Wrong CRC value (0x%" PRIx32 " instead of 0x%" PRIx32 ")\n", CrcValue, ExpectedValue);
    }
  }

  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Deinitializes the CRC before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  PRINTF("[INFO] Step 3: de-init\n");
  mx_example_crc_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
