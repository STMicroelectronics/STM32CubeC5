/**
  ******************************************************************************
  * file           : example.c
  * brief          : The controller's application handling a data transfer over
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

/* @user: define the responder 7-bit address */
#define I2C_RESPONDER_ADDR_7BIT         (0x3FU)

/* The responder 7-bit address must be shifted to the left before calling I2C APIs */
#define I2C_RESPONDER_ADDR_7BIT_SHIFTED (I2C_RESPONDER_ADDR_7BIT << 1U)

/* @user: set the payload size in bytes (The max payload size  = 254).
          The I2C message is a string made of this payload and terminated by '\0'. */
#define I2C_PAYLOAD_SIZE                (40U)

/** Size of the TX and RX buffers in bytes.
  * +1 is used to store the null character as the transmitted I2C message is a null-terminated string.
  */
#define BUFFER_SIZE                     (I2C_PAYLOAD_SIZE + 1U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/** BufferA, BufferB: fixed-size buffers to transfer alternately.
  * @user: it is possible to modify the messages content and length, update I2C_PAYLOAD_SIZE if necessary.
  */
static const uint8_t BufferA[BUFFER_SIZE] = "I2C Two Boards Communication - Message A";
static const uint8_t BufferB[BUFFER_SIZE] = "I2C Two Boards Communication - Message B";

/* Pointer to the buffer used for transmission */
const uint8_t *pTxData = NULL;

/* Buffer used for reception */
uint8_t RxBuffer[BUFFER_SIZE];

/* Private functions prototype -----------------------------------------------*/
static inline void UpdateBuffers(void);
static app_status_t HandleTransferCplt(void);
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
  uint32_t com_attempts      = 0U; /* memorizes the number of attempts made to exchange a message between the boards */
  app_status_t return_status = EXEC_STATUS_ERROR;

  UpdateBuffers();

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Start a new communication attempt */
    com_attempts ++;


    /** ########## Step 2 ##########
      * MX_I2Cx transmits in polling mode the buffer pointed by pTxData, containing a null-terminated string.
      */
    if (I2C_Transmit(I2C_RESPONDER_ADDR_7BIT_SHIFTED, pTxData, BUFFER_SIZE, I2C_XFER_TIMEOUT_MS) != SYSTEM_OK)
    {
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
      /* Handle I2C data transmission error, then restart the communication */
      return_status  = HandleTransferError();
      continue;
#endif /* USE_LL_APP_ERROR */
    }


    /** ########## Step 3 ##########
      * MX_I2Cx receives in polling mode a buffer containing a null-terminated string.
      */
    if (I2C_Receive(I2C_RESPONDER_ADDR_7BIT_SHIFTED, RxBuffer, BUFFER_SIZE, I2C_XFER_TIMEOUT_MS) != SYSTEM_OK)
    {
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
      /* Handle I2C data transmission error, then restart the communication */
      return_status  = HandleTransferError();
      continue;
#endif /* USE_LL_APP_ERROR */
    }


    /** ########## Step 4 ##########
      * Checks the correctness of the received buffer after the data transfer is completed.
      */
    return_status = HandleTransferCplt();
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


/** brief:  Updates the Tx buffer pointer and clears the Rx buffer before starting a new transfer.
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
  * retval: example status
  */
static app_status_t HandleTransferCplt(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (memcmp(pTxData, RxBuffer, BUFFER_SIZE) == 0)
  {
    return_status = EXEC_STATUS_OK;
  }

  return return_status;
} /* end HandleTransferCplt */

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
