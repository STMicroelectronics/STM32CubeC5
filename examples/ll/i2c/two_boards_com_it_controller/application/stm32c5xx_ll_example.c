/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : The controller's application handling a data transfer over
  *                  I2C in interrupt mode with LL API.
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
typedef enum
{
  I2C_XFER_NONE = 0U,
  I2C_XFER_TX = 1U,
  I2C_XFER_RX = 2U
} i2c_xfer_t;

/* Private define ------------------------------------------------------------*/
#define MAX_NBYTE_SIZE          (255U)
/** Mask used to clear the transfer configuration bits in I2C CR2 while preserving
  * the other control bits. It removes the address, addressing mode, byte count,
  * reload, and transfer direction fields after a transfer completes.
  */
#define I2C_XFER_INFO ~((uint32_t)(I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN))

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const uint8_t *TxData;
static uint8_t *RxData;
static uint16_t TxCount;
static uint16_t RxCount;
static uint32_t xfer_size;
static uint32_t xfer_mode;
static uint32_t device_address;
/* Tracks the current I2C transfer state shared between the main flow and interrupt handlers. */
static volatile i2c_xfer_t I2C_CurrentXfer = I2C_XFER_NONE;

/* Private functions prototype -----------------------------------------------*/
static void I2C_FlushTxDR(void);
static void I2C_ITComplete(uint32_t it_flags);
void mx_i2cx_ev_irqhandler(void);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
static void I2C_ITError(void);
void mx_i2cx_err_irqhandler(void);
#endif /* USE_LL_APP_ERROR */

/* Private functions ---------------------------------------------------------*/


/** brief:  MX_I2Cx transmit an amount of data in non-blocking mode with interrupt.
  * param   device_address: Target device address.
  * param   tx_buffer:      Pointer to transmission data buffer.
  * param   size:           Number of bytes to transfer.
  */
inline void I2C_Transmit_IT(uint32_t device_address, const void *tx_buffer, uint32_t size)
{
  I2C_CurrentXfer = I2C_XFER_TX;
  TxData = (const uint8_t *)tx_buffer;
  TxCount = size;

  if (TxCount > MAX_NBYTE_SIZE)
  {
    xfer_size = MAX_NBYTE_SIZE;
    xfer_mode = LL_I2C_MODE_RELOAD;
  }
  else
  {
    xfer_size = TxCount;
    xfer_mode = LL_I2C_MODE_AUTOEND;
  }

  LL_I2C_HandleTransfer(MX_I2Cx, device_address, LL_I2C_ADDRSLAVE_7BIT, xfer_size, xfer_mode,
                        LL_I2C_GENERATE_START_WRITE);

  /* Enable interrupts */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_I2C_EnableIT(MX_I2Cx, LL_I2C_CR1_ERRIE | LL_I2C_CR1_TCIE | LL_I2C_CR1_STOPIE \
                  | LL_I2C_CR1_NACKIE | LL_I2C_CR1_TXIE);
#else
  LL_I2C_EnableIT(MX_I2Cx, LL_I2C_CR1_TCIE | LL_I2C_CR1_STOPIE \
                  | LL_I2C_CR1_NACKIE | LL_I2C_CR1_TXIE);
#endif /* USE_LL_APP_ERROR */
}

/** brief:  MX_I2Cx receive an amount of data in non-blocking mode with interrupt.
  * param   device_address: Target device address.
  * param   rx_buffer:      Pointer to reception data buffer.
  * param   size:           Number of bytes to transfer.
  */
inline void I2C_Receive_IT(uint32_t device_address, uint8_t *rx_buffer, uint32_t size)
{
  I2C_CurrentXfer = I2C_XFER_RX;
  RxData = rx_buffer;
  RxCount = size;

  if (RxCount > MAX_NBYTE_SIZE)
  {
    xfer_size = MAX_NBYTE_SIZE;
    xfer_mode = LL_I2C_MODE_RELOAD;
  }
  else
  {
    xfer_size = RxCount;
    xfer_mode = LL_I2C_MODE_AUTOEND;
  }

  LL_I2C_HandleTransfer(MX_I2Cx, device_address, LL_I2C_ADDRSLAVE_7BIT, xfer_size, xfer_mode,
                        LL_I2C_GENERATE_START_READ);

  /* Enable interrupts */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_I2C_EnableIT(MX_I2Cx, LL_I2C_CR1_ERRIE | LL_I2C_CR1_TCIE | LL_I2C_CR1_STOPIE \
                  | LL_I2C_CR1_NACKIE | LL_I2C_CR1_RXIE);
#else
  LL_I2C_EnableIT(MX_I2Cx, LL_I2C_CR1_TCIE | LL_I2C_CR1_STOPIE \
                  | LL_I2C_CR1_NACKIE | LL_I2C_CR1_RXIE);
#endif /* USE_LL_APP_ERROR */
}


/**
  *  Handles the MX_I2Cx event interrupts.
  */
void mx_i2cx_ev_irqhandler(void)
{
  uint32_t it_flags = LL_I2C_READ_REG(MX_I2Cx, ISR);
  uint32_t it_sources = LL_I2C_READ_REG(MX_I2Cx, CR1);

  if ((it_flags & LL_I2C_ISR_NACKF) != 0U)
  {
    if ((it_sources & LL_I2C_CR1_NACKIE) != 0U)
    {
      LL_I2C_ClearFlag_NACK(MX_I2Cx);
      /* No need to generate STOP, it is automatically done */
      I2C_FlushTxDR();
    }
  }
  else if ((it_flags & LL_I2C_ISR_RXNE) != 0U)
  {
    if ((it_sources & LL_I2C_CR1_RXIE) != 0U)
    {
      /* Remove RXNE flag on temporary variable as read done */
      it_flags &= ~LL_I2C_ISR_RXNE;

      *RxData = LL_I2C_ReceiveData8(MX_I2Cx);
      RxData++;
      RxCount--;
      xfer_size--;
    }
  }
  else if ((it_flags & LL_I2C_ISR_TXIS) != 0U)
  {
    if ((it_sources & LL_I2C_CR1_TXIE) != 0U)
    {
      LL_I2C_TransmitData8(MX_I2Cx, *TxData);
      TxData++;
      TxCount--;
      xfer_size--;
    }
  }
  else if ((it_flags & LL_I2C_ISR_TCR) != 0U)
  {
    if ((it_sources & LL_I2C_CR1_TCIE) != 0U)
    {
      if ((TxCount != 0U) && (xfer_size == 0U))
      {
        device_address = LL_I2C_GetSlaveAddr(MX_I2Cx);

        if (TxCount > MAX_NBYTE_SIZE)
        {
          xfer_size = MAX_NBYTE_SIZE;
          xfer_mode = LL_I2C_MODE_RELOAD;
        }
        else
        {
          xfer_size = TxCount;
          xfer_mode = LL_I2C_MODE_AUTOEND;
        }

        LL_I2C_HandleTransfer(MX_I2Cx,
                              device_address,
                              LL_I2C_ADDRSLAVE_7BIT,
                              xfer_size,
                              xfer_mode,
                              LL_I2C_GENERATE_NOSTARTSTOP);
      }
      else
      {
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
        I2C_ITError();
#endif /* USE_LL_APP_ERROR */
      }
    }
  }
  else if ((it_flags & LL_I2C_ISR_TC) != 0U)
  {
    if ((it_sources & LL_I2C_CR1_TCIE) != 0U)
    {
      if (TxCount == 0U)
      {
        if (LL_I2C_IsEnabledAutoEndMode(MX_I2Cx) == 0U)
        {
          /* Generate a stop condition in case of no auto end mode */
          LL_I2C_GenerateStopCondition(MX_I2Cx);
        }
      }
      else
      {
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
        I2C_ITError();
#endif /* USE_LL_APP_ERROR */
      }
    }
  }
  else
  {
    /* Nothing to do */
  }

  if (((it_flags & LL_I2C_ISR_STOPF) != 0U) && ((it_sources & LL_I2C_CR1_STOPIE) != 0U))
  {
    I2C_ITComplete(it_flags);
  }
}


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/**
  *  Handles the MX_I2Cx error interrupts.
  */
void mx_i2cx_err_irqhandler(void)
{
  uint32_t it_flags = LL_I2C_READ_REG(MX_I2Cx, ISR);

  /* I2C bus error interrupt occurred */
  if ((it_flags & LL_I2C_ISR_BERR) != 0U)
  {
    LL_I2C_ClearFlag_BERR(MX_I2Cx);
  }

  /* I2C over-run/under-run interrupt occurred */
  if ((it_flags & LL_I2C_ISR_OVR) != 0U)
  {
    LL_I2C_ClearFlag_OVR(MX_I2Cx);
  }

  /* I2C arbitration loss error interrupt occurred */
  if ((it_flags & LL_I2C_ISR_ARLO) != 0U)
  {
    LL_I2C_ClearFlag_ARLO(MX_I2Cx);
  }

  I2C_ITError();
}


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
  *  MX_I2Cx complete transmitreceive process.
  */
static void I2C_ITComplete(uint32_t it_flags)
{
  uint32_t tmp_it_flags = it_flags;
  i2c_xfer_t completed_xfer = I2C_CurrentXfer;

  LL_I2C_ClearFlag_STOP(MX_I2Cx);

  /* Disable transfer-related interrupts */
  LL_I2C_DisableIT(MX_I2Cx, LL_I2C_CR1_ERRIE | LL_I2C_CR1_STOPIE | LL_I2C_CR1_NACKIE
                   | LL_I2C_CR1_TXIE | LL_I2C_CR1_RXIE);

  /* Clear transfer-specific CR2 fields so the next transaction starts from a
     clean address/direction/size/reload configuration while preserving the other bits. */
  LL_I2C_WRITE_REG(MX_I2Cx, CR2, (LL_I2C_READ_REG(MX_I2Cx, CR2) & I2C_XFER_INFO));

  if ((tmp_it_flags & LL_I2C_ISR_NACKF) != 0U)
  {
    LL_I2C_ClearFlag_NACK(MX_I2Cx);

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    I2C_ITError();
    return;
#endif /* USE_LL_APP_ERROR */
  }

  /* Fetch last receive data if any */
  if ((tmp_it_flags & LL_I2C_ISR_RXNE) != 0U)
  {
    (void)LL_I2C_ReceiveData8(MX_I2Cx);
  }

  /* Flush any remaining TX state left by the completed transfer, especially
     after NACK/STOP, so stale TXIS/TXE conditions do not interfere with the next one. */
  I2C_FlushTxDR();

  I2C_CurrentXfer = I2C_XFER_NONE;

  if ((completed_xfer == I2C_XFER_RX) && (RxCount == 0U))
  {
    I2C_RxTransferCpltCallback();
  }
  else if ((completed_xfer == I2C_XFER_TX) && (TxCount == 0U))
  {
    I2C_TxTransferCpltCallback();
  }
  else
  {
    /* Nothing to do */
  }
}


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/**
  * brief:  MX_I2Cx Tx interrupts error process.
  */
static void I2C_ITError(void)
{
  I2C_CurrentXfer = I2C_XFER_NONE;

  /* Disable all interrupts */
  LL_I2C_DisableIT(MX_I2Cx, LL_I2C_CR1_ERRIE | LL_I2C_CR1_STOPIE | LL_I2C_CR1_NACKIE
                   | LL_I2C_CR1_TXIE | LL_I2C_CR1_RXIE);

  /* Flush any pending TX request so the peripheral does not keep a residual
     TXIS/TXE state when leaving the error path. */
  I2C_FlushTxDR();

  /* Check if a STOPF is detected */
  if (LL_I2C_IsActiveFlag_STOP(MX_I2Cx) != 0U)
  {
    if (LL_I2C_IsActiveFlag_NACK(MX_I2Cx) != 0U)
    {
      LL_I2C_ClearFlag_NACK(MX_I2Cx);
    }

    LL_I2C_ClearFlag_STOP(MX_I2Cx);
  }

  I2C_TransferErrorCallback();
}
#endif /* USE_LL_APP_ERROR */


/**
  * brief:  MX_I2Cx Tx data register flush process.
  */
static void I2C_FlushTxDR(void)
{
  /* If a pending TXIS flag is set, write a dummy data in TXDR to clear it */
  if (LL_I2C_IsActiveFlag_TXIS(MX_I2Cx) != 0U)
  {
    LL_I2C_TransmitData8(MX_I2Cx, 0x00U);
  }

  if (LL_I2C_IsActiveFlag_TXE(MX_I2Cx) == 0U)
  {
    LL_I2C_ClearFlag_TXE(MX_I2Cx);
  }
}
