/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : I3C direct IT controller example with LL API
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I3C_TRANSFER_MAX_BYTE  (16U)

#define I3C_TX_IT (LL_I3C_IER_FCIE | LL_I3C_IER_CFNFIE | LL_I3C_IER_TXFNFIE) /* Controller TX/DAA IT */
#define I3C_RX_IT (LL_I3C_IER_FCIE | LL_I3C_IER_CFNFIE | LL_I3C_IER_RXFNEIE) /* Controller RX IT     */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/**
  * I3C transfer context used by the interrupt handlers.
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
static void I3C_TransmitByteTreatment_IT(const uint8_t **p_tx_data, uint32_t *p_tx_size_byte);
static void I3C_ReceiveByteTreatment_IT(uint8_t **p_rx_data, uint32_t *p_rx_size_byte);
void mx_i3cx_ev_irqhandler(void);
void mx_i3cx_err_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/** brief: Assign target dynamic address in interrupt mode for the MX_I3Cx instance.
  */
inline void I3C_AssignDynamicAddr_IT(void)
{
  context = I3C_TRANSFER_DAA;

  LL_I3C_EnableIT(MX_I3Cx, (I3C_TX_IT | LL_I3C_IER_ERRIE));

  LL_I3C_EnableArbitrationHeader(MX_I3Cx);

  /* Start ENTDAA CCC (Dynamic Address Assignment). */
  LL_I3C_ControllerHandleCCC(MX_I3Cx, LL_I3C_BROADCAST_ENTDAA, 0UL, LL_I3C_GENERATE_STOP);
}


/** brief: Controller set dynamic address.
  * param: target_address Target dynamic address.
  */
inline void I3C_SetDynamicAddr(uint64_t target_address)
{
  LL_I3C_TransmitData8(MX_I3Cx, (uint8_t)target_address);
}


/** brief: Builds the transfer context for a sequence of I3C CCC operations for the MX_I3Cx instance.
  * param: tc_data  Pointer to the control buffer to be filled with transfer descriptors.
  * param: ccc_desc Pointer to an array of 8 CCC descriptor structures (see i3c_ccc_desc_t).
  */
inline void I3C_BuildCCCTransferContext(uint32_t *p_tc_data, const i3c_ccc_desc_t *ccc_desc)
{
  const uint32_t nb_define_bytes = ((uint32_t)LL_I3C_CONTROLLER_MTYPE_DIRECT & (uint32_t)LL_I3C_DEFINE_BYTE);

  for (uint32_t i = 0U; i < I3C_DESC_OPERATION_NUM; i++)
  {
    uint32_t stop_condition = 0U;

    if (i == (I3C_DESC_OPERATION_NUM - 1U))
    {
      stop_condition = (uint32_t)LL_I3C_GENERATE_STOP;
    }

    /* Fill control buffer with CCC command and target address */
    p_tc_data[2U * i] = nb_define_bytes | ((uint32_t)ccc_desc[i].ccc << I3C_CR_CCC_Pos)
                        | (uint32_t)LL_I3C_CONTROLLER_MTYPE_CCC | (uint32_t)LL_I3C_GENERATE_RESTART;
    p_tc_data[(2U * i) + 1U] = (ccc_desc[i].data_size_byte - nb_define_bytes) | ccc_desc[i].direction |
                               ((uint32_t)ccc_desc[i].target_addr << I3C_CR_ADD_Pos)
                               | (uint32_t)LL_I3C_CONTROLLER_MTYPE_DIRECT | stop_condition;
  }
}


/** brief: Starts a direct CCC transfer sequence in interrupt mode for the MX_I3Cx instance.
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
  I3C_TransmitByteTreatment_IT(&TxData, &TxCount);
  I3C_ReceiveByteTreatment_IT(&RxData, &RxCount);

  /* Enable TX/RX IT only when not in DAA phase. */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_I3C_EnableIT(MX_I3Cx, ((I3C_TX_IT | I3C_RX_IT) | LL_I3C_IER_ERRIE));
#else
  LL_I3C_EnableIT(MX_I3Cx, (I3C_TX_IT | I3C_RX_IT));
#endif /* USE_LL_APP_ERROR */

  LL_I3C_RequestTransfer(MX_I3Cx);
}


/**
  * Handles the MX_I3Cx event interrupts.
  */
void mx_i3cx_ev_irqhandler(void)
{
  uint64_t target_payload = 0UL;

  /* DAA (ENTDAA) context */
  if (context == I3C_TRANSFER_DAA)
  {
    /* handles the CCC DAA command. */
    if (LL_I3C_IsActiveMaskFlag_TXFNF(MX_I3Cx) != 0U)
    {

      /* Read 8 bytes of target payload (PID/BCR/DCR) from RX FIFO. */
      for (uint32_t index = 0U; index < 8U; index++)
      {
        target_payload |= ((uint64_t)LL_I3C_ReceiveData8(MX_I3Cx) << (index * 8U));
      }

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
  /* Direct CCC Tx RX context */
  else
  {
    if (LL_I3C_IsActiveMaskFlag_CFNF(MX_I3Cx) != 0U)
    {
      I3C_ControlDataTreatment(&TcData, &TcCount);
    }

    if (LL_I3C_IsActiveMaskFlag_TXFNF(MX_I3Cx) != 0U)
    {
      I3C_TransmitByteTreatment_IT(&TxData, &TxCount);
    }

    if (LL_I3C_IsActiveMaskFlag_RXFNE(MX_I3Cx) != 0U)
    {
      I3C_ReceiveByteTreatment_IT(&RxData, &RxCount);
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
#endif /* USE_LL_APP_ERROR*/

  I3C_ErrorCallback();
}


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

/** brief: Fills the MX_I3Cx TX FIFO with bytes as long as space remains.
  * param  tx_data:      Pointer to current TX pointer.
  * param  tx_size_byte: Pointer to remaining TX bytes.
  */
static void I3C_TransmitByteTreatment_IT(const uint8_t **p_tx_data, uint32_t *p_tx_size_byte)
{
  uint32_t transfer_count = *p_tx_size_byte;

  if (transfer_count > I3C_TRANSFER_MAX_BYTE)
  {
    transfer_count = I3C_TRANSFER_MAX_BYTE;
  }

  /* Transmit transfer_count bytes while TX FIFO not full */
  while ((LL_I3C_IsActiveFlag_TXFNF(MX_I3Cx) != 0U) && (transfer_count != 0U) && (*p_tx_size_byte != 0U))
  {
    LL_I3C_TransmitData8(MX_I3Cx, **p_tx_data);
    (*p_tx_data)++;
    (*p_tx_size_byte)--;
    transfer_count--;
  }
}


/** brief: Drains the MX_I3Cx RX FIFO into the user buffer using byte reads.
  * param  rx_data:      Pointer to current RX pointer.
  * param  rx_size_byte: Pointer to remaining RX bytes.
  */
static void I3C_ReceiveByteTreatment_IT(uint8_t **p_rx_data, uint32_t *p_rx_size_byte)
{
  uint32_t transfer_count = *p_rx_size_byte;

  if (transfer_count > I3C_TRANSFER_MAX_BYTE)
  {
    transfer_count = I3C_TRANSFER_MAX_BYTE;
  }

  /* Receive transfer_count bytes while RX FIFO not empty */
  while ((LL_I3C_IsActiveFlag_RXFNE(MX_I3Cx) != 0U) && (transfer_count != 0U) && (*p_rx_size_byte != 0U))
  {
    **p_rx_data = LL_I3C_ReceiveData8(MX_I3Cx);
    (*p_rx_data)++;
    (*p_rx_size_byte)--;
    transfer_count--;
  }
}