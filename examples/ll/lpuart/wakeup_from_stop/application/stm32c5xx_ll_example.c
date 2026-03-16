/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : LPUART communication in low power mode responder side using LL APIs.
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
uint8_t *pData;          /* Pointer used to manage Rx and Tx transfers buffers     */
volatile uint32_t Count; /* Variable used to manage Rx and Tx transfers data count */

/* Private functions prototype -----------------------------------------------*/
void mx_lpuartx_irqhandler(void);
static void HandleTxTransfer(void);
static void HandleRxTransfer(void);

/* Private functions ---------------------------------------------------------*/

/**
  * brief: MX_LPUARTx receive data in interrupt mode.
  * param: p_data    Pointer to data buffer.
  * param: size_byte Amount of data elements to be received in bytes.
  */
inline void UART_Receive_IT(void *p_data, uint32_t size_byte)
{
  /* Update the Rx transfers information to be reused within UART_Receive_IT and MX_LPUARTx_irqhandler */
  pData = (uint8_t *)p_data;
  Count = size_byte;

  LL_LPUART_EnableIT_RXNE_RXFNE(MX_LPUARTx);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_LPUART_EnableIT_ERROR(MX_LPUARTx);
#endif /* USE_LL_APP_ERROR */

  LL_LPUART_Enable(MX_LPUARTx);
}


/**
  * brief: MX_LPUARTx transmit data in interrupt mode.
  * param: p_data    Pointer to data buffer.
  * param: size_byte Amount of data elements to be transmitted in bytes.
  */
inline void UART_Transmit_IT(const void *p_data, uint32_t size_byte)
{
  /* Update the Tx transfers information to be reused within UART_Transmit_IT and MX_LPUARTx_irqhandler */
  pData = (uint8_t *)p_data;
  Count = size_byte;

  HandleTxTransfer();

  LL_LPUART_EnableIT_TXE_TXFNF(MX_LPUARTx);
}


/**
  * brief: Handles the MX_LPUARTx interrupts.
  */
void mx_lpuartx_irqhandler(void)
{
  uint32_t isr_flags = LL_LPUART_READ_REG(MX_LPUARTx, ISR);
  uint32_t cr1_its   = LL_LPUART_READ_REG(MX_LPUARTx, CR1);
  uint32_t cr3_its   = LL_LPUART_READ_REG(MX_LPUARTx, CR3);

  /* MX_LPUARTx in mode Receiver */
  if (((isr_flags & LL_LPUART_ISR_RXNE_RXFNE) != 0U)
      && (((cr1_its & LL_LPUART_CR1_RXNEIE_RXFNEIE) != 0U)
          || ((cr3_its & LL_LPUART_CR3_RXFTIE) != 0U)))
  {
    HandleRxTransfer();

    if (Count == 0U)
    {
      UART_RxTransferCpltCallback();
    }

    return;
  }

  /* MX_LPUARTx in mode Transmitter */
  if (((isr_flags & LL_LPUART_ISR_TXE_TXFNF) != 0U)
      && (((cr1_its & LL_LPUART_CR1_TXEIE_TXFNFIE) != 0U)
          || ((cr3_its & LL_LPUART_CR3_TXFTIE) != 0U)))
  {
    HandleTxTransfer();

    return;
  }

  /* MX_LPUARTx in mode Transmitter (transmission end) */
  if (((isr_flags & LL_LPUART_ISR_TC) != 0U) && ((cr1_its & LL_LPUART_CR1_TCIE) != 0U))
  {
    LL_LPUART_DisableIT_TC(MX_LPUARTx);

    UART_TxTransferCpltCallback();

    return;
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  /* MX_LPUARTx error interrupt occurred (noise error, overrun and frame error) */
  if (((isr_flags & (LL_LPUART_ISR_NE | LL_LPUART_ISR_ORE | LL_LPUART_ISR_FE)) != 0U)
      && ((cr3_its & LL_USART_CR3_EIE) != 0U))
  {
    LL_LPUART_DisableIT_ERROR(MX_LPUARTx);
    LL_LPUART_ClearFlag_ORE(MX_LPUARTx);
    LL_LPUART_ClearFlag_NE(MX_LPUARTx);
    LL_LPUART_ClearFlag_FE(MX_LPUARTx);

    UART_TransferErrorCallback();
  }
#endif /* USE_LL_APP_ERROR */
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
  * brief: Handles the MX_LPUARTx Tx transfer.
  */
static inline void HandleTxTransfer(void)
{
  if (Count == 0U)
  {
    LL_LPUART_DisableIT_TXE_TXFNF(MX_LPUARTx);
    LL_LPUART_EnableIT_TC(MX_LPUARTx);
  }
  else
  {
    LL_LPUART_TransmitData8(MX_LPUARTx, *pData);
    pData++;
    Count--;
  }
}


/**
  * brief: Handles the MX_LPUARTx Rx transfer.
  */
static inline void HandleRxTransfer(void)
{
  *pData = LL_LPUART_ReceiveData8(MX_LPUARTx);
  pData++;
  Count--;

  if (Count == 0U)
  {
    LL_LPUART_DisableIT_RXNE_RXFNE(MX_LPUARTx);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    LL_LPUART_DisableIT_ERROR(MX_LPUARTx);
#endif /* USE_LL_APP_ERROR */
  }
}
