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

/** brief: Checks whether the MX_I3Cx Dynamic Address Update (DAUPD) flag is active.
  * retval: 1 if flag is active, 0 otherwise
  */
uint32_t I3C_IsActiveFlagDynamicAddr(void);

/** brief: Gets the MX_I3Cx own dynamic address.
  * retval: value of Own Dynamic Address
  */
uint8_t I3C_GetOwnDynamicAddress(void);

/** brief: Clears the MX_I3Cx Dynamic Address Update (DAUPD) flag.
  * retval: none
  */
void I3C_ClearFlagDynamicAddr(void);

/** brief: Checks whether the MX_I3Cx Max Read Length Update (MRLUPD) flag is active.
  * retval: 1 if flag is active, 0 otherwise
  */
uint32_t I3C_IsActiveFlagMaxReadLength(void);

/** brief: Clears the MX_I3Cx Max Read Length Update (MRLUPD) flag.
  * retval: none
  */
void I3C_ClearFlagMaxReadLength(void);

/** brief: Gets the MX_I3Cx Max Read Length value.
  * retval: value of Max Read Length
  */
uint32_t I3C_GetMaxReadLength(void);

/** brief: Checks whether the MX_I3Cx Max Write Length Update (MWLUPD) flag is active.
  * retval: 1 if flag is active, 0 otherwise
  */
uint32_t I3C_IsActiveFlagMaxWriteLength(void);

/** brief: Clears the MX_I3Cx Max Write Length Update (MWLUPD) flag.
  * retval: none
  */
void I3C_ClearFlagMaxWriteLength(void);

/** brief: Gets the MX_I3Cx Max Write Length value.
  * retval: value of Max Write Length
  */
uint32_t I3C_GetMaxWriteLength(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_EXAMPLE_H */
