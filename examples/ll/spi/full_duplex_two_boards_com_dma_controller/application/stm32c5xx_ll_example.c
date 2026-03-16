/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : A full-duplex synchronous SPI communication as controller,
  *                  in DMA mode, using the LL API.
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
void mx_spix_irqhandler(void);
void mx_lpdmax_ch1y_irqhandler(void);
void mx_lpdmax_ch2y_irqhandler(void);

/* Private functions ---------------------------------------------------------*/
/**
  * brief:  Starts a full-duplex SPI transfer using DMA.
  * param  tx_buf: Pointer to transmission data buffer.
  * param  rx_buf: Pointer to reception data buffer.
  * param  size:   Number of bytes to transfer.
  */
inline void SPI_TransmitReceive_DMA(uint8_t *tx_buf, uint8_t *rx_buf, uint16_t size)
{
  /* Set the number of data at current transfer */
  LL_SPI_SetTransferSize(MX_SPIx, size);

  /* Enable interrupts */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_SPI_EnableIT(MX_SPIx, (LL_SPI_IT_EOT | LL_SPI_IT_OVR | LL_SPI_IT_UDR));
#else
  LL_SPI_EnableIT(MX_SPIx, LL_SPI_IT_EOT);
#endif /* USE_LL_APP_ERROR */

  /* Enable the Rx DMA Stream/Channel  */
  LL_DMA_ConfigAddresses(MX_LPDMAX_CH2Y_CH, LL_SPI_DMA_GetRxRegAddr(MX_SPIx), (uint32_t)rx_buf);
  LL_DMA_SetBlkDataLength(MX_LPDMAX_CH2Y_CH, size);
  LL_DMA_ClearFlag(MX_LPDMAX_CH2Y_CH, LL_DMA_FLAG_ALL);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_DMA_EnableIT(MX_LPDMAX_CH2Y_CH, (LL_DMA_IT_TC | LL_DMA_IT_DTE | LL_DMA_IT_USE));
#else
  LL_DMA_EnableIT(MX_LPDMAX_CH2Y_CH, LL_DMA_IT_TC);
#endif /* USE_LL_APP_ERROR */
  LL_DMA_EnableChannel(MX_LPDMAX_CH2Y_CH);

  /* Enable the Tx DMA Stream/Channel  */
  LL_DMA_ConfigAddresses(MX_LPDMAX_CH1Y_CH, (uint32_t)tx_buf, LL_SPI_DMA_GetTxRegAddr(MX_SPIx));
  LL_DMA_SetBlkDataLength(MX_LPDMAX_CH1Y_CH, size);
  LL_DMA_ClearFlag(MX_LPDMAX_CH1Y_CH, LL_DMA_FLAG_ALL);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_DMA_EnableIT(MX_LPDMAX_CH1Y_CH, (LL_DMA_IT_TC | LL_DMA_IT_DTE | LL_DMA_IT_USE));
#else
  LL_DMA_EnableIT(MX_LPDMAX_CH1Y_CH, LL_DMA_IT_TC);
#endif /* USE_LL_APP_ERROR */
  LL_DMA_EnableChannel(MX_LPDMAX_CH1Y_CH);

  /* Enable the Tx/Rx DMA */
  LL_SPI_EnableDMAReq_RX(MX_SPIx);
  LL_SPI_EnableDMAReq_TX(MX_SPIx);

  /* Enable SPI */
  LL_SPI_Enable(MX_SPIx);

  LL_SPI_StartMasterTransfer(MX_SPIx);
}


/**
  *  Handles the MX_SPIx interrupts.
  */
void mx_spix_irqhandler(void)
{
  uint32_t flag_status   = LL_SPI_READ_REG(MX_SPIx, IER);
  uint32_t pending_flags = flag_status & LL_SPI_READ_REG(MX_SPIx, SR);

  /* SPI End Of Transfer */
  if ((pending_flags & SPI_SR_EOT) != 0U)
  {
    LL_SPI_ClearFlag(MX_SPIx, (LL_SPI_FLAG_EOT | LL_SPI_FLAG_TXTF));

    LL_SPI_DisableIT_EOT(MX_SPIx);

    /* Reset the Tx/Rx DMA */
    LL_SPI_DisableDMAReq_TX(MX_SPIx);
    LL_SPI_DisableDMAReq_RX(MX_SPIx);

    SPI_TransferCpltCallback();
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  if ((pending_flags & SPI_SR_OVR) != 0UL)
  {
    LL_SPI_ClearFlag_OVR(MX_SPIx);
  }

  if ((pending_flags & SPI_SR_UDR) != 0UL)
  {
    LL_SPI_ClearFlag_UDR(MX_SPIx);
  }

  /* Disable SPI peripheral */
  LL_SPI_Disable(MX_SPIx);

  /* Disable all interrupts */
  LL_SPI_DisableIT(MX_SPIx, (LL_SPI_IT_EOT | LL_SPI_IT_OVR | LL_SPI_IT_UDR));

  SPI_TransferErrorCallback();
#endif /* USE_LL_APP_ERROR */
}


/**
  *  Handles the MX_LPDMAX_CH1Y_CH interrupts.
  */
void mx_lpdmax_ch1y_irqhandler(void)
{
  DMA_IRQHandler(MX_LPDMAX_CH1Y_CH);
}


/**
  *  Handles the MX_LPDMAX_CH2Y_CH interrupts.
  */
void mx_lpdmax_ch2y_irqhandler(void)
{
  DMA_IRQHandler(MX_LPDMAX_CH2Y_CH);
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

  /* Transfer Complete Interrupt management */
  if (STM32_READ_BIT((flags & its), LL_DMA_FLAG_TC) != 0U)
  {
    LL_DMA_ClearFlag_TC(channel);

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

    SPI_TransferErrorCallback();
  }
#endif /* USE_LL_APP_ERROR */
}
