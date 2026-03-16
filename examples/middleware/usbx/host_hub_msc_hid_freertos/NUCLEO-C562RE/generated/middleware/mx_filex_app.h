/**
  ******************************************************************************
  * @file    app_filex.h
  * @brief   FileX applicative header file
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
#ifndef APP_FILEX_H
#define APP_FILEX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "fx_api.h"
#include "ux_api.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
UINT app_filex_init(void);
UINT App_File_Create(FX_MEDIA *fx_media);
UINT App_File_Write(FX_MEDIA *fx_media);
UINT App_File_Read(FX_MEDIA *fx_media);
/* Private defines -----------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif /* APP_FILEX_H */
