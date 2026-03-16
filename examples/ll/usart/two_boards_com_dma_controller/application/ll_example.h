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
#ifndef USE_LL_APP_ERROR
#define USE_LL_APP_ERROR 0U
#endif /* USE_LL_APP_ERROR */

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** brief: MX_USARTx transmits data in DMA mode.
  * param: p_tx_buf  Pointer to transmission data buffer.
  * param: size_byte Number of bytes to transfer.
  */
void USART_Transmit_DMA(const void *p_tx_buf, uint16_t size_byte);

/** brief: MX_USARTx receives data in DMA mode.
  * param: p_rx_buf  Pointer to reception data buffer.
  * param: size_byte Number of bytes to transfer.
  */
void USART_Receive_DMA(void *p_rx_buf, uint16_t size_byte);

/** brief:  MX_USARTx transfer complete callback. */
void USART_TxTransferCpltCallback(void);

/** brief:  MX_USARTx reception complete callback. */
void USART_RxTransferCpltCallback(void);

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/** brief:  MX_USARTx transfer error callback. */
void USART_TransferErrorCallback(void);
#endif /* USE_LL_APP_ERROR */

/** brief: This function provides delay (in milliseconds) based on SysTick counter flag.
  * param: delay_ms Number of milliseconds to delay.
  */
void DelayMs(uint32_t delay_ms);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_EXAMPLE_H */
