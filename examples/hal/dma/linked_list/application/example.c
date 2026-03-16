/**
  ******************************************************************************
  * file           : example.c
  * brief          : Linked-list DMA transfer of a word data buffers
  *                  Called by the main after system initialization
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
#include <string.h> /* Used for string/buffer operations like memcmp() */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DMA_ALIGNMENT         32U /* user define to adjust the data alignment to the DMA IP of the SoC */
#define BUFFER1_SIZE_IN_WORDS 8U
#define BUFFER2_SIZE_IN_WORDS 16U
#define BUFFER3_SIZE_IN_WORDS 24U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_dma_handle_t *pDMA;   /* pointer referencing the DMA handle from the generated code */
hal_q_t          *pQueue; /* pointer referencing the Queue used for DMA linked list     */

/* Source buffers are declared as const, so that it is placed in Flash. */
/* @user: It is possible to modify buffers content and length, update BUFFERx_SIZE_IN_WORDS accordingly */
const uint32_t SrcBuffer1[BUFFER1_SIZE_IN_WORDS] =
{
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
};

const uint32_t SrcBuffer2[BUFFER2_SIZE_IN_WORDS] =
{
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
};

const uint32_t SrcBuffer3[BUFFER3_SIZE_IN_WORDS] =
{
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
};

__attribute__((section(".non_cacheable_variables"), aligned(DMA_ALIGNMENT)))
uint32_t DstBuffer1[BUFFER1_SIZE_IN_WORDS];
__attribute__((section(".non_cacheable_variables"), aligned(DMA_ALIGNMENT)))
uint32_t DstBuffer2[BUFFER2_SIZE_IN_WORDS];
__attribute__((section(".non_cacheable_variables"), aligned(DMA_ALIGNMENT)))
uint32_t DstBuffer3[BUFFER3_SIZE_IN_WORDS];

volatile uint8_t TransferError;    /* Set to 1 if a transfer error is detected        */
volatile uint8_t TransferComplete; /* Set to 1 if the transfer is correctly completed */

/* Private functions prototype -----------------------------------------------*/
static void TransferCompleteCallback(hal_dma_handle_t *handle_dma);
static void TransferErrorCallback(hal_dma_handle_t *handle_dma);

/** ---------------------------------------------------------------------
  * Applicative code demonstrating a DMA transfer from Flash to Ram.
  * This is the code you can reuse.
  * ---------------------------------------------------------------------
  */

app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1 ##########
    * Fill the DMA linkied list nodes with specifying the source address, the destination address and the data size in
    * bytes for each node.
    */
  if (mx_dma_node1_init(SrcBuffer1, DstBuffer1, BUFFER1_SIZE_IN_WORDS) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (mx_dma_node2_init(SrcBuffer2, DstBuffer2, BUFFER2_SIZE_IN_WORDS) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (mx_dma_node3_init(SrcBuffer3, DstBuffer3, BUFFER3_SIZE_IN_WORDS) != HAL_OK)
  {
    goto _app_init_exit;
  }


  /** ########## Step 2 ##########
    * Initialize the DMA linked list Queue object and build it according to nodes filled in previously.
    */
  pQueue = mx_queue1_init();
  if (pQueue == NULL)
  {
    goto _app_init_exit;
  }

  pQueue = mx_queue1_build();
  if (pQueue == NULL)
  {
    goto _app_init_exit;
  }


  /** ########## Step 3 ##########
    * Initialize the DMA channel in linked list mode and register transfer error and transfer complete user callbacks.
    */
  pDMA = mx_example_dma_init();
  if (pDMA == NULL)
  {
    goto _app_init_exit;
  }

  if (HAL_DMA_RegisterXferCpltCallback(pDMA, TransferCompleteCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_DMA_RegisterXferErrorCallback(pDMA, TransferErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 4 ##########
    * Starts the DMA linked list Queue execution in interrupt mode.
    */
  TransferError = 0;
  TransferComplete = 0;

  if (HAL_DMA_StartLinkedListXfer_IT(pDMA, pQueue) != HAL_OK)
  {
    goto _app_process_exit;
  }


  /** ########## Step 5 ##########
    * Enters sleep mode and waits for the DMA interrupt: transfer complete or transfer error.
    * Sleep mode is used to illustrate the interest of the DMA: the CPU is not involved further in the data transfer.
    */
  while (!TransferComplete && !TransferError)
  {
    __WFI();
  }

  if ((TransferComplete != 1) || (TransferError == 1))
  {
    goto _app_process_exit;
  }


  /** ########## Step 6 ##########
    * Checks DMA transfers for each DMA linked list node within the executed Queue.
    */
  if (memcmp(SrcBuffer1, DstBuffer1, (BUFFER1_SIZE_IN_WORDS * 4U)) != 0U)
  {
    goto _app_process_exit;
  }

  if (memcmp(SrcBuffer2, DstBuffer2, (BUFFER2_SIZE_IN_WORDS * 4U)) != 0U)
  {
    goto _app_process_exit;
  }

  if (memcmp(SrcBuffer3, DstBuffer3, (BUFFER3_SIZE_IN_WORDS * 4U)) != 0U)
  {
    goto _app_process_exit;
  }

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/**
  * ########## Step 7 ##########
  * Deinitializes the DMA channel instance and deinitilize the DMA linked list Queue.
  */
app_status_t app_deinit(void)
{
  mx_example_dma_deinit();

  mx_queue1_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/**
  * DMA transfer complete callback
  * @user This implementation of the DMA complete callback can be customized.
  * This function is executed when the transfer complete interrupt is generated.
  */
static void TransferCompleteCallback(hal_dma_handle_t *handle_dma)
{
  TransferComplete = 1;

  STM32_UNUSED(handle_dma);
}


/**
  * DMA transfer error callback
  * @user This implementation of the DMA error callback can be customized.
  * This function is executed when the transfer error interrupt is generated during the DMA transfer.
  */
static void TransferErrorCallback(hal_dma_handle_t *handle_dma)
{
  TransferError = 1;

  STM32_UNUSED(handle_dma);
}
