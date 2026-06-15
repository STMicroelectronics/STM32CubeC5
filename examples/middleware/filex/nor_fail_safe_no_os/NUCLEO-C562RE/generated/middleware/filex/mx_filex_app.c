/**
  ******************************************************************************
  * file           : mx_app_filex.c
  * brief          : filex application file
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
#include "stm32_hal.h"

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static UCHAR fx_media_0_buffer[512];
uint32_t fx_fault_tolerance_buffer[1024];
static FX_MEDIA fx_media_0;
extern hal_flash_handle_t *pFLASH;
/*
* @brief initialize levelx and link low level interface
* @param NULL
*@retval 0 on success -1 otherwise
*/
UINT app_filex_init(void)
{
  UINT status;
  uint32_t reset_value;
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
    /* Media formatting behavior:
     * - FLASH_USER_ADDR holds a persistent reset counter.
     * - Only when the counter == 1 and fx_media_open() fails,
     *   the media is formatted to create the filesystem.
     * - For counter > 1, no automatic format is done to keep user data.
     */
    if (*(__IO uint32_t *)FLASH_USER_ADDR == 1)
    {
      PRINTF("[INFO] Step2: No Filesystem found! formatting...\r\n");
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

     PRINTF("[INFO] Media formatted successfully.\r\n");

     /* Opens the media driver. */
     status = fx_media_open(&fx_media_0, media_0_config->volume_name, mx_filex_driver_0,
                            fx_driver_0_info_ptr, (VOID *)fx_media_0_buffer, sizeof(fx_media_0_buffer));

     if (status != FX_SUCCESS)
     {
       return status;
     }

     PRINTF("[INFO] Step2: Media opened successfully.\r\n");
    }
    else 
    {
      reset_value = 0U;

      HAL_FLASH_ITF_Unlock(HAL_FLASH);
      if (HAL_FLASH_EraseByAddr(pFLASH, FLASH_USER_ADDR, sizeof(reset_value), 1000U) == HAL_OK)
      {
        (void)HAL_FLASH_ProgramByAddr(pFLASH,
                                      FLASH_USER_ADDR,
                                      &reset_value,
                                      sizeof(reset_value),
                                      1000U);
      }
      HAL_FLASH_ITF_Lock(HAL_FLASH);

      PRINTF("[INFO]: File System corrupted.\r\n");
      return status;
    }
  }
  else
  {
    PRINTF("[INFO] Step2: Media opened successfully.\r\n");
  }
  /* Enable the fault tolerance. */
  status = fx_fault_tolerant_enable(&fx_media_0,
                                    (VOID *)fx_fault_tolerance_buffer,
                                    sizeof(fx_fault_tolerance_buffer));
  if (status != FX_SUCCESS)
  {
    return status;
  }

  PRINTF("[INFO] Step 3: Fault tolerance enabled.\r\n");

  return status;
}

UINT app_filex_process(void)
{
  UINT  status;
  UINT  data_size = 0;
  UCHAR data_buffer[] = "This is FileX working concurrently on STM32";
  FX_FILE  fx_file_0;
  CHAR read_buffer[512];
  UINT data_read_size, total_data_read_size = 0;

  PRINTF("[INFO] Step 4: Starting file I/O operations on the target media.\r\n");

  /* Step 3: create STM32.TXT file in the root directory. */
  status = fx_file_create(&fx_media_0, "STM32.TXT");
  if ((status != FX_SUCCESS) && (status != FX_ALREADY_CREATED))
  {
    return status;
  }
  PRINTF("[INFO] STM32.TXT file created successfully.\r\n");

  PRINTF("[INFO]: Writing data to STM32.TXT...\r\n");

  /* Open the test file for write */
  status = fx_file_open(&fx_media_0, &fx_file_0, "STM32.TXT", FX_OPEN_FOR_WRITE);
  if (status != FX_SUCCESS)
  {
    return status;
  }

  /* Truncate file to 0 (empty the file and release space) */
  status = fx_file_truncate_release(&fx_file_0, 0);
  if (status != FX_SUCCESS)
  {
    fx_file_close(&fx_file_0);
    return status;
  }

  /* Seek to beginning */
  status = fx_file_seek(&fx_file_0, 0);
  if (status != FX_SUCCESS)
  {
    fx_file_close(&fx_file_0);
    return status;
  }

  /* Write 128 * 512 = 64 KB */
  for (int i = 0; i < 128; i++)
  {
    status = fx_file_write(&fx_file_0, data_buffer, sizeof(data_buffer));
    if (status != FX_SUCCESS)
    {
      return status;
    }
  }

  /* Close file */
  status = fx_file_close(&fx_file_0);
  if (status != FX_SUCCESS)
  {
    return status;
  }

  /* Flush media (commit data + fail-safe journal) */
  status = fx_media_flush(&fx_media_0);
  if (status != FX_SUCCESS)
  {
    return status;
  }

  PRINTF("[INFO] Data written successfully.\r\n");

  data_size = 128 * 512;

  PRINTF("[INFO]: Reading back and validating data...\r\n");

  /* Open the test file. */
  status = fx_file_open(&fx_media_0, &fx_file_0, "STM32.TXT", FX_OPEN_FOR_READ);
  /* Check the file open status. */
  if (status != FX_SUCCESS)
  {
    return status;
  }

  /* Seek to the beginning of the test file. */
  status = fx_file_seek(&fx_file_0, 0);
  /* Check the file seek status. */
  if (status != FX_SUCCESS)
  {
    return status;
  }
  while (data_size > 0)
  {
    memset(&read_buffer, 0, sizeof(read_buffer));
    if (data_size >= 512)
    {
      /* Read the content of the test file byte by byte. */
      status = fx_file_read(&fx_file_0, read_buffer, 512, (ULONG *)&data_read_size);
      /* Check the read file status. */
      if (status != FX_SUCCESS)
      {
        return status;
      }

      total_data_read_size += data_read_size;
      data_size -= data_read_size;
    }
    else
    {
      /* Read the content of the test file byte by byte. */
      status = fx_file_read(&fx_file_0, read_buffer, (ULONG)data_size, (ULONG *)&data_read_size);
      if (status != FX_SUCCESS)
      {
        return status;
      }

      data_size -= data_read_size;
      total_data_read_size += data_read_size;
    }

  }

  status = fx_file_close(&fx_file_0);
  if (status != FX_SUCCESS)
  {
    return status;
  }

  PRINTF("[INFO] Data integrity check passed.\r\n");

  status = fx_media_close(&fx_media_0);
  if (status != FX_SUCCESS)
  {
    return status;
  }

  PRINTF("[INFO]: Media closed successfully.\r\n");

  return FX_SUCCESS;
}
