/**
  ******************************************************************************
  * @file    mx_levelx_app.c
  * @brief   LevelX applicative file
  ******************************************************************************
  * @attention
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
/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

typedef struct task_data {
               UCHAR data_written[2048];
               UCHAR data_read[2048];
}task_data_t;

static task_data_t task_one_data;
static task_data_t task_two_data;

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
LX_NAND_FLASH NANDFlashObj;
ULONG memory_ptr[8192];

/* The applicative tasks demonstrating the nand_multi_access_freertos usage */
TaskHandle_t main_task;
TaskHandle_t app_task_one;
TaskHandle_t app_task_two;
SemaphoreHandle_t printf_mutex;

/* Declaration of the Event Group */
EventGroupHandle_t xEventGroup;

/* Event Flags Bits */
const EventBits_t xBit1 = (1 << 0);
const EventBits_t xBit2 = (1 << 1);
const EventBits_t xBit3 = (1 << 2);

/* Private function prototypes -----------------------------------------------*/
static VOID main_task_entry(VOID *args);
static VOID app_task_entry_one(VOID *args);
static VOID app_task_entry_two(VOID *args);

/** @brief  Application LevelX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT app_levelx_init(VOID)
{
  UINT ret;

  /** ########## Step 1 ##########
    * Prepare concurrent execution context.
    * Creates an event and 3 tasks with the same priority.
    */

  PRINTF("[INFO] Step 1: Prepare concurrent execution context\r\n");

  /* Create the 1st concurrent task.  */
  ret = xTaskCreate(app_task_entry_one, "App Task one", APP_DEFAULT_STACK_SIZE, (VOID *)&task_one_data,
                    APP_TASK_PRIORITY, &app_task_one);
  if (ret != pdPASS)
  {
    return LX_FALSE;
  }
  /* Create the 2nd concurrent task */
  ret = xTaskCreate(app_task_entry_two, "App Task two", APP_DEFAULT_STACK_SIZE, (VOID *)&task_two_data,
                    APP_TASK_PRIORITY, &app_task_two);
  if (ret != pdPASS)
  {
    return LX_FALSE;
  }

  /* Create the main task.  */
  ret = xTaskCreate(main_task_entry, "Main Task ", APP_DEFAULT_STACK_SIZE, (VOID *) NULL,
                    MAIN_TASK_PRIORITY, &main_task);
  if (ret != pdPASS)
  {
    return LX_FALSE;
  }

  /* Creation of the Event Group */
  xEventGroup = xEventGroupCreate();

  /* Create the printf mutex */
  printf_mutex = xSemaphoreCreateMutex();

  /** ########## Step 2 ##########
    * Initialize LevelX NAND flash.
    */

  PRINTF("[INFO] Step 2: Initialize LevelX NAND flash\r\n");
  /* Initialize LevelX NAND flash */
  lx_nand_flash_initialize();

  return LX_SUCCESS;
}

/**
  * @brief perform main task operations
  * @param args pointer to task argument (unused)
  * @retval None
  */
VOID main_task_entry(VOID *args)
{
  TaskHandle_t BackgroundTask = xTaskGetHandle("Background Task");
  UINT status = LX_ERROR;
  VOID *nand_ctx = mx_lx_nand_flash_driver_0_get_ctx();

  /** ########## Step 4 ##########
    * Format and open LevelX NAND flash.
    */

  PRINTF("[INFO] Step 4: Format and open LevelX NAND flash\r\n");

  /* Format the NAND flash driver.  */
  status = lx_nand_flash_format_extended(&NANDFlashObj, "nand flash", mx_lx_nand_flash_driver_0_initialize, nand_ctx, memory_ptr, sizeof(memory_ptr));

  /* Check the NAND flash driver format status.  */
  if (status != LX_SUCCESS)
  {
    /* Error formatting the driver, notify the background task.  */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  /* Open the NAND flash driver.  */
  status = lx_nand_flash_open_extended(&NANDFlashObj, "nand flash", mx_lx_nand_flash_driver_0_initialize, nand_ctx, memory_ptr, sizeof(memory_ptr));

  /* Check the NAND flash driver open status.  */
  if (status != LX_SUCCESS)
  {
    /* Error opening the driver, notify the background task.  */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  xEventGroupSetBits(xEventGroup, xBit1);

  /* Waiting for both xBit2 and xBit3 to be set. */
  if (xEventGroupWaitBits(xEventGroup, xBit2 | xBit3, pdTRUE, pdTRUE, TIMEOUT) == (xBit2 | xBit3))
  {
    PRINTF("[INFO] Step 5: All concurrent read, write, and data integrity checks completed successfully for both tasks\r\n");

    /** ########## Step 6 ##########
      * Close the NAND flash driver.
      */

    PRINTF("[INFO] Step 6: Close the NAND flash driver\r\n");

    /* Close the NAND flash driver.  */
    status = lx_nand_flash_close(&NANDFlashObj);

    /* Check the NAND flash driver close status.  */
    if (status != LX_SUCCESS)
    {
      /* Error closing the media, notify the background task.  */
      xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
    }

  }
  else
  {
    /* Either xBit2 or xBit3 (or both) were not set. Handle the error. */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  /* Notify the background task that the example runs as expected. */
  xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_SUCCESS, eSetValueWithOverwrite);

  vTaskDelete(main_task);

}

/** ########## Step 5 ##########
  * Read, write and data integrity check for both tasks.
  */

/**
  * @brief perform operations for task one
  * @param args pointer to task context structure
  * @retval None
  */
VOID app_task_entry_one(VOID *args)
{
  TaskHandle_t BackgroundTask = xTaskGetHandle("Background Task");

  UINT status = LX_ERROR;
  srand(xTaskGetTickCount());
  UINT i, j;
  ULONG app_nand_sector;
  task_data_t *pdata = (task_data_t *)args;

  /* Waiting for an event from main task */
  xEventGroupWaitBits(xEventGroup, xBit1, pdTRUE, pdFALSE, TIMEOUT);

  /* Multi-access read/write loop */
  for (j = 0; j < 5; j++)
  {
    app_nand_sector = rand() % 1024;
    for (i = 0; i < sizeof(pdata->data_written); i++)
    {
      pdata->data_written[i] = (UCHAR)(rand() & 0xFF);
    }


    /* Write data to the test sector.  */
    status = lx_nand_flash_sector_write(&NANDFlashObj, app_nand_sector, (VOID *)pdata->data_written);
    if (status != LX_SUCCESS)
    {
      /* Error writing the data, notify the background task.  */
      xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
    }

    /* Read data from the test sector.  */
    status = lx_nand_flash_sector_read(&NANDFlashObj, app_nand_sector, (VOID *)pdata->data_read);
    if (status != LX_SUCCESS)
    {
      xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
    }

    /* Verify data integrity.  */
    if (memcmp(pdata->data_written, pdata->data_read, sizeof(pdata->data_written)) != 0)
    {
      /* Error writing the data, notify the background task.  */
      xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
    }

    /* Release the test sector. */
    lx_nand_flash_sector_release(&NANDFlashObj, app_nand_sector);

    /* Log the output for this iteration */
    xSemaphoreTake(printf_mutex, portMAX_DELAY);
    PRINTF("[INFO] Step 5, Task 1, Iteration %d: Read, write, and data integrity check completed successfully for sector %lu\r\n",
           j + 1, app_nand_sector);
    xSemaphoreGive(printf_mutex);

    /* Yield to encourage interleaving */
    taskYIELD();
  }

  /* Sending a signal to main task */
  xEventGroupSetBits(xEventGroup, xBit2);

  /* Notify the background task that the example runs as expected. */
  if (xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_SUCCESS, eSetValueWithOverwrite) == pdFAIL)
  {
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  vTaskDelete(app_task_one);
}

/**
  * @brief perform operations for task two
  * @param args pointer to task context structure
  * @retval None
  */
VOID app_task_entry_two(VOID *args)
{
 TaskHandle_t BackgroundTask = xTaskGetHandle("Background Task");

  UINT status = LX_ERROR;
  srand(xTaskGetTickCount());
  UINT i, j;
  ULONG app_nand_sector;
  task_data_t *pdata = (task_data_t *)args;

  /* Waiting for an event from main task */
  xEventGroupWaitBits(xEventGroup, xBit1, pdTRUE, pdFALSE, TIMEOUT);

  /* Multi-access read/write loop */
  for (j = 0; j < 5; j++)
  {
    app_nand_sector = rand() % 1024;
    for (i = 0; i < sizeof(pdata->data_written); i++)
    {
      pdata->data_written[i] = (UCHAR)(rand() & 0xFF);
    }


    /* Write data to the test sector.  */
    status = lx_nand_flash_sector_write(&NANDFlashObj, app_nand_sector, (VOID *)pdata->data_written);
    if (status != LX_SUCCESS)
    {
      /* Error writing the data, notify the background task.  */
      xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
    }

    /* Read data from the test sector.  */
    status = lx_nand_flash_sector_read(&NANDFlashObj, app_nand_sector, (VOID *)pdata->data_read);
    if (status != LX_SUCCESS)
    {
      /* Error writing the data, notify the background task.  */
      xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
    }

    /* Verify data integrity.  */
    if (memcmp(pdata->data_written, pdata->data_read, sizeof(pdata->data_written)) != 0)
    {
      /* Error writing the data, notify the background task.  */
      xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
    }

    /* Release the test sector. */
    lx_nand_flash_sector_release(&NANDFlashObj, app_nand_sector);

    /* Log the output for this iteration */
    xSemaphoreTake(printf_mutex, portMAX_DELAY);
    PRINTF("[INFO] Step 5, Task 2, Iteration %d: Read, write, and data integrity check completed successfully for sector %lu\r\n",
           j + 1, app_nand_sector);
    xSemaphoreGive(printf_mutex);

    /* Yield to encourage interleaving */
    taskYIELD();
  }

  /* Sending a signal to main task */
  xEventGroupSetBits(xEventGroup, xBit3);

  /* Notify the background task that the example runs as expected. */
  if (xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_SUCCESS, eSetValueWithOverwrite) == pdFAIL)
  {
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  vTaskDelete(app_task_two);
}
