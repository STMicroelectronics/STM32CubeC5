/**
  ******************************************************************************
  * @file           : mx_filex_app.h
  * @brief          : Header for mx_filex_app.c file.
  *                  This file contains the common defines of the application.
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
#ifndef MX_FILEX_APP_H
#define MX_FILEX_APP_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "mx_filex_interfaces.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
UINT app_filex_init(void);
UINT app_filex_process(void);

/* Private defines -----------------------------------------------------------*/
/** logging macro - just redirects to printf()
  * libc's stdout is redirected to UART thanks to the Basic stdio utility.
  *
  * @user You can enable logs by defining USE_TRACE=1 in the build options.
  */
#if defined(USE_TRACE) && USE_TRACE != 0
#define PRINTF(...)    printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif /* defined(USE_TRACE) && USE_TRACE != 0 */
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* MX_FILEX_APP_H */
