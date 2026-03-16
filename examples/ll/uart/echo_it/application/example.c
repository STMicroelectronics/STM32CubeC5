/**
  ******************************************************************************
  * file           : example.c
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
#include "example.h"
#include "ll_example.h"
#include <string.h> /* importing memcmp and memset functions */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* @user: set the payload size in bytes. The UART message is a string made of this payload and terminated by '\0'. */
#define UART_PAYLOAD_SIZE (9U)
/** Size of the buffer in bytes.
  * +1 is used to store the null character as the received UART message is a null-terminated string.
  */
#define BUFFER_SIZE (UART_PAYLOAD_SIZE + 1U)

/* Exported macro ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t TxTransferComplete; /* Set to 1 if the transmit sequence is correctly completed    */
volatile uint8_t RxTransferComplete; /* Set to 1 if the receive sequence is correctly completed     */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
volatile uint8_t TransferError;      /* Set to 1 if a transmission or a reception error is detected */
#endif /* USE_LL_APP_ERROR */

/* Buffer used for transmission to be sent at the start */
static uint8_t TxStartMessage[] = "\r\n\nIT-Please use the input line below to send data to the device\n\r\
[input]<<";

/* Buffer used for transmission and reception */
uint8_t DataBuffer[BUFFER_SIZE] = {0U};

/* Private functions prototype -----------------------------------------------*/
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
static app_status_t HandleTransferError(void);
#endif /* USE_LL_APP_ERROR */

/** ########## Step 1 ##########
  * The init of the MX_UARTx instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_uartx_init() != NULL)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Clear the MX_UARTx interrupts flags before starting a new transfer */
  TxTransferComplete = 0U;
  RxTransferComplete = 0U;
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  TransferError      = 0U;
#endif /* USE_LL_APP_ERROR */

  /** ########## Step 2 ##########
    * Transmits the start message in interrupt mode using MX_UARTx.
    */
  UART_Transmit_IT(TxStartMessage, (sizeof(TxStartMessage) - 1U));

  /** ########## Step 3 ##########
    * Waits for one of these MX_UARTx interrupts: transfer complete or transfer error.
    */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  while ((TxTransferComplete == 0U) && (TransferError == 0U))
#else
  while (TxTransferComplete == 0U)
#endif /* USE_LL_APP_ERROR */
  {
    /** Put the CPU in Wait For Interrupt state. An MX_UARTx interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
      */
    __WFI();
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  if (TransferError == 1U)
  {
    /* An error occurs during transfer */
    return_status = HandleTransferError();
    goto _app_process_exit;
  }
#endif /* USE_LL_APP_ERROR */

  /** ########## Step 4 ##########
    * Starts the reception in interrupt mode using MX_UARTx.
    */
  memset(DataBuffer, 0x00, BUFFER_SIZE);

  /** The detection of new line ('\n') character or the amount of data elements is received marks the end
    * of the reception.
    */
  UART_ReceiveUntilCM_IT(DataBuffer, BUFFER_SIZE, '\n');

  /** ########## Step 5 ##########
    * Waits for one of these MX_UARTx interrupts: transfer complete or transfer error.
    */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  while ((RxTransferComplete == 0U) && (TransferError == 0U))
#else
  while (RxTransferComplete == 0U)
#endif /* USE_LL_APP_ERROR */
  {
    /** Put the CPU in Wait For Interrupt state. An MX_UARTx interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
      */
    __WFI();
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  if (TransferError == 1U)
  {
    /* An error occurs during transfer */
    return_status = HandleTransferError();
    goto _app_process_exit;
  }
#endif /* USE_LL_APP_ERROR */

  /** ########## Step 6 ##########
    * Transmits the received buffer back to the user.
    */
  TxTransferComplete = 0U;

  UART_Transmit_IT(DataBuffer, BUFFER_SIZE);

  /** ########## Step 7 ##########
    * Waits for one of these MX_UARTx interrupts: transfer complete or transfer error.
  */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  while ((TxTransferComplete == 0U) && (TransferError == 0U))
#else
  while (TxTransferComplete == 0U)
#endif /* USE_LL_APP_ERROR */
  {
    /** Put the CPU in Wait For Interrupt state. An MX_UARTx interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
      */
    __WFI();
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  if (TransferError == 1)
  {
    /* An error occurs during transfer */
    return_status = HandleTransferError();
    goto _app_process_exit;
  }
#endif /* USE_LL_APP_ERROR */

  return_status = EXEC_STATUS_OK;

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
_app_process_exit:
#endif /* USE_LL_APP_ERROR */
  return return_status;
} /* end app_process */


/**
  * Deinitializes the MX_UARTx instance.
  */
app_status_t app_deinit(void)
{
  mx_uartx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** This function is executed when the Tx transfer complete interrupt is generated.
  * @user: the Tx transfer callback can be customized according to the application needs.
  */
void UART_TxTransferCpltCallback(void)
{
  TxTransferComplete = 1U;
}

/** This function is executed when the Rx transfer complete interrupt is generated.
  * @user: the Rx transfer callback can be customized according to the application needs.
  */
void UART_RxTransferCpltCallback(void)
{
  RxTransferComplete = 1U;
}

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/** This function is executed when the transfer error interrupt is generated.
  * @user: the Error transfer callback can be customized according to the application needs.
  */
void UART_TransferErrorCallback(void)
{
  TransferError = 1U;
}
#endif /* USE_LL_APP_ERROR */


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/** brief:  This function is executed in case of a data transfer error.
  *  user:  - This function implementation only illustrates error processing.
  *         - It can be customized to match the application recovery strategy.
  * retval: example status
  */
static app_status_t HandleTransferError(void)
{
  /* Take the time to check the setup before restarting the communication */
  DelayMs(1000U);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
#endif /* USE_LL_APP_ERROR */
