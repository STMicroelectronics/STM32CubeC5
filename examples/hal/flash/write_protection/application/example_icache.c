/**
  ******************************************************************************
  * file           : example.c
  * brief          : FLASH write protection.
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
#include <string.h> /* importing memset, memcpy and memcmp functions */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FLASH_START_PAGE 0U
#define FLASH_PAGE_NUMBERS FLASH_PAGE_NB
/* Private macro -------------------------------------------------------------*/
/* Data buffer size in number of word */
#define BUFFER_SIZE   (FLASH_PAGE_SIZE / sizeof(uint32_t))
/* Erase timeout in millisecond */
#define TIMEOUT_MS   100

/* Private variables ---------------------------------------------------------*/
uint32_t WriteBuffer[BUFFER_SIZE] __ALIGNED(8); /* write data buffer */

uint32_t ReadBuffer[BUFFER_SIZE] __ALIGNED(8); /* read data buffer  */

uint32_t pFlashAddress;

hal_flash_handle_t *pFLASH;  /* pointer referencing the FLASH handle from the generated code */

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * Initialize FLASH peripherals.
  * Unlock the access to the flash control register.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pFLASH = mx_example_flash_init();

  if (pFLASH == NULL)
  {
    goto _app_init_exit;
  }

  /** Unlock the FLASH configuration settings.
    * Use the HAL_FLASH_ITF component to configure Option Bytes and control settings.
    * This is required to unlock write access to the registers involved in this configuration.
    */
  if (HAL_FLASH_ITF_Unlock(HAL_FLASH) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /**
    * Unlock the Option Bytes (OB) control.
    * OB_Unlock is required for option bytes access and forces ITF_Unlock if not already done.
    */
  if (HAL_FLASH_ITF_OB_Unlock(HAL_FLASH) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_FLASH_ITF_OB_DisablePageWRP(HAL_FLASH, EXAMPLE_FLASH_BANK, FLASH_START_PAGE, FLASH_PAGE_NUMBERS) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_FLASH_ITF_OB_Program(HAL_FLASH) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_FLASH_ITF_OB_IsLocked(HAL_FLASH) == HAL_FLASH_ITF_OB_UNLOCKED
      && HAL_FLASH_ITF_IsLocked(HAL_FLASH) == HAL_FLASH_ITF_UNLOCKED)
  {
    PRINTF("[INFO] Step 1: Device initialization COMPLETED and the Flash access is UNLOCKED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }
  else
  {
    PRINTF("[ERROR] Step 1: Device initialization ERROR: Flash control operations are still LOCKED.\n");
    return_status = EXEC_STATUS_ERROR;
  }

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status = HAL_OK;
  uint32_t     error_codes;
  int32_t      cmp_result;

  /* FLASH address used to store the data buffers */
  pFlashAddress = (FLASH_BASE + FLASH_BANK_SIZE + FLASH_PAGE_SIZE);

  memset(WriteBuffer, 0xDE, (BUFFER_SIZE * sizeof(uint32_t)));
  memset(ReadBuffer, 0x00, (BUFFER_SIZE * sizeof(uint32_t)));

  /** ########## Step 2 ##########
    * Erase the desired flash sector.
    * Enables page-level write protection for the desired pages, then program the option bytes.
    */

  /** ICACHE speeds up code execution by storing instructions from FLASH memory.
    * When FLASH is programmed or erased, its contents change. If ICACHE is enabled
    * during this, it can hold outdated instructions from the modified FLASH area.
    */
  HAL_ICACHE_Stop(mx_icache_gethandle());

  if (HAL_FLASH_EraseByAddr(pFLASH, pFlashAddress, FLASH_PAGE_SIZE, TIMEOUT_MS) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_FLASH_ITF_OB_EnablePageWRP(HAL_FLASH, EXAMPLE_FLASH_BANK, FLASH_START_PAGE, FLASH_PAGE_NUMBERS) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_FLASH_ITF_OB_Program(HAL_FLASH) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2: Flash erase COMPLETED and write protection mechanism is ENABLED.\n");

  /** ########## Step 3 ##########
    * Programs the flash memory by specifying the address of the flash, the data to be programmed,
    * and the size of this data in bytes and check that write operation is not allowed in those pages.
    */
  hal_status = HAL_FLASH_ProgramByAddr(pFLASH, pFlashAddress, WriteBuffer, FLASH_PAGE_SIZE, TIMEOUT_MS);

  error_codes = (HAL_FLASH_GetLastErrorCodes(pFLASH) & HAL_FLASH_ERROR_WRP);

  memcpy(ReadBuffer, (uint32_t *)pFlashAddress, (BUFFER_SIZE * sizeof(uint32_t)));

  cmp_result = memcmp(ReadBuffer, WriteBuffer, (BUFFER_SIZE * sizeof(uint32_t)));

  if ((error_codes != 0U) && (hal_status == HAL_ERROR) && (cmp_result != 0U))
  {
    PRINTF("[INFO] Step 3: Write protection is ENABLED and all write operation is not permitted.\n");
  }
  else
  {
    PRINTF("[ERROR] Step 3: Write protection is DISABLED, please verify the current write protection settings.\n");
    goto _app_process_exit;
  }

  /** ########## Step 4 ##########
    * Disables the flash write protection mechanism for the previous flash pages.
    */
  if (HAL_FLASH_ITF_OB_DisablePageWRP(HAL_FLASH, EXAMPLE_FLASH_BANK, FLASH_START_PAGE, FLASH_PAGE_NUMBERS) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_FLASH_ITF_OB_Program(HAL_FLASH) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 4: WRP are DISABLED for all pages in Bank 2, and option bytes are programmed.\n");

  /** ########## Step 5 ##########
    * Reprograms the flash memory, and lock option bytes and the flash interface programming.
    * Then check that data is written correctly in the chosen memory address.
    */
  hal_status = HAL_FLASH_ProgramByAddr(pFLASH, pFlashAddress, WriteBuffer, FLASH_PAGE_SIZE, TIMEOUT_MS);

  memcpy(ReadBuffer, (uint32_t *)pFlashAddress, (BUFFER_SIZE * sizeof(uint32_t)));

  cmp_result = memcmp(ReadBuffer, WriteBuffer, (BUFFER_SIZE * sizeof(uint32_t)));

  if ((hal_status == HAL_OK) && (cmp_result == 0U))
  {
    PRINTF("[INFO] Step 5: Flash programming COMPLETED.\n");
  }
  else
  {
    PRINTF("[ERROR] Step 5: Flash programming ERROR.\n");
    goto _app_process_exit;
  }

  /* Enable ICACHE once Flash programming is finished */
  HAL_ICACHE_Start(mx_icache_gethandle(), HAL_ICACHE_IT_NONE);

  if (HAL_FLASH_ITF_OB_Lock(HAL_FLASH) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_FLASH_ITF_Lock(HAL_FLASH) != HAL_OK)
  {
    goto _app_process_exit;
  }

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 6 ##########
  * Deinitializes the FLASH instance before application exit.
  */
app_status_t app_deinit(void)
{
  PRINTF("[INFO] Step 6: de-init \n");

  mx_example_flash_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */

