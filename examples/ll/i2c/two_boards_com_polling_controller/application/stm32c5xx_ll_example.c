/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : The controller's application handling a data transfer over
  *                  I2C in polling mode with LL API.
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
#define I2C_XFER_INFO ~((uint32_t)(I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN))
#define I2C_FLAG_INACTIVE (0U)
#define I2C_FLAG_ACTIVE   (1U)
#define MAX_NBYTE_SIZE    (255U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
static system_status_t I2C_WaitOnFlagUntilTimeout(uint32_t flag, uint32_t status, uint32_t timeout_ms);
static system_status_t I2C_WaitOnRXNEFlagUntilTimeout(uint32_t timeout_ms);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
static system_status_t I2C_IsErrorOccurred(uint32_t it_flags, uint32_t timeout_ms);
#endif /* USE_LL_APP_ERROR */

/* Private functions ---------------------------------------------------------*/

/**
  * brief:  MX_I2Cx transmit an amount of data in blocking mode (polling mode).
  * param:  device_address Target device address.
  * param:  p_tx_buffer    Pointer to transmission data buffer.
  * param:  size_byte      Number of bytes to transfer.
  * param:  timeout_ms     Timeout duration in millisecond (This parameter is unused when USE_LL_APP_TIMEOUT == 0).
  * retval: SYSTEM_OK When data is successfully transmitted.
  * retval: SYSTEM_PERIPHERAL_ERROR When data is not successfully transmitted.
  */
inline system_status_t I2C_Transmit(uint32_t device_address,
                                    const void *p_tx_buffer,
                                    uint32_t size_byte,
                                    uint32_t timeout_ms)
{
  const uint8_t *tx_data = (const uint8_t *)p_tx_buffer;
  uint32_t tx_count      = size_byte;
  uint32_t xfer_size;
  system_status_t return_status = SYSTEM_OK;

  if (I2C_WaitOnFlagUntilTimeout(LL_I2C_ISR_BUSY, I2C_FLAG_ACTIVE, timeout_ms) == SYSTEM_OK)
  {
    /* Send slave address */
    /* Set NBYTES to write and reload if tx_count > MAX_NBYTE_SIZE and generate RESTART */
    if (tx_count > MAX_NBYTE_SIZE)
    {
      xfer_size = MAX_NBYTE_SIZE;

      /* Master Generate Start condition for a write request :                */
      /*    - to the Slave with a 7-Bit address                               */
      /*    - with a reload mode condition generation when transmit all bytes */
      LL_I2C_HandleTransfer(MX_I2Cx,
                            device_address,
                            LL_I2C_ADDRSLAVE_7BIT,
                            size_byte,
                            LL_I2C_MODE_RELOAD,
                            LL_I2C_GENERATE_START_WRITE);
    }
    else
    {
      xfer_size = tx_count;

      /* Master Generate Start condition for a write request :               */
      /*    - to the Slave with a 7-Bit address                              */
      /*    - with an auto stop condition generation when transmit all bytes */
      LL_I2C_HandleTransfer(MX_I2Cx,
                            device_address,
                            LL_I2C_ADDRSLAVE_7BIT,
                            size_byte,
                            LL_I2C_MODE_AUTOEND,
                            LL_I2C_GENERATE_START_WRITE);
    }

    while (tx_count > 0U)
    {
      return_status = I2C_WaitOnFlagUntilTimeout(LL_I2C_ISR_TXIS, I2C_FLAG_INACTIVE, timeout_ms);
      if (return_status == SYSTEM_OK)
      {
        /* Write data to TXDR */
        LL_I2C_TransmitData8(MX_I2Cx, *tx_data);
        tx_data++;
        tx_count--;
        xfer_size--;

        if ((tx_count != 0U) && (xfer_size == 0U))
        {
          /* Wait until TCR flag is set */
          return_status = I2C_WaitOnFlagUntilTimeout(LL_I2C_ISR_TCR, I2C_FLAG_INACTIVE, timeout_ms);
          if (return_status == SYSTEM_OK)
          {
            if (tx_count > MAX_NBYTE_SIZE)
            {
              xfer_size = MAX_NBYTE_SIZE;
              LL_I2C_HandleTransfer(MX_I2Cx,
                                    device_address,
                                    LL_I2C_ADDRSLAVE_7BIT,
                                    size_byte,
                                    LL_I2C_MODE_RELOAD,
                                    LL_I2C_GENERATE_NOSTARTSTOP);
            }
            else
            {
              xfer_size = tx_count;
              LL_I2C_HandleTransfer(MX_I2Cx,
                                    device_address,
                                    LL_I2C_ADDRSLAVE_7BIT,
                                    size_byte,
                                    LL_I2C_MODE_AUTOEND,
                                    LL_I2C_GENERATE_NOSTARTSTOP);
            }
          }
        }
      }

      if (return_status != SYSTEM_OK)
      {
        break;
      }
    }

    if (return_status == SYSTEM_OK)
    {
      /* Wait until STOPF flag is set */
      if (I2C_WaitOnFlagUntilTimeout(LL_I2C_ISR_STOPF, I2C_FLAG_INACTIVE, timeout_ms) == SYSTEM_OK)
      {
        LL_I2C_ClearFlag_STOP(MX_I2Cx);
        LL_I2C_WRITE_REG(MX_I2Cx, CR2, (LL_I2C_READ_REG(MX_I2Cx, CR2) & I2C_XFER_INFO));
      }
    }
  }

  return return_status;
}


/**
  * brief:  MX_I2Cx receive an amount of data in blocking mode (polling mode).
  * param:  device_address Target device address.
  * param:  p_rx_buffer    Pointer to reception data buffer.
  * param:  size_byte      Number of bytes to transfer.
  * param:  timeout_ms     Timeout duration in millisecond (This parameter is unused when USE_LL_APP_TIMEOUT == 0).
  * retval: SYSTEM_OK When data is successfully received.
  * retval: SYSTEM_PERIPHERAL_ERROR When data is not successfully received.
  */
inline system_status_t I2C_Receive(uint32_t device_address,
                                   void *p_rx_buffer,
                                   uint32_t size_byte,
                                   uint32_t timeout_ms)
{
  uint8_t *rx_data  = (uint8_t *)p_rx_buffer;
  uint32_t rx_count = size_byte;
  uint32_t xfer_size;
  system_status_t return_status = SYSTEM_OK;

  if (I2C_WaitOnFlagUntilTimeout(LL_I2C_ISR_BUSY, I2C_FLAG_ACTIVE, timeout_ms) == SYSTEM_OK)
  {
    /* Send slave address */
    /* Set NBYTES to write and reload if rx_count > MAX_NBYTE_SIZE and generate RESTART */
    if (rx_count > MAX_NBYTE_SIZE)
    {
      xfer_size = MAX_NBYTE_SIZE;

      /* Master Generate Start condition for a read request :                */
      /*    - to the Slave with a 7-Bit address                               */
      /*    - with a reload mode condition generation when receive all bytes */
      LL_I2C_HandleTransfer(MX_I2Cx,
                            device_address,
                            LL_I2C_ADDRSLAVE_7BIT,
                            size_byte,
                            LL_I2C_MODE_RELOAD,
                            LL_I2C_GENERATE_START_READ);
    }
    else
    {
      xfer_size = rx_count;

      /* Master Generate Start condition for a write request :               */
      /*    - to the Slave with a 7-Bit address                              */
      /*    - with an auto stop condition generation when transmit all bytes */
      LL_I2C_HandleTransfer(MX_I2Cx,
                            device_address,
                            LL_I2C_ADDRSLAVE_7BIT,
                            size_byte,
                            LL_I2C_MODE_AUTOEND,
                            LL_I2C_GENERATE_START_READ);
    }

    while (rx_count > 0U)
    {
      return_status = I2C_WaitOnRXNEFlagUntilTimeout(timeout_ms);
      if (return_status == SYSTEM_OK)
      {
        *rx_data = LL_I2C_ReceiveData8(MX_I2Cx);
        rx_data++;
        xfer_size--;
        rx_count--;

        if ((rx_count != 0U) && (xfer_size == 0U))
        {
          /* Wait until TCR flag is set */
          return_status = I2C_WaitOnFlagUntilTimeout(LL_I2C_ISR_TCR, I2C_FLAG_INACTIVE, timeout_ms);
          if (return_status == SYSTEM_OK)
          {
            if (rx_count > MAX_NBYTE_SIZE)
            {
              xfer_size = MAX_NBYTE_SIZE;
              LL_I2C_HandleTransfer(MX_I2Cx,
                                    device_address,
                                    LL_I2C_ADDRSLAVE_7BIT,
                                    size_byte,
                                    LL_I2C_MODE_RELOAD,
                                    LL_I2C_GENERATE_NOSTARTSTOP);
            }
            else
            {
              xfer_size = rx_count;
              LL_I2C_HandleTransfer(MX_I2Cx,
                                    device_address,
                                    LL_I2C_ADDRSLAVE_7BIT,
                                    size_byte,
                                    LL_I2C_MODE_AUTOEND,
                                    LL_I2C_GENERATE_NOSTARTSTOP);
            }
          }
        }
      }

      if (return_status != SYSTEM_OK)
      {
        break;
      }
    }

    if (return_status == SYSTEM_OK)
    {
      return_status = I2C_WaitOnFlagUntilTimeout(LL_I2C_ISR_STOPF, I2C_FLAG_INACTIVE, timeout_ms);
      if (return_status == SYSTEM_OK)
      {
        LL_I2C_ClearFlag_STOP(MX_I2Cx);
        LL_I2C_WRITE_REG(MX_I2Cx, CR2, (LL_I2C_READ_REG(MX_I2Cx, CR2) & I2C_XFER_INFO));
      }
    }
  }

  return return_status;
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


/** brief:  This function waits until a flag is no longer in the specified status.
  * param:  flag       Specifies the I2C flag to check
  * param:  status     The actual flag status (SET or RESET)
  * param:  timeout_ms Timeout duration in millisecond (This parameter is unused when USE_LL_APP_TIMEOUT == 0).
  * retval: system status (see system_status_t).
  */
static system_status_t I2C_WaitOnFlagUntilTimeout(uint32_t flag, uint32_t status, uint32_t timeout_ms)
{
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  uint32_t it_flags = LL_I2C_READ_REG(MX_I2Cx, ISR);
#endif /* USE_LL_APP_ERROR */
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
  uint32_t timeout = timeout_ms;
#endif /* USE_LL_APP_TIMEOUT */
  system_status_t return_status = SYSTEM_OK;

  while (LL_I2C_IsActiveFlag(MX_I2Cx, flag) == status)
  {
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    /* Check if an error is detected */
    if (I2C_IsErrorOccurred(it_flags, timeout_ms) != SYSTEM_OK)
    {
      return_status = SYSTEM_PERIPHERAL_ERROR;
      break;
    }
#endif /* USE_LL_APP_ERROR */

#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
    {
      if (timeout-- == 0)
      {
        return_status = SYSTEM_PERIPHERAL_ERROR;
        break;
      }
    }
#endif /* USE_LL_APP_TIMEOUT */

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    it_flags = LL_I2C_READ_REG(MX_I2Cx, ISR);
#endif /* USE_LL_APP_ERROR */
  }

  return return_status;
}


/**
  * brief:  This function handles I2C Communication timeout for specific usage of RXNE flag.
  * param:  timeout_ms Timeout duration in millisecond (This parameter is unused when USE_LL_APP_TIMEOUT == 0).
  * retval: system status (see system_status_t).
  */
static system_status_t I2C_WaitOnRXNEFlagUntilTimeout(uint32_t timeout_ms)
{
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  uint32_t it_flags = LL_I2C_READ_REG(MX_I2Cx, ISR);
#endif /* USE_LL_APP_ERROR */
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
  uint32_t timeout = timeout_ms;
#endif /* USE_LL_APP_TIMEOUT */
  system_status_t return_status = SYSTEM_OK;

  while (LL_I2C_IsActiveFlag_RXNE(MX_I2Cx) == 0U)
  {
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    /* Check if an error is detected */
    if (I2C_IsErrorOccurred(it_flags, timeout_ms) != SYSTEM_OK)
    {
      return_status = SYSTEM_PERIPHERAL_ERROR;
      break;
    }
#endif /* USE_LL_APP_ERROR */

    /* Check if a STOPF is detected */
    if (LL_I2C_IsActiveFlag_STOP(MX_I2Cx) != 0U)
    {
      /* Check if an RXNE is pending */
      /* Store last receive data */
      if ((LL_I2C_IsActiveFlag_RXNE(MX_I2Cx) != 0U))
      {
        break;
      }
      else
      {
        if (LL_I2C_IsActiveFlag_NACK(MX_I2Cx) != 0U)
        {
          LL_I2C_ClearFlag_NACK(MX_I2Cx);
        }

        LL_I2C_ClearFlag_STOP(MX_I2Cx);
        LL_I2C_WRITE_REG(MX_I2Cx, CR2, (LL_I2C_READ_REG(MX_I2Cx, CR2) & I2C_XFER_INFO));
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
        return_status = SYSTEM_PERIPHERAL_ERROR;
        break;
#endif /* USE_LL_APP_ERROR */
      }
    }

#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
    {
      if (timeout-- == 0)
      {
        if (LL_I2C_IsActiveFlag_RXNE(MX_I2Cx) == 0U)
        {
          return_status = SYSTEM_PERIPHERAL_ERROR;
          break;
        }
      }
    }
#endif /* USE_LL_APP_TIMEOUT */

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    it_flags = LL_I2C_READ_REG(MX_I2Cx, ISR);
#endif /* USE_LL_APP_ERROR */
  }

  return return_status;
}


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/**
  * brief:  This function handles errors detection during an I2C communication.
  * param:  it_flags   Flags register before function call.
  * param:  timeout_ms Timeout duration in millisecond (This parameter is unused when USE_LL_APP_TIMEOUT == 0).
  * retval: system status (see system_status_t).
  */
static system_status_t I2C_IsErrorOccurred(uint32_t it_flags, uint32_t timeout_ms)
{
  uint32_t tmp_register;
  uint32_t tmp_it_flags = it_flags;
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
  uint32_t timeout = timeout_ms;
#endif /* USE_LL_APP_TIMEOUT */
  system_status_t return_status = SYSTEM_OK;

  if ((tmp_it_flags & LL_I2C_ISR_NACKF) != 0U)
  {
    LL_I2C_ClearFlag_NACK(MX_I2Cx);

    /* Wait until STOP flag is set or timeout occurred */
    /* AutoEnd must be initiated after AF */
    while ((LL_I2C_IsActiveFlag_STOP(MX_I2Cx) == 0U) && (return_status == SYSTEM_OK))
    {
      /* Check for the timeout */
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
      if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
      {
        if (timeout-- == 0)
        {
#endif /* USE_LL_APP_TIMEOUT */
          tmp_register = (uint32_t)(LL_I2C_READ_REG(MX_I2Cx, CR2) & I2C_CR2_STOP);

          /* In case of I2C still busy, try to regenerate a STOP manually */
          if ((LL_I2C_IsActiveFlag_BUSY(MX_I2Cx) != 0U) && (tmp_register != I2C_CR2_STOP))
          {
            LL_I2C_GenerateStopCondition(MX_I2Cx);
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
            timeout = timeout_ms;
#endif /* USE_LL_APP_TIMEOUT */
          }

          while (LL_I2C_IsActiveFlag_STOP(MX_I2Cx) == 0U)
          {
            /* Check for the timeout */
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
            if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
            {
              if (timeout-- == 0)
              {
                return_status = SYSTEM_PERIPHERAL_ERROR;
                break;
              }
            }
#endif /* USE_LL_APP_TIMEOUT */
          }
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
        }
      }
#endif /* USE_LL_APP_TIMEOUT */
    }

    /* In case STOP flag is detected, clear it */
    if (return_status == SYSTEM_OK)
    {
      LL_I2C_ClearFlag_STOP(MX_I2Cx);
    }

    return_status = SYSTEM_PERIPHERAL_ERROR;
  }

  if ((LL_I2C_READ_REG(MX_I2Cx, ISR) & (I2C_ISR_OVR | I2C_ISR_ARLO | I2C_ISR_BERR)) != 0U)
  {
    LL_I2C_WRITE_REG(MX_I2Cx, ICR, (I2C_ISR_OVR | I2C_ISR_ARLO | I2C_ISR_BERR));

    return_status = SYSTEM_PERIPHERAL_ERROR;
  }

  if (return_status != SYSTEM_OK)
  {
    /* If a pending TXIS flag is set */
    /* Write a dummy data in TXDR to clear it */
    if (LL_I2C_IsActiveFlag_TXIS(MX_I2Cx) != 0U)
    {
      LL_I2C_TransmitData8(MX_I2Cx, 0x00U);
    }

    if (LL_I2C_IsActiveFlag_TXE(MX_I2Cx) == 0U)
    {
      LL_I2C_ClearFlag_TXE(MX_I2Cx);
    }

    LL_I2C_WRITE_REG(MX_I2Cx, CR2, (LL_I2C_READ_REG(MX_I2Cx, CR2) & I2C_XFER_INFO));
  }

  return return_status;
}
#endif /* USE_LL_APP_ERROR */
