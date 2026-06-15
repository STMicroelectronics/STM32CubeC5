/**
  ******************************************************************************
  * @file    mx_usbx_host.h
  * @brief   USBX Host applicative header file
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
#ifndef MX_USBX_HOST_H
#define MX_USBX_HOST_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* Includes ----------------------------------------------------------*/
#include "ux_api.h"
#include "mx_usbx_host_msc.h"
#include "mx_hal_def.h"
#include "mx_usbx_app.h"
#include "mx_advanced_trace_app.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
UINT app_usbx_host_init(VOID);
UINT app_usbx_host_deinit(VOID);
/* Private defines -----------------------------------------------------------*/
#define USBH_USRLOG(...)   do {ADV_TRACE_FSend(__VA_ARGS__);\
ADV_TRACE_FSend("\n");} while(0)

#define USBH_ERRLOG(...)   do {ADV_TRACE_FSend("ERROR: ") ;\
  ADV_TRACE_FSend(__VA_ARGS__);\
  ADV_TRACE_FSend("\n");} while(0)


#ifdef __cplusplus
}
#endif

#endif /* MX_USBX_HOST_H */
