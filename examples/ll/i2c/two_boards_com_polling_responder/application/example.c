/**
  ******************************************************************************
  * file           : example.c
  * brief          : The responder's application handling a data transfer over
  *                  I2C in polling mode with LL API.
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
#define MAX_COM_ATTEMPTS                (10U)

/* @user: set the timeout period in milliseconds for transmitting and receiving data */
#define I2C_XFER_TIMEOUT_MS             (3000U)

/* @user: set the payload size in bytes. The I2C message is a string made of this payload and terminated by '\0'. */
#define I2C_PAYLOAD_SIZE                (40U)

/** Size of the TX and RX buffers in bytes.
  * +1 is used to store the null character as the transmitted I2C message is a null-terminated string.
  */
#define BUFFER_SIZE                     (I2C_PAYLOAD_SIZE + 1U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Buffer used for reception (sent back to the controller) */
uint8_t RxBuffer[BUFFER_SIZE];

/* Private functions prototype -----------------------------------------------*/
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
static app_status_t HandleTransferError(void);
#endif /* USE_LL_APP_ERROR */

/** ########## Step 1 ##########
  * The init of the MX_I2Cx instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_i2cx_init() != NULL)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  uint32_t com_attempts      = 0U; /* counts the attempts made to exchange a message between the boards */
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Clear the Rx buffer before starting a new transfer */
  memset(RxBuffer, 0U, BUFFER_SIZE);

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Start a new communication attempt */
    com_attempts ++;


    /** ########## Step 2 ##########
      * MX_I2Cx receives in polling mode a buffer containing a null-terminated string.
      */
    if (I2C_Receive(RxBuffer, BUFFER_SIZE, I2C_XFER_TIMEOUT_MS) != SYSTEM_OK)
    {
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
      /* Handle I2C data transmission error, then restart the communication */
      return_status  = HandleTransferError();
      continue;
#endif /* USE_LL_APP_ERROR */
    }


    /** ########## Step 3 ##########
      * MX_I2Cx transmits in polling mode the received buffer, containing a null-terminated string.
      */
    if (I2C_Transmit(RxBuffer, BUFFER_SIZE, I2C_XFER_TIMEOUT_MS) != SYSTEM_OK)
    {
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
      /* Handle I2C data transmission error, then restart the communication */
      return_status  = HandleTransferError();
      continue;
#endif /* USE_LL_APP_ERROR */
    }

    /* Report the communication status: Data Transfer completed */
    return_status = EXEC_STATUS_OK;
  } /* end while */

  return return_status;
} /* end app_process */


/** De-initializes the I2C instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_i2cx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


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
