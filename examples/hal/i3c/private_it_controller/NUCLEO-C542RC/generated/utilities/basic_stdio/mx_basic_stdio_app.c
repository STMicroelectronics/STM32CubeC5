/**
  ******************************************************************************
  * @file : mx_basic_stdio_app.c
  * @brief : basic_stdio mx file
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

/* Includes ------------------------------------------------------------------*/
#include "mx_basic_stdio_app.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/******************************************************************************/
/* Exported functions for mx_basic_stdio in basic_stdio utility  */
/******************************************************************************/

system_status_t mx_basic_stdio_init(void)
{
  system_status_t return_status = SYSTEM_OK ;

  /* Trace initialization */
  BASIC_STDIO_Init(BASIC_STDIO_UART_GETHANDLE());
  return return_status;
}
