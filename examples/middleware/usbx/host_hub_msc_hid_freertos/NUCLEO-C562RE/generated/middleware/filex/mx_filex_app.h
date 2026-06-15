/**
  ******************************************************************************
  * @file           : mx_filex_app.h
  * @brief          : Header for mx_filex_app.c file.
  *                  This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_filex_license.md file
  * in the same directory as the generated code.
  * If no mx_filex_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __MX_FILEX_APP_H__
#define __MX_FILEX_APP_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "fx_api.h"
#include "mx_filex_interfaces.h"
#include "ux_api.h"

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
UINT app_filex_init(VOID);
UINT App_File_Create(FX_MEDIA *fx_media);
UINT App_File_Write(FX_MEDIA *fx_media);
UINT App_File_Read(FX_MEDIA *fx_media);
/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif /* __MX_FILEX_APP_H__ */
