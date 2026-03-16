/**
  ******************************************************************************
  * file           : example.c
  * brief          : A half-duplex synchronous SPI communication as responder,
  *                  in polling mode, using the HAL API.
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
/* @user: must be equal to the size (in bytes) of the buffer received from the controller */
#define BUFFER_SIZE 53U
/* @user: Set the timeout period in milliseconds for the transmission and the reception of data */
#define TIMEOUT 1000U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_spi_handle_t *pSPI; /* Pointer referencing the SPI handle from the generated code */


/* Buffer used for reception (sent back to the controller) */
uint8_t RxBuffer[BUFFER_SIZE] = {0U};

/* Private functions prototype -----------------------------------------------*/
static app_status_t HandleTransferCplt(void);
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t spi_error_code);


/** ########## Step 1 ##########
  * The init of the SPI instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status;

  pSPI = mx_example_spi_init();

  if (pSPI != NULL)
  {
    PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }
  else
  {
    PRINTF("[ERROR] Step 1: Device initialization ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status;   /* memorizes the HAL status of the SPI TX/RX operations */
  uint32_t com_attempts = 0U; /* memorizes the number of attempts made to exchange a message between the boards */
  uint32_t spi_error_code;   /* memorizes the SPI error code limited to the last process */

  /* Clear the Rx buffer before starting a new transfer */
  memset(RxBuffer, 0U, BUFFER_SIZE);

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Start a new communication attempt */
    com_attempts ++;

    /** ########## Step 2 ##########
      * Receives in polling mode a buffer containing a null-terminated string using SPI.
      */
    hal_status = HAL_SPI_Receive(pSPI, RxBuffer, BUFFER_SIZE, TIMEOUT);
    if (hal_status != HAL_OK)
    {

      /* Handle SPI data reception error, then restart the communication */
      spi_error_code = HAL_SPI_GetLastErrorCodes(pSPI);
      return_status  = HandleTransferError(hal_status, spi_error_code);
      continue;
    }

    /** ########## Step 3 ##########
      * Transmits in polling mode the received buffer, containing a null-terminated string, using SPI.
      */
    hal_status = HAL_SPI_Transmit(pSPI, RxBuffer, BUFFER_SIZE, TIMEOUT);
    if (hal_status != HAL_OK)
    {
      /* Handle SPI data transmission error, then restart the communication */
      spi_error_code = HAL_SPI_GetLastErrorCodes(pSPI);
      return_status  = HandleTransferError(hal_status, spi_error_code);
      continue;
    }

    /** ########## Step 4 ##########
      * Checks the correctness of the received buffer after the data transfer is completed.
      */
    return_status = HandleTransferCplt();

  } /* end while */

  return return_status;
} /* end app_process */


/** De-initializes the SPI before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{

  mx_example_spi_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** brief:  Checks the correctness of the data when the transfer is completed.
  * param:  none
  * retval: example status
  */
static app_status_t HandleTransferCplt(void)
{
  PRINTF("[INFO] Responder - Message received and sent back: %s \n", RxBuffer);

  return EXEC_STATUS_OK;
} /* end HandleTransferCplt */


/** brief:  This function is executed in case of a data transfer error.
  * param hal_status:  HAL status of the SPI TX/RX operations.
  * param spi_error_code:  SPI Error Code.
  * retval: example status
  */
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t spi_error_code)
{
  PRINTF("[ERROR] Responder - Communication ERROR: hal_status = %" PRIu32 ", \
  HAL_SPI_GetLastErrorCodes = %" PRIu32 ". TRYING AGAIN.\n", hal_status, spi_error_code);

  /* Take the time to check the setup before restarting the communication */
  HAL_Delay(1000U);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
