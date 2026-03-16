/**
  ******************************************************************************
  * file           : example.c
  * brief          : The responder side in a polling mode USART communication
  *                  using HAL API.
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
/* @user: set the timeout period in milliseconds for transmitting and receiving data */
#define TIMEOUT 1000U
/* @user: must be equal to the size (in bytes) of the buffer received from the controller */
#define BUFFER_SIZE 43U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_usart_handle_t *pUSART; /* Pointer referencing the USART handle from the generated code */

volatile uint8_t TxTransferComplete; /* Set to 1 if the write transfer is correctly completed */
volatile uint8_t RxTransferComplete; /* Set to 1 if the read transfer is correctly completed */
volatile uint8_t TransferError;      /* Set to 1 if a transmission or a reception error is detected */

/* Buffer used for reception (sent back to the controller)*/
uint8_t RxBuffer[BUFFER_SIZE] = {0U};

/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * The init of the USART instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pUSART = mx_example_usart_init();

  if (pUSART != NULL)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  uint32_t com_attempts = 0; /* memorizes the number of attempts made to exchange a message between the boards */

  /* Clear the Rx buffer before starting a new transfer */
  memset(RxBuffer, 0U, BUFFER_SIZE);

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Clear the USART interrupts flags before starting a new transfer */
    TxTransferComplete = 0U;
    RxTransferComplete = 0U;
    TransferError = 0U;

    /* Start a new communication attempt */
    com_attempts ++;

    /** ########## Step 2 ##########
      * Receives in polling mode a buffer, within a specific timeout.
      */
    if (HAL_USART_Receive(pUSART, RxBuffer, BUFFER_SIZE, TIMEOUT) != HAL_OK)
    {
      /* An error occurs during the USART write transfer. Restart the communication */
      continue;
    }


    /** ########## Step 3 ##########
      * Transmits in polling mode a buffer, within a specific timeout.
      */
    if (HAL_USART_Transmit(pUSART, RxBuffer, BUFFER_SIZE, TIMEOUT) != HAL_OK)
    {
      /* An error occurs during the USART read transfer. Restart the communication */
      continue;
    }


    /* Checks the correctness of the received buffer after the data transfer is completed */
    return_status = EXEC_STATUS_OK;

  } /* end while */

  return return_status;
} /* end app_process */


/** De-initializes the USART instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_usart_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
