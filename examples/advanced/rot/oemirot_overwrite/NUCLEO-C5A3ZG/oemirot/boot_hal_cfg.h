/**
  ******************************************************************************
  * @file    boot_hal_cfg.h
  * @brief   Configuration flags for OEMiRoT
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BOOT_HAL_CFG_H
#define BOOT_HAL_CFG_H

/* Includes ------------------------------------------------------------------*/
#include "mx_hal_def.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Exported macro ------------------------------------------------------------*/

/* OB RDP_LEVEL minimal check */
/* Do not change manually. Automatically configured here by provisioning script according to user choice */
#define OEMIROT_OB_RDP_LEVEL_MIN HAL_FLASH_ITF_OB_RDP_LEVEL_0

/* Tampers protection activation */
#define OEMIROT_TAMPER_ENABLE

/* MPU protection activation */
#define OEMIROT_MPU_ENABLE

/* Flow control protection activation */
#define OEMIROT_FLOW_CONTROL

/* Bypass images control on wake-up from Low power */
#define OEMIROT_FAST_WAKE_UP

#if defined(OEMIROT_DEV_MODE)
/* Error handler stops execution (instead of resetting) */
#define OEMIROT_STOP_ON_ERROR
#endif /* OEMIROT_DEV_MODE */

/* Exported types ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BOOT_HAL_CFG_H */
