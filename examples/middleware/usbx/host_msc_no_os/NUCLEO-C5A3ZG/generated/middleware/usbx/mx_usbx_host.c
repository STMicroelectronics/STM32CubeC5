/**
  ******************************************************************************
  * @file    mx_usbx_host.c
  * @brief   USBX Host applicative source file
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
#include "mx_usbx_host.h"
#include "ux_hcd_stm32.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UX_HOST_CLASS_STORAGE *storage = UX_NULL;
UX_HOST_CLASS_STORAGE_MEDIA *storage_media = UX_NULL;
extern UINT is_media_opened;
extern UX_HOST_CLASS_STORAGE *storage;
extern UX_HOST_CLASS_STORAGE_MEDIA *storage_media;
extern UINT start_Files_operations;
hal_hcd_handle_t *p_usb_host = UX_NULL;
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
  status = ux_host_stack_initialize(ux_host_event_callback);
  if (status != UX_SUCCESS)
  {
    return status;
  }

  /* Register a callback error function */
  ux_utility_error_callback_register(&ux_host_error_callback);
  /* Register storage class. */
  status = ux_host_stack_class_register(_ux_system_host_class_storage_name, ux_host_class_storage_entry);
  if (status != UX_SUCCESS)
  {
    return status;
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
  UINT status = UX_SUCCESS;
  if (p_usb_host != UX_NULL)
  {
  /* Unregister all the USB host controllers available in this system. */
  ux_host_stack_hcd_unregister(_ux_system_host_hcd_stm32_name, 0U,
                               (ULONG)p_usb_host);
    p_usb_host = UX_NULL;
  }

  /* Unregister storage class. */
  status = ux_host_stack_class_unregister(ux_host_class_storage_entry);
  if (status != UX_SUCCESS)
  {
    return status;
  }

  /* The code below is required for uninstalling the host portion of USBX.  */
  status = ux_host_stack_uninitialize();

  if (status != UX_SUCCESS)
  {
    return status;
  }
  return UX_SUCCESS;
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

      /* Check if the current class is Storage */
      if (current_class -> ux_host_class_entry_function == ux_host_class_storage_entry)
      {
        if (storage == UX_NULL)
        {
          /* Get current Storage Instance */
          storage = (UX_HOST_CLASS_STORAGE *)current_instance;

          USBH_USRLOG("USB Mass Storage Device Plugged\n");
          USBH_USRLOG("PID: %#x\n", (UINT)storage -> ux_host_class_storage_device -> ux_device_descriptor.idProduct);
          USBH_USRLOG("VID: %#x\n", (UINT)storage -> ux_host_class_storage_device -> ux_device_descriptor.idVendor);
        }
      }

      break;

    case UX_DEVICE_REMOVAL:

      if ((VOID *)storage == current_instance)
      {
        /* Clear storage media instance & media file */
        storage = UX_NULL;
        storage_media = UX_NULL;
        is_media_opened = 0;
        USBH_USRLOG("USB Mass Storage Device Unplugged\n");
      }

      break;

    case UX_STORAGE_MEDIA_INSERTION:

      storage_media = (UX_HOST_CLASS_STORAGE_MEDIA *)current_instance;
      if (storage_media -> ux_host_class_storage_media_lun != 0)
      {
        storage_media = UX_NULL;
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
      USBH_USRLOG("USB Device Enumeration Failure\n");
      break;

    case  UX_NO_DEVICE_CONNECTED:
      USBH_USRLOG("USB Device disconnected\n");
      break;

    default:
      break;
  }
}
/**
  * @brief  Application USBX Host Process.
  * @retval none
  */
UINT app_usbx_host_process(VOID)
{
  UINT return_status = UX_SUCCESS;
  if (p_usb_host == UX_NULL)
  {
    mx_example_hcd_init();

    p_usb_host = mx_usb_drd_fs_host_gethandle();

    /* Register all the USB host controllers available in this system. */
    return_status = ux_host_stack_hcd_register(_ux_system_host_hcd_stm32_name,
                                               _ux_hcd_stm32_initialize, 0U,
                                               (ULONG)p_usb_host);

    if (return_status != UX_SUCCESS)
    {
      return return_status;
    }
  }
  return_status = ux_host_stack_tasks_run();
  usbh_msc_process();
  return return_status;
}
