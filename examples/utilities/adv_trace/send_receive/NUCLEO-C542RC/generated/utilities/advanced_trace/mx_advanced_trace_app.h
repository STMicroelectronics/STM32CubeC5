/**
  ******************************************************************************
  * @file           : mx_advanced_trace_app.h
  * @brief          : header for mx_advanced_trace_app.c
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_advanced_trace_license.md file
  * in the same directory as the generated code.
  * If no mx_advanced_trace_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_ADV_TRACE_H
#define MX_ADV_TRACE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "adv_trace_core.h"
#include "mx_def.h"
#include "mx_hal_def.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Exported defines --------------------------------------------------------- */

/******************************************************************************/
/* Exported defines for mx_advanced_trace in advanced_trace utility                     */
/******************************************************************************/

/* Utility interface handle */
#define ADV_TRACE_UART_GETHANDLE      mx_usart2_uart_gethandle 

#define MX_TS_OFF (0)            /*!< Log without TimeStamp */

/* Exported functions ------------------------------------------------------- */

/******************************************************************************/
/* Exported functions for mx_advanced_trace in advanced_trace utility  */
/******************************************************************************/
/**
  * @brief advanced_trace initialization function
  * This function call the advanced_trace init function
  */
system_status_t mx_adv_trace_init(void);

/**
  * @brief  Receive User callback function for handling data.
  *
  * This function is marked as __WEAK, meaning it can be overridden by a user-defined
  * implementation. It is intended to be used as a callback function for handling the received data.
  */
void adv_trace_rx_user_callback(uint8_t *p_data, uint16_t size_byte, uint8_t error);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_ADV_TRACE_H */
