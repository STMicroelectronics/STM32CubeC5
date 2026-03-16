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
/* Exported constants --------------------------------------------------------*/
#ifndef USE_LL_APP_ERROR
#define USE_LL_APP_ERROR 0U
#endif /* USE_LL_APP_ERROR */

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** brief: Activates the MX_I3Cx notification.
  */
void I3C_ActivateNotification(void);

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/** brief:  Gets the MX_I3Cx own dynamic address.
  * retval: Value of the own dynamic address.
  */
uint8_t I3C_GetOwnDynamicAddress(void);
#endif /* USE_LL_APP_ERROR */

/** brief: Requests an IBI in interrupt mode.
  * param: p_payload         Pointer to the payload bytes.
  * param: payload_size_byte Payload size in bytes.
  */
void I3C_RequestIBI_IT(const uint8_t *p_payload, uint32_t payload_size_byte);

/** brief: MX_I3Cx DAA complete callbacks.
  */
void I3C_DAACompleteCallback(void);

/** brief: MX_I3Cx IBI complete callbacks.
  */
void I3C_IBICompleteCallback(void);

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/** brief: MX_I3Cx error callbacks.
  */
void I3C_ErrorCallback(void);
#endif /* USE_LL_APP_ERROR */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_EXAMPLE_H */
