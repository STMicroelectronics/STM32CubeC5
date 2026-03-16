/**
  ******************************************************************************
  * @file    app_usbx_device_cdc_acm_uart.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_USBX_DEVICE_CDC_ACM_UART_H
#define APP_USBX_DEVICE_CDC_ACM_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/* Private includes ----------------------------------------------------------*/
#include "ux_api.h"
#include "ux_device_class_cdc_acm.h"
#include "mx_usbx_app_device.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
VOID USBD_CDC_ACM_Activate(VOID *cdc_acm_instance);
VOID USBD_CDC_ACM_Deactivate(VOID *cdc_acm_instance);
VOID USBD_CDC_ACM_ParameterChange(VOID *cdc_acm_instance);
VOID usbx_cdc_acm_read_thread_entry(VOID *args);
VOID usbx_cdc_acm_write_thread_entry(VOID *args);
extern hal_status_t HAL_UART_Transmit_DMA(hal_uart_handle_t *huart, const void *p_data, uint32_t size_byte);
extern hal_status_t HAL_UART_Receive_DMA(hal_uart_handle_t *huart, void *p_data, uint32_t size_byte);
/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* APP_USBX_DEVICE_CDC_ACM_UART_H */
