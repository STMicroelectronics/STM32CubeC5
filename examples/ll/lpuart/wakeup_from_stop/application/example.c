/**
  ******************************************************************************
  * file           : example.c
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
#include "example.h"
#include "ll_example.h"
#include <string.h> /* importing memcmp and memset functions */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* @user: set the maximum number of attempts to complete a data transfer between the two boards */
#define MAX_COM_ATTEMPTS (10U)

/* @user: set the payload size in bytes:
          The MX_LPUARTx message is a string made of this payload and terminated by '\0'. */
#define UART_PAYLOAD_SIZE (41U)

/**
  * Size of the TX and RX buffers in bytes.
  * +1 is used to store the null character as the transmitted MX_LPUARTx message is a null-terminated string.
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

/* Buffer used for reception (sent back to the controller) */
uint8_t RxBuffer[BUFFER_SIZE];

/* Private functions prototype -----------------------------------------------*/
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
static app_status_t HandleTransferError(void);
#endif /* USE_LL_APP_ERROR */

/** ########## Step 1 ##########
  * The init of the MX_LPUARTx instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_lpuartx_init() != NULL)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  uint32_t com_attempts = 0; /* memorizes the number of attempts made to exchange a message between the boards */
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Clear the Rx buffer before starting a new transfer */
  memset(RxBuffer, 0U, BUFFER_SIZE);

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Clear the MX_LPUARTx interrupts flags before starting a new transfer */
    TxTransferComplete = 0U;
    RxTransferComplete = 0U;

    /* Start a new communication attempt */
    com_attempts ++;


    /** ########## Step 2 ##########
      * MX_LPUARTx receives a buffer containing a null-terminated string in interrupt mode.
      */
    UART_Receive_IT(RxBuffer, BUFFER_SIZE);


    /** ########## Step 3 ##########
      * Waits for one of these MX_LPUARTx interrupts: read transfer complete or transfer error.
      */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    while ((RxTransferComplete == 0U) && (TransferError == 0U))
#else
    while (RxTransferComplete == 0U)
#endif /* USE_LL_APP_ERROR */
    {
      /* Put the CPU in low power mode. An MX_LPUARTx interrupt can wake up the CPU */
      mx_pwr_enter_low_power();
    }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    if ((mx_pwr_system_was_low_power() == 0U) || (TransferError != 0U))
    {
      /* An error occurs during transfer */
      return_status = HandleTransferError();
      continue;
    }
#endif /* USE_LL_APP_ERROR */

    /* Reconfigure the system clock configuration for systick counting */
    mx_pwr_exit_low_power();

    /** ########## Step 4 ##########
      * Starts the MX_LPUARTx transmission in interrupt.
      */
    UART_Transmit_IT(RxBuffer, BUFFER_SIZE);

    /** ########## Step 5 ##########
      * Waits for one of these MX_LPUARTx interrupts: write transfer complete or transfer error.
      */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    while ((TxTransferComplete == 0U) && (TransferError == 0U))
#else
    while (TxTransferComplete == 0U)
#endif /* USE_LL_APP_ERROR */
    {
      /** Put the CPU in Wait For Interrupt state. An MX_LPUARTx interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
        */

      __WFI();
    }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    if (TransferError == 1)
    {
      /* An error occurs during transfer */
      return_status = HandleTransferError();
      continue;
    }
#endif /* USE_LL_APP_ERROR */

    return_status = EXEC_STATUS_OK;
  } /* end while */

  return return_status;
} /* end app_process */


/**
  * Deinitializes the MX_LPUARTx instance.
  */
app_status_t app_deinit(void)
{
  mx_lpuartx_deinit();

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
