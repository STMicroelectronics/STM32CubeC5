/**
  ******************************************************************************
  * file           : example.c
  * brief          : The Controller write/read data buffers from EEPROM within
  *                  I2C peripheral in a non-blocking mode using DMA and HAL API.
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
#include <string.h>       /* required for memset() and memcmp() */
#include "example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** M24128-D EEPROM Sector Organization
  * - Memory size : 128 Kbit (16 kbytes).
  * - Byte per page: 64 Bytes.
  * - Total number of pages : 256 pages.
  */
#define M24128D_ADDRESS       0xA2 /* b3 = 0 ; b2 = 0 ; b1 = 1 */
#define M24128D_START_ADDRESS 0U
#define BUFFER_SIZE           128U
#define PAGE_SIZE             64U
#define TIMEOUT_MS            1000U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_i2c_handle_t *pI2C;
volatile uint8_t TxTransferComplete; /* Set to 1 if the write transfer is correctly completed */
volatile uint8_t RxTransferComplete; /* Set to 1 if the read transfer is correctly completed */
volatile uint8_t TransferError;      /* Set to 1 if a transmission or a reception error is detected */

uint8_t TxBuffer[BUFFER_SIZE] __ALIGNED(8); /* Transmit buffer used for EEPROM write operations */
uint8_t RxBuffer[BUFFER_SIZE] __ALIGNED(8); /* Receive buffer used for EEPROM read operations */

/* Private functions prototype -----------------------------------------------*/
static void BufferFill(uint8_t *pbuffer, uint32_t buff_length);
static void MasterMemTxTransferCpltCallback(hal_i2c_handle_t *hi2c);
static void MasterMemRxTransferCpltCallback(hal_i2c_handle_t *hi2c);
static void TransferErrorCallback(hal_i2c_handle_t *hi2c);
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t i2c_error_code);

/** ########## Step 1 ##########
  * The init of the I2C and DMA instances are triggered by the applicative code.
  * The user callbacks for I2C master TX/RX transfer completed and transfer error are registered.
  */
app_status_t app_init(void)
{

  app_status_t return_status = EXEC_STATUS_ERROR;

  pI2C = mx_example_i2c_init();

  if (pI2C == NULL)
  {
    goto _app_init_exit;
  }

  /* Register the I2C master callbacks */
  if (HAL_I2C_MASTER_RegisterMemTxCpltCallback(pI2C, MasterMemTxTransferCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_I2C_MASTER_RegisterMemRxCpltCallback(pI2C, MasterMemRxTransferCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_I2C_RegisterErrorCallback(pI2C, TransferErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** Writes an amount of data to the M24128-D EEPROM to be written then read and verified
  */
app_status_t app_process(void)
{
  uint32_t i2c_error_code;
  uint32_t Remaining_Bytes = BUFFER_SIZE;
  uint32_t page_index = 0U;
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status = HAL_ERROR;

  /* Prepare the data to be written to the Flash memory */
  BufferFill(TxBuffer, BUFFER_SIZE);

  while (Remaining_Bytes > 0U)
  {
    /**
      * ########## Step 2 ##########
      * Waits the M24128-D EEPROM  to become ready for communication.
      */

    /* Wait until the EEPROM (M24128-D) acknowledges its I2C address,
       indicating that the internal write cycle is complete. */
    while (HAL_I2C_MASTER_PollForSlaveReady(pI2C, M24128D_ADDRESS, TIMEOUT_MS) != HAL_OK)
    {
      /** Enter low-power mode (WFI) between polling attempts,
        * next systick interrupt will wake-up the CPU for the next try.
        */
      __WFI();
    }


    /**
      * ########## Step 3 ##########
      * Writes a data buffer to the EEPROM memory, in DMA mode.
      * The buffer is written in chunks of PAGE_SIZE bytes until all data
      * has been stored in the EEPROM. After each page write:
      * - Remaining_data is decreased by PAGE_SIZE.
      * - page_index is incremented to address the next EEPROM page.
      */
    hal_status = HAL_I2C_MASTER_MemWrite_DMA(pI2C,
                                             M24128D_ADDRESS,
                                             (M24128D_START_ADDRESS + page_index * PAGE_SIZE),
                                             HAL_I2C_MEM_ADDR_16BIT,
                                             &TxBuffer[page_index * PAGE_SIZE],
                                             PAGE_SIZE);
    if (hal_status != HAL_OK)
    {
      /* An error occurred at the startup of the I2C write transfer. Report it, then exit */
      i2c_error_code = HAL_I2C_GetLastErrorCodes(pI2C);
      return_status  = HandleTransferError(hal_status, i2c_error_code);

      goto _app_process_exit;
    }

    /**
      * Waits for one of these I2C interrupts: write transfer complete or transfer error.
      */
    while ((TxTransferComplete == 0U) && (TransferError == 0U))
    {
      /** Put the CPU in Wait For Interrupt state. An I2C interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the DMA. It can be replaced by your own code.
        */
      __WFI();
    }

    if (TransferError == 1U)
    {
      /* An error occurred at the startup of the I2C write transfer. Report it, then exit */
      i2c_error_code = HAL_I2C_GetLastErrorCodes(pI2C);
      return_status  = HandleTransferError(hal_status, i2c_error_code);

      goto _app_process_exit;
    }

    TxTransferComplete = 0U;
    TransferError = 0U;

    /* Update the remaining data */
    Remaining_Bytes -= PAGE_SIZE;

    /* incrememnt the page index */
    page_index++;
  } /* end of EEPROM pages write loop */

  PRINTF("[INFO] Step 3: Write operation to the EEPROM is COMPLETED.\n");

  /* Reset default values of Remaining_Bytes and page_index variables */
  Remaining_Bytes = BUFFER_SIZE;
  page_index = 0U;


  /**
    * ########## Step 4 ##########
    * Reads the same data buffer from the EEPROM, and checks that it matches the buffer written in Step 3.
    */

  /* Wait until the EEPROM (M24128-D) acknowledges its I2C address,
     indicating that the internal write cycle is complete. */
  while (HAL_I2C_MASTER_PollForSlaveReady(pI2C, M24128D_ADDRESS, TIMEOUT_MS) != HAL_OK)
  {
    /** Enter low-power mode (WFI) between polling attempts,
      * next systick interrupt will wake-up the CPU for the next try.
      */
    __WFI();
  }

  /* Reads the entire buffer from the EEPROM in a single operation */
  hal_status = HAL_I2C_MASTER_MemRead_DMA(pI2C,
                                          M24128D_ADDRESS,
                                          M24128D_START_ADDRESS,
                                          HAL_I2C_MEM_ADDR_16BIT,
                                          RxBuffer,
                                          BUFFER_SIZE);
  if (hal_status != HAL_OK)
  {
    /* An error occurs at the startup of the I2C read transfer. Report it, then exit */
    i2c_error_code = HAL_I2C_GetLastErrorCodes(pI2C);
    return_status  = HandleTransferError(hal_status, i2c_error_code);

    goto _app_process_exit;
  }

  /**
    * Waits for one of these I2C interrupts: read transfer complete or transfer error.
    */
  while ((RxTransferComplete == 0U) && (TransferError == 0U))
  {
    /** Put the CPU in Wait For Interrupt state. An I2C interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the DMA. It can be replaced by your own code.
      */
    __WFI();
  }

  if (TransferError == 1)
  {
    /* An error occurred at the startup of the I2C read transfer. Report it, then exit */
    i2c_error_code = HAL_I2C_GetLastErrorCodes(pI2C);
    return_status  = HandleTransferError(hal_status, i2c_error_code);

    goto _app_process_exit;
  }

  /* Compare the two data buffers */
  if (memcmp(TxBuffer, RxBuffer, BUFFER_SIZE) != 0U)
  {
    PRINTF("[ERROR] Step 4: Data MISMATCH between written and read buffers.\n");
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 4 : Read operation from the EEPROM is COMPLETED.\nREAD/WRITE buffers are IDENTICAL\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/**
  * ########## Step 5 ##########
  * De-initializes the I2C and DMA instances before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_i2c_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** Master transmission end of transfer callback
  * @user This implementation of the I2C master Tx transfer complete callback can be customized.
  * This function is executed if the master Tx transfer complete interrupt is generated.
  */
static void MasterMemTxTransferCpltCallback(hal_i2c_handle_t *hi2c)
{
  /* Asynchronous processing related to step 3 */
  TxTransferComplete = 1U;
} /* end MasterMemTxTransferCpltCallback */


/** Master reception end of transfer callback
  * @user This implementation of the I2C master Rx transfer complete callback can be customized.
  * This function is executed if the master Rx transfer complete interrupt is generated.
  */
static void MasterMemRxTransferCpltCallback(hal_i2c_handle_t *hi2c)
{
  /* Asynchronous processing related to step 5 */
  RxTransferComplete = 1U;
} /* end MasterMemRxTransferCpltCallback */


/** I2C transfer error callback
  * @user This implementation of the I2C master transfer error callback can be customized.
  * This function is executed if the master transfer error interrupt is generated.
  */
static void TransferErrorCallback(hal_i2c_handle_t *hi2c)
{
  /* Asynchronous processing related to step 3 and step 5 */
  TransferError = 1U;
} /* end TransferErrorCallback */


/** brief:  fills a buffer with values
  * param:  pbuffer: pointer to a buffer to fill
  * param:  buff_length: buffer length
  * retval: None
  */
static void BufferFill(uint8_t *pbuffer, uint32_t buff_length)
{
  uint32_t buff_index;

  /* Fill the buffer with different values */
  for (buff_index = 0; buff_index < buff_length; buff_index++)
  {
    pbuffer[buff_index] = buff_index;
  }
} /* end BufferFill */


/** brief:  This function is executed in case of a data transfer error.
  * param hal_status:  HAL status of the I2C TX/RX operations.
  * param i2c_error_code:  I2C Error Code.
  * retval: example status
  */
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t i2c_error_code)
{
  PRINTF("[ERROR] Controller - Communication ERROR: hal_status = %" PRIu32 ", \
  HAL_I2C_GetLastErrorCodes = %" PRIu32 ". TRYING AGAIN.\n", hal_status, i2c_error_code);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
