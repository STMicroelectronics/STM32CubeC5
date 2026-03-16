/**
  ******************************************************************************
  * file           : mx_levelx_app.h
  * brief          : Header for mx_levelx_app.c
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
#ifndef MX_LEVELX_APP_H
#define MX_LEVELX_APP_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "mx_levelx_interfaces.h"
#include "lx_port.h"
/* Exported functions prototypes ---------------------------------------------*/
UINT app_levelx_init(VOID);
UINT app_levelx_process(VOID);

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
#endif /* MX_LEVELX_APP_H */
