/**
  ******************************************************************************
  * file           : example.c
  * brief          : The controller side in a polling mode USART communication
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
/* @user: set the payload size in bytes. The USART message is a string made of this payload and terminated by '\0'. */
#define USART_PAYLOAD_SIZE 42U
/**
  * Size of the TX and RX buffers in bytes.
  * +1 is used to store the null character as the transmitted USART message is a null-terminated string.
  */
#define BUFFER_SIZE (USART_PAYLOAD_SIZE + 1U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_usart_handle_t *pUSART; /* Pointer referencing the USART handle from the generated code */

/** BufferA, BufferB: fixed-size buffers to transfer alternately.
  * @user: it is possible to modify the buffers content and length, update BUFFER_SIZE if necessary.
  */
static const uint8_t BufferA[BUFFER_SIZE] = "USART Two Boards Communication - Message A";
static const uint8_t BufferB[BUFFER_SIZE] = "USART Two Boards Communication - Message B";
/* Pointer to the buffer used for transmission */
const uint8_t *pTxData = NULL;
/* Buffer used for reception */
uint8_t RxBuffer[BUFFER_SIZE] = {0U};

/* Private functions prototype -----------------------------------------------*/
static inline void UpdateBuffers(void);
static app_status_t HandleTransferCplt(void);


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
  uint32_t com_attempts = 0U; /* memorizes the number of attempts made to exchange a message between the boards */

  UpdateBuffers();

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Start a new communication attempt */
    com_attempts ++;

    /** ########## Step 2 ##########
      * Transmits in polling mode a buffer, within a specific timeout.
      */
    if (HAL_USART_Transmit(pUSART, pTxData, BUFFER_SIZE, TIMEOUT) != HAL_OK)
    {
      /* An error occurs during the USART write transfer. Restart the communication */
      continue;
    }


    /** ########## Step 3 ##########
      * Receives in polling mode a buffer, within a specific timeout.
      */
    if (HAL_USART_Receive(pUSART, RxBuffer, BUFFER_SIZE, TIMEOUT) != HAL_OK)
    {
      /* An error occurs during the USART read transfer. Restart the communication */
      continue;
    }


    /** ########## Step 4 ##########
      * Checks the correctness of the received buffer after the data transfer is completed.
      */
    return_status = HandleTransferCplt();

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


/** brief:  Updates the TX buffer pointer and clears the RX buffer before starting a new transfer.
  * param:  none
  * retval: none
  */
static inline void UpdateBuffers(void)
{
  /* Toggle between BufferA and BufferB to select the message to send */
  if (pTxData == BufferA)
  {
    pTxData = BufferB;
  }
  else
  {
    pTxData = BufferA;
  }

  memset(RxBuffer, 0U, BUFFER_SIZE);
} /* end UpdateBuffers */


/** brief:  Checks the correctness of the data when the transfer is completed.
  * param:  none
  * retval: example status
  */
static app_status_t HandleTransferCplt(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (memcmp(pTxData, RxBuffer, BUFFER_SIZE) == 0)
  {
    return_status = EXEC_STATUS_OK;
  }
  else
  {
    /* Add a delay before restarting the communication */
    HAL_Delay(1000U);
  }

  return return_status;
} /* end HandleTransferCplt */
