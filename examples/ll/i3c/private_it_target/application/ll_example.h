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

/**
  * MX_I3Cx transfer context.
  */
typedef enum
{
  I3C_TRANSFER_RX = 0U,  /* MX_I3Cx receive data  */
  I3C_TRANSFER_TX = 1U,  /* MX_I3Cx transmit data */
} i3c_transfer_context_t;

/* Exported constants --------------------------------------------------------*/
#ifndef USE_LL_APP_ERROR
#define USE_LL_APP_ERROR 0U
#endif /* USE_LL_APP_ERROR */

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** brief: Activates the MX_I3Cx notification.
  */
void I3C_ActivateNotification(void);

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/** brief:  Gets the MX_I3Cx own dynamic address.
  * retval: Value of the own dynamic address.
  */
uint8_t I3C_GetOwnDynamicAddress(void);
#endif /* USE_LL_APP_ERROR */

/** brief: Starts a MX_I3Cx target receive operation in interrupt mode.
  * param: p_rx_data Pointer to the receive buffer.
  * param: size_byte Number of bytes expected to be received.
  */
void I3C_Receive_IT(uint8_t *p_rx_data, uint32_t size_byte);

/** brief: Starts a MX_I3Cx target transmit operation in interrupt mode.
  * param: p_tx_data Pointer to the transmit payload buffer.
  * param: size_byte Number of bytes to be transmitted.
  */
void I3C_Transmit_IT(const uint8_t *p_tx_data, uint32_t size_byte);

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/**
  * brief: This function provides delay (in milliseconds) based on SysTick counter flag.
  * param:  delay_ms Number of milliseconds to delay.
  */
void DelayMs(uint32_t delay_ms);
#endif /* USE_LL_APP_ERROR */

/** brief: MX_I3Cx DAA complete callbacks.
  */
void I3C_DAACompleteCallback(void);

/** brief: MX_I3Cx RX complete callbacks.
  */
void I3C_RxCompleteCallback(void);

/** brief: MX_I3Cx TX complete callbacks.
  */
void I3C_TxCompleteCallback(void);

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/** brief: MX_I3Cx error callbacks.
  */
void I3C_ErrorCallback(void);
#endif /* USE_LL_APP_ERROR */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_EXAMPLE_H */
