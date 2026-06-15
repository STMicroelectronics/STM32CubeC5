/**
  ******************************************************************************
  * @file    mx_usbx_host_msc.h
  * @brief   USBX Host MSC applicative header file
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
#ifndef MX_USBX_HOST_MSC_H
#define MX_USBX_HOST_MSC_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"
#include "ux_host_class_storage.h"
#include "fx_api.h"
#include "mx_usbx_app.h"
#include "mx_filex_app.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
UINT usbh_msc_process(VOID);
VOID _ux_host_class_storage_driver_entry(FX_MEDIA *media);
/* Exported defines ----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif  /* MX_USBX_HOST_MSC_H */
