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
/* Descriptor for a single MX_I3Cx private transfer frame (Controller side). */
typedef struct
{
  uint8_t  tgt_addr;        /* 7-bit dynamic or static target address placed on the bus. */
  uint32_t data_size_byte;  /* Number of data bytes in the private data phase. */
  uint32_t direction;       /* Data phase direction: READ or WRITE. */
} i3c_private_desc_t;

/**
  * MX_I3Cx transfer context.
  */
typedef enum
{
  I3C_TRANSFER_DAA   = 0U,  /* MX_I3Cx DAA data                  */
  I3C_TRANSFER_TX_RX = 1U,  /* MX_I3Cx transmit and receive data */
} i3c_transfer_context_t;

/* Exported constants --------------------------------------------------------*/
#ifndef USE_LL_APP_ERROR
#define USE_LL_APP_ERROR 0U
#endif /* USE_LL_APP_ERROR */

/* Number of private descriptors used for private transfers */
#define CONTROL_BUFFER_SIZE        (2U)

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** brief: Assign target dynamic address in interrupt mode for the MX_I3Cx instance.
  */
void I3C_AssignDynamicAddr_IT(void);

/** brief:  Controller set dynamic address.
  * param:  target_address Target dynamic address.
  */
void I3C_SetDynamicAddr(uint64_t target_address);

/** brief: Builds a control-word transfer context for a sequence of private frames for the MX_I3Cx instance.
  * param p_tc_data: Pointer to the control buffer to be filled.
  * param p_desc:    Pointer to an array of private transfer descriptors.
  */
void I3C_BuildPrivateTransferContext(uint32_t *p_tc_data, const i3c_private_desc_t *p_desc);

/** brief: Starts a private transfer in interrupt mode for the MX_I3Cx instance.
  * param  p_tc_data:    Pointer to the control buffer describing the transfer.
  * param  tc_size_word: Number of control words in the control buffer.
  * param  p_tx_data:    Pointer to the transmit payload buffer.
  * param  tx_size_byte: Number of bytes to transmit.
  * param  p_rx_data:    Pointer to the receive buffer.
  * param  rx_size_byte: Number of bytes to receive.
  */
void I3C_Transfer_IT(const uint32_t *p_tc_data,
                     uint32_t tc_size_word,
                     const uint8_t *p_tx_data,
                     uint32_t tx_size_byte,
                     uint8_t *p_rx_data,
                     uint32_t rx_size_byte);

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/**
  * brief: This function provides delay (in milliseconds) based on SysTick counter flag.
  * param delay_ms: Number of milliseconds to delay.
  */
void DelayMs(uint32_t delay_ms);
#endif /* USE_LL_APP_ERROR */

/** brief: MX_I3Cx target request dynamic address callbacks.
  */
void I3C_TargetReqDynamicAddrCallback(void);

/** brief: MX_I3Cx DAA complete callbacks.
  */
void I3C_DAACompleteCallback(void);

/** brief: MX_I3Cx private transfer complete callbacks.
  */
void I3C_TransferCompleteCallback(void);

/** brief: MX_I3Cx error callback.
  */
void I3C_ErrorCallback(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_EXAMPLE_H */
