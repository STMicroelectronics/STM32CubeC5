/**
  ******************************************************************************
  * file           : ll_example.h
  * brief          : Header for stm32ynxx_ll_example.c files.
  *                  This file contains example-specific declarations to interface with example.c functions.
  ******************************************************************************
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LL_EXAMPLE_H
#define LL_EXAMPLE_H

/* Includes ------------------------------------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "mx_hal_def.h"       /* aliases to the target-specific generated code */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Exported types ------------------------------------------------------------*/
/* Target descriptor structure for ENTDAA multi-target management */
typedef struct
{
  char     *target_name;       /* Human-readable name or reference for the target device */
  uint32_t target_id;          /* Unique identifier for the target on the I3C bus (can link to index in device table) */
  uint64_t target_bcr_dcr_pid; /* Concatenated value of PID, BCR, and DCR for device identification and ENTDAA
                                  verification */
  uint8_t  static_addr;        /* Static address from device datasheet (used before dynamic assignment) */
  uint8_t  dynamic_addr;       /* Dynamic address assigned by controller during ENTDAA */
} target_desc_t;

/* Exported constants --------------------------------------------------------*/
#ifndef USE_LL_APP_ERROR
#define USE_LL_APP_ERROR 0U
#endif /* USE_LL_APP_ERROR */

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* brief: Activates notification of the MX_I3Cx instance. */
void I3C_ActivateNotification(void);

/* brief: Assign target dynamic address in interrupt mode for the MX_I3Cx instance. */
void I3C_AssignDynamicAddr_IT(void);

/** brief: Controller set dynamic address.
  * param: target_address Target dynamic address.
  */
void I3C_SetDynamicAddr(uint64_t target_address);

/* brief: MX_I3Cx hotjoin request callbacks. */
void I3C_HotJoinReqCallback(void);

/* brief: MX_I3Cx DAA complete callbacks. */
void I3C_DAACpltCallback(void);

/* brief: MX_I3Cx target request dynamic address callbacks. */
void I3C_TargetReqDynamicAddrCallback(uint64_t targetPayload);

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/* brief: MX_I3Cx DAA error callbacks */
void I3C_DAAErrorCallback(void);
#endif /* USE_LL_APP_ERROR */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_EXAMPLE_H */
