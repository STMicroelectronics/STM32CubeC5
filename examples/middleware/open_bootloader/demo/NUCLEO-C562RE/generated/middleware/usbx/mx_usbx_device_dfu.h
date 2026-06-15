/**
  ******************************************************************************
  * @file    mx_usbx_device_dfu.h
  * @brief   USBX Device DFU applicative header file
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
#ifndef MX_USBX_DEVICE_DFU_H
#define MX_USBX_DEVICE_DFU_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"
#include "ux_device_class_dfu.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
VOID usbd_dfu_activate(VOID *dfu_instance);
VOID usbd_dfu_deactivate(VOID *dfu_instance);
UINT usbd_dfu_get_status(VOID *dfu_instance, ULONG *media_status);
UINT usbd_dfu_read(VOID *dfu_instance, ULONG block_number, UCHAR *data_pointer,
                   ULONG length, ULONG *actual_length);
UINT usbd_dfu_write(VOID *dfu_instance, ULONG block_number, UCHAR *data_pointer,
                    ULONG length, ULONG *media_status);
UINT usbd_dfu_notify(VOID *dfu_instance, ULONG notification);
#ifdef UX_DEVICE_CLASS_DFU_CUSTOM_REQUEST_ENABLE
UINT usbd_dfu_custom_request(VOID *dfu_instance, UX_SLAVE_TRANSFER *transfer);
#endif /* UX_DEVICE_CLASS_DFU_CUSTOM_REQUEST_ENABLE */

/* Exported defines ----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif  /* MX_USBX_DEVICE_DFU_H */