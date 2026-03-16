/**
  ******************************************************************************
  * @file    com.h
  * @brief   This file contains definitions for COM module functionalities.
  ******************************************************************************
  * @attention
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
#ifndef COM_H
#define COM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include "mx_hal_def.h"

/* Constants used by Serial Command Line Mode */
#define TX_TIMEOUT          ((uint32_t)100U)
#define RX_TIMEOUT          ((uint32_t)2000U)

/* Maximum Timeout values for flags waiting loops.
   You might modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#define COM_UART_TIMEOUT_MAX                    1000U

hal_status_t  COM_Init(void);
hal_status_t  COM_DeInit(void);
hal_status_t  COM_Transmit(uint8_t *Data, uint16_t uDataLength, uint32_t uTimeout);
hal_status_t  COM_Transmit_Y(uint8_t *Data, uint16_t uDataLength, uint32_t uTimeout);

hal_status_t  COM_Receive(uint8_t *Data, uint16_t uDataLength, uint32_t uTimeout);
hal_status_t  COM_Flush(void);
hal_status_t  COM_Transmit_Y(uint8_t *Data, uint16_t uDataLength, uint32_t uTimeout);
hal_status_t  COM_Receive_Y(uint8_t *Data, uint16_t uDataLength, uint32_t uTimeout);
hal_status_t  COM_Y_On(uint8_t AbortChar);
hal_status_t  COM_Y_Off(void);
hal_status_t Ymodem_HeaderPktRxCpltCallback(uint32_t uFlashDestination, uint32_t uFileSize);
hal_status_t Ymodem_DataPktRxCpltCallback(uint8_t *pData, uint32_t uFlashDestination, uint32_t uSize);

#ifdef __cplusplus
}
#endif

#endif /* COM_H */

