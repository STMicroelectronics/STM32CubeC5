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
UX_HOST_CLASS_HID          *hid_instance;
UX_HOST_CLASS_HID_MOUSE    *mouse;
UX_HOST_CLASS_HID_KEYBOARD *keyboard;
static hal_hcd_handle_t *p_usb_host = UX_NULL;
static UINT ux_host_event_callback(ULONG event, UX_HOST_CLASS *current_class, VOID *current_instance);
static VOID ux_host_error_callback(UINT system_level, UINT system_context, UINT error_code);

/* Exported functions --------------------------------------------------------*/

/** ########## Step 3 ##########
  *  Initialization of USBX HID Host Mouse
  *  and Keyboard Class.
  */
/**
  * @brief  Application USBX Host Initialization.
    * @retval status
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
  /* Register HID host class. */
  status = ux_host_stack_class_register(_ux_system_host_class_hid_name, ux_host_class_hid_entry);
  if (status != UX_SUCCESS)
  {
    return status;
  }
  /* Register HID class for Mouse. */
  status = ux_host_class_hid_client_register(_ux_system_host_class_hid_client_mouse_name, ux_host_class_hid_mouse_entry);
  if (status != UX_SUCCESS)
  {
    return status;
  }
  /* Register HID class for Keyboard. */
  status = ux_host_class_hid_client_register(_ux_system_host_class_hid_client_keyboard_name, ux_host_class_hid_keyboard_entry);
  if (status != UX_SUCCESS)
  {
    return status;
  }

  /* Start Application Message */
  USBH_USRLOG("**** USB OTG HID Host **** ");
  USBH_USRLOG("USB Host library started. ");

  /* Wait for Device to be attached */
  USBH_USRLOG("Starting HID Application ");
  USBH_USRLOG("Connect your HID Device ");
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

  /* Unregister HID class. */
  status = ux_host_stack_class_unregister(ux_host_class_hid_entry);
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
static UINT ux_host_event_callback(ULONG event, UX_HOST_CLASS *current_class, VOID *current_instance)
{
  UINT status = UX_SUCCESS;
  UX_HOST_CLASS_HID_CLIENT *client  = (UX_HOST_CLASS_HID_CLIENT *)current_instance;

  switch (event)
  {
    case UX_DEVICE_INSERTION:

      /* Get current Hid Class */
      if (current_class -> ux_host_class_entry_function == ux_host_class_hid_entry)
      {
        if (hid_instance == UX_NULL)
        {
          /* Get current Hid Instance */
          hid_instance = (UX_HOST_CLASS_HID *)current_instance;
        }
      }

      break;

    case UX_DEVICE_REMOVAL:

      /* Free HID Instance */
      if ((VOID *)hid_instance == current_instance)
      {
        hid_instance = UX_NULL;
      }

      break;

    case UX_HID_CLIENT_INSERTION:

      USBH_USRLOG("\nHID Client Plugged");

      /* Check the HID_client if this is a HID keyboard device */
      if (client -> ux_host_class_hid_client_handler == ux_host_class_hid_keyboard_entry)
      {
        /* Get current Hid Client */
        if (keyboard == UX_NULL)
        {
          keyboard = client -> ux_host_class_hid_client_local_instance;

          USBH_USRLOG("HID_Keyboard_Device");
          USBH_USRLOG("PID: %#x", (UINT)keyboard
                      ->ux_host_class_hid_keyboard_hid->ux_host_class_hid_device->ux_device_descriptor.idProduct);
          USBH_USRLOG("VID: %#x", (UINT)keyboard
                      ->ux_host_class_hid_keyboard_hid->ux_host_class_hid_device->ux_device_descriptor.idVendor);
          USBH_USRLOG("USB HID Host Keyboard App...");
          USBH_USRLOG("keyboard is ready...");
        }
      }

      /* Check the HID_client if this is a HID mouse device */
      if (client -> ux_host_class_hid_client_handler == ux_host_class_hid_mouse_entry)
      {
        /* Get current Hid Client */
        if (mouse == UX_NULL)
        {
          mouse = client -> ux_host_class_hid_client_local_instance;

          USBH_USRLOG("HID_Mouse_Device");
          USBH_USRLOG("PID: %#x", (UINT)mouse
                      ->ux_host_class_hid_mouse_hid->ux_host_class_hid_device->ux_device_descriptor.idProduct);
          USBH_USRLOG("VID: %#x", (UINT)mouse
                      ->ux_host_class_hid_mouse_hid->ux_host_class_hid_device->ux_device_descriptor.idVendor);
          USBH_USRLOG("USB HID Host Mouse App...");
          USBH_USRLOG("Mouse is ready...");
        }
      }

      break;

    case UX_HID_CLIENT_REMOVAL:

      /* Clear hid client local instance */
      if ((VOID *)keyboard == client -> ux_host_class_hid_client_local_instance)
      {
        /* Clear hid keyboard instance */
        keyboard = UX_NULL;

        USBH_USRLOG("\nHID Client Keyboard Unplugged");
      }

      if ((VOID *)mouse == client -> ux_host_class_hid_client_local_instance)
      {
        /* Clear hid mouse instance */
        mouse = UX_NULL;

        USBH_USRLOG("\nHID Client Mouse Unplugged");
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
static VOID ux_host_error_callback(UINT system_level, UINT system_context, UINT error_code)
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
  ux_host_stack_tasks_run();
  usbh_hid_mouse_process();
  usbh_hid_keyboard_process();
  return return_status;
}
