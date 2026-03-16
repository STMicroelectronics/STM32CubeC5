/**
  ******************************************************************************
  * @file    mx_usbx_device.h
  * @brief   USBX Device applicative header file
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
#ifndef MX_USBX_DEVICE_H
#define MX_USBX_DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ----------------------------------------------------------*/
#include "ux_api.h"
#include "ux_stm32_device_descriptors_config.h"
#include "ux_stm32_device_descriptors.h"
#include "mx_hal_def.h"
#include "mx_usbx_app.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
UINT app_usbx_device_init(VOID);
UINT app_usbx_device_deinit(VOID);
UINT app_usbx_device_process(VOID);
/* Exported defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* MX_USBX_DEVICE_H */
