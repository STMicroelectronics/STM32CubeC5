/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : This example code shows how to use UART LL API to transmit
  *                  and receive a data buffer with a communication process based on IT transfer.
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
uint8_t *Data;           /* Pointer used to manage Rx and Tx transfers buffers     */
volatile uint32_t Count; /* Variable used to manage Rx and Tx transfers data count */

/* Private functions prototype -----------------------------------------------*/
static void HandleTxTransfer(void);
static void HandleRxTransfer(void);
void mx_uartx_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * brief: Transmit MX_UARTx data in interrupt mode.
  * param: p_data    Pointer to data buffer.
  * param: size_byte Amount of data elements to be transmitted.
  */
inline void UART_Transmit_IT(const void *p_data, uint32_t size_byte)
{
  /* Update the Tx transfers information to be reused within UART_Transmit_IT and mx_uartx_irqhandler */
  Data  = (uint8_t *)p_data;
  Count = size_byte;

  HandleTxTransfer();

  LL_USART_EnableIT_TXE_TXFNF(MX_UARTx);
}


/**
  * brief: Receive MX_UARTx data in interrupt mode.
  * param: p_data    Pointer to data buffer.
  * param: size_byte Amount of data elements to be received.
  * param: character Character to match in the received sequence.
  */
inline void UART_ReceiveUntilCM_IT(void *p_data, uint32_t size_byte, uint8_t character)
{
  /* Update the Rx transfers information to be reused within UART_Receive_IT and mx_uartx_irqhandler */
  Data  = (uint8_t *)p_data;
  Count = size_byte;

  LL_USART_Disable(MX_UARTx);
  LL_USART_SetNodeAddress(MX_UARTx, (uint32_t)character);
  LL_USART_Enable(MX_UARTx);

  LL_USART_EnableIT_CM(MX_UARTx);
  LL_USART_EnableIT_RXNE_RXFNE(MX_UARTx);
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
  uint32_t cr3_its   = LL_USART_READ_REG(MX_UARTx, CR3);

  /* MX_UARTx in mode Receiver */
  if (((isr_flags & LL_USART_ISR_RXNE_RXFNE) != 0U)
      && (((cr1_its & LL_USART_CR1_RXNEIE_RXFNEIE) != 0U)
          || ((cr3_its & LL_USART_CR3_RXFTIE) != 0U)))
  {
    HandleRxTransfer();

    if (Count == 0U)
    {
      UART_RxTransferCpltCallback();
    }

    return;
  }

  if (((isr_flags & LL_USART_ISR_CMF) != 0U) && ((cr1_its & LL_USART_CR1_CMIE) != 0U))
  {
    /* Clear CM flag */
    LL_USART_ClearFlag_CM(MX_UARTx);
    LL_USART_DisableIT_CR1(MX_UARTx, LL_USART_CR1_CMIE);
  }

  /* MX_UARTx in mode Transmitter */
  if (((isr_flags & LL_USART_ISR_TXE_TXFNF) != 0U)
      && (((cr1_its & LL_USART_CR1_TXEIE_TXFNFIE) != 0U)
          || ((cr3_its & LL_USART_CR3_TXFTIE) != 0U)))
  {
    HandleTxTransfer();

    return;
  }

  /* MX_UARTx in mode Transmitter (transmission end) */
  if (((isr_flags & LL_USART_ISR_TC) != 0U) && ((cr1_its & LL_USART_CR1_TCIE) != 0U))
  {
    LL_USART_DisableIT_TC(MX_UARTx);

    UART_TxTransferCpltCallback();

    return;
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  /* MX_UARTx error interrupt occurred (noise error, overrun and frame error) */
  if (((isr_flags & (LL_USART_ISR_NE | LL_USART_ISR_ORE | LL_USART_ISR_FE)) != 0U)
      && ((cr3_its & LL_USART_CR3_EIE) != 0U))
  {
    LL_USART_DisableIT_ERROR(MX_UARTx);

    LL_USART_ClearFlag(MX_UARTx, (LL_USART_ISR_NE | LL_USART_ISR_ORE | LL_USART_ISR_FE));
    LL_USART_ClearFlag_NE(MX_UARTx);

    UART_TransferErrorCallback();
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


/**
  * brief: Handles the MX_UARTx Tx transfer.
  */
static void HandleTxTransfer(void)
{
  if (Count == 0U)
  {
    LL_USART_DisableIT_TXE_TXFNF(MX_UARTx);
    LL_USART_EnableIT_TC(MX_UARTx);
  }
  else
  {
    LL_USART_TransmitData8(MX_UARTx, *Data);
    Data++;
    Count--;
  }
}


/**
  * brief: Handles the MX_UARTx Rx transfer.
  */
static void HandleRxTransfer(void)
{
  *Data = LL_USART_ReceiveData8(MX_UARTx);
  Data++;
  Count--;

  if (Count == 0U)
  {
    LL_USART_DisableIT_RXNE_RXFNE(MX_UARTx);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    LL_USART_DisableIT_ERROR(MX_UARTx);
#endif /* USE_LL_APP_ERROR */
  }
}
