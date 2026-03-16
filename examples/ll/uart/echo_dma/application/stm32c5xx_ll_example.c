/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : This example code shows how to use UART LL API to transmit
  *                  and receive a data buffer with a communication process based on DMA transfer.
  *                  The communication is done with the serial terminal emulator PC application.
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
void mx_uartx_irqhandler(void);
void mx_lpdmax_ch1y_irqhandler(void);
void mx_lpdmax_ch2y_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * brief: Transmit MX_UARTx data in DMA mode.
  * param p_data Pointer to data buffer.
  * param size_byte Amount of data elements to be transmitted.
  */
inline void UART_Transmit_DMA(const void *p_data, uint32_t size_byte)
{
  LL_DMA_DisableChannel(MX_LPDMAX_CH1Y_CH);

  LL_DMA_ConfigAddresses(MX_LPDMAX_CH1Y_CH,
                         (uint32_t)p_data,
                         LL_USART_DMA_GetRegAddr(MX_UARTx, LL_USART_DMA_REG_DATA_TRANSMIT));
  LL_DMA_SetBlkDataLength(MX_LPDMAX_CH1Y_CH, size_byte);

  LL_DMA_DisableIT(MX_LPDMAX_CH1Y_CH, LL_DMA_IT_ALL);
  LL_DMA_ClearFlag(MX_LPDMAX_CH1Y_CH, LL_DMA_FLAG_ALL);

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_DMA_EnableIT(MX_LPDMAX_CH1Y_CH, (LL_DMA_IT_TC | LL_DMA_IT_DTE | LL_DMA_IT_USE));
#else
  LL_DMA_EnableIT(MX_LPDMAX_CH1Y_CH, LL_DMA_IT_TC);
#endif /* USE_LL_APP_ERROR */

  LL_DMA_EnableChannel(MX_LPDMAX_CH1Y_CH);

  LL_USART_ClearFlag_TC(MX_UARTx);

  LL_USART_EnableDMAReq_TX(MX_UARTx);
}


/**
  * brief: Receive MX_UARTx data in DMA mode.
  * param p_data Pointer to data buffer.
  * param size_byte Amount of data elements to be received.
  * param character Character to match in the received sequence.
  */
inline void UART_ReceiveUntilCM_DMA(void *p_data, uint32_t size_byte, uint8_t character)
{
  LL_USART_Disable(MX_UARTx);
  LL_USART_SetNodeAddress(MX_UARTx, (uint32_t)character);
  LL_USART_Enable(MX_UARTx);

  LL_USART_EnableIT_CM(MX_UARTx);

  LL_DMA_DisableChannel(MX_LPDMAX_CH2Y_CH);

  LL_DMA_ConfigAddresses(MX_LPDMAX_CH2Y_CH,
                         LL_USART_DMA_GetRegAddr(MX_UARTx, LL_USART_DMA_REG_DATA_RECEIVE),
                         (uint32_t)p_data);
  LL_DMA_SetBlkDataLength(MX_LPDMAX_CH2Y_CH, size_byte);

  LL_DMA_DisableIT(MX_LPDMAX_CH2Y_CH, LL_DMA_IT_ALL);
  LL_DMA_ClearFlag(MX_LPDMAX_CH2Y_CH, LL_DMA_FLAG_ALL);

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_DMA_EnableIT(MX_LPDMAX_CH2Y_CH, (LL_DMA_IT_TC | LL_DMA_IT_DTE | LL_DMA_IT_USE));
#else
  LL_DMA_EnableIT(MX_LPDMAX_CH2Y_CH, LL_DMA_IT_TC);
#endif /* USE_LL_APP_ERROR */

  LL_DMA_EnableChannel(MX_LPDMAX_CH2Y_CH);

  LL_USART_EnableDMAReq_RX(MX_UARTx);

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_USART_EnableIT_ERROR(MX_UARTx);
#endif /* USE_LL_APP_ERROR */
}


/**
  * brief: Handles the MX_UARTx interrupts.
  */
void mx_uartx_irqhandler(void)
{
  uint32_t isr_flags = LL_USART_READ_REG(MX_UARTx, ISR);
  uint32_t cr1_its   = LL_USART_READ_REG(MX_UARTx, CR1);

  if (((isr_flags & LL_USART_ISR_CMF) != 0U) && ((cr1_its & LL_USART_CR1_CMIE) != 0U))
  {
    /* Clear CM flag */
    LL_USART_DisableIT_CM(MX_UARTx);
    LL_USART_ClearFlag_CM(MX_UARTx);
    LL_USART_DisableDMAReq_RX(MX_UARTx);
  }

  /* UART in mode Transmitter (transmission end) */
  if (((isr_flags & LL_USART_ISR_TC) != 0U) && ((cr1_its & LL_USART_CR1_TCIE) != 0U))
  {
    LL_USART_DisableIT_TC(MX_UARTx);

    UART_TxTransferCpltCallback();
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  /* MX_UARTx error interrupt occurred (noise error, overrun and frame error) */
  if ((isr_flags & (LL_USART_ISR_NE | LL_USART_ISR_ORE | LL_USART_ISR_FE)) != 0U)
  {
    LL_USART_DisableIT_ERROR(MX_UARTx);

    LL_USART_ClearFlag(MX_UARTx, (LL_USART_ISR_NE | LL_USART_ISR_ORE | LL_USART_ISR_FE));
    LL_USART_ClearFlag_NE(MX_UARTx);

    UART_TransferErrorCallback();
  }
#endif /* USE_LL_APP_ERROR */
}


/**
  *  Handles the MX_LPDMAX_CH1Y_CH interrupts.
  */
void mx_lpdmax_ch1y_irqhandler(void)
{
  DMA_IRQHandler(MX_LPDMAX_CH1Y_CH);

  LL_USART_DisableDMAReq_TX(MX_UARTx);

  LL_USART_EnableIT_TC(MX_UARTx);
}


/**
  *  Handles the MX_LPDMAx_CH2y interrupts.
  */
void mx_lpdmax_ch2y_irqhandler(void)
{
  DMA_IRQHandler(MX_LPDMAX_CH2Y_CH);

  LL_USART_DisableDMAReq_RX(MX_UARTx);

  UART_RxTransferCpltCallback();
}


/**
  * brief: This function provides delay (in milliseconds) based on SysTick counter flag.
  * param: delay_ms Number of milliseconds to delay.
  */
inline void DelayMs(uint32_t delay_ms)
{
  LL_Delay_NoISR(delay_ms);
}


/**
  * brief: Handles the DMA channels interrupts.
  * param: channel pointer to the DMA channel.
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
  if (((flags & its) & LL_DMA_FLAG_TC) != 0U)
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

    UART_TransferErrorCallback();
  }
#endif /* USE_LL_APP_ERROR */
}
