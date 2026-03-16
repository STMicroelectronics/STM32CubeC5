/**
  ******************************************************************************
  * file           : example.c
  * brief          : The responder side in a polling mode UART communication
  *                  between two boards using HAL API.
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
/* @user: set the timeout period in milliseconds for receiving and transmitting data */
#define TIMEOUT 1000U
/* @user: must be equal to the size (in bytes) of the buffer received from the controller */
#define BUFFER_SIZE 42U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_uart_handle_t *pUART; /* pointer referencing the UART handle from the generated code */

/* Buffer used for reception (sent back to the controller) */
uint8_t RxBuffer[BUFFER_SIZE] = {0};

/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * The init of the UART instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pUART = mx_example_uart_init();

  if (pUART != NULL)
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
  memset(RxBuffer, 0, BUFFER_SIZE);

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Start a new communication attempt */
    com_attempts ++;

    /** ########## Step 2 ##########
      * Receives in polling mode a buffer, within a specific timeout.
      */
    if (HAL_UART_Receive(pUART, RxBuffer, BUFFER_SIZE, TIMEOUT) != HAL_OK)
    {
      /* An error occurs during the UART read transfer. Restart the communication */
      continue;
    }

    /** ########## Step 3 ##########
      * Transmits in polling mode the received buffer, within a specific timeout.
      */
    if (HAL_UART_Transmit(pUART, RxBuffer, BUFFER_SIZE, TIMEOUT) != HAL_OK)
    {
      /* An error occurs during the UART write transfer. Restart the communication */
      continue;
    }

    /* Report the communication status: Data Transfer completed */
    return_status = EXEC_STATUS_OK;

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
