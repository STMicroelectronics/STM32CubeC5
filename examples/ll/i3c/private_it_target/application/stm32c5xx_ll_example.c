/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : I3C private IT target example with LL API
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

#define I3C_TX_IT (LL_I3C_IER_FCIE | LL_I3C_IER_TXFNFIE) /* Target TX IT */
#define I3C_RX_IT (LL_I3C_IER_FCIE | LL_I3C_IER_RXFNEIE) /* Target RX IT */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
  * Transfer context for the MX_I3Cx instance, used by the interrupt handlers.
  * TxData/TxCount, RxData/RxCount: current buffer pointers and remaining payload bytes to transmit/receive.
  */
static const uint8_t *TxData;
static uint8_t *RxData;
static uint32_t TxCount;
static uint32_t RxCount;
static i3c_transfer_context_t context;

/* Private functions prototype -----------------------------------------------*/
static void I3C_TransmitWordTreatment_IT(const uint8_t **p_tx_data, uint32_t *p_tx_size_byte);
static void I3C_ReceiveWordTreatment_IT(uint8_t **p_rx_data, uint32_t *p_rx_size_byte);
void mx_i3cx_ev_irqhandler(void);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
void mx_i3cx_err_irqhandler(void);
#endif /* USE_LL_APP_ERROR */

/* Private functions ---------------------------------------------------------*/

/** brief: Activates the MX_I3Cx notification.
  */
inline void I3C_ActivateNotification(void)
{
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_I3C_EnableIT(MX_I3Cx, (LL_I3C_IER_DAUPDIE | LL_I3C_IER_ERRIE));
#else
  LL_I3C_EnableIT(MX_I3Cx, (LL_I3C_IER_DAUPDIE));
#endif /* USE_LL_APP_ERROR */
}

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/** brief:  Gets the MX_I3Cx own dynamic address.
  * retval: Value of the own dynamic address.
  */
inline uint8_t I3C_GetOwnDynamicAddress(void)
{
  return (LL_I3C_GetOwnDynamicAddress(MX_I3Cx));
}
#endif /* USE_LL_APP_ERROR */

/** brief: Starts a MX_I3Cx target receive operation in interrupt mode.
  * param: p_rx_data Pointer to the receive buffer.
  * param: size_byte Number of bytes expected to be received.
  */
inline void I3C_Receive_IT(uint8_t *p_rx_data, uint32_t size_byte)
{
  context = I3C_TRANSFER_RX;
  RxData  = p_rx_data;
  RxCount = size_byte;

  /* Pre-fill RX FIFO */
  I3C_ReceiveWordTreatment_IT(&RxData, &RxCount);

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_I3C_EnableIT(MX_I3Cx, (I3C_RX_IT | LL_I3C_IER_ERRIE));
#else
  LL_I3C_EnableIT(MX_I3Cx, (I3C_RX_IT));
#endif /* USE_LL_APP_ERROR */
}


/** brief: Starts a MX_I3Cx target transmit operation in interrupt mode.
  * param: p_tx_data Pointer to the transmit payload buffer.
  * param: size_byte Number of bytes to be transmitted.
  */
inline void I3C_Transmit_IT(const uint8_t *p_tx_data, uint32_t size_byte)
{
  context = I3C_TRANSFER_TX;
  TxData  = p_tx_data;
  TxCount = size_byte;

  /* Pre-fill TX FIFO */
  I3C_TransmitWordTreatment_IT(&TxData, &TxCount);

  LL_I3C_ConfigTxPreload(MX_I3Cx, (uint16_t)TxCount);

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_I3C_EnableIT(MX_I3Cx, (I3C_TX_IT | LL_I3C_IER_ERRIE));
#else
  LL_I3C_EnableIT(MX_I3Cx, (I3C_TX_IT));
#endif /* USE_LL_APP_ERROR */
}


/** brief: Handles the MX_I3Cx event interrupts.
  */
void mx_i3cx_ev_irqhandler(void)
{
  /* Dynamic Address Assignment update event: own dynamic address is updated. */
  if (LL_I3C_IsActiveMaskFlag_DAUPD(MX_I3Cx) != 0U)
  {
    LL_I3C_ClearFlag_DAUPD(MX_I3Cx);

    I3C_DAACompleteCallback();

    return;
  }

  /* Private transfer events. */
  if (LL_I3C_IsActiveMaskFlag_RXFNE(MX_I3Cx) != 0U)
  {
    I3C_ReceiveWordTreatment_IT(&RxData, &RxCount);
  }

  if (LL_I3C_IsActiveMaskFlag_TXFNF(MX_I3Cx) != 0U)
  {
    I3C_TransmitWordTreatment_IT(&TxData, &TxCount);
  }

  /* Frame complete event. */
  if (LL_I3C_IsActiveMaskFlag_FC(MX_I3Cx) != 0U)
  {
    LL_I3C_ClearFlag_FC(MX_I3Cx);

    if ((context == I3C_TRANSFER_RX) && (RxCount == 0UL))
    {
      LL_I3C_DisableIT(MX_I3Cx, I3C_RX_IT);

      I3C_RxCompleteCallback();
    }

    if ((context == I3C_TRANSFER_TX) && (TxCount == 0UL))
    {
      LL_I3C_DisableIT(MX_I3Cx, I3C_TX_IT);

      I3C_TxCompleteCallback();
    }
  }
}


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/** brief: Handles the MX_I3Cx error interrupts.
  */
void mx_i3cx_err_irqhandler(void)
{
  LL_I3C_ClearFlag_ERR(MX_I3Cx);

  I3C_ErrorCallback();
}
#endif /* USE_LL_APP_ERROR */


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/**
  * brief: This function provides delay (in milliseconds) based on SysTick counter flag.
  * param  delay_ms: Number of milliseconds to delay.
  */
inline void DelayMs(uint32_t delay_ms)
{
  LL_Delay_NoISR(delay_ms);
}
#endif /* USE_LL_APP_ERROR */

/** brief: Drains the MX_I3Cx RX FIFO by word, at most I3C_TRANSFER_MAX_WORD.
  * param  p_rx_data: Pointer to current RX pointer.
  * param  p_rx_size_byte: Pointer to remaining RX bytes.
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


/** brief: Fills the MX_I3Cx TX FIFO by word, at most I3C_TRANSFER_MAX_WORD.
  * param  p_tx_data: Pointer to current TX pointer.
  * param  p_tx_size_byte: Pointer to remaining TX bytes.
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
