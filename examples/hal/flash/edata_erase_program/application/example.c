/**
  ******************************************************************************
  * file           : example.c
  * brief          : FLASH EDATA erase program.
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
/* Private macro -------------------------------------------------------------*/
/* Data buffer size in number of word */
#define BUFFER_SIZE   (FLASH_EDATA_PAGE_SIZE / sizeof(uint32_t))
/* Erase timeout in millisecond */
#define TIMEOUT_MS   100
/* EDATA erase start address */
#define FLASH_EDATA_ERASE_START_ADDRESS (FLASH_EDATA_BASE + FLASH_EDATA_BANK_SIZE)
/* Private variables ---------------------------------------------------------*/
hal_flash_handle_t *pFLASH;                     /* pointer referencing the FLASH handle from the generated code */
uint32_t FlashAddress;                          /* EDATA area address for read/write/erase operations           */
uint32_t WriteBuffer[BUFFER_SIZE] __ALIGNED(8); /* values to be written in EDATA (const)                        */
uint32_t ReadBuffer[BUFFER_SIZE] __ALIGNED(8);  /* buffer to read back from EDATA                               */
volatile uint8_t EccErrorDetection = 0U;        /* flag to report ecc error on EDATA read operation             */

/* Private functions prototype -----------------------------------------------*/
/* User-dfined function to copy word data size */
static void WordCopy(uint32_t *dest, uint32_t src, uint32_t size_word);
/* User-defined callback to manage the expected ECC error. */
static hal_status_t ECCErrorCallback(hal_flash_handle_t *hflash, hal_flash_bank_t bank);


/** ########## Step 1 ##########
  * Initializes FLASH peripherals.
  * Unlocks the access to the flash control register.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pFLASH = mx_example_flash_init();

  if (pFLASH == NULL)
  {
    goto _app_init_exit;
  }

  if (HAL_FLASH_RegisterECCErrorCallback(pFLASH, ECCErrorCallback) != HAL_OK)
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

  if (HAL_FLASH_ITF_OB_IsEnabledEDATAArea(HAL_FLASH) == HAL_FLASH_ITF_OB_EDATA_AREA_ENABLED)
  {
    PRINTF("[INFO] Step 1: Device initialization COMPLETED within EDATA area is ENABLED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }
  else
  {
    PRINTF("[ERROR] Step 1: Device initialization ERROR: Flash EDATA area is still DISABLED.\n");
    return_status = EXEC_STATUS_ERROR;
  }

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* FLASH address used to store the data buffers */
  FlashAddress = FLASH_EDATA_ERASE_START_ADDRESS;

  /* Fill WriteBuffer and ReadBuffer */
  memset(WriteBuffer, 0xDE, (BUFFER_SIZE * sizeof(uint32_t)));
  memset(ReadBuffer, 0x00, (BUFFER_SIZE * sizeof(uint32_t)));

  /** ########## Step 2 ##########
    * Erases the EDATA flash memory area.
    * Programs the EDATA flash memory area.
    * Checks that the data is written in the desired location.
    */

  if (HAL_FLASH_EDATA_EraseByAddr(pFLASH, FlashAddress, FLASH_EDATA_PAGE_SIZE, TIMEOUT_MS) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_FLASH_EDATA_ProgramByAddr(pFLASH, FlashAddress, WriteBuffer, FLASH_EDATA_PAGE_SIZE, TIMEOUT_MS) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Read back from programmed EDATA area: no ECC error expected */
  WordCopy(ReadBuffer, FlashAddress, BUFFER_SIZE);

  if ((EccErrorDetection != 0) || (memcmp(ReadBuffer, WriteBuffer, (BUFFER_SIZE * sizeof(uint32_t))) != 0U))
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2: EDATA area programming COMPLETED.\n");

  /** ########## Step 3 ##########
    * Erase the EDATA flash memory area.
    * Try to read back the erased data.
    */
  if (HAL_FLASH_EDATA_EraseByAddr(pFLASH, FlashAddress, FLASH_EDATA_PAGE_SIZE, TIMEOUT_MS) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /** Read back from erased EDATA area.
    * An ECC error occurs, and the execution flow enters the NMI handler.
    */
  WordCopy(ReadBuffer, FlashAddress, BUFFER_SIZE);

  /* reset EccErrorDetection, because the behavior is normal when reading from an erased EDATA area */
  EccErrorDetection = 0U;
  /** ########## Step 5 ##########
    * Back from the NMI handler: ECC check has been disabled.
    * Compare the read data with the expected data.
    */
  if (memcmp(ReadBuffer, WriteBuffer, (BUFFER_SIZE * sizeof(uint32_t))) == 0U)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 5: EDATA area is ERASED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  /* Final check to make sure no unexpected ECC error occurred. */
  if (EccErrorDetection == 1U)
  {
    PRINTF("[ERROR] An abnormal ECC error detection occurred during the execution.\n");
    return_status = EXEC_STATUS_ERROR;
  }
  /* Back to main() */
  return return_status;
} /* end app_process */


/** ########## Step 6 ##########
  * Deinitializes the FLASH instance before application exit.
  */
app_status_t app_deinit(void)
{
  PRINTF("[INFO] Step 6: de-init \n");
  /**
    * Locks the flash configuration.
    * This prevents any unwanted modification.
  */
  if (HAL_FLASH_ITF_Lock(HAL_FLASH) != HAL_OK)
  {
    return EXEC_STATUS_ERROR;
  }

  mx_example_flash_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** Word Copy function.
  * This function copies size_word consecutive 32bit words from src to dest.
  */
static void WordCopy(uint32_t *dest, uint32_t src, uint32_t size_word)
{
  for (uint32_t index = 0; index < size_word; index++)
  {
    *(dest + index)  = (*(__IO uint32_t *)(src + (index * sizeof(uint32_t))));
  }
}


/** ECC error callback.
  * This function executed when a double ECC error occurs during reading back erased EDATA memory.
  */
static hal_status_t ECCErrorCallback(hal_flash_handle_t *hflash, hal_flash_bank_t bank)
{
  STM32_UNUSED(bank);
  hal_status_t status =  HAL_ERROR; /* lock in MMI handler if error is unexpected */
  hal_flash_ecc_info_t info;

  /** ########## Step 4 ##########
    * Manage the ECC error generated when reading back erased EDATA memory.
    */
  HAL_FLASH_ECC_GetInfo(pFLASH, bank, &info); /* Retrieve information about the ECC failure */

  if (info.addr >= (FLASH_EDATA_ERASE_START_ADDRESS)
      && info.addr < (FLASH_EDATA_ERASE_START_ADDRESS + FLASH_EDATA_PAGE_SIZE))
  {
    status = HAL_OK;          /** Back to the example execution flow, the error is only relative to edata area and
                                * let application decide the action to take.
                                */
    EccErrorDetection = 1U;
  } /* else: ECC error is not detected in the expected address range */
  /** Abnormal case: ECC error occurring at an unexpected step, or not in the expected address range.
    * lock in MMI handler.
    */
  return status;
}