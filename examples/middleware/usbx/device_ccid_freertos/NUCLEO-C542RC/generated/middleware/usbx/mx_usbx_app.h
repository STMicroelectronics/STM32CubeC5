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
#define APP_DEFAULT_STACK_SIZE              15360
#define UX_DEVICE_APP_THREAD_STACK_SIZE     1024
#define UX_DEVICE_APP_THREAD_PRIO           10
#define UX_DEVICE_APP_THREAD_NAME           "app_ux_device_thread_entry"

#define UX_DEVICE_APP_THREAD_PREEMPTION_THRESHOLD  UX_DEVICE_APP_THREAD_PRIO
#ifndef UX_DEVICE_APP_THREAD_TIME_SLICE
#define UX_DEVICE_APP_THREAD_TIME_SLICE     ux_no_time_slice
#endif
#ifndef UX_DEVICE_APP_THREAD_START_OPTION
#define UX_DEVICE_APP_THREAD_START_OPTION   UX_AUTO_START
#endif
/** Direct to task notification event sent directly to the Idle task to indicate the example status.
  * Attention: configTASK_NOTIFICATION_ARRAY_ENTRIES does not need to be set to 2
  * as we process only 1 notification (success or error).
  */
typedef enum
{
  NOTIFICATION_VALUE_UNKNOWN = 0x00,   /* Default value. */
  NOTIFICATION_VALUE_SUCCESS = 0x01,   /* The example ran as expected. */
  NOTIFICATION_VALUE_ERROR   = 0x02    /* Unrecoverable error encountered while running the example. */
} ExampleStatusNotification_t;
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
UINT app_usbx_init(VOID);
UINT app_usbx_deinit(VOID);
/* Exported defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_USBX_APP_H */
