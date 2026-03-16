/**
  ******************************************************************************
  * @file    mx_usbx_device_hid_mouse.h
  * @brief   USBX Device Mouse HID applicative header file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_USBX_DEVICE_HID_MOUSE_H
#define MX_USBX_DEVICE_HID_MOUSE_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"
#include "ux_device_class_hid.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
VOID usbd_hid_mouse_activate(VOID *hid_instance);
VOID usbd_hid_mouse_deactivate(VOID *hid_instance);
UCHAR *usbd_hid_mouse_report_address(VOID);
ULONG usbd_hid_mouse_report_length(VOID);
ULONG usbd_hid_mouse_report_id(VOID);
UINT usbd_hid_mouse_set_report(UX_SLAVE_CLASS_HID *hid_instance, UX_SLAVE_CLASS_HID_EVENT *hid_event);
UINT usbd_hid_mouse_get_report(UX_SLAVE_CLASS_HID *hid_instance, UX_SLAVE_CLASS_HID_EVENT *hid_event);
VOID usbd_hid_mouse_process(VOID);
/* Exported defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif  /* MX_USBX_DEVICE_HID_MOUSE_H */