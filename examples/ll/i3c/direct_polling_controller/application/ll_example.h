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
/* Descriptor for a single I3C CCC (Common Command Code) operation. */
typedef struct
{
  uint8_t  target_addr;      /* 7-bit dynamic or static target address placed on the bus. */
  uint8_t  ccc;              /* 7-bit CCC opcode (direct or broadcast) per I3C specification section CCC. */
  uint32_t data_size_byte;   /* Number of data bytes associated with the CCC. */
  uint32_t direction;        /* Data phase direction: READ or WRITE. */
} i3c_ccc_desc_t;

/* Exported constants --------------------------------------------------------*/
/* @user MX_I3Cx descriptor operation number: update operation number to add or remove operations from
                                              DirectWriteRead_CCC_Descriptor                           */
#define I3C_DESC_OPERATION_NUM      (8U)

#ifndef USE_LL_APP_ERROR
#define USE_LL_APP_ERROR 0U
#endif /* USE_LL_APP_ERROR */

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** brief: Assign target dynamic address in polling mode for the MX_I3Cx instance.
  * param:  target_dynamic_addr     Target dynamic address.
  * retval: system_status_t Status of the operation (see system_status_t).
  */
system_status_t I3C_AssignDynamicAddress(uint8_t target_dynamic_addr);

/** brief: Builds the transfer context for a sequence of I3C CCC operations for the MX_I3Cx instance.
  * param: p_tc_data  Pointer to the control buffer to be filled with transfer descriptors.
  * param: ccc_desc   Pointer to an array of 8 CCC descriptor structures (see i3c_ccc_desc_t).
  */
void I3C_BuildCCCTransferContext(uint32_t *p_tc_data, const i3c_ccc_desc_t *ccc_desc);

/** brief: Executes an I3C transfer using the provided transfer context in polling mode for the MX_I3Cx instance.
  * param:  p_tc_data     Pointer to the control buffer describing the transfer sequence.
  * param:  tc_size_word  Number of control words in the transfer context buffer.
  * param:  p_tx_data     Pointer to the transmit payload buffer.
  * param:  tx_size_byte  Number of bytes to transmit.
  * param:  rx_data       Pointer to the receive buffer.
  * param:  rx_size_byte  Number of bytes to receive.
  */
void I3C_Transfer(const uint32_t *p_tc_data,
                  uint32_t tc_size_word,
                  const uint8_t *p_tx_data,
                  uint32_t tx_size_byte,
                  uint8_t *p_rx_data,
                  uint32_t rx_size_byte);

/** brief: This function provides delay (in milliseconds) based on SysTick counter flag.
  * param: delay_ms Number of milliseconds to delay.
  */
void DelayMs(uint32_t delay_ms);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_EXAMPLE_H */

