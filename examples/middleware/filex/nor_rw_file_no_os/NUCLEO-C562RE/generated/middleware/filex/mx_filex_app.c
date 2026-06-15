/**
  ******************************************************************************
  * @file           : mx_filex_app.c
  * @brief          : FileX applicative file.
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
#include "mx_filex_app.h"
/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static UCHAR fx_media_0_buffer[512];

static FX_MEDIA fx_media_0;
static FX_FILE  fx_file_0;
VOID app_task_entry(VOID *args);
/*
* @brief initialize levelx and link low level interface
* @param NULL
*@retval 0 on success -1 otherwise
*/
UINT app_filex_init(void)
{
  UINT status;
  VOID *fx_driver_0_info_ptr;
  MX_FILEX_MEDIA_FORMAT_CONFIG *media_0_config;

  fx_system_initialize();

  /* get the context for the NOR Flash */

  fx_driver_0_info_ptr = mx_filex_driver_0_get_ctx();

  /** ########## Step 2 ##########
    * open the external memory.
    */
  status = fx_media_open(&fx_media_0, "MEDIA_0", mx_filex_driver_0,
                         fx_driver_0_info_ptr, (VOID *)fx_media_0_buffer, sizeof(fx_media_0_buffer));
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

    /* Check the media format status */
    if (status != FX_SUCCESS)
    {
      return status;
    }

    PRINTF("[INFO] Media formatted successfully.\n");

    /* Opens the media driver. */
    status = fx_media_open(&fx_media_0, media_0_config->volume_name, mx_filex_driver_0,
                           fx_driver_0_info_ptr, (VOID *)fx_media_0_buffer, sizeof(fx_media_0_buffer));

    if (status != FX_SUCCESS)
    {
      return status;
    }

    PRINTF("[INFO] Step 2: Media opened successfully.\n");
  }
  else
  {
    PRINTF("[INFO] Step 2: Media opened successfully.\n");
  }

  return status;
}

/**
  * @brief levelx process function
  * @param NULL
  *@retval 0 on success -1 otherwise
  */

UINT app_filex_process(void)
{
  UINT status;
  ULONG bytes_read;
  CHAR data[] = "This is FileX working on STM32";
  CHAR read_buffer[32];

  /** ########## Step 3 ##########
    * Creates a file called STM32.TXT in the root directory.
    */

  PRINTF("[INFO] Step3: Creating STM32.TXT file...\n");

  status = fx_file_create(&fx_media_0, "STM32.TXT");

  if ((status != FX_SUCCESS) && (status != FX_ALREADY_CREATED))
  {
    return status;
  }
  PRINTF("[INFO] STM32.TXT file created successfully.\n");

  PRINTF("[INFO] Step 4: Writing data to STM32.TXT...\n");

  /* Open the test file.  */
  status = fx_file_open(&fx_media_0, &fx_file_0, "STM32.TXT", FX_OPEN_FOR_WRITE);

  /* Check the file open status. */
  if (status != FX_SUCCESS)
  {
    /* Error opening file, call error handler. */
    return status;
  }

  /* Seek to the beginning of the test file. */
  status =  fx_file_seek(&fx_file_0, 0);

  /* Check the file seek status. */
  if (status != FX_SUCCESS)
  {
    /* Error performing file seek, call error handler. */
    return status;
  }

  /** ########## Step 4 ##########
    * Write a data to the test file.
    */
  status =  fx_file_write(&fx_file_0, data, sizeof(data));

  /* Check the file write status. */
  if (status != FX_SUCCESS)
  {
    /* Error writing to a file, call error handler. */
    return status;
  }

  /* Close the test file.  */
  status =  fx_file_close(&fx_file_0);

  /* Check the file close status. */
  if (status != FX_SUCCESS)
  {
    /* Error closing the file, call error handler. */
    return status;
  }

  status = fx_media_flush(&fx_media_0);

  /* Check the media flush  status. */
  if (status != FX_SUCCESS)
  {
    /* Error closing the file, call error handler. */
    return status;
  }

  PRINTF("[INFO] Data written successfully.\n");

  PRINTF("[INFO] Step 5: Reading read_buffer...\n");

  /* Open the test file. */
  status =  fx_file_open(&fx_media_0, &fx_file_0, "STM32.TXT", FX_OPEN_FOR_READ);

  /* Check the file open status. */
  if (status != FX_SUCCESS)
  {
    /* Error opening file, call error handler. */
    return status;
  }

  /* Seek to the beginning of the test file. */
  status = fx_file_seek(&fx_file_0, 0);

  /* Check the file seek status. */
  if (status != FX_SUCCESS)
  {
    /* Error performing file seek, call error handler. */
    return status;
  }

  /** ########## Step 5 ##########
    * Read data from the file into read_buffer.
    */
  status =  fx_file_read(&fx_file_0, read_buffer, sizeof(data), &bytes_read);

  /* Check the file read status.  */
  if ((status != FX_SUCCESS) || (bytes_read != sizeof(data)))
  {
    /* Error reading file, call error handler. */
    return status;
  }


  /* Close the test file.  */
  status = fx_file_close(&fx_file_0);

  /* Check the file close status. */
  if (status != FX_SUCCESS)
  {
    /* Error closing the file, call error handler. */
    return status;
  }

  PRINTF("[INFO] Data reading successfully.\n");

  /* Close the media.  */
  status = fx_media_close(&fx_media_0);

  return status;
}
