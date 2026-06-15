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
#include "mx_usbx_device_ccid.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_pcd_handle_t *p_usb_device = UX_NULL;
extern UX_DEVICE_CLASS_CCID_HANDLES usbd_ccid_handles;
extern ULONG usbd_ccid_clocks[];
extern ULONG usbd_ccid_data_rates[];
TaskHandle_t ux_device_app_thread;
TaskHandle_t usbd_ccid_thread;

USB_DEVICE_ENDPOINT_HANDLE ccid_endpoints_app[] =
{
    {
    USBD_CCID_EPOUT_ADDR,
    USBD_CCID_EPOUT_TYPE,
    USBD_CCID_EPOUT_FS_MPS,
    USBD_CCID_EPOUT_FS_BINTERVAL,
#if USBD_HIGH_SPEED_SUPPORTED == 1U
    USBD_CCID_EPOUT_HS_MPS,
    USBD_CCID_EPOUT_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  },
    {
    USBD_CCID_EPIN_ADDR,
    USBD_CCID_EPIN_TYPE,
    USBD_CCID_EPIN_FS_MPS,
    USBD_CCID_EPIN_FS_BINTERVAL,
#if USBD_HIGH_SPEED_SUPPORTED == 1U
    USBD_CCID_EPIN_HS_MPS,
    USBD_CCID_EPIN_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  },
  {
    USBD_CCID_EPCTL_ADDR,
    USBD_CCID_EPCTL_TYPE,
    USBD_CCID_EPCTL_FS_MPS,
    USBD_CCID_EPCTL_FS_BINTERVAL,
#if USBD_HIGH_SPEED_SUPPORTED == 1U
    USBD_CCID_EPCTL_HS_MPS,
    USBD_CCID_EPCTL_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  }
};

USB_DEVICE_INTERFACE_HANDLE ccid_interface_app[] =
{
  {
    USBD_CCID_ITF_NUMBERS,
    USBD_CCID_ITF_ATL_SETTING,
    USBD_CCID_ITF_EP_NUMBERS,
    USBD_CCID_ITF_CLASS,
    USBD_CCID_ITF_SUBCLASS,
    USBD_CCID_ITF_PROTOCOL,
    USBD_CCID_ITF_STR_DESC_IDX,
    0x00U,
    ccid_endpoints_app,
    0x00U,
  }
};

/* Private function prototypes -----------------------------------------------*/
VOID app_ux_device_thread_entry(void *argument);

/** ########## Step 3 ##########
  *  Initialization of USBX Device CCID Class.
  */
/**
  * @brief  Application USBX Device Initialization.
  */
UINT app_usbx_device_init(VOID)
{
  UINT status = UX_SUCCESS;
  USB_DESCRIPTOR                    usbd_desc;
  ULONG                             report_desc_len;
  UX_DEVICE_CLASS_CCID_PARAMETER    ccid_parameter;
  report_desc_len = sizeof(ccid_parameter);
  usb_device_descriptor_register_class(USBD_CLASS_TYPE_CCID, ccid_interface_app, &report_desc_len);

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
  /* Initialize the ccid class parameters for the device */
  ccid_parameter.ux_device_class_ccid_instance_activate   = usbd_ccid_activate;
  ccid_parameter.ux_device_class_ccid_instance_deactivate = usbd_ccid_deactivate;
  ccid_parameter.ux_device_class_ccid_handles             = &usbd_ccid_handles;
  ccid_parameter.ux_device_class_ccid_clocks              = usbd_ccid_clocks;
  ccid_parameter.ux_device_class_ccid_data_rates          = usbd_ccid_data_rates;
  ccid_parameter.ux_device_class_ccid_max_n_slots         = USBD_CCID_MAX_SLOT_INDEX;
  ccid_parameter.ux_device_class_ccid_max_n_busy_slots    = USBD_CCID_MAX_BUSY_SLOTS;
  ccid_parameter.ux_device_class_ccid_max_transfer_length = USBD_CCID_MAX_BLOCK_SIZE_HEADER;
  ccid_parameter.ux_device_class_ccid_n_clocks            = USBD_CCID_N_CLOCKS;
  ccid_parameter.ux_device_class_ccid_n_data_rates        = USBD_CCID_N_DATA_RATES;

  /* Initialize the device ccid class */
  status = ux_device_stack_class_register(_ux_system_device_class_ccid_name,
                                          ux_device_class_ccid_entry,
                                          0x01,
                                          ccid_interface_app->interface_numbers,
                                          &ccid_parameter);

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
    /* Create the device application main thread */
  if (xTaskCreate(usbd_ccid_thread_entry,"app_ccid_thread_entry",
                  UX_DEVICE_APP_THREAD_STACK_SIZE, NULL,
                  UX_DEVICE_APP_THREAD_PRIO, &usbd_ccid_thread) != pdPASS)
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

  /* Unregister ccid class. */
  status = ux_device_stack_class_unregister(_ux_system_device_class_ccid_name, ux_device_class_ccid_entry);
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
