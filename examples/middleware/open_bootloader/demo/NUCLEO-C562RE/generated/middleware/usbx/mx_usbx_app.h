/**
  ******************************************************************************
  * @file    mx_usbx_app.h
  * @brief   USBX applicative header file
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
#ifndef MX_USBX_APP_H
#define MX_USBX_APP_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"
#include "stm32_hal.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define APP_DEFAULT_STACK_SIZE              2048
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
UINT app_usbx_init(VOID);
UINT app_usbx_deinit(VOID);
VOID app_usbx_process(VOID);
/* Exported defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_USBX_APP_H */