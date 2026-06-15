/**
  ******************************************************************************
  * @file    mx_usbx_device_video.h
  * @brief   USBX Device Video applicative header file
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
#ifndef MX_USBX_DEVICE_VIDEO_H
#define MX_USBX_DEVICE_VIDEO_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"
#include "ux_device_class_video.h"
#include "mx_usbx_device.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#ifndef USBD_VIDEO_STREAM_NUMBER
#define USBD_VIDEO_STREAM_NUMBER  1
#endif

#ifndef USBD_VIDEO_PAYLOAD_BUFFER_NUMBER
#define USBD_VIDEO_PAYLOAD_BUFFER_NUMBER  8
#endif
/* Exported macros -----------------------------------------------------------*/
/* Exported functions prototypes -------------------------------------------- */
VOID usbd_video_activate(VOID *video_instance);
VOID usbd_video_deactivate(VOID *video_instance);
VOID usbd_video_stream_change(UX_DEVICE_CLASS_VIDEO_STREAM *video_stream, ULONG alternate_setting);
VOID usbd_video_stream_payload_done(UX_DEVICE_CLASS_VIDEO_STREAM *video_stream, ULONG length);
UINT usbd_video_stream_request(UX_DEVICE_CLASS_VIDEO_STREAM *video_stream, UX_SLAVE_TRANSFER *transfer);
ULONG usbd_video_stream_get_max_payload_buffer_size(VOID);
VOID usbd_video_thread_entry(VOID *args);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_USBX_DEVICE_VIDEO_H */
