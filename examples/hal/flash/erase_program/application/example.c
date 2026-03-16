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
/* @user: set the timeout period in milliseconds for erasing and programming data */
#define TIMEOUT 3000U

/* The data to be programmed and its size is measured in bytes */
const uint32_t DataBuffer[] = { 0x12345678,
                                0x87654321,
                                0x12344321,
                                0x56788765
                              };
#define BUFFER_SIZE  (sizeof(DataBuffer))

/** The address at which the erasing and the programming process begin
  * is located at the end of the flash memory.
  */
uint32_t FlashUserStartAddr;

/* Private variables ---------------------------------------------------------*/
hal_flash_handle_t *pFLASH;  /* pointer referencing the FLASH handle from the generated code */
/* Private macro -------------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * The init of the Flash instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pFLASH = mx_example_flash_init();

  HAL_FLASH_ITF_Unlock(HAL_FLASH);

  if (pFLASH != NULL)
  {
    PRINTF("[INFO] Step 1: Device initialization COMPLETED and the Flash access is unlocked.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** Set the buffer address to the end of flash
    * Note that FLASH_BASE and FLASH_SIZE values are defined by DFP pack
  */
  FlashUserStartAddr = (uint32_t)(FLASH_BASE + FLASH_SIZE - BUFFER_SIZE);

  /** ########## Step 2 ##########
    * To perform erasure, you need to specify the address and the size of the data that needs to be erased
  */
  if (HAL_FLASH_EraseByAddr(pFLASH, FlashUserStartAddr, BUFFER_SIZE, TIMEOUT) != HAL_OK)
  {
    goto _app_exit;
  }
  PRINTF("[INFO] Step 2: The selected Flash area is erased\n");

  /** ########## Step 3 ##########
      * Programs, the flash memory by indicating the address, the data to be programmed and its size.
      */
  if (HAL_FLASH_ProgramByAddr(pFLASH, FlashUserStartAddr, DataBuffer, BUFFER_SIZE, TIMEOUT) != HAL_OK)
  {
    goto _app_exit;
  }

  PRINTF("[INFO] Step 3: The memory Flash is programmed\n");
  return_status = EXEC_STATUS_OK;

_app_exit:
  return return_status;
} /* end app_process */


/** ########## Step 4 ##########
  * lock the FLASH control register access and deinitializes the FLASH instance.
  */
app_status_t app_deinit(void)
{
  HAL_FLASH_ITF_Lock(HAL_FLASH);

  PRINTF("[INFO] Step 4: Device de-init COMPLETED\n");

  mx_example_flash_deinit();
  return EXEC_STATUS_OK;
} /* end app_deinit */
