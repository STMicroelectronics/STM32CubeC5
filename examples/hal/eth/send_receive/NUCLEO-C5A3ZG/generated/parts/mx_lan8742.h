/**
  ******************************************************************************
  * file           : mx_lan8742.h
  * brief          : Header for mx_lan8742.c file.
  ******************************************************************************
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
#ifndef __MX_LAN8742_H
#define __MX_LAN8742_H

/* Includes ------------------------------------------------------------------*/
#include "lan8742.h"
#include "mx_hal_def.h" /* aliases to the target-specific generated code */

#ifdef __cplusplus
extern "C" {
#endif
/* Private defines -----------------------------------------------------------*/

/* LAN8742 device ID */
#define MX_LAN8742_0  0
/* PHY address for LAN8742 device */
#define MX_LAN8742_0_ETH_ADDRESS     0

/* Private macros ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
lan8742_obj_t *mx_lan8742_getObj(void);
#ifdef __cplusplus
}
#endif

#endif /* __MX_LAN8742_H */