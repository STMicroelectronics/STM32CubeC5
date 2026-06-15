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
#include "mx_usbx_device_cdc_acm.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_pcd_handle_t *p_usb_device = UX_NULL;
TaskHandle_t ux_device_app_thread;
TaskHandle_t usbx_cdc_acm_read_thread;
TaskHandle_t usbx_cdc_acm_write_thread;
UX_EVENT_FLAGS_GROUP EventFlag;
/* Endpoint handles CONTROL for CDC_ACM */
USB_DEVICE_ENDPOINT_HANDLE cdc_acm_ctl_endpoints_app[] =
{
  {
    USBD_CDC_ACM_EPNTF_ADDR,
    USBD_CDC_ACM_EPNTF_TYPE,
    USBD_CDC_ACM_EPNTF_FS_MPS,
    USBD_CDC_ACM_EPNTF_FS_BINTERVAL,
#if USBD_HIGH_SPEED_SUPPORTED == 1U
    USBD_CDC_ACM_EPNTF_HS_MPS,
    USBD_CDC_ACM_EPNTF_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  }
};

/* Endpoint handles DATA for CDC_ACM */
USB_DEVICE_ENDPOINT_HANDLE cdc_acm_data_endpoints_app[] =
{
  {
    USBD_CDC_ACM_EPIN_ADDR,
    USBD_CDC_ACM_EPIN_TYPE,
    USBD_CDC_ACM_EPIN_FS_MPS,
    USBD_CDC_ACM_EPIN_FS_BINTERVAL,
#if USBD_HIGH_SPEED_SUPPORTED == 1U
    USBD_CDC_ACM_EPIN_HS_MPS,
    USBD_CDC_ACM_EPIN_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  },
  {
    USBD_CDC_ACM_EPOUT_ADDR,
    USBD_CDC_ACM_EPOUT_TYPE,
    USBD_CDC_ACM_EPOUT_FS_MPS,
    USBD_CDC_ACM_EPOUT_FS_BINTERVAL,
#if USBD_HIGH_SPEED_SUPPORTED == 1U
    USBD_CDC_ACM_EPOUT_HS_MPS,
    USBD_CDC_ACM_EPOUT_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  }
};

/* Interface handles for CDC_ACM */
USB_DEVICE_INTERFACE_HANDLE cdc_acm_interface_app[] =
{
  {
    USBD_CDC_ACM_CTL_NUMBERS,
    USBD_CDC_ACM_CTL_ATL_SETTING,
    USBD_CDC_ACM_CTL_EP_NUMBERS,
    USBD_CDC_ACM_CTL_CLASS,
    USBD_CDC_ACM_CTL_SUBCLASS,
    USBD_CDC_ACM_CTL_PROTOCOL,
    USBD_CDC_ACM_CTL_STR_DESC_IDX,
    0x00U,
    cdc_acm_ctl_endpoints_app,
    0x00U,
  },
  {
    USBD_CDC_ACM_DATA_NUMBERS,
    USBD_CDC_ACM_DATA_ATL_SETTING,
    USBD_CDC_ACM_DATA_EP_NUMBERS,
    USBD_CDC_ACM_DATA_CLASS,
    USBD_CDC_ACM_DATA_SUBCLASS,
    USBD_CDC_ACM_DATA_PROTOCOL,
    USBD_CDC_ACM_DATA_STR_DESC_IDX,
    0x00U,
    cdc_acm_data_endpoints_app,
    0x00U,
  }
};
/* Private function prototypes -----------------------------------------------*/
VOID app_ux_device_thread_entry(void *argument);
/**
  * @brief  Application USBX Device Initialization.
  */
UINT app_usbx_device_init(VOID)
{
  /** ########## Step 3 ##########
    *  Initialization of USBX CDC_ACM Device Class
    */
  UINT status = UX_SUCCESS;
  USB_DESCRIPTOR                    usbd_desc;
  ULONG                             report_desc_len;
  UX_SLAVE_CLASS_CDC_ACM_PARAMETER  cdc_acm_parameter;
  report_desc_len = sizeof(cdc_acm_parameter);
  usb_device_descriptor_register_class(USBD_CLASS_TYPE_CDC_ACM, cdc_acm_interface_app, &report_desc_len);

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
  /* Initialize the cdc acm class parameters for the device */
  cdc_acm_parameter.ux_slave_class_cdc_acm_instance_activate   = usbd_cdc_acm_activate;
  cdc_acm_parameter.ux_slave_class_cdc_acm_instance_deactivate = usbd_cdc_acm_deactivate;
  cdc_acm_parameter.ux_slave_class_cdc_acm_parameter_change    = usbd_cdc_acm_ParameterChange;

  /* Initialize the device hid Mouse class */
  status = ux_device_stack_class_register(_ux_system_slave_class_cdc_acm_name,
                                          ux_device_class_cdc_acm_entry,
                                          0x01,
                                          cdc_acm_interface_app->interface_numbers,
                                          &cdc_acm_parameter);

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
/* Create the usbx_cdc_acm_read_thread_entry thread */
  status = xTaskCreate(usbx_cdc_acm_read_thread_entry, "usbx cdc acm_read thread entry",
                       UX_DEVICE_APP_THREAD_STACK_SIZE, (VOID *) NULL,
                       UX_DEVICE_APP_THREAD_PRIO, &usbx_cdc_acm_read_thread);
  if (status != pdPASS)
  {
    return UX_ERROR;
  }

  /* Create the usbx cdc acm write thread */
  status = xTaskCreate(usbx_cdc_acm_write_thread_entry, "usbx cdc acm write thread entry",
                       UX_DEVICE_APP_THREAD_STACK_SIZE, (VOID *) NULL,
                       UX_DEVICE_APP_THREAD_PRIO, &usbx_cdc_acm_write_thread);
  if (status != pdPASS)
  {
    return UX_ERROR;
  }

  /* Create the event flags group */
  if (ux_utility_event_flags_create(&EventFlag, "Event Flag") != UX_SUCCESS)
  {
    return UX_EVENT_ERROR;
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

  /* Unregister CDC ACM class. */
  status = ux_device_stack_class_unregister(_ux_system_slave_class_cdc_acm_name, ux_device_class_cdc_acm_entry);
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

  /* Initialization of USB device */
  if (p_usb_device == UX_NULL)
  {
  	 mx_example_pcd_init();
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
