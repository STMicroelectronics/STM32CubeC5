/**
  ******************************************************************************
  * @file    mx_usbx_host_hid_mouse.h
  * @brief   USBX Host Mouse HID applicative header file
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
#ifndef MX_USBX_HOST_HID_MOUSE_H
#define MX_USBX_HOST_HID_MOUSE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ----------------------------------------------------------*/
#include "ux_api.h"
#include "ux_host_class_hid.h"
#include "ux_host_class_hid_mouse.h"
#include "mx_usbx_host.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
VOID usbh_hid_mouse_process(VOID);
/* Exported defines ----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif  /* MX_USBX_HOST_HID_MOUSE_H */
