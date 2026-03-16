/**
  ******************************************************************************
  * file           : example.c
  * brief          : The responder's application handling a data transfer over
  *                  I2C in polling mode with HAL API.
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
#define TIMEOUT 3000U
/* @user: must be equal to the size (in bytes) of the buffer received from the controller */
#define BUFFER_SIZE 41U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_i2c_handle_t *pI2C;  /* pointer referencing the I2C handle from the generated code */

/* Buffer used for reception (sent back to the controller) */
uint8_t RxBuffer[BUFFER_SIZE] = {0U};

/* Private functions prototype -----------------------------------------------*/
static app_status_t HandleTransferCplt(void);
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t i2c_error_code);


/** ########## Step 1 ##########
  * The init of the I2C instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pI2C = mx_example_i2c_init();

  if (pI2C != NULL)
  {
    PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status;   /* memorizes the HAL status of the I2C TX/RX operations */
  uint32_t com_attempts = 0U; /* counts the attempts made to exchange a message between the boards */
  uint32_t i2c_error_code;   /* memorizes the I2C error code limited to the last process */

  /* Clear the Rx buffer before starting a new transfer */
  memset(RxBuffer, 0U, BUFFER_SIZE);

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Start a new communication attempt */
    com_attempts ++;

    /** ########## Step 2 ##########
      * Receives in polling mode a buffer containing a null-terminated string using I2C.
      */
    hal_status = HAL_I2C_SLAVE_Receive(pI2C, RxBuffer, BUFFER_SIZE, TIMEOUT);

    if (hal_status != HAL_OK)
    {
      /* Handle I2C data reception error, then restart the communication */
      i2c_error_code = HAL_I2C_GetLastErrorCodes(pI2C);
      return_status  = HandleTransferError(hal_status, i2c_error_code);
      continue;
    }

    /** ########## Step 3 ##########
      * Transmits in polling mode the received buffer, containing a null-terminated string, using I2C.
      */
    hal_status = HAL_I2C_SLAVE_Transmit(pI2C, RxBuffer, BUFFER_SIZE, TIMEOUT);

    if (hal_status != HAL_OK)
    {
      /* Handle I2C data transmission error, then restart the communication */
      i2c_error_code = HAL_I2C_GetLastErrorCodes(pI2C);
      return_status  = HandleTransferError(hal_status, i2c_error_code);
      continue;
    }

    /* Report the communication status: Data Transfer completed */
    return_status = HandleTransferCplt();
  } /* end while */

  return return_status;
} /* end app_process */


/** De-initializes the I2C instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_i2c_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** brief:  This function is executed when the data transfer is completed.
  * param:  none
  * retval: example status
  */
static app_status_t HandleTransferCplt(void)
{
  PRINTF("[INFO] Responder - Message received and sent back: %s \n", RxBuffer);

  return EXEC_STATUS_OK;
} /* end HandleTransferCplt */

/** brief:  This function is executed in case of a data transfer error.
  * param hal_status:  HAL status of the I2C TX/RX operations.
  * param i2c_error_code:  I2C Error Code.
  * retval: example status
  */
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t i2c_error_code)
{
  PRINTF("[ERROR] Responder - Communication ERROR: hal_status = %" PRIu32 ", \
  HAL_I2C_GetLastErrorCodes = %" PRIu32 ". TRY AGAIN.\n", hal_status, i2c_error_code);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */



