/**
  ******************************************************************************
  * @file    mx_usbx_device.c
  * @brief   USBX Device applicative source file
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
#include "mx_usbx_device.h"
#include "ux_dcd_stm32.h"
#include "mx_usbx_device_hid_mouse.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USB_DEVICE_ENDPOINT_HANDLE hid_mouse_endpoint_app[] =
{
  {
    USBD_HID_MOUSE_EPIN_ADDR,
    USBD_HID_MOUSE_EPIN_TYPE,
    USBD_HID_MOUSE_EPIN_FS_MPS,
    USBD_HID_MOUSE_EPIN_FS_BINTERVAL,
#if USBD_HIGH_SPEED_SUPPORTED == 1U
    USBD_HID_MOUSE_EPIN_HS_MPS,
    USBD_HID_MOUSE_EPIN_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
    NULL,
  }
};

USB_DEVICE_INTERFACE_HANDLE hid_mouse_interface_app[] =
{
  {
    USBD_HID_MOUSE_ITF_NUMBERS,
    USBD_HID_MOUSE_ITF_ATL_SETTING,
    USBD_HID_MOUSE_ITF_EP_NUMBERS,
    USBD_HID_MOUSE_ITF_CLASS,
    USBD_HID_MOUSE_ITF_SUBCLASS,
    USBD_HID_MOUSE_ITF_PROTOCOL,
    USBD_HID_MOUSE_ITF_STR_DESC_IDX,
    0x00U,                /* Used for report descriptor length */
    hid_mouse_endpoint_app,   /* HID mouse endpoint */
    0x00U,                /* No alternate setting interface */
    0x00U,                /* No next alternate setting interface */
  }
};
hal_pcd_handle_t *p_usb_device;
/* Private function prototypes -----------------------------------------------*/
/**
  * @brief  Application USBX Device Initialization.
  */
UINT app_usbx_device_init(VOID)
{
  /** ########## Step 3 ##########
   *  Initialization of USBX HID Device Mouse Class
   */
  UINT status = UX_SUCCESS;
  USB_DESCRIPTOR                    usbd_desc;
  ULONG                             report_desc_len;
  UX_SLAVE_CLASS_HID_PARAMETER      hid_mouse_parameter;
  report_desc_len = usbd_hid_mouse_report_length();
  usb_device_descriptor_register_class(USBD_CLASS_TYPE_HID, hid_mouse_interface_app, &report_desc_len);

  usb_device_descriptor_get_framework(&usbd_desc);

  /* Install the device portion of USBX */
  status = ux_device_stack_initialize(usbd_desc.device_high_speed.framework,
                                      usbd_desc.device_high_speed.framework_length,
                                      usbd_desc.device_full_speed.framework,
                                      usbd_desc.device_full_speed.framework_length,
                                      usbd_desc.string.framework,
                                      usbd_desc.string.framework_length,
                                      usbd_desc.languageid.framework,
                                      usbd_desc.languageid.framework_length,
                                      UX_NULL);

  if (status != UX_SUCCESS)
  {
    return status;
  }
  /* Initialize the hid mouse class parameters for the device */
  hid_mouse_parameter.ux_slave_class_hid_instance_activate           = usbd_hid_mouse_activate;
  hid_mouse_parameter.ux_slave_class_hid_instance_deactivate         = usbd_hid_mouse_deactivate;
  hid_mouse_parameter.ux_device_class_hid_parameter_report_address   = usbd_hid_mouse_report_address();
  hid_mouse_parameter.ux_device_class_hid_parameter_report_length    = usbd_hid_mouse_report_length();
  hid_mouse_parameter.ux_device_class_hid_parameter_report_id        = usbd_hid_mouse_report_id();
  hid_mouse_parameter.ux_device_class_hid_parameter_callback         = usbd_hid_mouse_set_report;
  hid_mouse_parameter.ux_device_class_hid_parameter_get_callback     = usbd_hid_mouse_get_report;

  /* Initialize the device hid Mouse class */
  status = ux_device_stack_class_register(_ux_system_slave_class_hid_name,
                                          ux_device_class_hid_entry,
                                          0x01,
                                          hid_mouse_interface_app->interface_numbers,
                                          &hid_mouse_parameter);

  if (status != UX_SUCCESS)
  {
    return status;
  }

  p_usb_device = mx_example_pcd_gethandle();

  /* initialize the device controller driver */
  status = ux_dcd_stm32_initialize(0, (ULONG)p_usb_device);

  if (status != UX_SUCCESS)
  {
    return status;
  }


  return UX_SUCCESS;
}

/**
  * @brief  Application USBX Device De-Initialization.
  * @retval none
  */
UINT app_usbx_device_deinit(VOID)
{
  UINT status = UX_SUCCESS;


  /* Unregister hid class. */
  status = ux_device_stack_class_unregister(_ux_system_slave_class_hid_name, ux_device_class_hid_entry);
  if (status != UX_SUCCESS)
  {
    return status;
  }

  /* The code below is required for uninstalling the device portion of USBX.  */
  status = ux_device_stack_uninitialize();

  if (status != UX_SUCCESS)
  {
    return status;
  }
  return UX_SUCCESS;
}
/**
  * @brief  Application USBX Device Process.
  * @retval none
  */
UINT app_usbx_device_process(VOID)
{
  UINT return_status = UX_SUCCESS;
  return_status = ux_device_stack_tasks_run();
  if (return_status != UX_STATE_RESET)
  {
    usbd_hid_mouse_process();
  }
  return return_status;
}
