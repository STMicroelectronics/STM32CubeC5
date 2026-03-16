/**
  ******************************************************************************
  * @file    app_usbx.h
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
#ifndef APP_USBX_H
#define APP_USBX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Private includes ----------------------------------------------------------*/
#include "mx_usbx_app_host.h"
#include "stm32_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USBX_HOST_MEMORY_STACK_SIZE       1024*32

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

/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* APP_USBX_H */
