/**
  ******************************************************************************
  * file           : example.c
  * brief          : w25n01gvxx SPI NAND Flash memory read/write example
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
#include <string.h>             /* required for memset() and memcmp() */
#include "example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @user: configure the buffer size, the number of bytes be written to memory
  * The size must be taken into consideration because the memory does not support wrapping.
  */
#define BUFFER_SIZE        4096U

/** @user: configure the start block address and start page address to be written to
  * The block address must be less than W25N01GVXX_NUM_OF_BLOCK.
  * The page address must be less than W25N01GVXX_NUM_PAGE_PER_BLOCK.
  */
#define W25N01GVXX_BLOCK_START_ADDRESS     5U
#define W25N01GVXX_PAGE_START_ADDRESS      2U


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
w25n01gvxx_obj_t *pFlashObj;

uint8_t TxBuffer[BUFFER_SIZE] = {0U}; /* buffer used for write operations */
uint8_t RxBuffer[BUFFER_SIZE] = {0U}; /* buffer used for read operations */

/* Private functions prototype -----------------------------------------------*/
static void BufferFill(uint8_t *pbuffer, uint32_t buff_length);


/** ########## Step 1 ##########
  * The init of the W25N01GVXX part is triggered by the applicative code.
  */
app_status_t app_init(void)
{

  app_status_t return_status = EXEC_STATUS_ERROR;
  uint8_t id[4] = {0U};

  /* Retrieve and store the w25n01gvxx object pointer */
  pFlashObj = mx_w25n01gvxx_1_getobject();

  /* Prepare the data to be written to the Flash memory */
  BufferFill(TxBuffer, BUFFER_SIZE);

  /* Part initialization */
  if (w25n01gvxx_init(pFlashObj, MX_W25N01GVXX_1) != W25N01GVXX_OK)
  {
    goto _app_init_exit;
  }
  /* Read the device ID */
  if (w25n01gvxx_read_id(pFlashObj, id) != W25N01GVXX_OK)
  {
    goto _app_init_exit;
  }
  /* Verify the device ID */
  if ((id[1] != 0xEF) || (id[2] != 0xAA) || (id[3] != 0x21))
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

  /**
    * ########## Step 2 ##########
    * Erases the block of the memory in which we want to write.
    */
  /* With the default page/block addresses and size, only 1 block need to be erased */
  if (w25n01gvxx_erase_blocks(pFlashObj, W25N01GVXX_BLOCK_START_ADDRESS, 1U) != W25N01GVXX_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2: Block(s) erased.\n");

  /**
    * ########## Step 3 ##########
    * Writes a data buffer to the flash memory.
    */
  if (w25n01gvxx_write(pFlashObj, W25N01GVXX_BLOCK_START_ADDRESS,
                       W25N01GVXX_PAGE_START_ADDRESS, TxBuffer, BUFFER_SIZE) != W25N01GVXX_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3: Data buffer written to the flash memory.\n");
  /**
    * ########## Step 4 ##########
    * Reads the same data buffer from the flash memory, through one data transfer, in continuous mode,
    * and checks that it matches the buffer written in step 3.
    */
  /* Set the read mode to continuous */
  if (w25n01gvxx_set_read_mode(pFlashObj, W25N01GVXX_CONTINUOUS_MODE) != W25N01GVXX_OK)
  {
    goto _app_process_exit;
  }
  /* Read from the external memory */
  if (w25n01gvxx_read(pFlashObj, W25N01GVXX_BLOCK_START_ADDRESS,
                      W25N01GVXX_PAGE_START_ADDRESS, RxBuffer, BUFFER_SIZE) != W25N01GVXX_OK)
  {
    goto _app_process_exit;
  }
  /* Compare the two data buffers */
  if (memcmp(TxBuffer, RxBuffer, BUFFER_SIZE) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 4: Data read from the flash memory in continuous mode and matching the written value.\n");
  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 5 ##########
  * Deinitializes the W25N01GVXX before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  if (w25n01gvxx_deinit(pFlashObj) != W25N01GVXX_OK)
  {
    return EXEC_STATUS_ERROR;
  }
  mx_example_memory_bus_deinit();
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
