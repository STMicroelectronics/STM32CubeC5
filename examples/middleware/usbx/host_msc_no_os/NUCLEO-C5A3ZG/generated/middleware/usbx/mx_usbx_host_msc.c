/**
  ******************************************************************************
  * @file    mx_usbx_host_msc.c
  * @brief   USBX Host MSC applicative source file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_usbx_license.md file
  * in the same directory as the generated code.
  * If no mx_usbx_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mx_usbx_host_msc.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UCHAR media_buffer[4096];
extern UX_HOST_CLASS_STORAGE_MEDIA *storage_media;
FX_MEDIA media;
UINT is_media_opened = 0;
/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Function implementing usbh_msc_process.
  * @param  none
  * @retval none
  */
UINT usbh_msc_process(VOID)
{
  UINT status = UX_BUSY;

  /* Check if storage media is available */
  if (storage_media)
  {
    if (is_media_opened == 0)
    {
      /* Try to open the media again after formatting */
      status = fx_media_open(&media, "USB_DISK", _ux_host_class_storage_driver_entry, storage_media, media_buffer, sizeof(media_buffer));

      if (status != FX_SUCCESS)
      {
        return UX_ERROR;
      }

      is_media_opened = 1;

      /* Start File operations */
      USBH_USRLOG("\n*** Start Files operations ***\n");

      /* Create a file and check status */
      if (App_File_Create(&media) == UX_SUCCESS)
      {
        USBH_USRLOG("File TEST.TXT Created \n");

        /* Start write File Operation */
        USBH_USRLOG("Write Process ...... \n");

        /* Check status */
        if (App_File_Write(&media) == UX_SUCCESS)
        {
          USBH_USRLOG("Write Process Success \n");

          /* Start Read File Operation and comparison operation */
          USBH_USRLOG("Read Process  ...... \n");

          /* Check Read Operation */
          if (App_File_Read(&media) == UX_SUCCESS)
          {
            USBH_USRLOG("Read Process Success  \n");
            USBH_USRLOG("File Closed \n");
            USBH_USRLOG("*** End Files operations ***\n");
          }
          else
          {
            USBH_ERRLOG("!! Read Process Fail !! \n");
          }
        }
        else
        {
          USBH_ERRLOG("!! Write Process Fail !! ");
        }
      }
      else
      {
        USBH_ERRLOG(" !! Could Not Create TEST.TXT File !! ");
      }

      /* Close the media. */
      status =  fx_media_close(&media);

      /* Check the media close status. */
      if (status != FX_SUCCESS)
      {
        USBH_ERRLOG("!! Close Media Fail !! ");

        /* Error closing the media, call error handler. */
        return UX_ERROR;
      }
    }
  }
  return status;
}

