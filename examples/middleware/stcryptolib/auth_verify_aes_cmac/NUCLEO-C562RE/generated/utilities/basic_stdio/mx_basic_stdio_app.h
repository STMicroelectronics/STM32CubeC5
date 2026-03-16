/**
  ******************************************************************************
  * @file           : mx_basic_stdio_app.h
  * @brief          : header for mx_basic_stdio_app.c
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_basic_stdio_license.md file
  * in the same directory as the generated code.
  * If no mx_basic_stdio_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_BASIC_STDIO_APP_H
#define MX_BASIC_STDIO_APP_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "basic_stdio_core.h"
#include "mx_def.h"
#include "mx_hal_def.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Exported defines --------------------------------------------------------- */

/******************************************************************************/
/* Exported defines for mx_basic_stdio in basic stdio utility                 */
/******************************************************************************/
/* Utility interface handle */
#define BASIC_STDIO_UART_GETHANDLE      mx_usart2_uart_gethandle 

/* Exported functions ------------------------------------------------------- */

/******************************************************************************/
/* Exported functions for mx_basic_stdio in basic stdio utility               */
/******************************************************************************/

/**
  * @brief basic_stdio initialization function
  * This function call the BASIC_STDIO_Init function
  */
system_status_t mx_basic_stdio_init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_BASIC_STDIO_APP_H */
