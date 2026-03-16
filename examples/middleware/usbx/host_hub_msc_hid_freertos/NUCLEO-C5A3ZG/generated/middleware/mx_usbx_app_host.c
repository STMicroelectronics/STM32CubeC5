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
#define HID_INSTANCE     3
#define MSC_INSTANCE     3
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UX_HOST_CLASS_HUB           *hub_instance;
UX_HOST_CLASS_HID           *hid_instance[HID_INSTANCE];
UX_HOST_CLASS_HID_MOUSE     *mouse;
UX_HOST_CLASS_HID_KEYBOARD  *keyboard;
UX_HOST_CLASS_STORAGE       *storage_instance[MSC_INSTANCE];
UX_HOST_CLASS_STORAGE_MEDIA *storage_media;
FX_MEDIA                    *media[MSC_INSTANCE];
UX_EVENT_FLAGS_GROUP        ux_app_EventFlag;
UINT                        msc_index, hid_index;

TaskHandle_t ux_host_app_thread_handle;
TaskHandle_t hid_mouse_thread_handle;
TaskHandle_t hid_keyboard_thread_handle;
TaskHandle_t msc_process_thread_handle;
/* Exported variables --------------------------------------------------------*/
hal_hcd_handle_t *p_usb_host; /* pointer referencing the USB Host handle from the generated code */
extern hal_uart_handle_t gh_usart1;
/* Private function prototypes -----------------------------------------------*/
static UINT ux_host_event_callback(ULONG event, UX_HOST_CLASS *current_class, VOID *current_instance);
static VOID ux_host_error_callback(UINT system_level, UINT system_context, UINT error_code);
/* Exported functions --------------------------------------------------------*/
/** ########## Step 3 ##########
  *  Initialization of USBX HOST HUB MSC HID
  *  and Keyboard Class.
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

  /* Initialize the host hub class */
  if (ux_host_stack_class_register(_ux_system_host_class_hub_name,
                                   ux_host_class_hub_entry) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Initialize the host hid class */
  if (ux_host_stack_class_register(_ux_system_host_class_hid_name,
                                   ux_host_class_hid_entry) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Initialize the host hid mouse client */
  if (ux_host_class_hid_client_register(_ux_system_host_class_hid_client_mouse_name,
                                        ux_host_class_hid_mouse_entry) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Initialize the host hid keyboard client */
  if (ux_host_class_hid_client_register(_ux_system_host_class_hid_client_keyboard_name,
                                        ux_host_class_hid_keyboard_entry) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

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

  /* Create the hid mouse thread */
  status = xTaskCreate(usbh_hid_mouse_thread_entry, "hid mouse thread entry",
                       UX_HOST_APP_THREAD_STACK_SIZE, NULL,
                       APP_DEFAULT_PRIORITY, &hid_mouse_thread_handle);
  if (status != pdPASS)
  {
    return UX_ERROR;
  }

  /* Create the hid keyboard thread */
  status = xTaskCreate(usbh_hid_keyboard_thread_entry, "hid keyboard thread entry",
                       UX_HOST_APP_THREAD_STACK_SIZE, NULL,
                       APP_DEFAULT_PRIORITY, &hid_keyboard_thread_handle);
  if (status != pdPASS)
  {
    return UX_ERROR;
  }

  /* Create the storage applicative process thread */
  status = xTaskCreate(msc_process_thread_entry, "msc process thread entry",
                       (2 * UX_HOST_APP_THREAD_STACK_SIZE), NULL,
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
  USBH_USRLOG("**** USB OTG HS HUB HID MSC Host **** \n");
  USBH_USRLOG("USB Host library started.\n");

  /* Wait for Device to be attached */
  USBH_USRLOG("Starting HUB Application");
  USBH_USRLOG("Connect your HUB Device");

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

  /* USER CODE BEGIN ux_host_event_callback0 */
  /* Get current Hid Client */
  UX_HOST_CLASS_HID_CLIENT *client  = (UX_HOST_CLASS_HID_CLIENT *)current_instance;
  /* USER CODE END ux_host_event_callback0 */
  UINT idx;

  switch (event)
  {
    case UX_DEVICE_INSERTION:

      /* USER CODE BEGIN UX_DEVICE_INSERTION */

      /* Get current HUB Class */
      if (current_class -> ux_host_class_entry_function == ux_host_class_hub_entry)
      {
        if (hub_instance == UX_NULL)
        {
          /* Get current HUB Instance */
          hub_instance = (UX_HOST_CLASS_HUB *)current_instance;

          USBH_USRLOG("HUB_Device");
          USBH_USRLOG("PID: %#x ", (UINT)hub_instance ->ux_host_class_hub_device->
                      ux_device_descriptor.idProduct);
          USBH_USRLOG("VID: %#x ", (UINT)hub_instance ->ux_host_class_hub_device->
                      ux_device_descriptor.idVendor);
          USBH_USRLOG("USB HUB Host App\n\n");
        }
      }

      /* Get current Hid Class */
      if (current_class -> ux_host_class_entry_function == ux_host_class_hid_entry)
      {
        for (idx = 0; idx < HID_INSTANCE;)
        {
          if (hid_instance[idx] != NULL)
          {
            /* Move to next HID instance */
            idx ++;
          }
          else
          {
            /* Get current HID Instance */
            hid_instance[idx] = (UX_HOST_CLASS_HID *)current_instance;
            hid_index = idx;
            break;
          }
        }
      }

      /* Get current Storage Class */
      if (current_class -> ux_host_class_entry_function == ux_host_class_storage_entry)
      {

        for (idx = 0; idx < MSC_INSTANCE;)
        {
          if (storage_instance[idx] != NULL)
          {
            /* Move to next storage instance */
            idx ++;
          }
          else
          {
            /* Get current Storage Instance */
            storage_instance[idx] = (UX_HOST_CLASS_STORAGE *)current_instance;
            msc_index = idx;
            break;
          }
        }

        USBH_USRLOG("\nUSB Mass Storage Device Plugged");
        USBH_USRLOG("PID: %#x ", (UINT)storage_instance[msc_index]-> ux_host_class_storage_device ->
                    ux_device_descriptor.idProduct);
        USBH_USRLOG("VID: %#x ", (UINT)storage_instance[msc_index] -> ux_host_class_storage_device ->
                    ux_device_descriptor.idVendor);

        /* Get the storage media */
        storage_media = (UX_HOST_CLASS_STORAGE_MEDIA *)current_class -> ux_host_class_media;

        if (storage_media[msc_index].ux_host_class_storage_media_lun != 0)
        {
          storage_media = UX_NULL;
        }
        else
        {
          /* Get the media file */
          media[msc_index] = &storage_media[msc_index].ux_host_class_storage_media;

          /* Check the storage class state */
          if (storage_instance[msc_index] -> ux_host_class_storage_state ==  UX_HOST_CLASS_INSTANCE_LIVE)
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

      /* USER CODE END UX_DEVICE_INSERTION */

      break;

    case UX_DEVICE_REMOVAL:

      /* USER CODE BEGIN UX_DEVICE_REMOVAL */

      /* Free HUB Instance */
      if ((VOID *)hub_instance == current_instance)
      {
        hub_instance = UX_NULL;

        USBH_USRLOG("\nHUB Device Unplugged");
      }

      /* Clear storage media instance & media file */
      for (msc_index = 0; msc_index < MSC_INSTANCE; msc_index++)
      {
        if ((VOID *)storage_instance[msc_index] == current_instance)
        {
          /* Free Storage Instance */
          storage_instance[msc_index] = UX_NULL;
          storage_media = UX_NULL;
          /* Free Media */
          media[msc_index] = NULL;

          USBH_USRLOG("\nUSB Mass Storage Device Unplugged");
        }
      }

      /* Clear hid instance*/
      for (hid_index = 0; hid_index < HID_INSTANCE; hid_index++)
      {
        if ((VOID *)hid_instance[hid_index] == current_instance)
        {
          /* Free HID Instance */
          hid_instance[hid_index] = UX_NULL;
        }
      }

      /* USER CODE END UX_DEVICE_REMOVAL */

      break;

    case UX_HID_CLIENT_INSERTION:

      /* USER CODE BEGIN UX_HID_CLIENT_INSERTION */

      USBH_USRLOG("\nHID Client Plugged");

      /* Check the HID_client if this is a HID keyboard device */
      if (client -> ux_host_class_hid_client_handler == ux_host_class_hid_keyboard_entry)
      {
        /* Get current Hid Client */
        if (keyboard == UX_NULL)
        {
          keyboard = client -> ux_host_class_hid_client_local_instance;

          USBH_USRLOG("HID_Keyboard_Device");
          USBH_USRLOG("PID: %#x ", (UINT)keyboard
                      ->ux_host_class_hid_keyboard_hid->ux_host_class_hid_device->ux_device_descriptor.idProduct);
          USBH_USRLOG("VID: %#x ", (UINT)keyboard
                      ->ux_host_class_hid_keyboard_hid->ux_host_class_hid_device->ux_device_descriptor.idVendor);
          USBH_USRLOG("USB HID Host Keyboard App...");
          USBH_USRLOG("keyboard is ready...\n");
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
          USBH_USRLOG("PID: %#x ", (UINT)mouse
                      ->ux_host_class_hid_mouse_hid->ux_host_class_hid_device->ux_device_descriptor.idProduct);
          USBH_USRLOG("VID: %#x ", (UINT)mouse
                      ->ux_host_class_hid_mouse_hid->ux_host_class_hid_device->ux_device_descriptor.idVendor);
          USBH_USRLOG("USB HID Host Mouse App...");
          USBH_USRLOG("Mouse is ready...\n");
        }
      }
      /* USER CODE END UX_HID_CLIENT_INSERTION */

      break;

    case UX_HID_CLIENT_REMOVAL:

      /* USER CODE BEGIN UX_HID_CLIENT_REMOVAL */

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

      /* USER CODE END UX_HID_CLIENT_REMOVAL */

      break;

    case UX_DEVICE_CONNECTION:

      /* USER CODE BEGIN UX_DEVICE_CONNECTION */

      /* USER CODE END UX_DEVICE_CONNECTION */

      break;

    case UX_DEVICE_DISCONNECTION:

      /* USER CODE BEGIN UX_DEVICE_DISCONNECTION */

      /* USER CODE END UX_DEVICE_DISCONNECTION */

      break;

    default:

      /* USER CODE BEGIN EVENT_DEFAULT */

      /* USER CODE END EVENT_DEFAULT */

      break;
  }

  /* USER CODE BEGIN ux_host_event_callback1 */

  /* USER CODE END ux_host_event_callback1 */

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
