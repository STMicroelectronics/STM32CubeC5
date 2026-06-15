/**
  ******************************************************************************
  * @file           : mx_advanced_trace_app.h
  * @brief          : Header for the Advanced Trace initialization source file
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
/* Private function prototypes -----------------------------------------------*/

/* Exported defines ----------------------------------------------------------*/

/******************************************************************************/
/* Exported defines for Advanced Trace initialization                         */
/******************************************************************************/

/* Advanced Trace UART handle accessor. */
#define ADV_TRACE_UART_GETHANDLE      mx_usart2_uart_gethandle


#define MX_TS_OFF                               (0)  /*!< Log without timestamp. */


/* Exported functions --------------------------------------------------------*/

/******************************************************************************/
/* Exported functions for Advanced Trace initialization                       */
/******************************************************************************/
/**
  * @brief  Initialize the Advanced Trace utility and apply the generated runtime configuration.
  * @retval SYSTEM_OK                Advanced Trace initialization completed successfully.
  * @retval SYSTEM_PERIPHERAL_ERROR  Advanced Trace initialization failed.
  */
system_status_t mx_adv_trace_init(void);

/**
  * @brief  Default weak reception callback prototype.
  * @param  p_data     Pointer to the received data buffer.
  * @param  size_byte  Number of received bytes.
  * @param  error      Reception error status.
  * @note   Override this function to handle received data in the application.
  */
void adv_trace_rx_user_callback(uint8_t *p_data, uint16_t size_byte, uint8_t error);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_ADV_TRACE_H */
