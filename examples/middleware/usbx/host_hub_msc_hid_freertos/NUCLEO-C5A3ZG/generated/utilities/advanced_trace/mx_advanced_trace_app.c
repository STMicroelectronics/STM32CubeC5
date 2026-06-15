/**
  ******************************************************************************
  * @file    mx_advanced_trace_app.c
  * @brief   Advanced Trace initialization source file
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

/* Includes ------------------------------------------------------------------*/
#include "mx_advanced_trace_app.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/******************************************************************************/
/* Exported functions for Advanced Trace initialization                       */
/******************************************************************************/

/**
  * @brief  Initialize the Advanced Trace utility and apply the generated runtime configuration.
  * @retval SYSTEM_OK                Advanced Trace initialization completed successfully.
  * @retval SYSTEM_PERIPHERAL_ERROR  Advanced Trace initialization failed.
  */
system_status_t mx_adv_trace_init(void)
{
  system_status_t return_status = SYSTEM_PERIPHERAL_ERROR;

  /* Initialize the Advanced Trace utility. */
  if (ADV_TRACE_Init(ADV_TRACE_UART_GETHANDLE()) == ADV_TRACE_OK)
  {
    return_status = SYSTEM_OK;
  }




  /* Start the reception process. */
  (void)ADV_TRACE_StartRxProcess(adv_trace_rx_user_callback);


  return return_status;
}
/**
  * @brief  Default weak reception callback.
  * @param  p_data     Pointer to the received data buffer.
  * @param  size_byte  Number of received bytes.
  * @param  error      Reception error status.
  * @note   Override this function to handle received data in the application.
  */
__WEAK void adv_trace_rx_user_callback(uint8_t *p_data, uint16_t size_byte, uint8_t error)
{
  /* Default implementation intentionally left empty. */
  (void)p_data;
  (void)size_byte;
  (void)error;
}


