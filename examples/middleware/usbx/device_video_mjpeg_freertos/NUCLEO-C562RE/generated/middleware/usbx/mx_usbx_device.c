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
#include "mx_usbx_device_video.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USB_DEVICE_ENDPOINT_HANDLE video_endpoints_app[] =
{
  {
    USBD_VIDEO_EPIN_ADDR,
    USBD_VIDEO_EPIN_TYPE,
    USBD_VIDEO_EPIN_FS_MPS,
    USBD_VIDEO_EPIN_FS_BINTERVAL,
#if USBD_HIGH_SPEED_SUPPORTED == 1U
    USBD_VIDEO_EPIN_HS_MPS,
    USBD_VIDEO_EPIN_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  }
};

USB_DEVICE_INTERFACE_HANDLE next_alt_setting_app[] =
{
  {
    USBD_VIDEO_VS_AS_ITF_NUMBERS,
    USBD_VIDEO_VS_AS_ITF_ATL_SETTING,
    USBD_VIDEO_VS_AS_ITF_EP_NUMBERS,
    USBD_VIDEO_VS_AS_ITF_CLASS,
    USBD_VIDEO_VS_AS_ITF_SUBCLASS,
    USBD_VIDEO_VS_AS_ITF_PROTOCOL,
    USBD_VIDEO_VS_AS_ITF_STR_DESC_IDX,
    0x00U,
    video_endpoints_app,
    0x00U,
  }
};

/* Interface handles for VIDEO */
USB_DEVICE_INTERFACE_HANDLE video_interface_app[] =
{
  {
    USBD_VIDEO_ITF_NUMBERS,
    USBD_VIDEO_ITF_ATL_SETTING,
    USBD_VIDEO_ITF_EP_NUMBERS,
    USBD_VIDEO_ITF_CLASS,
    USBD_VIDEO_ITF_SUBCLASS,
    USBD_VIDEO_ITF_PROTOCOL,
    USBD_VIDEO_ITF_STR_DESC_IDX,
    0x00U,
    0x00U,
    0x00U,
    0x00U,
  },
  {
    USBD_VIDEO_VS_ITF_NUMBERS,
    USBD_VIDEO_VS_ITF_ATL_SETTING,
    USBD_VIDEO_VS_ITF_EP_NUMBERS,
    USBD_VIDEO_VS_ITF_CLASS,
    USBD_VIDEO_VS_ITF_SUBCLASS,
    USBD_VIDEO_VS_ITF_PROTOCOL,
    USBD_VIDEO_VS_ITF_STR_DESC_IDX,
    0x00U,
    0x00U,
    0x00U,
    next_alt_setting_app,
  }
};

hal_pcd_handle_t *p_usb_device = UX_NULL;
TaskHandle_t ux_device_app_thread;
/* Private function prototypes -----------------------------------------------*/
VOID app_ux_device_thread_entry(void *argument);
VOID usbd_video_thread_entry(VOID *args);
/**
  * @brief  Application USBX Device Initialization.
  */
UINT app_usbx_device_init(VOID)
{
  UINT status = UX_SUCCESS;
  USB_DESCRIPTOR                    usbd_desc;
  ULONG                             report_desc_len;
  UX_DEVICE_CLASS_VIDEO_PARAMETER   video_parameter;
  static UX_DEVICE_CLASS_VIDEO_STREAM_PARAMETER video_stream_parameter[USBD_VIDEO_STREAM_NUMBER] = {0};
  report_desc_len = sizeof(video_parameter);
  usb_device_descriptor_register_class(USBD_CLASS_TYPE_VIDEO, video_interface_app, &report_desc_len);

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
  /* Initialize the video class parameters for the device */
  video_stream_parameter[0].ux_device_class_video_stream_parameter_callbacks.ux_device_class_video_stream_change       = usbd_video_stream_change;
  video_stream_parameter[0].ux_device_class_video_stream_parameter_callbacks.ux_device_class_video_stream_payload_done = usbd_video_stream_payload_done;
  video_stream_parameter[0].ux_device_class_video_stream_parameter_callbacks.ux_device_class_video_stream_request      = usbd_video_stream_request;
  video_stream_parameter[0].ux_device_class_video_stream_parameter_max_payload_buffer_nb                               = USBD_VIDEO_PAYLOAD_BUFFER_NUMBER;
  video_stream_parameter[0].ux_device_class_video_stream_parameter_max_payload_buffer_size                             = usbd_video_stream_get_max_payload_buffer_size();

  /* Set the parameters for Video device */
  video_parameter.ux_device_class_video_parameter_streams_nb                                                           = USBD_VIDEO_STREAM_NUMBER;
  video_parameter.ux_device_class_video_parameter_streams                                                              = video_stream_parameter;
  video_parameter.ux_device_class_video_parameter_callbacks.ux_slave_class_video_instance_activate                     = usbd_video_activate;
  video_parameter.ux_device_class_video_parameter_callbacks.ux_slave_class_video_instance_deactivate                   = usbd_video_deactivate;

  video_stream_parameter[0].ux_device_class_video_stream_parameter_thread_entry      = ux_device_class_video_write_thread_entry;
  video_stream_parameter[0].ux_device_class_video_stream_parameter_thread_stack_size = UX_DEVICE_CLASS_VIDEO_THREAD_STACK_SIZE;

  /* Initialize the device video class */
  status = ux_device_stack_class_register(_ux_system_device_class_video_name,
                                          ux_device_class_video_entry,
                                          0x01,
                                          video_interface_app->interface_numbers,
                                          &video_parameter);

  if (status != UX_SUCCESS)
  {
    return status;
  }
  /* Create the device application main thread */
  if (xTaskCreate(app_ux_device_thread_entry,UX_DEVICE_APP_THREAD_NAME,
                  UX_DEVICE_APP_THREAD_STACK_SIZE, NULL,
                  UX_DEVICE_APP_THREAD_PRIO, &ux_device_app_thread) != pdPASS)
  {
    return UX_THREAD_ERROR;
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

  if (p_usb_device != UX_NULL)
  {
  /* Unregister USB device controller. */
  status = _ux_dcd_stm32_uninitialize(0, (ULONG)p_usb_device);

  if (status != UX_SUCCESS)
  {
    return status;
  }
    p_usb_device = UX_NULL;
  }

  /* Unregister video class. */
  status = ux_device_stack_class_unregister(_ux_system_device_class_video_name, ux_device_class_video_entry);
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
  * @brief  Function implementing app_ux_device_thread_entry.
  * @param  thread_input: User thread input parameter.
  * @retval none
  */
VOID app_ux_device_thread_entry(void *argument)
{
  UINT status = UX_SUCCESS;

  UX_PARAMETER_NOT_USED(argument);

  mx_example_pcd_init();

  /* Initialization of USB device */
  if (p_usb_device == UX_NULL)
  {
    /* Initialization of USB device */
    p_usb_device = mx_usb_drd_fs_device_gethandle();

    /* Initialize the device controller driver */
    status = ux_dcd_stm32_initialize(0, (ULONG)p_usb_device);
    if (status != UX_SUCCESS)
    {
      return;
    }
  }

  /* Suspend the USB device thread */
  vTaskSuspend(ux_device_app_thread);
}
