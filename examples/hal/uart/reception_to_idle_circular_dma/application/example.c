/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to use the HAL UART API for reception to IDLE event
                     in circular DMA mode.

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
#include "example.h"
#include <string.h> /* importing memcmp and memset functions */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* @user: set the maximum number of attempts to complete a data transfer between the two boards */
#define MAX_COM_ATTEMPTS 10U
#define TIMEOUT 1000U /* Timeout period, in milliseconds, of the UART on transmission of a character */

/** The maximum size of the reception buffer in bytes.
  * @user: Set the maximum size of the buffer.
  */
#define BUFFER_SIZE 50U
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_uart_handle_t *pUART;  /* pointer referencing the UART handle from the generated code */

/* Text strings printed on PC Com port for user information */
uint8_t TxStartMessage[] = "\r\nUART Example : Enter characters to fill reception buffers.\r\n";

/* Buffer used in User callback */
uint8_t RxBuffer[BUFFER_SIZE] = {0U};
/* Size of the received buffer */
uint32_t NbReceivedChars;

volatile uint8_t RxTransferComplete; /* Set to 1 if the read transfer is correctly completed */
volatile uint8_t TransferError;      /* Set to 1 if a transmission or a reception error is detected */

/* Flag set in RX transfer complete callback to trigger data transmission (loopback) in app_process() */
volatile uint8_t TxUartFlag;

/* Buffer used to store data ready for user loopback */
uint8_t TxBuffer[BUFFER_SIZE] = {0U};

/* Private functions prototype -----------------------------------------------*/

/* Functions allowing the user to configure dynamically the UART callbacks instead of weak functions */
static void RxReceiverCpltCallback(hal_uart_handle_t *pUART, uint32_t size_byte, hal_uart_rx_event_types_t rx_event);
static void TransferErrorCallback(hal_uart_handle_t *pUART);

/** ########## Step 1 ##########
  * The init of the UART and DMA instances are triggered by the applicative code.
  * The UART callbacks for RX transfer completed and transfer error are registered.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pUART = mx_example_uart_init();
  if (pUART == NULL)
  {
    goto _app_init_exit;
  }

  /* Register the UART callbacks */
  if (HAL_UART_RegisterRxCpltCallback(pUART, RxReceiverCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_UART_RegisterErrorCallback(pUART, TransferErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status;   /* memorizes the HAL status of the UART TX/RX operations */
  uint32_t com_attempts = 0; /* memorizes the number of attempts made to exchange a message between the boards */


  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Clear the UART interrupts flags before starting a new transfer */
    RxTransferComplete = 0U;
    TransferError = 0U;
    TxUartFlag = 0U;
    NbReceivedChars = 0U;

    /* Start a new communication attempt */
    com_attempts ++;

    /** ########## Step 2 ##########
      * Send an information message to the user via the serial communication port (PC Com port).
      */
    hal_status = HAL_UART_Transmit(pUART, TxStartMessage, sizeof(TxStartMessage) - 1, TIMEOUT);
    if (hal_status != HAL_OK)
    {
      /* An error occurs at the startup of the UART transmission. Restart the communication */
      continue;
    }


    /** ########## Step 3 ##########
      * Start continuous data reception via UART using DMA.
      */

    /**  Initializes Rx sequence using Reception To Idle event API.
      *  Use of HAL_UART_ReceiveToIdle_DMA service, will generate calls to user defined
      *  HAL_UARTEx_RxEventCallback callback
      * for each occurrence of following events :
        - the expected number of data is received
        - IDLE event on UART Rx line (indicating a pause is UART reception flow)
      */

    hal_status = HAL_UART_ReceiveToIdle_DMA(pUART, RxBuffer, BUFFER_SIZE);

    /** ########## Step 4 ##########
        * Waits for one of these UART interrupts: read transfer complete or transfer error.
        */
    while ((RxTransferComplete == 0) && (TransferError == 0))
    {
      /** Put the CPU in Wait For Interrupt state. An UART interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
        */
      __WFI();
    }

    if (TransferError == 1)
    {
      /* Error while uart reception. Restart the communication */
      continue;
    }
    /** ########## Step 5 ##########
      * The received data are sent back on UART Tx (loopback)
      */

    if (TxUartFlag == 1)
    {
      HAL_UART_Transmit(pUART, TxBuffer, NbReceivedChars, TIMEOUT);
    }
    else
    {
      /* It handles a transfer error */
      goto _app_process_exit;
    }

    return_status = EXEC_STATUS_OK;
_app_process_exit:
    return return_status;
  } /* end while */

  return return_status;
} /* end app_process */


/** De-initializes the UART instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_uart_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** UART transfer error callback
  * This implementation of the UART transfer error callback can be customized.
  * This function is executed when the transfer error interrupt is generated.
  */

static void TransferErrorCallback(hal_uart_handle_t *huart)
{
  TransferError = 1U;
}


/** Reception end of transfer completed callback
  * This implementation of the UART Rx transfer complete callback can be customized.
  * This function is executed when the Rx transfer complete interrupt is generated.
  */

static void RxReceiverCpltCallback(hal_uart_handle_t *huart, uint32_t size_byte, hal_uart_rx_event_types_t rx_event)
{
  static uint8_t old_pos = 0;
  uint8_t i;

  /* Check if number of received data in reception buffer has changed */
  if (size_byte != old_pos)
  {
    /* Set the RxTransferComplete to 1 if the read transfer is correctly completed */
    RxTransferComplete = 1U;
    /* Check if position of index in reception buffer has simply be increased
       or if end of buffer has been reached */
    if (size_byte > old_pos)
    {
      /* Current position is higher than previous one */
      NbReceivedChars = size_byte - old_pos;
      /* Copy received data in "User" buffer for evacuation */
      for (i = 0; i < NbReceivedChars; i++)
      {
        TxBuffer[i] = RxBuffer[old_pos + i];
      }
    }
    else
    {
      /* Current position is lower than previous one : end of buffer has been reached */
      /* First copy data from current position till end of buffer */
      NbReceivedChars = BUFFER_SIZE - old_pos;
      /* Copy received data in "User" buffer for evacuation */
      for (i = 0; i < NbReceivedChars; i++)
      {
        TxBuffer[i] = RxBuffer[old_pos + i];
      }
      /* Check and continue with beginning of buffer */
      if (size_byte > 0)
      {
        for (i = 0; i < size_byte; i++)
        {
          TxBuffer[NbReceivedChars + i] = RxBuffer[i];
        }
        NbReceivedChars += size_byte;
      }
    }
    /* Set the TX flag to 1 once the buffer is ready to be sent back to the user
       The actual transmission is handled in app_process() to avoid doing it inside the IRQ handler. */
    TxUartFlag = 1;
  }
  /* Update old_pos as new reference of position in User Rx buffer that
     indicates position to which data have been processed */
  old_pos = size_byte;
}

