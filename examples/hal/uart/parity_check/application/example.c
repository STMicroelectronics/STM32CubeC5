/**
  ******************************************************************************
  * file           : example.c
  * brief   This sample code shows how to use UART HAL API to transmit
    *          and receive a data buffer over UART using the HAL API in interrupt mode.
    *          It demonstrates UART communication with parity error detection,
    *          including dynamic parity configuration and error handling.
    *          The scenario is driven by a serial terminal emulator on the PC.
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

/* Exported macro ------------------------------------------------------------*/

/* Size of Reception buffer */
#define BUFFER_SIZE 10U
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_uart_handle_t *pUART; /* Pointer referencing the UART handle from the generated code */

volatile uint8_t TxTransferComplete; /* Set to 1 if the transmit sequence is correctly completed */
volatile uint8_t RxTransferComplete; /* Set to 1 if the receive sequence is correctly completed */
volatile uint8_t TransferError;      /* Set to 1 if a transmission or a reception error is detected */
volatile uint8_t ParityErrorCount;   /* Counts parity errors */

/* Buffer used for reception */
uint8_t DataBuffer[BUFFER_SIZE];

/* Messages sent during communication */
static const uint8_t TxStartMessage[] = "\nUART ParityCheck via IT\n\rEnter 10 chars:\n";
static const uint8_t TxParityErrorMessage[] = "\nParity error detected. Parity set to even.\nResend 10 chars.\n";
static const uint8_t TxEndMessage[] = "\nNo Parity error detected, end of example\n";

/* Private functions prototype -----------------------------------------------*/
static void TxTransferCpltCallback(hal_uart_handle_t *pUART);
static void RxTransferCpltCallback(hal_uart_handle_t *pUART, uint32_t size_byte, hal_uart_rx_event_types_t rx_event);
static void TransferErrorCallback(hal_uart_handle_t *pUART);

/** ########## Step 1 ##########
  * The init of the UART instance and IT instances is triggered by the applicative code.
  * The user callbacks for UART TX/RX transfer completed and transfer error are registered.
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
  if (HAL_UART_RegisterTxCpltCallback(pUART, TxTransferCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_UART_RegisterRxCpltCallback(pUART, RxTransferCpltCallback) != HAL_OK)
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

  /* Clear the UART interrupts flags before starting a new transfer */
  TxTransferComplete = 0U;
  RxTransferComplete = 0U;
  TransferError = 0U;
  ParityErrorCount = 0U;

  /** ########## Step 2 ##########
    * Transmit start message in interrupt mode and wait for completion or error.
  */
  hal_status = HAL_UART_Transmit_IT(pUART, TxStartMessage, sizeof(TxStartMessage) - 1);
  if (hal_status != HAL_OK)
  {
    goto _app_process_exit;
  }

  while ((TxTransferComplete == 0) && (TransferError == 0))
  {
    /** Put the CPU in Wait For Interrupt state. An UART interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
      */
    __WFI();
  }

  if (TransferError == 1)
  {
    goto _app_process_exit;
  }

  /** ########## Step 3 ##########
    * Change parity to EVEN to trigger parity errors,
    * Start reception in interrupt mode and wait for reception completion or error.
  */
  hal_status = HAL_UART_SetParity(pUART, HAL_UART_PARITY_EVEN);
  if (hal_status != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Clear the RxBuffer before starting a new reception */
  memset(DataBuffer, 0x00, BUFFER_SIZE);

  hal_status = HAL_UART_Receive_IT(pUART, DataBuffer, BUFFER_SIZE);

  if (hal_status != HAL_OK)
  {
    goto _app_process_exit;
  }

  while ((RxTransferComplete == 0) && (TransferError == 0))
  {
    /** Put the CPU in Wait For Interrupt state. An UART interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
      */
    __WFI();
  }

  if (TransferError == 1)
  {
    goto _app_process_exit;
  }

  /** ########## Step 4 ##########
    * Check parity errors on received data.
    * If parity error is detected for all characters, revert parity to ODD.
  */
  if (ParityErrorCount == BUFFER_SIZE)
  {
    hal_status = HAL_UART_SetParity(pUART, HAL_UART_PARITY_ODD);
    if (hal_status != HAL_OK)
    {
      goto _app_process_exit;
    }

    /** ########## Step 5 ##########
      * Transmit the parity error message and restart reception simultaneously.
      * Wait for both transmission and reception to complete or for an error to occur.
      */
    TxTransferComplete = 0U;
    TransferError = 0U;
    RxTransferComplete = 0U;

    hal_status = HAL_UART_Transmit_IT(pUART, TxParityErrorMessage, sizeof(TxParityErrorMessage) - 1);
    if (hal_status != HAL_OK)
    {
      goto _app_process_exit;
    }

    /* Clear the RxBuffer before starting a new reception */
    memset(DataBuffer, 0x00, BUFFER_SIZE);

    hal_status = HAL_UART_Receive_IT(pUART, DataBuffer, BUFFER_SIZE);
    if (hal_status != HAL_OK)
    {
      goto _app_process_exit;
    }

    while (((TxTransferComplete == 0) || (RxTransferComplete == 0)) && (TransferError == 0))
    {
      /** Put the CPU in Wait For Interrupt state. An UART interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
        */
      __WFI();
    }

    if (TransferError == 1)
    {
      goto _app_process_exit;
    }
  }
  else
  {
    goto _app_process_exit;
  }

  /** ########## Step 6 ##########
    * Transmits the received buffer back to the user and wait for completion or error.
  */
  TxTransferComplete = 0U;
  TransferError = 0U;

  hal_status = HAL_UART_Transmit_IT(pUART, DataBuffer, BUFFER_SIZE);
  if (hal_status != HAL_OK)
  {
    goto _app_process_exit;
  }

  while ((TxTransferComplete == 0) && (TransferError == 0))
  {
    /** Put the CPU in Wait For Interrupt state. An UART interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
      */
    __WFI();
  }

  if (TransferError == 1)
  {
    goto _app_process_exit;
  }

  /** ########## Step 7 ##########
    * Transmit the end message and wait for completion or error.
    */
  TxTransferComplete = 0U;
  TransferError = 0U;

  hal_status = HAL_UART_Transmit_IT(pUART, TxEndMessage, sizeof(TxEndMessage) - 1);
  if (hal_status != HAL_OK)
  {
    goto _app_process_exit;
  }

  while ((TxTransferComplete == 0) && (TransferError == 0))
  {
    /** Put the CPU in Wait For Interrupt state. An UART interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
      */
    __WFI();
  }

  if (TransferError == 1)
  {
    goto _app_process_exit;
  }
  else
  {
    /* The complete RX-TX cycle has been executed without error */
    return_status = EXEC_STATUS_OK;
  }
_app_process_exit:
  return return_status;
} /* end app_process */


/* Transmission end of transfer completed callback */
static void TxTransferCpltCallback(hal_uart_handle_t *huart)
{
  TxTransferComplete = 1U;
}

/* Reception end of transfer completed callback */
static void RxTransferCpltCallback(hal_uart_handle_t *huart, uint32_t size_byte, hal_uart_rx_event_types_t rx_event)
{
  RxTransferComplete = 1U;
}

/* UART transfer error callback */
static void TransferErrorCallback(hal_uart_handle_t *huart)
{
  if ((pUART->last_reception_error_codes & HAL_UART_RECEIVE_ERROR_PE) != 0U)
  {
    /* In case of parity error */
    ParityErrorCount++;

    /* Parity error code clear*/
    pUART->last_reception_error_codes = (pUART->last_reception_error_codes & (~HAL_UART_RECEIVE_ERROR_PE));
  }
  else
  {
    TransferError = 1U;
  }
}

/* De-initializes the UART and IT instances before leaving the scenario. */
app_status_t app_deinit(void)
{
  mx_example_uart_deinit();
  return EXEC_STATUS_OK;
} /* end app_deinit */

