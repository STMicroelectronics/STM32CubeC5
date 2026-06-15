/**
  ******************************************************************************
  * file           : example.c
  * brief          : w25q128j flash memory read/write example
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

#include <string.h>     /* required for memcmp()       */

#include "example.h"

/* Private macro -------------------------------------------------------------*/

/** @user: configure the buffer size, the number of bytes be written to memory
  */
#define BUFFER_SIZE                  4096U

/** @user: configure the start address to write/read from.
  * The W25Q128J uses zero-based addressing.
  */
#define W25Q128J_START_ADDRESS       0U

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* @user: The maximum data bus width used by DMA in STM32 devices is 64 bits.
          Therefore, 8-byte alignment is the minimum recommended alignment for DMA buffers across STM32 devices. */
#define DMA_ACCESS_ALIGNMENT        (8U)

/* Private variables ---------------------------------------------------------*/
__attribute__((section("non_cacheable_tx_buffer"), aligned(DMA_ACCESS_ALIGNMENT)))
uint8_t TxBuffer[BUFFER_SIZE] = {0U}; /* buffer used for write operations */
__attribute__((section("non_cacheable_rx_buffer"), aligned(DMA_ACCESS_ALIGNMENT)))
uint8_t RxBuffer[BUFFER_SIZE] = {0U}; /* buffer used for read operations */
__attribute__((section("non_cacheable_part_obj"), aligned(DMA_ACCESS_ALIGNMENT)))
w25q128j_obj_t      *pFlashObj;
volatile uint32_t callback_flag;

/* Private functions prototype -----------------------------------------------*/

static void BufferFill(uint8_t *pbuffer, uint32_t buff_length);
static void callback(w25q128j_obj_t *pobj, void *arg)
{
  volatile uint32_t *flag = arg;
  ++*flag;
}

/**
  * ########## Step 1 ##########
  * The initialization of the W25Q128J is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  uint8_t id[2] = {0U};

  /* Retrieve and store the w25q128j object pointer */
  pFlashObj = mx_external_mem_getobject();

  /* Part initialization */
  if (w25q128j_init(pFlashObj, MX_EXTERNAL_MEM) != W25Q128J_OK)
  {
    goto _app_init_exit;
  }
  /* Read the device ID */
  if (w25q128j_read_id(pFlashObj, id) != W25Q128J_OK)
  {
    goto _app_init_exit;
  }
  /* Verify the device ID */
  if ((id[0] != 0xEF) || (id[1] != 0x17))
  {
    goto _app_init_exit;
  }
  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  w25q128j_status_t error_status;
  w25q128j_async_write_phase_t async_write_phase;

  /* Prepare the data to be written to the flash memory */
  BufferFill(TxBuffer, BUFFER_SIZE);

  /**
    * ########## Step 2 ##########
    * erase the sector in which we want to write.
    */

  /* sector erase */
  if (w25q128j_erase_sectors(pFlashObj, W25Q128J_START_ADDRESS, BUFFER_SIZE) != W25Q128J_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2: Sector erased .\n");
  /**
    * ########## Step 3 ##########
    * Writes a data buffer to the Flash Memory asynchronously.
    */
  if (w25q128j_register_write_cplt_callback(pFlashObj, callback, (void *) &callback_flag) != W25Q128J_OK)
  {
    goto _app_process_exit;
  }
  /* Start the asynchronous write to the flash memory */
  if (w25q128j_write_dma_async(pFlashObj, TxBuffer, W25Q128J_START_ADDRESS, BUFFER_SIZE) != W25Q128J_OK)
  {
    goto _app_process_exit;
  }
  do
  {
    /* Handle wait state management - Poll the busy bit when necessary */
    error_status = w25q128j_exec_data_handler(pFlashObj);
    /* Get the current phase of the asynchronous write operation */
    async_write_phase = w25q128j_get_async_write_phase(pFlashObj);
    /* wait until the asynchronous write operation is complete or an error occurs */
  } while ((async_write_phase != W25Q128J_ASYNC_WRITE_IDLE) && (error_status != W25Q128J_ERROR));
  if (error_status != W25Q128J_OK)
  {
    /* Timeout error */
    goto _app_process_exit;
  }
  callback_flag = 0; /* reset flag */

  PRINTF("[INFO] Step 3: Data buffer written to the flash memory asynchronously.\n");

  /**
    * ########## Step 4 ##########
    * Reads the same data buffer from the external memory asynchronously,
    * and checks that it matches the buffer written in Step 3.
    */
  if (w25q128j_register_read_cplt_callback(pFlashObj, callback, (void *) &callback_flag) != W25Q128J_OK)
  {
    goto _app_process_exit;
  }
  /* Read from the flash memory */
  if (w25q128j_read_dma(pFlashObj, RxBuffer, W25Q128J_START_ADDRESS, BUFFER_SIZE) != W25Q128J_OK)
  {
    goto _app_process_exit;
  }

  while (callback_flag == 0);

  /* Compare the two data buffers */
  if (memcmp(TxBuffer, RxBuffer, BUFFER_SIZE) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 4: Data buffer read from the flash memory asynchronously and matching the initial value.\n");
  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/**
  * ########## Step 5 ##########
  * Deinitializes the W25Q128J before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  if (w25q128j_deinit(pFlashObj) != W25Q128J_OK)
  {
    return EXEC_STATUS_ERROR;
  }
  PRINTF("[INFO] Step 5: Device de-initialization.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */


/**
  * brief:  fills a buffer with arbitrary values
  * param:  pbuffer: pointer to a buffer to fill
  * param:  buff_length: buffer length
  * retval: None
  */
static void BufferFill(uint8_t *pbuffer, uint32_t buff_length)
{
  /* Fills the buffer with an arbitrary content.
   * Here we just loop over sequential integers (0..255).
   */
  for (uint32_t buff_index = 0; buff_index < buff_length; buff_index++)
  {
    pbuffer[buff_index] = (uint8_t)(buff_index & 0xFF);
  }
}
