/**
  ******************************************************************************
  * @file    mx_usbx_device_hid_mouse.c
  * @brief   USBX Device Mouse HID applicative source file
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
#include "mx_usbx_device_hid_mouse.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CURSOR_STEP     10
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UX_SLAVE_CLASS_HID      *p_hid_mouse;
extern volatile uint8_t ButtonState;
UCHAR usbd_hid_mouse_report_descriptor[] =
{
  0x05U, 0x01U, /* Usage Page (Generic Desktop)*/
  0x09U, 0x02U, /* Usage (Mouse) */
  0xA1U, 0x01U, /* Collection (Application) */
  0x09U, 0x01U, /* Usage (Pointer) */

  0xA1U, 0x00U, /* Collection (Physical) */
  0x05U, 0x09U, /* Usage Page (Buttons) */
  0x19U, 0x01U, /* Usage Minimum (01U) */
  0x29U, 0x03U, /* Usage Maximum (03U) */

  0x15U, 0x00U, /* Logical Minimum (0U) */
  0x25U, 0x01U, /* Logical Maximum (1U) */
  0x95U, 0x03U, /* Report Count (3U) */
  0x75U, 0x01U, /* Report Size (1U) */

  0x81U, 0x02U, /* Input(Data, Variable, Absolute) 3U button bit fields */
  0x95U, 0x01U, /* Report Count (1U) */
  0x75U, 0x05U, /* Report Size (5U) */
  0x81U, 0x01U, /* Input (Constant), 5U constant field */

  0x05U, 0x01U, /* Usage Page (Generic Desktop) */
  0x09U, 0x30U, /* Usage (X) */
  0x09U, 0x31U, /* Usage (Y) */
  0x09U, 0x38U, /* Usage (Z) */

  0x15U, 0x81U, /* Logical Minimum (-127) */
  0x25U, 0x7FU, /* Logical Maximum (127) */
  0x75U, 0x08U, /* Report Size (8U) */
  0x95U, 0x03U, /* Report Count (3U) */

  0x81U, 0x06U, /* Input(Data, Variable, Relative), Three position bytes (X & Y & Z)*/
  0xC0U, 0xC0U  /* End collection, Close Pointer collection*/
};
/* Private function prototypes -----------------------------------------------*/
static VOID usbd_hid_mouse_get_pointer_data(UX_SLAVE_CLASS_HID_EVENT *p_hid_event);

/**
  * @brief  usbd_hid_mouse_activate
  *         This function is called when insertion of a HID Mouse device.
  * @retval none
  */
VOID usbd_hid_mouse_activate(VOID *hid_instance)
{
  /* Save the HID mouse instance */
  if (p_hid_mouse == UX_NULL)
  {
    p_hid_mouse = (UX_SLAVE_CLASS_HID *) hid_instance;
  }

  return;
}

/**
  * @brief  usbd_hid_mouse_deactivate
  *         This function is called when extraction of a HID Mouse device.
  * @param  hid_instance: Pointer to the hid class instance.
  * @retval none
  */
VOID usbd_hid_mouse_deactivate(VOID *hid_instance)
{
  /* Reset the HID mouse instance */
  if (p_hid_mouse == hid_instance)
  {
    p_hid_mouse = UX_NULL;
  }

  return;
}

/**
  * @brief  usbd_hid_mouse_set_report
  *         This function is invoked when the host sends a HID SET_REPORT
  *         to the application over Endpoint 0.
  * @retval status
  */
UINT usbd_hid_mouse_set_report(UX_SLAVE_CLASS_HID *hid_instance,
                               UX_SLAVE_CLASS_HID_EVENT *hid_event)
{
  UINT status = UX_SUCCESS;

  UX_PARAMETER_NOT_USED(hid_instance);
  UX_PARAMETER_NOT_USED(hid_event);

  return status;
}

/**
  * @brief  usbd_hid_mouse_get_report
  *         This function is invoked when host is requesting event through
  *         control GET_REPORT request.
  * @retval status
  */
UINT usbd_hid_mouse_get_report(UX_SLAVE_CLASS_HID *hid_instance,
                               UX_SLAVE_CLASS_HID_EVENT *hid_event)
{
  UINT status = UX_SUCCESS;

  UX_PARAMETER_NOT_USED(hid_instance);
  UX_PARAMETER_NOT_USED(hid_event);

  return status;
}

/**
  * @brief  usbd_hid_mouse_report_address
  *         Return the device HID MOUSE Report Descriptor
  * @retval Pointer to HID Report Descriptor buffer
  */
UCHAR *usbd_hid_mouse_report_address(VOID)
{
  return usbd_hid_mouse_report_descriptor;
}

/**
  * @brief  usbd_hid_mouse_report_length
  *         Return the device HID Report Descriptor
  * @retval Size of HID Report Descriptor buffer
  */
ULONG usbd_hid_mouse_report_length(VOID)
{
  return sizeof(usbd_hid_mouse_report_descriptor);
}

/**
  * @brief  usbd_hid_mouse_report_id
  *         Return the device HID Report ID Support
  * @retval UX_FALSE / UX_TRUE
  */
ULONG usbd_hid_mouse_report_id(VOID)
{
  return UX_FALSE;
}

/**
  * @brief  usbd_hid_mouse_process
  *         Run HID mouse task
  * @param  none
  * @retval none
  */
VOID usbd_hid_mouse_process(VOID)
{
  UX_SLAVE_CLASS_HID_EVENT hid_event;

  /* Check if the device state already configured */
  if (p_hid_mouse != UX_NULL)
  {
    ux_utility_memory_set(&hid_event, 0, sizeof(UX_SLAVE_CLASS_HID_EVENT));

    /* Check if user button is pressed */
    if (ButtonState != 0U)
    {
      /* Get the new position */
      usbd_hid_mouse_get_pointer_data(&hid_event);

      /* Send an event to the hid */
      ux_device_class_hid_event_set(p_hid_mouse, &hid_event);

      /* Reset User Button state */
      ButtonState = 0U;
    }
  }
}

/**
  * @brief  usbd_hid_mouse_get_pointer_data
  *         Gets Pointer Data.
  * @retval None
  */
static VOID usbd_hid_mouse_get_pointer_data(UX_SLAVE_CLASS_HID_EVENT *p_hid_event)
{
  static int8_t counter = 0;
  static int8_t x = 0, y = 0;

  /* Reset counter */
  if (counter == 16)
  {
    counter = 0;
  }
  /* Update pointer position x and y */
  if ((counter >= 0) && (counter < 4))
  {
    y = 0;
    x = CURSOR_STEP;
  }
  else if ((counter >= 4) && (counter < 8))
  {
    x = 0;
    y = CURSOR_STEP;
  }
  else if ((counter >= 8) && (counter < 12))
  {
    y = 0;
    x = -CURSOR_STEP;
  }
  else
  {
    x = 0;
    y = -CURSOR_STEP;
  }

  /* Increment counter */
  counter++;

  /* Mouse event. Length is fixed to 4 */
  p_hid_event->ux_device_class_hid_event_length = 4;
  p_hid_event->ux_device_class_hid_event_buffer[0] = 0;
  p_hid_event->ux_device_class_hid_event_buffer[1] = x;
  p_hid_event->ux_device_class_hid_event_buffer[2] = y;
  p_hid_event->ux_device_class_hid_event_buffer[3] = 0;
}