/**
  ******************************************************************************
  * @file    com.c
  * @brief   COM module.
  *          This file provides set of firmware functions to manage Com
  *          functionalities.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "com.h"

const char YMODEM_IT_MSG[] = "\r\nYmodem stop by printf\r\n";

static hal_uart_handle_t   *p_uart_handle; /*!< Uart Handler*/
static volatile uint32_t Ymodem = 0;       /*!< set to 1 when Ymodem uses UART> */
static uint8_t Abort;

/**
  * @brief  Initialize COM module.
  * @param  None.
  * @retval HAL Status.
  */
hal_status_t  COM_Init(void)
{
#if defined(__GNUC__)
  setvbuf(stdout, NULL, _IONBF, 0);
#endif /* __GNUC__ */

  p_uart_handle = mx_usart2_uart_init();

  if (p_uart_handle == NULL)
  {
    return HAL_ERROR;
  }

  if (HAL_UART_EnableFifoMode(p_uart_handle) != HAL_OK)
  {
    return HAL_ERROR;
  }

  if (HAL_UART_DisableRxOverRunDetection(p_uart_handle) != HAL_OK)
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  DeInitialize COM module.
  * @retval None.
  * @retval HAL Status.
  */
hal_status_t  COM_DeInit(void)
{
  mx_basic_stdio_deinit();
  return HAL_OK;
}

/**
  * @brief Transmit Data.
  * @param uDataLength: Data pointer to the Data to transmit.
  * @param uTimeout: Timeout duration.
  * @retval Status of the Transmit operation.
  */
hal_status_t COM_Transmit(uint8_t *Data, uint16_t uDataLength, uint32_t uTimeout)
{
  hal_status_t status;
  uint32_t i;
  if (Ymodem)
  {
    /* send abort Y modem transfer sequence */
    for (i = 0; i < 5; i++)
    {
      status = HAL_UART_Transmit(p_uart_handle, (uint8_t *)&Abort, 1, uTimeout);
      if (status != HAL_OK)
      {
        return status;
      }
    }
    status = HAL_UART_Transmit(p_uart_handle, (uint8_t *)YMODEM_IT_MSG, sizeof(YMODEM_IT_MSG), uTimeout);
    if (status != HAL_OK)
    {
      return status;
    }
    Ymodem = 0;
  }
  return HAL_UART_Transmit(p_uart_handle, (uint8_t *)Data, uDataLength, uTimeout);
}

/**
  * @brief Transmit Data function for Y modem.
  * @param uDataLength: Data pointer to the Data to transmit.
  * @param uTimeout: Timeout duration.
  * @retval Status of the Transmit operation.
  */
hal_status_t COM_Transmit_Y(uint8_t *Data, uint16_t uDataLength, uint32_t uTimeout)
{
  if (Ymodem)
  {
    return HAL_UART_Transmit(p_uart_handle, (uint8_t *)Data, uDataLength, uTimeout);
  }
  else
    return HAL_ERROR;
}

/**
  * @brief Receive Data.
  * @param uDataLength: Data pointer to the Data to receive.
  * @param uTimeout: Timeout duration.
  * @retval Status of the Receive operation.
  */
hal_status_t COM_Receive(uint8_t *Data, uint16_t uDataLength, uint32_t uTimeout)
{
  if (!Ymodem)
  {
    return HAL_UART_Receive(p_uart_handle, (uint8_t *)Data, uDataLength, uTimeout);
  }
  else
    return HAL_ERROR;
}
/**
  * @brief Receive Data function Y Modem.
  * @param uDataLength: Data pointer to the Data to receive.
  * @param uTimeout: Timeout duration.
  * @retval Status of the Receive operation.
  */
hal_status_t COM_Receive_Y(uint8_t *Data, uint16_t uDataLength, uint32_t uTimeout)
{
  if (Ymodem)
  {
    return HAL_UART_Receive(p_uart_handle, (uint8_t *)Data, uDataLength, uTimeout);
  }
  else
    return HAL_BUSY;
}

/**
  * @brief  Flush COM Input.
  * @param None.
  * @retval HAL_Status.
  */
hal_status_t COM_Flush(void)
{
  /* Clean the input path */
  LL_USART_RequestRxDataFlush((USART_TypeDef *)((uint32_t)p_uart_handle->instance));
  LL_USART_RequestTxDataFlush((USART_TypeDef *)((uint32_t)p_uart_handle->instance));
  return HAL_OK;
}

hal_status_t  COM_Y_On(uint8_t AbortChar)
{
  hal_status_t status = HAL_ERROR;
  if (!Ymodem)
  {
    Ymodem = 1;
    Abort = AbortChar;
    status = HAL_OK;
  }
  return status;
}
hal_status_t  COM_Y_Off(void)
{
  hal_status_t status = HAL_ERROR;
  if (Ymodem)
  {
    Ymodem = 0;
    status = HAL_OK;
  }
  return status;
}

/**
  * @brief  Ymodem Header Packet Transfer completed callback.
  * @param  FileSize Dimension of the file that will be received.
  * @retval None
  */
__weak hal_status_t Ymodem_HeaderPktRxCpltCallback(uint32_t uFlashDestination, uint32_t uFileSize)
{

  /* NOTE : This function must not be modified, when the callback is needed,
            the Ymodem_HeaderPktRxCpltCallback could be implemented in the user file
   */
  return HAL_OK;
}

/**
  * @brief  Ymodem Data Packet Transfer completed callback.
  * @param  pData Pointer to the buffer.
  * @param  Size Packet dimension.
  * @retval None
  */
__weak hal_status_t Ymodem_DataPktRxCpltCallback(uint8_t *pData, uint32_t uFlashDestination, uint32_t uSize)
{

  /* NOTE : This function must not be modified, when the callback is needed,
            the Ymodem_DataPktRxCpltCallback could be implemented in the user file
   */
  return HAL_OK;
}

