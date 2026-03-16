/**
  ******************************************************************************
  * @file    app_usbx_host.h
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
#ifndef APP_USBX_HOST_H
#define APP_USBX_HOST_H

#ifdef __cplusplus
extern "C" {
#endif

/* Private includes ----------------------------------------------------------*/
#include "ux_api.h"
#include "ux_host_class_hid.h"
#include "ux_hcd_stm32.h"
#include "mx_usbx_app_host_hid_mouse.h"
#include "mx_usbx_app_host_hid_keyboard.h"
#include "ux_host_class_hid_mouse.h"
#include "ux_host_class_hid_keyboard.h"
#include "mx_usbx_app_host_msc.h"
#include "ux_host_class_storage.h"
#include "ux_host_class_hub.h"
#include "mx_hal_def.h"
#include "mx_adv_trace_app.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
UINT app_usbx_host_init(VOID);
UINT app_usbx_host_deinit(VOID);

/* Private defines -----------------------------------------------------------*/
#define USBH_USRLOG(...)   ADV_TRACE_FSend(__VA_ARGS__);\
  ADV_TRACE_FSend("\n");

#define USBH_ERRLOG(...)   ADV_TRACE_FSend("ERROR: ") ;\
  ADV_TRACE_FSend(__VA_ARGS__);\
  ADV_TRACE_FSend("\n");

#ifdef __cplusplus
}
#endif

#endif /* APP_USBX_HOST_H */
