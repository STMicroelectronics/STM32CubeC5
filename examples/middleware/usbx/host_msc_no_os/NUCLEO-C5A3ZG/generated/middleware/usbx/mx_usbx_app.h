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
#include "mx_advanced_trace_app.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define APP_DEFAULT_STACK_SIZE              24576
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
VOID app_usbx_process(VOID);
/* Exported defines -----------------------------------------------------------*/
#define USBH_USRLOG(...)   do {ADV_TRACE_FSend(__VA_ARGS__);\
ADV_TRACE_FSend("\n");} while(0)

#define USBH_ERRLOG(...)   do {ADV_TRACE_FSend("ERROR: ") ;\
  ADV_TRACE_FSend(__VA_ARGS__);\
  ADV_TRACE_FSend("\n");} while(0)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_USBX_APP_H */
