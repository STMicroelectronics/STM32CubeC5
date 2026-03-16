/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : The USART controller communication side in DMA mode using LL API.
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
static void DMA_IRQHandler(DMA_Channel_TypeDef *channel);
void mx_usartx_irqhandler(void);
void mx_lpdmax_ch1y_irqhandler(void);
void mx_lpdmax_ch2y_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/** brief: MX_USARTx transmits data in DMA mode.
  * param: p_tx_buf  Pointer to transmission data buffer.
  * param: size_byte Number of bytes to transfer.
  */
inline void USART_Transmit_DMA(const void *p_tx_buf, uint16_t size_byte)
{
  /* Enable the Tx DMA Channel */
  LL_DMA_ConfigAddresses(MX_LPDMAX_CH1Y_CH, (uint32_t)p_tx_buf,
                         LL_USART_DMA_GetRegAddr(MX_USARTx, LL_USART_DMA_REG_DATA_TRANSMIT));

  LL_DMA_SetBlkDataLength(MX_LPDMAX_CH1Y_CH, size_byte);

  LL_DMA_ClearFlag(MX_LPDMAX_CH1Y_CH, LL_DMA_FLAG_ALL);

  LL_DMA_DisableIT(MX_LPDMAX_CH1Y_CH, LL_DMA_IT_ALL);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_DMA_EnableIT(MX_LPDMAX_CH1Y_CH, (LL_DMA_IT_TC | LL_DMA_IT_DTE | LL_DMA_IT_USE));
#else
  LL_DMA_EnableIT(MX_LPDMAX_CH1Y_CH, LL_DMA_IT_TC);
#endif /* USE_LL_APP_ERROR */

  LL_DMA_EnableChannel(MX_LPDMAX_CH1Y_CH);

  LL_USART_ClearFlag_TC(MX_USARTx);

  /* Enable the Tx DMA */
  LL_USART_EnableDMAReq_TX(MX_USARTx);
}


/** brief: MX_USARTx receives data in DMA mode.
  * param: p_rx_buf  Pointer to reception data buffer.
  * param: size_byte Number of bytes to transfer.
  */
inline void USART_Receive_DMA(void *p_rx_buf, uint16_t size_byte)
{
  /* Enable the Rx DMA Channel */
  LL_DMA_ConfigAddresses(MX_LPDMAX_CH2Y_CH,
                         LL_USART_DMA_GetRegAddr(MX_USARTx, LL_USART_DMA_REG_DATA_RECEIVE),
                         (uint32_t)p_rx_buf);

  LL_DMA_SetBlkDataLength(MX_LPDMAX_CH2Y_CH, size_byte);

  LL_DMA_ClearFlag(MX_LPDMAX_CH2Y_CH, LL_DMA_FLAG_ALL);

  LL_DMA_DisableIT(MX_LPDMAX_CH2Y_CH, LL_DMA_IT_ALL);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_DMA_EnableIT(MX_LPDMAX_CH2Y_CH, (LL_DMA_IT_TC | LL_DMA_IT_DTE | LL_DMA_IT_USE));
#else
  LL_DMA_EnableIT(MX_LPDMAX_CH2Y_CH, LL_DMA_IT_TC);
#endif /* USE_LL_APP_ERROR */

  LL_DMA_EnableChannel(MX_LPDMAX_CH2Y_CH);

  /* Enable the Tx DMA channel to generate the clock to the responder */
  LL_DMA_ConfigAddresses(MX_LPDMAX_CH1Y_CH,
                         (uint32_t)p_rx_buf,
                         LL_USART_DMA_GetRegAddr(MX_USARTx, LL_USART_DMA_REG_DATA_TRANSMIT));

  LL_DMA_SetBlkDataLength(MX_LPDMAX_CH1Y_CH, size_byte);

  LL_DMA_ClearFlag(MX_LPDMAX_CH1Y_CH, LL_DMA_FLAG_ALL);

  LL_DMA_DisableIT(MX_LPDMAX_CH1Y_CH, LL_DMA_IT_ALL);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_DMA_EnableIT(MX_LPDMAX_CH1Y_CH, (LL_DMA_IT_TC | LL_DMA_IT_DTE | LL_DMA_IT_USE));
#else
  LL_DMA_EnableIT(MX_LPDMAX_CH1Y_CH, LL_DMA_IT_TC);
#endif /* USE_LL_APP_ERROR */

  LL_DMA_EnableChannel(MX_LPDMAX_CH1Y_CH);

  LL_USART_EnableIT_ERROR(MX_USARTx);

  /* Enable the Tx and Rx DMA */
  LL_USART_EnableDMAReq_TX(MX_USARTx);
  LL_USART_EnableDMAReq_RX(MX_USARTx);
}


/**
  *  Handles the MX_USARTx interrupts.
  */
void mx_usartx_irqhandler(void)
{
  uint32_t flag_status   = LL_USART_READ_REG(MX_USARTx, ISR);
  uint32_t cr1_its       = LL_USART_READ_REG(MX_USARTx, CR1);
  uint32_t cr3_its       = LL_USART_READ_REG(MX_USARTx, CR3);

  /* USART End Of Transfer */
  if (((flag_status & LL_USART_ISR_TC) != 0U) && ((cr1_its & LL_USART_CR1_TCIE) != 0U))
  {
    /* Disable the USART Transmit Complete Interrupt */
    LL_USART_DisableIT_TC(MX_USARTx);

    /* Disable the USART Error Interrupt: (Frame error, noise error, overrun error) */
    LL_USART_DisableIT_ERROR(MX_USARTx);

    /* Clear overrun flag and discard the received data */
    LL_USART_ClearFlag_ORE(MX_USARTx);

    LL_USART_RequestRxDataFlush(MX_USARTx);

    USART_TxTransferCpltCallback();

    return;
  }

  /* USART Over-Run interrupt occurred */
  if (((flag_status & LL_USART_ISR_ORE) != 0U)
      && (((cr1_its & LL_USART_CR1_RXNEIE_RXFNEIE) != 0U)
          || ((cr3_its & (LL_USART_CR3_RXFTIE | LL_USART_CR3_EIE)) != 0U)))
  {
    LL_USART_ClearFlag_ORE(MX_USARTx);
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  /* USART frame error interrupt occurred */
  if (((flag_status & LL_USART_ISR_FE) != 0U) && ((cr3_its & LL_USART_CR3_EIE) != 0U))
  {
    LL_USART_ClearFlag_FE(MX_USARTx);
  }

  /* USART noise error interrupt occurred */
  if (((flag_status & LL_USART_ISR_NE) != 0U) && ((cr3_its & LL_USART_CR3_EIE) != 0U))
  {
    LL_USART_ClearFlag_NE(MX_USARTx);
  }
#endif /* USE_LL_APP_ERROR */
}


/**
  *  Handles the MX_LPDMAx_CH1y interrupts.
  */
void mx_lpdmax_ch1y_irqhandler(void)
{
  DMA_IRQHandler(MX_LPDMAX_CH1Y_CH);

  LL_USART_DisableDMAReq_TX(MX_USARTx);

  LL_USART_EnableIT_TC(MX_USARTx);
}


/**
  *  Handles the MX_LPDMAx_CH2y interrupts.
  */
void mx_lpdmax_ch2y_irqhandler(void)
{
  DMA_IRQHandler(MX_LPDMAX_CH2Y_CH);

  LL_USART_DisableDMAReq_RX(MX_USARTx);

  USART_RxTransferCpltCallback();
}


/**
  * brief: Handles the DMA channels interrupts.
  * param: channel pointer to the LPDMA channel.
  */
static inline void DMA_IRQHandler(DMA_Channel_TypeDef *channel)
{
  DMA_TypeDef *instance;
  uint32_t    flags;
  uint32_t    its;
  uint32_t    dma_channel;

  instance     = LL_DMA_GET_INSTANCE(channel);
  dma_channel  = LL_DMA_GET_CHANNEL_IDX(channel);
  its          = LL_DMA_READ_REG((channel), CCR);

  /* Check DMA channel active interrupts */
  if (LL_DMA_IsActiveFlag_MIS(instance, dma_channel) == 0U)
  {
    return; /* the global interrupt flag for the current channel is down , nothing to do */
  }

  flags = LL_DMA_READ_REG((channel), CSR);

  /* Half Transfer Complete Interrupt management */
  if (STM32_READ_BIT((flags & its), LL_DMA_FLAG_HT) != 0U)
  {
    LL_DMA_ClearFlag_HT(channel);

    if (STM32_READ_BIT((flags & its), LL_DMA_FLAG_TC) == 0U)
    {
      return;
    }
  }

  /* Transfer Complete Interrupt management */
  if (STM32_READ_BIT((flags & its), LL_DMA_FLAG_TC) != 0U)
  {
    LL_DMA_ClearFlag_TC(channel);

    /* Check if there are remaining data */
    if (LL_DMA_IsActiveFlag_IDLE(channel) != 0U)
    {
      LL_DMA_ClearFlag_HT(channel);
      LL_DMA_DisableIT(channel, LL_DMA_IT_ALL);
    }
    return;
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  /* Error Interrupt management */
  LL_DMA_ClearFlag(channel, LL_DMA_FLAG_ALL);

  /* Check error flags */
  if (((flags & its) & (LL_DMA_FLAG_DTE | LL_DMA_FLAG_USE)) != 0U)
  {
    LL_DMA_ResetChannel(channel);

    LL_DMA_DisableIT(channel, LL_DMA_IT_ALL);

    USART_TransferErrorCallback();
  }
#endif /* USE_LL_APP_ERROR */
}

/**
  * brief: This function provides delay (in milliseconds) based on SysTick counter flag.
  * param: delay_ms Number of milliseconds to delay.
  */
inline void DelayMs(uint32_t delay_ms)
{
  LL_Delay_NoISR(delay_ms);
}
