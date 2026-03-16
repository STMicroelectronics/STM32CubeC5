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
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* @user: You can set the number of times that the IWDG will be refreshed before generating the reset */
#define PROCESS_LOOP_NUMBER      3U
#define IWDG_MIN_TIME_VALUE_MS   0U
#define IWDG_MAX_TIME_VALUE_MS   4000U
#define IWDG_EWU_TIME_VALUE_MS   1000U
#define IWDG_VALID_REFRESH_MS    500U
#define IWDG_INVALID_REFRESH  (IWDG_MAX_TIME_VALUE_MS + (IWDG_MAX_TIME_VALUE_MS / 10))

#define FLASH_OPERATION_TIMEOUT  3000U

/* The data to be programmed and its size is measured in bytes */
uint8_t DataBuffer[] = {0x45, 0x78, 0x61, 0x6D, 0x70, 0x6C, 0x65, 0x20, 0x69, 0x77,
                        0x64, 0x67, 0x20, 0x65, 0x61, 0x72, 0x6C, 0x79, 0x20, 0x69,
                        0x6E, 0x74, 0x65, 0x72, 0x72, 0x75, 0x70, 0x74, 0x00, 0x00,
                        0x00, 0x00
                        };

#define BUFFER_SIZE  (sizeof(DataBuffer))
uint8_t buffer[BUFFER_SIZE];

uint32_t MemoryUserStartAdd ;
/* Private variables ---------------------------------------------------------*/

hal_iwdg_handle_t *pIWDG;    /* pointer referencing the IWDG handle from the generated code */
hal_flash_handle_t *pFLASH;  /* pointer referencing the FLASH handle from the generated code */


uint8_t IWDG_RestartFromIWDGReset = 0U;  /* Flag to know if the device restart due to IWDG reset */
/* Private functions prototype -----------------------------------------------*/
static void EarlyWakeupCallback(hal_iwdg_handle_t *hiwdg);


/** ########## Step 1 ##########
  * The initialization of the IWDG instance is triggered by the applicative code.
  * If the device is restarted due to a IWDG reset, the IWDG initialization is bypassed.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  MemoryUserStartAdd = (uint32_t)(FLASH_BASE + FLASH_SIZE - BUFFER_SIZE);

  /* The initialization of the FLASH instance is triggered by the applicative code. */
  pFLASH = mx_example_flash_init();
  if (pFLASH == NULL)
  {
    goto _app_init_exit;
  }

  HAL_FLASH_ITF_Unlock(HAL_FLASH_GetInstance(pFLASH));

  /** Check if the system was reset by the Independent Watchdog (IWDG).
    * If yes, clear the reset flags and mark the restart as caused by the watchdog.
    */
  if ((HAL_RCC_GetResetSource() & HAL_RCC_RESET_FLAG_IWDG) != 0U)
  {
    HAL_RCC_ClearResetFlags();
    IWDG_RestartFromIWDGReset = 1U;
    return_status = EXEC_STATUS_INIT_OK;
    goto _app_init_exit;
  }

  /** Initialization of the IWDG.
    * The IWDG is initialized with the parameters that can be configured by the user through the cube interface.
    */
  pIWDG = mx_example_iwdg_init();
  if (pIWDG == NULL)
  {
    goto _app_init_exit;
  }

  /* Register the callback for IWDG Early Wakeup interrupt */
  if (HAL_IWDG_RegisterEarlyWakeupCallback(pIWDG, EarlyWakeupCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /** Start the Independent Watchdog (IWDG) with timing values.
    * IWDG_MIN_TIME_VALUE: Minimum timeout
    * IWDG_MAX_TIME_VALUE: Maximum timeout
    * IWDG_EWU_TIME_VALUE: Early WakeUp timeout
    */
  if (HAL_IWDG_Start(pIWDG, IWDG_MIN_TIME_VALUE_MS, IWDG_MAX_TIME_VALUE_MS, IWDG_EWU_TIME_VALUE_MS) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** Checking if the device restarted due to the IWDG. If not, the IWDG reset is triggered on purpose.
    */
  if (IWDG_RestartFromIWDGReset == 0U)
  {
    /* Watchdog has not fired, so the next steps demonstrate how IWDG triggers reset */

    /** ########## Step 2 ##########
      * The purpose is to refresh the watchdog PROCESS_LOOP_NUMBER times and to prevent watchdog reset.
      */
    for (uint8_t i = 0; i < PROCESS_LOOP_NUMBER; i++)
    {
      HAL_Delay(IWDG_VALID_REFRESH_MS); /* IWDG refresh delay */

      if (HAL_IWDG_Refresh(pIWDG) != HAL_OK)
      {
        goto _app_process_exit;
      }

      PRINTF("[INFO] Step 2: IWDG refreshed.\n");
    }

    /** ########## Step 3 ##########
      * Do not refresh the IWDG to trigger a reset.
      * The MCU is expected to wait for the EarlyWakeup interrupt.
      * This deliberate action tests the watchdog reset functionality.
      */
    PRINTF("[INFO] Step 3: IWDG not refreshed on purpose.\n");
    HAL_Delay(IWDG_INVALID_REFRESH);
    PRINTF("[ERROR] The IWDG is supposed to reset before reaching this stage due to the Step 3.\n");
  }
  else
  {
    /* Watchdog has already fired, that means the previous steps (Step1,2,3,4) have already been executed */

    /** ########## Step 5 ##########
      * Reads flash memory and verifies it against the original data.
      * The device is restarted due to IWDG
      */
    for (uint8_t i = 0; i < BUFFER_SIZE; i++)
    {
      /* Read data from flash memory */
      buffer[i] = *(uint8_t *)(MemoryUserStartAdd + i);
    }

    /* Compare flash data with original data */
    if (memcmp(buffer, DataBuffer, sizeof(BUFFER_SIZE)) != 0)
    {
      goto _app_process_exit;
    }
    PRINTF("[INFO] Step 5: Device's restart completed due to IWDG. Data verification completed.\n");
    return_status = EXEC_STATUS_OK;
  }

_app_process_exit:
  return return_status;
} /* end app_process */


/** Nothing to do because the IWDG IP cannot be de-initialized.
  */
app_status_t app_deinit(void)
{
  HAL_FLASH_ITF_Lock(HAL_FLASH_GetInstance(pFLASH));
  mx_example_flash_deinit();
  return EXEC_STATUS_OK;
} /* end app_deinit */


/** Callback for IWDG Early Wakeup interrupt
  * @user This callback can be customized.
  */
static void EarlyWakeupCallback(hal_iwdg_handle_t *hiwdg)
{
  if (HAL_FLASH_EraseByAddr(pFLASH, MemoryUserStartAdd, BUFFER_SIZE, FLASH_OPERATION_TIMEOUT) == HAL_OK)
  {
    /** ########## Step 4 ##########
      * Programs the memory by indicating the address, the data to be programmed and its size.
      */
    if (HAL_FLASH_ProgramByAddr(pFLASH, MemoryUserStartAdd, (const uint32_t *)DataBuffer,
                                BUFFER_SIZE, FLASH_OPERATION_TIMEOUT) == HAL_OK)
    {
      PRINTF("[INFO] Step 4: The memory is programmed during the interrupt.\n");
    }
  }
}
