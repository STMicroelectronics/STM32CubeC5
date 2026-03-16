/**
  ******************************************************************************
  * @file    ux_stm32_device_descriptors_config.h
  * @brief   USBX Device descriptor header file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UX_STM32_DEVICE_DESCRIPTORS_CONFIG_H
#define UX_STM32_DEVICE_DESCRIPTORS_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define USBD_HIGH_SPEED_SUPPORTED               0U

#define USBD_CDC_ACM_CLASS_ACTIVATED            1U
#define USBD_COMPOSITE_USE_IAD                  1U
/* Exported types ------------------------------------------------------------*/
#define USBD_VENDOR_ID                          1155
#define USBD_PRODUCT_ID                         22450
#define USBD_LANG_ID_STRING                     1033
#define USBD_MANUFACTURER_STRING                "STMicroelectronics"
#define USBD_PRODUCT_STRING                     "STM32 Virtual ComPort"
#define USBD_SERIAL_NUMBER_STRING               "CDC_ACM001"

/* Endpoint IN for CDC_ACM */
#define USBD_CDC_ACM_EPIN_ADDR                  0x81
#define USBD_CDC_ACM_EPIN_TYPE                  UX_BULK_ENDPOINT
#define USBD_CDC_ACM_EPIN_FS_MPS                64
#define USBD_CDC_ACM_EPIN_FS_BINTERVAL          0

/* Endpoint OUT for CDC_ACM */
#define USBD_CDC_ACM_EPOUT_ADDR                 0x01
#define USBD_CDC_ACM_EPOUT_TYPE                 UX_BULK_ENDPOINT
#define USBD_CDC_ACM_EPOUT_FS_MPS               64
#define USBD_CDC_ACM_EPOUT_FS_BINTERVAL         0

/* Endpoint notification for CDC_ACM */
#define USBD_CDC_ACM_EPNTF_ADDR                 0x82
#define USBD_CDC_ACM_EPNTF_TYPE                 UX_INTERRUPT_ENDPOINT
#define USBD_CDC_ACM_EPNTF_FS_MPS               8
#define USBD_CDC_ACM_EPNTF_FS_BINTERVAL         16

#ifdef __cplusplus
}
#endif
#endif  /* UX_STM32_DEVICE_DESCRIPTORS_CONFIG_H */
