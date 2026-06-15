
/**
  ******************************************************************************
  * @file           : mx_levelx_app.h
  * @brief          : header for the mx_levelx_interfaces.c
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_levelx_license.md file
  * in the same directory as the generated code.
  * If no mx_levelx_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_LEVELX_APP_H
#define MX_LEVELX_APP_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "lx_api.h"
#include "mx_hal_def.h"
#include "mx_levelx_interfaces.h"

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/
UINT app_levelx_init(VOID);
UINT app_levelx_process(VOID);

/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* MX_LEVELX_APP_H */
