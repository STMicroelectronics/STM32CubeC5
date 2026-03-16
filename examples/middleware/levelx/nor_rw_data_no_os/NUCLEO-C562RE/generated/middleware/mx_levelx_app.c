/**
  ******************************************************************************
  * file           : mx_levelx_app.c
  * brief          : levelx application file
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
#include "mx_levelx_app.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* LevelX assumes that all buffers are aligned to LX_NOR_SECTOR_SIZE bytes for ULONG access */
static ULONG data[LX_NOR_SECTOR_SIZE];
static ULONG read_buffer[LX_NOR_SECTOR_SIZE];

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
LX_NOR_FLASH NORFlashObj;

/*
 * @brief Initialize LevelX and link low level interface
 * @param driver_initialize: function pointer to the driver initialize function
 * @retval 0 on success -1 otherwise
 */
UINT app_levelx_init(VOID)
{
  UINT status = LX_SUCCESS;

  /* Initialize LevelX NOR flash */
  PRINTF("[INFO] Step1: Initializing LevelX NOR flash...\n");
  status = lx_nor_flash_initialize();

  /* Check if the NOR flash driver initialized successfully */
  if (status != LX_SUCCESS)
  {
    PRINTF("[ERROR] Failed to initialize LevelX NOR flash. Status: %u\n", status);
    return -1;
  }
  PRINTF("[INFO] LevelX NOR flash initialized successfully.\n");
  /* Initialize LevelX NOR flash */
  PRINTF("[INFO] Formatting LevelX NOR flash...\n");
  status = lx_nor_flash_format(&NORFlashObj, "nor flash", mx_lx_nor_flash_driver_0_initialize, mx_lx_nor_flash_driver_0_get_ctx());

  /* Check if the NOR flash driver format successfully */
  if (status != LX_SUCCESS)
  {
    PRINTF("[ERROR] Failed to format LevelX NOR flash. Status: %u\n", status);
    return -1;
  }
  PRINTF("[INFO] LevelX NOR flash formatted successfully.\n");

  /* Open the NOR flash driver */
  PRINTF("[INFO] Opening NOR flash driver...\n");
  status = lx_nor_flash_open_extended(&NORFlashObj, "nor flash", mx_lx_nor_flash_driver_0_initialize, mx_lx_nor_flash_driver_0_get_ctx());

  /* Check if the NOR flash driver opened successfully */
  if (status != LX_SUCCESS)
  {
    PRINTF("[ERROR] Failed to open NOR flash driver. Status: %u\n", status);
    return -1;
  }
  PRINTF("[INFO] NOR flash driver opened successfully.\n");

  return status;
}

/*
 * @brief perform read/write/erase operations on NOR flash
 * @param NULL
 * @retval 0 on success -1 otherwise
 */
UINT app_levelx_process(VOID)
{
  UINT status = LX_SUCCESS;
  ULONG app_nor_sector = rand() % 1024;
  UINT i;
  CHAR* str = "Hello world!";

  for (i = 0; i < strlen(str); i++)
  {
    data[i] = (ULONG)str[i];
  }

  /** ########## Step 2 ##########
    * Writes data to the test sector.
  */
  PRINTF("[INFO] Step2: Writing data to the test sector\n");
  status = lx_nor_flash_sector_write(&NORFlashObj, app_nor_sector, (VOID *)data);
  if (status != LX_SUCCESS)
  {
    PRINTF("[ERROR] Failed to write data to the test sector. Status: %u\n", status);
    return -1;
  }
  PRINTF("[INFO] Successfully wrote data to the test sector\n");

  /** ########## Step 3 ##########
    * Reads data from the test sector.
  */
  PRINTF("[INFO] Step3: Reading data from the test sector\n");
  status = lx_nor_flash_sector_read(&NORFlashObj, app_nor_sector, (VOID *)read_buffer);
  if (status != LX_SUCCESS)
  {
    PRINTF("[ERROR] Failed to read data from the test sector. Status: %u\n", status);
    return -1;
  }
  PRINTF("[INFO] Successfully read data from the test sector\n");

  /** ########## Step 4 ##########
    * Verifies data integrity.
  */
  PRINTF("[INFO] Step4: Verifying data integrity\n");
  if (memcmp(data, read_buffer, sizeof(data)) != 0)
  {
    PRINTF("[ERROR] Data integrity check failed. Status: %u\n", status);
    return -1;
  }
  PRINTF("[INFO] Data integrity check passed\n");

  /** ########## Step 5 ##########
    * Releases the test sector.
  */
  PRINTF("[INFO] Step5: Releasing the test sector...\n");
  status = lx_nor_flash_sector_release(&NORFlashObj, app_nor_sector);
  if (status != LX_SUCCESS)
  {
    PRINTF("[ERROR] Failed to release the test sector. Status: %u\n", status);
    return -1;
  }
  PRINTF("[INFO] Successfully released the test sector\n");

  /** ########## Step 6 ##########
    * Closes the NOR flash driver.
  */
  status = lx_nor_flash_close(&NORFlashObj);
  if (status != LX_SUCCESS)
  {
    PRINTF("[ERROR] Failed to close NOR flash driver. Status: %u\n", status);
    return -1;
  }
  PRINTF("[INFO] Step6: NOR flash driver closed successfully.\n");

  return status;
}
