/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : I3C private IT controller example with LL API
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

/* Includes ------------------------------------------------------------------*/
#include "ll_example.h"

/* Private define ------------------------------------------------------------*/
#define I3C_TRANSFER_MAX_WORD  (4U)

#define I3C_TX_IT  (LL_I3C_IER_FCIE | LL_I3C_IER_CFNFIE | LL_I3C_IER_TXFNFIE) /* Controller TX/DAA IT */
#define I3C_RX_IT  (LL_I3C_IER_FCIE | LL_I3C_IER_CFNFIE | LL_I3C_IER_RXFNEIE) /* Controller RX IT     */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
  * Transfer context for the MX_I3Cx instance, used by the interrupt handlers.
  * TcData/TcCount: current control-word pointer and remaining 32-bit words to write into CR (Control FIFO).
  * TxData/TxCount, RxData/RxCount: current buffer pointers and remaining payload bytes to transmit/receive.
  */
static const uint32_t *TcData;
static const uint8_t *TxData;
static uint8_t *RxData;
static uint32_t TcCount;
static uint32_t TxCount;
static uint32_t RxCount;
static i3c_transfer_context_t context;

/* Private functions prototype -----------------------------------------------*/
static void I3C_ControlDataTreatment(const uint32_t **p_tc_data, uint32_t *p_tc_size_word);
static void I3C_TransmitWordTreatment_IT(const uint8_t **p_tx_data, uint32_t *p_tx_size_byte);
static void I3C_ReceiveWordTreatment_IT(uint8_t **p_rx_data, uint32_t *p_rx_size_byte);
void mx_i3cx_ev_irqhandler(void);
void mx_i3cx_err_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/** brief: Builds a control-word transfer context for a sequence of private frames for the MX_I3Cx instance.
  * param: p_tc_data Pointer to the control buffer to be filled.
  * param: p_desc    Pointer to an array of private transfer descriptors.
  */
inline void I3C_BuildPrivateTransferContext(uint32_t *p_tc_data, const i3c_private_desc_t *p_desc)
{
  for (uint32_t i = 0UL; i < CONTROL_BUFFER_SIZE; i++)
  {
    p_tc_data[i] = (p_desc[i].data_size_byte
                    | p_desc[i].direction
                    | (p_desc[i].tgt_addr << I3C_CR_ADD_Pos)
                    | ((uint32_t)(LL_I3C_GENERATE_STOP | LL_I3C_CONTROLLER_MTYPE_PRIVATE)));
  }
}


/** brief: Assign target dynamic address in interrupt mode for the MX_I3Cx instance.
  */
inline void I3C_AssignDynamicAddr_IT(void)
{
  context = I3C_TRANSFER_DAA;

  LL_I3C_EnableIT(MX_I3Cx, (I3C_TX_IT | LL_I3C_IER_ERRIE));

  LL_I3C_EnableArbitrationHeader(MX_I3Cx);

  /* Write ENTDAA CCC information in the control register */
  LL_I3C_ControllerHandleCCC(MX_I3Cx, LL_I3C_BROADCAST_ENTDAA, 0UL, LL_I3C_GENERATE_STOP);
}


/** brief:  Controller set dynamic address.
  * param:  target_address Target dynamic address.
  */
inline void I3C_SetDynamicAddr(uint64_t target_address)
{
  LL_I3C_TransmitData8(MX_I3Cx, (uint8_t)target_address);
}


/** brief: Starts a private transfer in interrupt mode for the MX_I3Cx instance.
  * param p_tc_data:    Pointer to the control buffer describing the transfer.
  * param tc_size_word: Number of control words in the control buffer.
  * param p_tx_data:    Pointer to the transmit buffer.
  * param tx_size_byte: Number of bytes to transmit.
  * param p_rx_data:    Pointer to the receive buffer.
  * param rx_size_byte: Number of bytes to receive.
  */
inline void I3C_Transfer_IT(const uint32_t *p_tc_data,
                            uint32_t tc_size_word,
                            const uint8_t *p_tx_data,
                            uint32_t tx_size_byte,
                            uint8_t *p_rx_data,
                            uint32_t rx_size_byte)
{
  context = I3C_TRANSFER_TX_RX;
  TcData  = p_tc_data;
  TcCount = tc_size_word;
  TxData  = p_tx_data;
  TxCount = tx_size_byte;
  RxData  = p_rx_data;
  RxCount = rx_size_byte;

  /* Pre-fill TX and RX FIFOs */
  I3C_TransmitWordTreatment_IT(&TxData, &TxCount);
  I3C_ReceiveWordTreatment_IT(&RxData, &RxCount);

  /* Enable TX/RX IT only when not in DAA phase. */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_I3C_EnableIT(MX_I3Cx, ((I3C_TX_IT | I3C_RX_IT) | LL_I3C_IER_ERRIE));
#else
  LL_I3C_EnableIT(MX_I3Cx, (I3C_TX_IT | I3C_RX_IT));
#endif /* USE_LL_APP_ERROR */

  LL_I3C_RequestTransfer(MX_I3Cx);
}


/** brief: Handles the MX_I3Cx event interrupts.
  */
void mx_i3cx_ev_irqhandler(void)
{
  uint64_t tgt_payload = 0UL;

  /* DAA (ENTDAA) context */
  if (context == I3C_TRANSFER_DAA)
  {
    /* handles the CCC DAA command. */
    if (LL_I3C_IsActiveMaskFlag_TXFNF(MX_I3Cx) != 0U)
    {
      /* Retrieve the 64 bits payload used to handle the target request dynamic address callback */
      tgt_payload = (uint64_t)LL_I3C_ReceiveData32(I3C1);
      tgt_payload |= (uint64_t)(((uint64_t)LL_I3C_ReceiveData32(I3C1)) << 32U);

      I3C_TargetReqDynamicAddrCallback();
    }

    /* Check frame complete flag. */
    if (LL_I3C_IsActiveMaskFlag_FC(MX_I3Cx) != 0U)
    {
      LL_I3C_DisableIT(MX_I3Cx, I3C_TX_IT);

      LL_I3C_ClearFlag_FC(MX_I3Cx);

      I3C_DAACompleteCallback();
    }
  }
  /* Private Tx RX context */
  else
  {
    if (LL_I3C_IsActiveMaskFlag_CFNF(MX_I3Cx) != 0U)
    {
      I3C_ControlDataTreatment(&TcData, &TcCount);
    }

    if (LL_I3C_IsActiveMaskFlag_TXFNF(MX_I3Cx) != 0U)
    {
      I3C_TransmitWordTreatment_IT(&TxData, &TxCount);
    }

    if (LL_I3C_IsActiveMaskFlag_RXFNE(MX_I3Cx) != 0U)
    {
      I3C_ReceiveWordTreatment_IT(&RxData, &RxCount);
    }

    /* Frame complete event. */
    if (LL_I3C_IsActiveMaskFlag_FC(MX_I3Cx) != 0U)
    {
      LL_I3C_ClearFlag_FC(MX_I3Cx);

      if (TcCount == 0U)
      {
        LL_I3C_DisableIT(MX_I3Cx, (I3C_TX_IT | I3C_RX_IT));

        I3C_TransferCompleteCallback();
      }
      else
      {
        LL_I3C_RequestTransfer(MX_I3Cx);
      }
    }
  }
}


/** brief: Handles the MX_I3Cx error interrupts.
  */
void mx_i3cx_err_irqhandler(void)
{
  LL_I3C_ClearFlag_ERR(MX_I3Cx);

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_I3C_DisableIT(MX_I3Cx, I3C_RX_IT | I3C_TX_IT);

  /* Flush the different Fifos to generate an automatic stop mode link to underflow or overflow detection timeout */
  LL_I3C_RequestFifosFlush(MX_I3Cx, (I3C_CFGR_TXFLUSH | I3C_CFGR_RXFLUSH | I3C_CFGR_SFLUSH | I3C_CFGR_CFLUSH));
#endif /* USE_LL_APP_ERROR*/

  I3C_ErrorCallback();
}


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/**
  * brief: This function provides delay (in milliseconds) based on SysTick counter flag.
  * param: delay_ms Number of milliseconds to delay.
  */
inline void DelayMs(uint32_t delay_ms)
{
  LL_Delay_NoISR(delay_ms);
}
#endif /* USE_LL_APP_ERROR */


/** brief: Writes one control word into the MX_I3Cx Control FIFO when space is available.
  * param: p_tc_data      Pointer to current control-word pointer.
  * param: p_tc_size_word Pointer to remaining control words.
  */
static void I3C_ControlDataTreatment(const uint32_t **p_tc_data, uint32_t *p_tc_size_word)
{
  if ((LL_I3C_IsActiveFlag_CFNF(MX_I3Cx) != 0U) && (*p_tc_size_word > 0U))
  {
    LL_I3C_WRITE_REG(MX_I3Cx, CR, **p_tc_data);
    (*p_tc_data)++;
    (*p_tc_size_word)--;
  }
}


/** brief: Fills the MX_I3Cx TX FIFO by word, at most I3C_TRANSFER_MAX_WORD.
  * param: p_tx_data      Pointer to current TX pointer.
  * param: p_tx_size_byte Pointer to remaining TX bytes.
  */
static void I3C_TransmitWordTreatment_IT(const uint8_t **p_tx_data, uint32_t *p_tx_size_byte)
{
  uint32_t transfer_count = *p_tx_size_byte / 4U;

  if (transfer_count > I3C_TRANSFER_MAX_WORD)
  {
    transfer_count = I3C_TRANSFER_MAX_WORD;
  }

  /* Transmit transfer_count while TX FIFO not full */
  while ((LL_I3C_IsActiveFlag_TXFNF(MX_I3Cx) != 0U) && (transfer_count != 0U))
  {
    LL_I3C_TransmitData32(MX_I3Cx, *((const uint32_t *)(uint32_t)(*p_tx_data)));
    (*p_tx_data) += 4U;
    *p_tx_size_byte -= 4U;

    transfer_count--;
  }
}


/** brief: Drains the MX_I3Cx RX FIFO by word, at most I3C_TRANSFER_MAX_WORD.
  * param: p_rx_data      Pointer to current RX pointer.
  * param: p_rx_size_byte Pointer to remaining RX bytes.
  */
static void I3C_ReceiveWordTreatment_IT(uint8_t **p_rx_data, uint32_t *p_rx_size_byte)
{
  uint32_t transfer_count = *p_rx_size_byte / 4U;

  if (transfer_count > I3C_TRANSFER_MAX_WORD)
  {
    transfer_count = I3C_TRANSFER_MAX_WORD;
  }

  /* Receive transfer_count while RX FIFO not empty */
  while ((LL_I3C_IsActiveFlag_RXFNE(MX_I3Cx) != 0U) && (transfer_count != 0U))
  {
    *((uint32_t *)(uint32_t)(*p_rx_data)) = LL_I3C_ReceiveData32(MX_I3Cx);
    (*p_rx_data) += 4U;
    *p_rx_size_byte -= 4U;

    transfer_count--;
  }
}
