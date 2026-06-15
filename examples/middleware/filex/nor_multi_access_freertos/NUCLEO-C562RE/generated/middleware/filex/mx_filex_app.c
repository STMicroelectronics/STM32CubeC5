/**
  ******************************************************************************
  * @file    mx_filex_app.c
  * @brief   FileX applicative file
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
#include "mx_filex_app.h"
/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
extern TaskHandle_t BackgroundTask;
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Define FileX global data structures.  */
static FX_MEDIA fx_media_0;
static FX_FILE  fx_file_one;
static FX_FILE  fx_file_two;
static UCHAR fx_media_0_buffer[512];

/* The applicative tasks demonstrating the nor_multi_access_freertos usage */
TaskHandle_t main_task;
TaskHandle_t app_task_one;
TaskHandle_t app_task_two;

/* Declaration of the Event Group */
EventGroupHandle_t xEventGroup;

/* Event Flags Bits */
const EventBits_t xBit1 = (1 << 0);
const EventBits_t xBit2 = (1 << 1);
const EventBits_t xBit3 = (1 << 2);

/* Private function prototypes -----------------------------------------------*/
VOID main_task_entry(VOID *args);
VOID app_task_entry_one(VOID *args);
VOID app_task_entry_two(VOID *args);

/** @brief  Application FileX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
/** ########## Step 1 ##########
  * Creates a event and 3 tasks with the same priority.
  */
UINT app_filex_init(VOID)
{
  UINT ret;

  /* Create the 1st concurrent thread.  */
  ret = xTaskCreate(app_task_entry_one, "App Task one", APP_DEFAULT_STACK_SIZE, (VOID *) NULL,
                    APP_TASK_PRIORITY, &app_task_one);
  if (ret != pdPASS)
  {
    return FX_PTR_ERROR;
  }
  /* Create the 2nd concurrent thread */
  ret = xTaskCreate(app_task_entry_two, "App Task two", APP_DEFAULT_STACK_SIZE, (VOID *) NULL,
                    APP_TASK_PRIORITY, &app_task_two);
  if (ret != pdPASS)
  {
    return FX_PTR_ERROR;
  }

  /* Create the main thread.  */
  ret = xTaskCreate(main_task_entry, "Main Task ", APP_DEFAULT_STACK_SIZE, (VOID *) NULL,
                    MAIN_TASK_PRIORITY, &main_task);
  if (ret != pdPASS)
  {
    return FX_PTR_ERROR;
  }

  /* Creation of the Event Group */
  xEventGroup = xEventGroupCreate();

  PRINTF("[INFO] Step 1: tasks and event created.\n");

  /* Initializes FileX.*/
  fx_system_initialize();

  return FX_SUCCESS;
}

/** ########## Step 3 ##########
  * Use 'task entry' to initialize the nor flash driver and open it as a FileX Media.
  */
VOID main_task_entry(VOID *args)
{
  UINT status = FX_SUCCESS;
  VOID *fx_driver_0_info_ptr;
  MX_FILEX_MEDIA_FORMAT_CONFIG *media_0_config;

  /* get the context for the NOR Flash */
  fx_driver_0_info_ptr = mx_filex_driver_0_get_ctx();

  /* Opens the fx_media_0 driver. */

  status = fx_media_open(&fx_media_0, "STM32_fx_media_0", mx_filex_driver_0, fx_driver_0_info_ptr,
                         (VOID *) fx_media_0_buffer, sizeof(fx_media_0_buffer));

  if (status != FX_SUCCESS)
  {
    media_0_config = mx_filex_driver_0_get_media_format_config();

    /* Format the media */
    status = fx_media_format(&fx_media_0, mx_filex_driver_0, fx_driver_0_info_ptr, (VOID *)fx_media_0_buffer,
                             sizeof(fx_media_0_buffer), media_0_config->volume_name, media_0_config->number_of_fats,
                             media_0_config->directory_entries, media_0_config->hidden_sectors,
                             media_0_config->total_sectors, media_0_config->bytes_per_sector,
                             media_0_config->sectors_per_cluster, media_0_config->heads,
                             media_0_config->sectors_per_track);

    if (status != FX_SUCCESS)
    {
      xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
    }

    PRINTF("[INFO] Media formatted successfully.\n");

    status = fx_media_open(&fx_media_0, "STM32_fx_media_0", mx_filex_driver_0, fx_driver_0_info_ptr,
                           (VOID *) fx_media_0_buffer, sizeof(fx_media_0_buffer));

    if (status != FX_SUCCESS)
    {
      xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
    }

  }

  PRINTF("[INFO] Step 3: Media opened successfully.\n");

  xEventGroupSetBits(xEventGroup, xBit1);

  if ((xEventGroupWaitBits(xEventGroup, xBit2, pdTRUE, pdTRUE, TIMEOUT) & xBit2) == xBit2)
  {
    PRINTF("[INFO] Step 4: STM32_FILE1.TXT file creates and reads data.\n");
  }
  else
  {
    /* xBit2 was not set. Handle the error. */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  if ((xEventGroupWaitBits(xEventGroup, xBit3, pdTRUE, pdTRUE, TIMEOUT) & xBit3) == xBit3)
  {
    PRINTF("[INFO] Step 5: STM32_FILE2.TXT file creates and reads data.\n");
  }
  else
  {
    /* xBit3 was not set. Handle the error. */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  /* Close the media.  */
  status = fx_media_close(&fx_media_0);

  /* Check the media close status.  */
  if (status != FX_SUCCESS)
  {
    /* Error closing the media, notify the background task.  */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }
  /** ########## Step 6 ##########
    * Complete the scenario by ensuring that the media is properly closed.
    */

  vTaskDelete(app_task_one);
  vTaskDelete(app_task_two);


  while (1)
  {
    /* Notify the background task that the example runs as expected. */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_SUCCESS, eSetValueWithOverwrite);
  }
}

/** ########## Step 4 ##########
  * Use 'task one' to perform create, write, and read operations on the file fx_file_one.
  */
VOID app_task_entry_one(VOID *args)
{
  /* Waiting for an event from main task */
  xEventGroupWaitBits(xEventGroup, xBit1, pdTRUE, pdFALSE, TIMEOUT);

  UINT status;
  ULONG bytes_read;
  CHAR read_buffer[32];
  CHAR data[] = "This is FileX working concurrently on STM32";

  /* Create a file called STM32_FILE1.TXT in the root directory.  */
  status = fx_file_create(&fx_media_0, "STM32_FILE1.TXT");

  /* Check the create status.  */
  if (status != FX_SUCCESS)
  {
    /* Check for an already created status. This is expected on the
    second pass of this loop!  */
    if (status != FX_ALREADY_CREATED)
    {
      /* Create error, notify the background task.  */
      xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
    }
  }

  /* Open the test file.  */
  status = fx_file_open(&fx_media_0, &fx_file_one, "STM32_FILE1.TXT", FX_OPEN_FOR_WRITE);

  /* Check the file open status.  */
  if (status != FX_SUCCESS)
  {
    /* Error opening file, notify the background task.  */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  /* Seek to the beginning of the test file.  */
  status = fx_file_seek(&fx_file_one, 0);

  /* Check the file seek status.  */
  if (status != FX_SUCCESS)
  {
    /* Error performing file seek, notify the background task.  */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  /* Write a string to the test file.  */
  status = fx_file_write(&fx_file_one, data, sizeof(data));

  /* Check the file write status.  */
  if (status != FX_SUCCESS)
  {
    /* Error writing to a file, notify the background task.  */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  /* Close the test file.  */
  status = fx_file_close(&fx_file_one);

  /* Check the file close status.  */
  if (status != FX_SUCCESS)
  {
    /* Error closing the file, notify the background task.  */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  status = fx_media_flush(&fx_media_0);

  /* Check the media flush  status.  */
  if (status != FX_SUCCESS)
  {
    /* Error closing the file, notify the background task.  */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  /* Open the test file.  */
  status = fx_file_open(&fx_media_0, &fx_file_one, "STM32_FILE1.TXT", FX_OPEN_FOR_READ);

  /* Check the file open status.  */
  if (status != FX_SUCCESS)
  {
    /* Error opening file, notify the background task.  */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  /* Seek to the beginning of the test file.  */
  status = fx_file_seek(&fx_file_one, 0);

  /* Check the file seek status.  */
  if (status != FX_SUCCESS)
  {
    /* Error performing file seek, notify the background task.  */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  /* Read the content of the test file.  */
  status = fx_file_read(&fx_file_one, read_buffer, sizeof(data), &bytes_read);

  /* Check the file read status.  */
  if ((status != FX_SUCCESS) || (bytes_read != sizeof(data)))
  {
    /* Error reading file, notify the background task.  */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  /* Close the test file.  */
  status = fx_file_close(&fx_file_one);

  /* Check the file close status.  */
  if (status != FX_SUCCESS)
  {
    /* Error closing the file, notify the background task.  */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  /* Sending a signal to main task */
  xEventGroupSetBits(xEventGroup, xBit2);

  /* Notify the background task that the example runs as expected. */
  if (xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_SUCCESS, eSetValueWithOverwrite) == pdFAIL)
  {
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  while (1)
  {
    vTaskDelay(100);
  }
}

/** ########## Step 5 ##########
  * Use 'task two' to execute creation, writing, and reading operations for the file fx_file_two.
  */
VOID app_task_entry_two(VOID *args)
{
  /* Waiting for an event from main task */
  xEventGroupWaitBits(xEventGroup, xBit1, pdTRUE, pdFALSE, TIMEOUT);

  UINT status;
  ULONG bytes_read;
  CHAR read_buffer[32];
  CHAR data[] = "This is FileX working concurrently on STM32";

  /* Create a file called STM32_FILE2.TXT in the root directory.  */
  status = fx_file_create(&fx_media_0, "STM32_FILE2.TXT");

  /* Check the create status.  */
  if (status != FX_SUCCESS)
  {
    /* Check for an already created status. This is expected on the
    second pass of this loop!  */
    if (status != FX_ALREADY_CREATED)
    {
      /* Create error, notify the background task.  */
      xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
    }
  }

  /* Open the test file.  */
  status = fx_file_open(&fx_media_0, &fx_file_two, "STM32_FILE2.TXT", FX_OPEN_FOR_WRITE);

  /* Check the file open status.  */
  if (status != FX_SUCCESS)
  {
    /* Error opening file, notify the background task.  */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  /* Seek to the beginning of the test file.  */
  status = fx_file_seek(&fx_file_two, 0);

  /* Check the file seek status.  */
  if (status != FX_SUCCESS)
  {
    /* Error performing file seek, notify the background task.  */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  /* Write a string to the test file.  */
  status = fx_file_write(&fx_file_two, data, sizeof(data));

  /* Check the file write status.  */
  if (status != FX_SUCCESS)
  {
    /* Error writing to a file, notify the background task.  */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  /* Close the test file.  */
  status = fx_file_close(&fx_file_two);

  /* Check the file close status.  */
  if (status != FX_SUCCESS)
  {
    /* Error closing the file, notify the background task.  */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  status = fx_media_flush(&fx_media_0);

  /* Check the media flush  status.  */
  if (status != FX_SUCCESS)
  {
    /* Error closing the file, notify the background task.  */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  /* Open the test file.  */
  status = fx_file_open(&fx_media_0, &fx_file_two, "STM32_FILE2.TXT", FX_OPEN_FOR_READ);

  /* Check the file open status.  */
  if (status != FX_SUCCESS)
  {
    /* Error opening file, notify the background task.  */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  /* Seek to the beginning of the test file.  */
  status = fx_file_seek(&fx_file_two, 0);

  /* Check the file seek status.  */
  if (status != FX_SUCCESS)
  {
    /* Error performing file seek, notify the background task.  */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  /* Read the content of the test file.  */
  status = fx_file_read(&fx_file_two, read_buffer, sizeof(data), &bytes_read);

  /* Check the file read status.  */
  if ((status != FX_SUCCESS) || (bytes_read != sizeof(data)))
  {
    /* Error reading file, notify the background task.  */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  /* Close the test file.  */
  status = fx_file_close(&fx_file_two);

  /* Check the file close status.  */
  if (status != FX_SUCCESS)
  {
    /* Error closing the file, notify the background task.  */
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  /* Sending a signal to main task */
  xEventGroupSetBits(xEventGroup, xBit3);

  /* Notify the background task that the example runs as expected. */
  if (xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_SUCCESS, eSetValueWithOverwrite) == pdFAIL)
  {
    xTaskNotify(BackgroundTask, NOTIFICATION_VALUE_ERROR, eSetValueWithOverwrite);
  }

  while (1)
  {
    vTaskDelay(100);
  }
}
