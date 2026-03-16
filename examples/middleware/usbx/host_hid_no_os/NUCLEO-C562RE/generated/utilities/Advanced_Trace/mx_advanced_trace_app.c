/**
  ******************************************************************************
  * @file : mx_advanced_trace_app.c
  * @brief : advanced_trace mx file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_adv_trace_license.md file
  * in the same directory as the generated code.
  * If no mx_adv_trace_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mx_advanced_trace_app.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/******************************************************************************/
/* Exported functions for mx_advanced_trace in advanced_trace utility  */
/******************************************************************************/

system_status_t mx_adv_trace_init(void)
{
  system_status_t return_status = SYSTEM_PERIPHERAL_ERROR ;

  /* Trace initialization */

  if (ADV_TRACE_Init(adv_trace_gethandle()) == ADV_TRACE_OK)
  {
    return_status = SYSTEM_OK;
  }



  (void)ADV_TRACE_StartRxProcess(adv_trace_rx_user_callback);

  return return_status;
}
/**
  * @brief  Receive User callback function for handling data.
  *
  * This function is marked as __WEAK, meaning it can be overridden by a user-defined
  * implementation. It is intended to be used as a callback function for handling the received data.
  */
__WEAK void adv_trace_rx_user_callback(uint8_t *p_data, uint16_t size_byte, uint8_t error)
{
  /* Default implementation */
  return;
}


