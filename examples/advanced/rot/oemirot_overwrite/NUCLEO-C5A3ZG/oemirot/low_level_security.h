/**
  ******************************************************************************
  * @file    low_level_security.h
  * @brief   Header for low_level_security.c module
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LOW_LEVEL_SECURITY_H
#define LOW_LEVEL_SECURITY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "boot_hal_cfg.h"
#include "flash_layout.h"

void  SECURITY_ApplyTamperEarlyStartup(void);
void  SECURITY_ApplyRamCfgEarlyStartup(void);
void  SECURITY_ApplyRunTimeProtections(void);
void  SECURITY_UpdateRunTimeProtections(void);
void  SECURITY_CheckStaticProtections(void);
void  SECURITY_UpdateLoaderRunTimeProtections(void);
void  SECURITY_SetLoaderCodeSecure(void);

#ifdef __cplusplus
}
#endif

#endif /* LOW_LEVEL_SECURITY_H */
