/**
  ******************************************************************************
  * @file    app_usbx_host.c
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
#include "mx_usbx_app_host.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define UX_HOST_APP_THREAD_STACK_SIZE   1024*2
#define APP_DEFAULT_PRIORITY       30
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UX_HOST_CLASS_STORAGE *storage;
UX_HOST_CLASS_STORAGE_MEDIA *storage_media;
FX_MEDIA *media;
UX_EVENT_FLAGS_GROUP ux_app_EventFlag;

TaskHandle_t ux_host_app_thread_handle;
TaskHandle_t msc_process_thread_handle;
/* Exported variables --------------------------------------------------------*/
hal_hcd_handle_t *p_usb_host;
/* Private function prototypes -----------------------------------------------*/
static UINT ux_host_event_callback(ULONG event, UX_HOST_CLASS *current_class, VOID *current_instance);
static VOID ux_host_error_callback(UINT system_level, UINT system_context, UINT error_code);
/* Exported functions --------------------------------------------------------*/
/** ########## Step 3 ##########
  *  Initialization of USBX MSC Host
  */
/**
  * @brief  Application USBX Host Initialization.
  * @param  memory_ptr: memory pointer
  */
UINT app_usbx_host_init(VOID)
{
  UINT status = UX_SUCCESS;

  /* Install the host portion of USBX */
  if (ux_host_stack_initialize(ux_host_event_callback) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Register a callback error function */
  ux_utility_error_callback_register(&ux_host_error_callback);

  /* Initialize the host msc class */
  if (ux_host_stack_class_register(_ux_system_host_class_storage_name,
                                   ux_host_class_storage_entry) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Get USB Host handle */
  p_usb_host = mx_example_hcd_gethandle();

  /* Register all the USB host controllers available in this system. */
  ux_host_stack_hcd_register(_ux_system_host_hcd_stm32_name,
                             _ux_hcd_stm32_initialize, 0U,
                             (ULONG)p_usb_host);

  /* Create the storage applicative process thread */
  status = xTaskCreate(msc_process_thread_entry, "msc process thread entry",
                       UX_HOST_APP_THREAD_STACK_SIZE, NULL,
                       APP_DEFAULT_PRIORITY, &msc_process_thread_handle);
  if (status != pdPASS)
  {
    return UX_ERROR;
  }

  /* Create the event flags group */
  status = _ux_os_utility_event_flags_create(&ux_app_EventFlag, "Event Flag");
  if (status != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Start Application Message */
  USBH_USRLOG("**** USB OTG MSC Host **** \n");
  USBH_USRLOG("USB Host library started.\n");

  /* Wait for Device to be attached */
  USBH_USRLOG("Starting MSC Application");
  USBH_USRLOG("Connect your MSC Device");

  return UX_SUCCESS;
}

/**
  * @brief  Application USBX Host De-Initialization.
  * @retval none
  */
UINT app_usbx_host_deinit(VOID)
{
  UINT ret = UX_SUCCESS;
  return ret;
}

/**
  * @brief  ux_host_event_callback
  *         This callback is invoked to notify application of instance changes.
  * @param  event: event code.
  * @param  current_class: Pointer to class.
  * @param  current_instance: Pointer to class instance.
  * @retval status
  */
UINT ux_host_event_callback(ULONG event, UX_HOST_CLASS *current_class, VOID *current_instance)
{
  UINT status = UX_SUCCESS;

  switch (event)
  {
    case UX_DEVICE_INSERTION:

      /* Get current Storage Class */
      if (current_class -> ux_host_class_entry_function == ux_host_class_storage_entry)
      {
        if (storage == UX_NULL)
        {
          /* Get current Storage Instance */
          storage = (UX_HOST_CLASS_STORAGE *)current_instance;

          USBH_USRLOG("\nUSB Mass Storage Device Plugged");
          USBH_USRLOG("PID: %#x ", (UINT)storage -> ux_host_class_storage_device -> ux_device_descriptor.idProduct);
          USBH_USRLOG("VID: %#x ", (UINT)storage -> ux_host_class_storage_device -> ux_device_descriptor.idVendor);

          /* Get the storage media */
          storage_media = (UX_HOST_CLASS_STORAGE_MEDIA *)current_class -> ux_host_class_media;

          if (storage_media -> ux_host_class_storage_media_lun != 0)
          {
            storage_media = UX_NULL;
          }
          else
          {
            /* Get the media file */
            media = &storage_media->ux_host_class_storage_media;
          }

          /* Check the storage class state */
          if (storage -> ux_host_class_storage_state ==  UX_HOST_CLASS_INSTANCE_LIVE)
          {
            /* Set STORAGE_MEDIA flag */
            if (_ux_os_utility_event_flags_set(&ux_app_EventFlag, 1, UX_OR) != UX_SUCCESS)
            {
              /* Report the error */
              USBH_ERRLOG("Error: Failed to set the event flag.\n");

              /* Pause execution for a while */
              vTaskDelay(pdMS_TO_TICKS(10));;
            }
          }
        }
      }

      break;

    case UX_DEVICE_REMOVAL:

      if ((VOID *)storage == current_instance)
      {
        /* Clear storage media instance & media file */
        storage = UX_NULL;
        storage_media = UX_NULL;
        media = UX_NULL;

        USBH_USRLOG("\nUSB Mass Storage Device Unplugged");
      }

      break;

    case UX_DEVICE_CONNECTION:
      break;

    case UX_DEVICE_DISCONNECTION:
      break;

    default:
      break;
  }

  return status;
}

/**
  * @brief ux_host_error_callback
  *         This callback is invoked to notify application of error changes.
  * @param  system_level: system level parameter.
  * @param  system_context: system context code.
  * @param  error_code: error event code.
  * @retval Status
  */
VOID ux_host_error_callback(UINT system_level, UINT system_context, UINT error_code)
{
  switch (error_code)
  {
    case UX_DEVICE_ENUMERATION_FAILURE:
      USBH_USRLOG("USB Device Enumeration Failure");
      break;

    case  UX_NO_DEVICE_CONNECTED:
      USBH_USRLOG("USB Device disconnected");
      break;

    default:
      break;
  }
}
