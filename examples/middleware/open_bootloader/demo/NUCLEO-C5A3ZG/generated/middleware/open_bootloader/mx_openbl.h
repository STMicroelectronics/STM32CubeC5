/**
  ******************************************************************************
  * @file    mx_openbl.h
  * @brief   Header for mx_openbl.c
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_freertos_license.md file
  * in the same directory as the generated code.
  * If no mx_freertos_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_OPENBL_H
#define MX_OPENBL_H

/* Includes ------------------------------------------------------------------*/
#include "openbl_types.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

openbl_status_t mx_openbl_init(void);
openbl_status_t mx_openbl_process(void);
uint32_t mx_openbl_get_detected_itf(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_OPENBL_H */
