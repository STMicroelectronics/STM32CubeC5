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

/* Exported variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** brief: Assign target dynamic address in interrupt mode for the MX_I3Cx instance.
  */
void I3C_AssignDynamicAddr_IT(void);

/** brief: Controller set dynamic address.
  * param: target_address Target dynamic address.
  */
void I3C_SetDynamicAddr(uint64_t target_address);

/** brief: Configures the bus device entries in the controller for the MX_I3Cx instance.
  * param: p_desc    Pointer to the devices descriptor.
  * param: device_nb Number of devices.
  */
void I3C_ConfigureBusDevices(const target_desc_t *p_desc, uint32_t device_nb);

/** brief: Activates notification of the MX_I3Cx instance.
  */
void I3C_ActivateNotification(void);

/** brief:  Gets the number of payload bytes received after IBI on the MX_I3Cx instance.
  * retval: Number of IBI payload bytes.
  */
uint32_t I3C_GetIBIPayloadByteCount(void);

/** brief:  Gets the IBI payload value received by the MX_I3Cx controller.
  * retval: IBI payload value.
  */
uint32_t I3C_GetIBIPayload(void);

/** brief: MX_I3Cx target request dynamic address callbacks.
  * param: target_payload Target payload value.
  */
void I3C_TargetReqDynamicAddrCallback(uint64_t target_payload);

/** brief: MX_I3Cx DAA complete callbacks.
  */
void I3C_DAACompleteCallback(void);

/** brief: MX_I3Cx IBI notification callbacks.
  */
void I3C_NotificationCallback(void);

/** brief: MX_I3Cx error callback.
  */
void I3C_ErrorCallback(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_EXAMPLE_H */
