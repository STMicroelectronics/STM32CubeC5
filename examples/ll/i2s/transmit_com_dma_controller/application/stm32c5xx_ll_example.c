/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : How to manage an I2S transmission in DMA mode as controller using the LL APIs.
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
void mx_i2sx_irqhandler(void);
void mx_lpdmax_chy_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/** brief: Starts MX_I2Sx transmission in DMA mode.
  * param: p_data      Pointer to transmission data buffer.
  * param: size_sample Amount of data to send.
  */
inline void I2S_Transmit_DMA(const void *p_data, uint32_t size_sample)
{
  LL_DMA_DisableChannel(MX_LPDMAX_CHY_CH);

  LL_DMA_ConfigAddresses(MX_LPDMAX_CHY_CH, (uint32_t)p_data, LL_SPI_DMA_GetTxRegAddr(MX_I2Sx));

  /* Set the number of data at current transfer */
  LL_DMA_SetBlkDataLength(MX_LPDMAX_CHY_CH, size_sample);

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_DMA_EnableIT(MX_LPDMAX_CHY_CH, (LL_DMA_IT_TC | LL_DMA_IT_DTE | LL_DMA_IT_USE));
#else
  LL_DMA_EnableIT(MX_LPDMAX_CHY_CH, LL_DMA_IT_TC);
#endif /* USE_LL_APP_ERROR */

  LL_DMA_EnableChannel(MX_LPDMAX_CHY_CH);

  LL_I2S_EnableDMAReq_TX(MX_I2Sx);

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_I2S_ClearFlag(MX_I2Sx, (LL_I2S_FLAG_UDR | LL_I2S_FLAG_OVR));

  LL_I2S_EnableIT(MX_I2Sx, (LL_I2S_IT_UDR | LL_I2S_IT_OVR));
#endif /* USE_LL_APP_ERROR */

  LL_I2S_Enable(MX_I2Sx);

  LL_I2S_StartTransfer(MX_I2Sx);
} /* I2S_Transmit_DMA */


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/**
  *  Handles the MX_I2Sx interrupts.
  */
void mx_i2sx_irqhandler(void)
{
  uint32_t flag_status   = LL_SPI_READ_REG(MX_I2Sx, IER);
  uint32_t pending_flags = flag_status & LL_SPI_READ_REG(MX_I2Sx, SR);

  if ((pending_flags & SPI_SR_OVR) != 0UL)
  {
    LL_SPI_ClearFlag_OVR(MX_I2Sx);
  }

  if ((pending_flags & SPI_SR_UDR) != 0UL)
  {
    LL_SPI_ClearFlag_UDR(MX_I2Sx);
  }

  LL_I2S_Disable(MX_I2Sx);

  LL_I2S_DisableIT(MX_I2Sx, (LL_I2S_IT_OVR | LL_I2S_IT_UDR));

  I2S_TransferErrorCallback();
} /* mx_adcx_irqhandler() */
#endif /* USE_LL_APP_ERROR */


/**
  *  Handles the MX_LPDMAX_CHY_CH interrupts.
  */
void mx_lpdmax_chy_irqhandler(void)
{
  DMA_TypeDef *instance;
  uint32_t    flags;
  uint32_t    its;
  uint32_t    dma_channel;

  instance    = LL_DMA_GET_INSTANCE(MX_LPDMAX_CHY_CH);
  dma_channel = LL_DMA_GET_CHANNEL_IDX(MX_LPDMAX_CHY_CH);
  its         = LL_DMA_READ_REG((MX_LPDMAX_CHY_CH), CCR);

  /* Check DMA channel active interrupts */
  if (LL_DMA_IsActiveFlag_MIS(instance, dma_channel) == 0U)
  {
    return; /* the global interrupt flag for the current channel is down , nothing to do */
  }

  flags = LL_DMA_READ_REG((MX_LPDMAX_CHY_CH), CSR);

  /* Transfer Complete Interrupt management */
  if (STM32_READ_BIT((flags & its), LL_DMA_FLAG_TC) != 0U)
  {
    LL_DMA_ClearFlag_TC(MX_LPDMAX_CHY_CH);

    I2S_TxTransferCpltCallback();
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  /* Error Interrupt management */
  LL_DMA_ClearFlag(MX_LPDMAX_CHY_CH, LL_DMA_FLAG_ALL);

  /* Check error flags */
  if (((flags & its) & (LL_DMA_FLAG_DTE | LL_DMA_FLAG_USE)) != 0U)
  {
    LL_DMA_ResetChannel(MX_LPDMAX_CHY_CH);

    LL_DMA_DisableIT(MX_LPDMAX_CHY_CH, LL_DMA_IT_ALL);

    I2S_TransferErrorCallback();
  }
#endif /* USE_LL_APP_ERROR */
}
