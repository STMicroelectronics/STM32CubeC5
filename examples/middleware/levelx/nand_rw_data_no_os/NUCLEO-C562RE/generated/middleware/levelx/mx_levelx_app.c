/**
  ******************************************************************************
  * @file           : mx_levelx_app.c
  * @brief          : LevelX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_levelx_license.md file
  * in the same directory as the generated code.
  * If no mx_levelx_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mx_levelx_app.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
static UCHAR data[W25N01GVXX_DATA_PAGE_SIZE];
static UCHAR read_buffer[W25N01GVXX_DATA_PAGE_SIZE];

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
LX_NAND_FLASH NANDFlashObj;
ULONG memory_ptr[8192];
/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Application LevelX Initialization.
  * @param  None
  * @retval 0 on success -1 otherwise
  */
UINT app_levelx_init(VOID)
{
  UINT status = LX_SUCCESS;

  /* Initialize LevelX NAND flash */
  PRINTF("[INFO] Step1: Initializing LevelX NAND flash...\n");
  status = lx_nand_flash_initialize();

  /* Check if the NAND flash driver initialized successfully */
  if (status != LX_SUCCESS)
  {
    PRINTF("[ERROR] Failed to initialize LevelX NAND flash. Status: %u\n", status);
    return -1;
  }
  PRINTF("[INFO] LevelX NAND flash initialized successfully\n");

  /* Format LevelX NAND flash */
  PRINTF("[INFO] Formatting LevelX NAND flash...\n");
  status = lx_nand_flash_format_extended(&NANDFlashObj, "nand flash", mx_lx_nand_flash_driver_0_initialize, mx_lx_nand_flash_driver_0_get_ctx(), memory_ptr, sizeof(memory_ptr));

  /* Check if the NAND flash driver format successfully */
  if (status != LX_SUCCESS)
  {
    PRINTF("[ERROR] Failed to format LevelX NAND flash. Status: %u\n", status);
    return -1;
  }
  PRINTF("[INFO] LevelX NAND flash formatted successfully\n");

  /* Open the NAND flash driver */
  PRINTF("[INFO] Opening NAND flash driver...\n");
  status = lx_nand_flash_open_extended(&NANDFlashObj, "nand flash", mx_lx_nand_flash_driver_0_initialize, mx_lx_nand_flash_driver_0_get_ctx(), memory_ptr, sizeof(memory_ptr));

  /* Check if the NAND flash driver opened successfully */
  if (status != LX_SUCCESS)
  {
    PRINTF("[ERROR] Failed to open NAND flash driver. Status: %u\n", status);
    return -1;
  }
  PRINTF("[INFO] NAND flash driver opened successfully\n");

  return status;
}

/*
 * @brief  LevelX operations processing
 * @param NULL
 * @retval 0 on success -1 otherwise
 */
UINT app_levelx_process(VOID)
{
  UINT status = LX_SUCCESS;
  ULONG app_nand_sector = rand() % 1024;
  UINT i;
  CHAR *str = "Hello world!";

  for (i = 0; i < strlen(str); i++)
  {
    data[i] = (ULONG)str[i];
  }

  /** ########## Step 2 ##########
    * Write data to the test sector.
  */
  PRINTF("[INFO] Step2: Writing data to the test sector\n");
  status = lx_nand_flash_sectors_write(&NANDFlashObj, app_nand_sector, (void *)data, 1);
  if (status != LX_SUCCESS)
  {
    PRINTF("[ERROR] Failed to write data to the test sector. Status: %u\n", status);
    return -1;
  }
  PRINTF("[INFO] Successfully wrote data to the test sector\n");

  /** ########## Step 3 ##########
    * Read data from the test sector.
  */
  PRINTF("[INFO] Step3: Reading data from the test sector\n");
  status = lx_nand_flash_sectors_read(&NANDFlashObj, app_nand_sector, (void *)read_buffer, 1);
  if (status != LX_SUCCESS)
  {
    PRINTF("[ERROR] Failed to read data from the test sector. Status: %u\n", status);
    return -1;
  }
  PRINTF("[INFO] Successfully read data from the test sector\n");

  /** ########## Step 4 ##########
    * Verify data integrity.
  */
  PRINTF("[INFO] Step4: Verifying data integrity...\n");
  if (memcmp(data, read_buffer, sizeof(data)) != 0)
  {
    PRINTF("[ERROR] Data integrity check failed. Status: %u\n", status);
    return -1;
  }
  PRINTF("[INFO] Data integrity check passed\n");

  /** ########## Step 5 ##########
    * Release the test sector.
  */
  PRINTF("[INFO] Step5: Releasing the test sector...\n");
  status = lx_nand_flash_sector_release(&NANDFlashObj, app_nand_sector);
  if (status != LX_SUCCESS)
  {
    PRINTF("[ERROR] Failed to release the test sector. Status: %u\n", status);
    return -1;
  }
  PRINTF("[INFO] Successfully released the test sector\n");

  /** ########## Step 6 ##########
    * Close the NAND flash driver.
  */
  status = lx_nand_flash_close(&NANDFlashObj);
  if (status != LX_SUCCESS)
  {
    PRINTF("[ERROR] Failed to close NAND flash driver. Status: %u\n", status);
    return -1;
  }
  PRINTF("[INFO] Step6: NAND flash driver closed successfully\n");

  return status;
}
