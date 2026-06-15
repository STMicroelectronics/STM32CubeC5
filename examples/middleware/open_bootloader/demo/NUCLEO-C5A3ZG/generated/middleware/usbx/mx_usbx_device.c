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
#include "mx_usbx_device_dfu.h"

#include "openbl_usb_dfu_descriptors.h"
#include "openbl_usb_dfu.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_pcd_handle_t *p_usb_device;
/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Application USBX Device Initialization.
  */
UINT app_usbx_device_init(VOID)
{
  UINT status = UX_SUCCESS;
  USB_DESCRIPTOR usbd_desc;
  UX_SLAVE_CLASS_DFU_PARAMETER dfu_parameter;

  UCHAR *string_framework;
  ULONG string_framework_length;

  status = usb_device_descriptor_register_class(USBD_CLASS_TYPE_DFU, dfu_interface, UX_NULL);
  if (status != UX_SUCCESS)
  {
    return status;
  }

  usb_device_descriptor_get_framework(&usbd_desc);

  /* Get String Framework and get the length */
  string_framework = OPENBL_USB_Desc_GetStringFramework(&string_framework_length);

  status = ux_device_stack_initialize(usbd_desc.device_high_speed.framework,
                                      usbd_desc.device_high_speed.framework_length,
                                      usbd_desc.device_full_speed.framework,
                                      usbd_desc.device_full_speed.framework_length,
                                      string_framework,
                                      string_framework_length,
                                      usbd_desc.languageid.framework,
                                      usbd_desc.languageid.framework_length,
                                      OPENBL_USB_DFU_ConnectionCallback);
  if (status != UX_SUCCESS)
  {
    return status;
  }

  ux_utility_memory_set(&dfu_parameter, 0, sizeof(UX_SLAVE_CLASS_DFU_PARAMETER));

  dfu_parameter.ux_slave_class_dfu_parameter_will_detach         = UX_FALSE;
  dfu_parameter.ux_slave_class_dfu_parameter_capabilities        = UX_SLAVE_CLASS_DFU_CAPABILITY_MANIFESTATION_TOLERANT |
                                                                   UX_SLAVE_CLASS_DFU_CAPABILITY_CAN_UPLOAD |
                                                                   UX_SLAVE_CLASS_DFU_CAPABILITY_CAN_DOWNLOAD;
  dfu_parameter.ux_slave_class_dfu_parameter_instance_activate   = OPENBL_USB_DFU_Activate;
  dfu_parameter.ux_slave_class_dfu_parameter_instance_deactivate = OPENBL_USB_DFU_Deactivate;
  dfu_parameter.ux_slave_class_dfu_parameter_read                = OPENBL_USB_DFU_Upload;
  dfu_parameter.ux_slave_class_dfu_parameter_write               = OPENBL_USB_DFU_Download;
  dfu_parameter.ux_slave_class_dfu_parameter_get_status          = OPENBL_USB_DFU_GetStatus;
  dfu_parameter.ux_slave_class_dfu_parameter_notify              = OPENBL_USB_DFU_Notify;

#ifdef UX_DEVICE_CLASS_DFU_CUSTOM_REQUEST_ENABLE
  dfu_parameter.ux_device_class_dfu_parameter_custom_request     = OPENBL_USB_FDU_Leave;
#endif /* UX_DEVICE_CLASS_DFU_CUSTOM_REQUEST_ENABLE */

#if USBD_HIGH_SPEED_SUPPORTED == 0U
  dfu_parameter.ux_slave_class_dfu_parameter_framework             = usbd_desc.device_full_speed.framework;
  dfu_parameter.ux_slave_class_dfu_parameter_framework_length      = usbd_desc.device_full_speed.framework_length;
#else
  dfu_parameter.ux_slave_class_dfu_parameter_framework             = usbd_desc.device_high_speed.framework;
  dfu_parameter.ux_slave_class_dfu_parameter_framework_length      = usbd_desc.device_high_speed.framework_length;
#endif /* USBD_HIGH_SPEED_SUPPORTED */

  status = ux_device_stack_class_register(_ux_system_slave_class_dfu_name,
                                          ux_device_class_dfu_entry,
                                          0x01U,
                                          dfu_interface->interface_numbers,
                                          &dfu_parameter);
  if (status != UX_SUCCESS)
  {
    return status;
  }

  p_usb_device = mx_usb_drd_fs_device_gethandle();

  status = ux_dcd_stm32_initialize(0, (ULONG)p_usb_device);

  if (status != UX_SUCCESS)
  {
    return status;
  }

  return status;
}

/**
  * @brief  Application USBX Device De-Initialization.
  * @retval none
  */
UINT app_usbx_device_deinit(VOID)
{
  UINT status = UX_SUCCESS;

  /* Unregister USB device controller. */
  status = _ux_dcd_stm32_uninitialize(0, (ULONG)&p_usb_device);

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
  return return_status;
}
