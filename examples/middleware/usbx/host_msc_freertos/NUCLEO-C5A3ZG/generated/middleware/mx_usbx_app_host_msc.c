/**
  ******************************************************************************
  * @file    app_usbx_host_msc.c
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "mx_usbx_app_host_msc.h"
#include "mx_usbx_app_host.h"
#include "mx_filex_app.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern FX_MEDIA *media;
extern UX_EVENT_FLAGS_GROUP ux_app_EventFlag;
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Function implementing msc_process_thread_entry.
  * @param  none
  * @retval none
  */
VOID msc_process_thread_entry(VOID *args)
{
  ULONG storage_media_flag = 0;

  while (1)
  {
    /* Wait until the requested flag STORAGE_MEDIA is received */
    if (_ux_os_utility_event_flags_get(&ux_app_EventFlag, 1, UX_OR_CLEAR,
                                       &storage_media_flag, UX_WAIT_FOREVER) != UX_SUCCESS)
    {
      /* Report the error */
      USBH_ERRLOG("Error: Failed to get the event flag.\n");

      /* Pause execution for a while */
      vTaskDelay(pdMS_TO_TICKS(10));;
    }

    /* Start file operations once the media is connected */
    if (media != NULL)
    {
      /* Start File operations */
      USBH_USRLOG("\n*** Start Files operations ***\n");

      /* Create a file and check status */
      if (App_File_Create(media) == UX_SUCCESS)
      {
        USBH_USRLOG("File TEST.TXT Created \n");

        /* Start write File Operation */
        USBH_USRLOG("Write Process ...... \n");

        /* Check status */
        if (App_File_Write(media) == UX_SUCCESS)
        {
          USBH_USRLOG("Write Process Success \n");

          /* Start Read File Operation and comparison operation */
          USBH_USRLOG("Read Process  ...... \n");

          /* Check Read Operation */
          if (App_File_Read(media) == UX_SUCCESS)
          {
            USBH_USRLOG("Read Process Success  \n");
            USBH_USRLOG("File Closed \n");
            USBH_USRLOG("*** End Files operations ***\n")
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
    }
    else
    {
      vTaskDelay(pdMS_TO_TICKS(10));
    }
  }
}

