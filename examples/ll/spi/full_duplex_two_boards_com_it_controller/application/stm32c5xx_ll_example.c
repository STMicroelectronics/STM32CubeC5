/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : A full-duplex synchronous SPI communication as controller,
  *                  in interrupt mode, using the LL API.
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
static uint8_t *TxData;
static uint8_t *RxData;
static uint16_t TxCount;
static uint16_t RxCount;
/* Private functions prototype -----------------------------------------------*/
void mx_spix_irqhandler(void);

/* Private functions ---------------------------------------------------------*/
/**
  * brief:  Starts a full-duplex SPI transfer using interrupts.
  * param  tx_buf: Pointer to transmission data buffer.
  * param  rx_buf: Pointer to reception data buffer.
  * param  size:   Number of bytes to transfer.
  */
inline void SPI_TransmitReceive_IT(uint8_t *tx_buf, uint8_t *rx_buf, uint16_t size)
{
  /* Update the Tx and Rx transfers information to be reused within SPI_TransmitReceive_IT and mx_spix_irqhandler */
  TxData = tx_buf;
  TxCount = size;
  RxData = rx_buf;
  RxCount = size;

  /* Set the number of data at current transfer */
  LL_SPI_SetTransferSize(MX_SPIx, TxCount);

  LL_SPI_Enable(MX_SPIx);

  /* Fill in the TxFIFO */
  while (LL_SPI_IsActiveFlag_TXP(MX_SPIx) != 0UL)
  {
    LL_SPI_TransmitData8(MX_SPIx, *TxData);
    TxData ++;
    TxCount--;
  }

  /* Enable interrupts */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_SPI_EnableIT(MX_SPIx, (LL_SPI_IT_EOT | LL_SPI_IT_DXP | LL_SPI_IT_OVR | LL_SPI_IT_UDR));
#else
  LL_SPI_EnableIT(MX_SPIx, (LL_SPI_IT_EOT | LL_SPI_IT_DXP));
#endif /* USE_LL_APP_ERROR */

  LL_SPI_StartMasterTransfer(MX_SPIx);
}


/**
  *  Handles the MX_SPIx interrupts.
  */
void mx_spix_irqhandler(void)
{
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  uint32_t flag_status    = LL_SPI_READ_REG(MX_SPIx, IER);
  uint32_t pending_flags  = flag_status & LL_SPI_READ_REG(MX_SPIx, SR);
#endif /* USE_LL_APP_ERROR */

  /* SPI Tx and Rx data */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  if (((LL_SPI_IsActiveFlag_OVR(MX_SPIx) == 0) && (LL_SPI_IsActiveFlag_UDR(MX_SPIx) == 0))
      && (LL_SPI_IsActiveFlag_DXP(MX_SPIx) != 0))
#else
  if (LL_SPI_IsActiveFlag_DXP(MX_SPIx) != 0)
#endif /* USE_LL_APP_ERROR */
  {
    if (TxCount > 0UL)
    {
      LL_SPI_TransmitData8(MX_SPIx, *TxData);
      TxData++;
      TxCount--;
    }

    if (RxCount > 0UL)
    {
      *RxData = LL_SPI_ReceiveData8(MX_SPIx);
      RxData++;
      RxCount--;
    }

    return;
  }

  /* SPI end of Tx and Rx transfers */
  if (LL_SPI_IsActiveFlag_EOT(MX_SPIx))
  {
    /* Clear EOT/DXP/TXTF flag */
    LL_SPI_ClearFlag(MX_SPIx, (LL_SPI_FLAG_EOT | LL_SPI_FLAG_DXP | LL_SPI_FLAG_TXTF));

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    LL_SPI_DisableIT(MX_SPIx, (LL_SPI_IT_EOT | LL_SPI_IT_DXP | LL_SPI_IT_OVR | LL_SPI_IT_UDR));
#else
    LL_SPI_DisableIT(MX_SPIx, (LL_SPI_IT_EOT | LL_SPI_IT_DXP));
#endif /* USE_LL_APP_ERROR */

    /* Transmit remaining data */
    while (RxCount != 0UL)
    {
      *RxData = LL_SPI_ReceiveData8(MX_SPIx);
      RxData++;
      RxCount--;
    }

    LL_SPI_Disable(MX_SPIx);

    SPI_TransferCpltCallback();

    return;
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
  LL_SPI_DisableIT(MX_SPIx, (LL_SPI_IT_EOT | LL_SPI_IT_DXP | LL_SPI_IT_OVR | LL_SPI_IT_UDR));

  SPI_TransferErrorCallback();
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
