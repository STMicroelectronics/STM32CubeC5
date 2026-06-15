/**
  ******************************************************************************
  * @file    ux_stm32_device_descriptors_config.h
  * @brief   Configuration for STM32 USBX device descriptors
  *
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
#ifndef UX_STM32_DEVICE_DESCRIPTORS_CONFIG_H
#define UX_STM32_DEVICE_DESCRIPTORS_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USBD_HIGH_SPEED_SUPPORTED               0U
#define USBD_COMPOSITE_USE_IAD                  1U
#define USBD_HID_MOUSE_ACTIVATED                0U
#define USBD_HID_KEYBOARD_ACTIVATED             0U
#define USBD_HID_CUSTOM_ACTIVATED               0U
#define USBD_CDC_ACM_CLASS_ACTIVATED            0U
#define USBD_MSC_CLASS_ACTIVATED                0U
#define USBD_DFU_CLASS_ACTIVATED                1U
#define USBD_AUDIO_CLASS_ACTIVATED              0U
#define USBD_VIDEO_CLASS_ACTIVATED              0U
#define USBD_PRINTER_CLASS_ACTIVATED            0U
#define USBD_CCID_CLASS_ACTIVATED               0U
#define USBD_MTP_CLASS_ACTIVATED                0U
#define USBD_VENDOR_ID                          1155
#define USBD_PRODUCT_ID                         57105
#define USBD_LANG_ID_STRING                     1033
#define USBD_MANUFACTURER_STRING                "STMicroelectronics"
#define USBD_PRODUCT_STRING                     "STM32 DFU Device"
#define USBD_SERIAL_NUMBER_STRING               "DFU00001"
#define USBD_MAX_POWER                          50
#define USBD_DFU_XFER_SIZE                      256
#define USBD_DFU_BM_ATTRIBUTES                  11
#define USBD_DFU_DETACH_TIMEOUT                 255
#define USBD_DFU_STRING_DESC                    "DFU Interface"
#define USBD_DFU_STRING_DESC_INDEX              0x06U
#define DFU_DESCRIPTOR_TYPE                     0x21U

#ifdef __cplusplus
}
#endif
#endif  /* UX_STM32_DEVICE_DESCRIPTORS_CONFIG_H */