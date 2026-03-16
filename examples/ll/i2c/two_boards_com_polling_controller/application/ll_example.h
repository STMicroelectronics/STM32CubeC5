/**
  ******************************************************************************
  * file           : ll_example.h
  * brief          : Header for stm32ynxx_ll_example.c files.
  *                  This file contains example-specific declarations to interface with example.c functions.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LL_EXAMPLE_H
#define LL_EXAMPLE_H

/* Includes ------------------------------------------------------------------*/
#include "mx_hal_def.h"       /* aliases to the target-specific generated code */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#ifndef USE_LL_APP_TIMEOUT
#define USE_LL_APP_TIMEOUT 0U
#endif /* USE_LL_APP_TIMEOUT */

#ifndef USE_LL_APP_ERROR
#define USE_LL_APP_ERROR 0U
#endif /* USE_LL_APP_ERROR */

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** brief:  MX_I2Cx transmit an amount of data in blocking mode (polling mode).
  * param:  device_address Target device address.
  * param:  p_tx_buffer    Pointer to transmission data buffer.
  * param:  size_byte      Number of bytes to transfer.
  * param:  timeout_ms     Timeout duration in millisecond (This parameter is unused when USE_LL_APP_TIMEOUT == 0).
  * retval: system status (see system_status_t).
  */
system_status_t I2C_Transmit(uint32_t device_address, const void *p_tx_buffer, uint32_t size_byte, uint32_t timeout_ms);

/** brief:  MX_I2Cx receive an amount of data in blocking mode (polling mode).
  * param:  device_address Target device address.
  * param:  p_rx_buffer    Pointer to reception data buffer.
  * param:  size_byte      Number of bytes to transfer.
  * param:  timeout_ms     Timeout duration in millisecond.
  * retval: system status (see system_status_t).
  */
system_status_t I2C_Receive(uint32_t device_address, void *p_rx_buffer, uint32_t size_byte, uint32_t timeout_ms);

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/** brief: This function provides delay (in milliseconds) based on SysTick counter flag.
  * param: delay_ms Number of milliseconds to delay.
  */
void DelayMs(uint32_t delay_ms);
#endif /* USE_LL_APP_ERROR */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_EXAMPLE_H */
