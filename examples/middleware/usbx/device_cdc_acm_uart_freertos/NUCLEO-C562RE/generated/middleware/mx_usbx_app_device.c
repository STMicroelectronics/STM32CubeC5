/**
  ******************************************************************************
  * @file    app_usbx_device.c
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
#include "mx_usbx_app_device.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TaskHandle_t app_ux_device_thread;
TaskHandle_t usbx_cdc_acm_read_thread;
TaskHandle_t usbx_cdc_acm_write_thread;
UX_EVENT_FLAGS_GROUP EventFlag;
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
VOID app_ux_device_thread_entry(VOID *args);
/* Exported functions --------------------------------------------------------*/
hal_pcd_handle_t *p_usb_device; /* pointer referencing the USB Device handle from the generated code */

/**
  * @brief  Application USBX Device Initialization.
  */
UINT app_usbx_device_init(VOID)
{
  /** ########## Step 3 ##########
    *  Initialization of USBX CDC_ACM Device Class
    */
  UINT status = UX_SUCCESS;
  USB_DESCRIPTOR usbd_desc;
  UX_SLAVE_CLASS_CDC_ACM_PARAMETER cdc_acm_parameter;

  ULONG report_desc_len = sizeof(cdc_acm_parameter);
  usb_device_descriptor_register_class(USBD_CLASS_TYPE_CDC_ACM, cdc_acm_interface, &report_desc_len);
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
  cdc_acm_parameter.ux_slave_class_cdc_acm_instance_activate   = USBD_CDC_ACM_Activate;
  cdc_acm_parameter.ux_slave_class_cdc_acm_instance_deactivate = USBD_CDC_ACM_Deactivate;
  cdc_acm_parameter.ux_slave_class_cdc_acm_parameter_change    = USBD_CDC_ACM_ParameterChange;

  /* Initialize the device hid Mouse class */
  status = ux_device_stack_class_register(_ux_system_slave_class_cdc_acm_name,
                                          ux_device_class_cdc_acm_entry,
                                          0x01,
                                          cdc_acm_interface->interface_numbers,
                                          &cdc_acm_parameter);

  if (status != UX_SUCCESS)
  {
    return status;
  }
  /* Create app_ux_device_thread. */
  status = xTaskCreate(app_ux_device_thread_entry, "app ux device thread entry",
                       APP_DEFAULT_STACK_SIZE, (VOID *) NULL,
                       APP_DEFAULT_PRIORITY, &app_ux_device_thread);
  if (status != pdPASS)
  {
    return UX_ERROR;
  }

  /* Create the usbx_cdc_acm_read_thread_entry thread */
  status = xTaskCreate(usbx_cdc_acm_read_thread_entry, "usbx cdc acm_read thread entry",
                       APP_DEFAULT_STACK_SIZE, (VOID *) NULL,
                       APP_DEFAULT_PRIORITY, &usbx_cdc_acm_read_thread);
  if (status != pdPASS)
  {
    return UX_ERROR;
  }

  /* Create the usbx cdc acm write thread */
  status = xTaskCreate(usbx_cdc_acm_write_thread_entry, "usbx cdc acm write thread entry",
                       APP_DEFAULT_STACK_SIZE, (VOID *) NULL,
                       APP_DEFAULT_PRIORITY, &usbx_cdc_acm_write_thread);
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
  UINT ret = UX_SUCCESS;

  return ret;
}


/**
  * @brief  Function implementing app_ux_device_thread_entry.
  * @param  thread_input: User thread input parameter.
  * @retval none
  */
VOID app_ux_device_thread_entry(VOID *args)
{
  /* Initialize the usbx system component */
  mx_example_pcd_init();
  /* Initialization of USB device */
  p_usb_device = mx_example_pcd_gethandle();

  /* initialize the device controller driver */
  ux_dcd_stm32_initialize(0, (ULONG)p_usb_device);

  /* sleep for 10ms */
  ux_utility_thread_sleep(MS_TO_TICK(10));

  /* Suspend the USB device thread */
  vTaskSuspend(app_ux_device_thread);
}

