/**
  ******************************************************************************
  * file           : example.c
  * brief          : DMA transfer trigger another DMA transfer in a different memory.
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
#define DMA_TIMEOUT               1000U
#define DMA_ALIGNMENT             32U    /* Adjust the data alignment */
#define BUFFER_SIZE_IN_WORDS      32U
#define TRIGGERCHANNEL_BLOCK_SIZE 4U
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_dma_handle_t *pDMA_Triggering; /* pointer to handle of Triggering Channel */
hal_dma_handle_t *pDMA_Triggered;  /* pointer to handle of Triggered Channel */

/* SrcTriggeringBuffer is declared as const, so that it is placed in Flash. */
/* It is possible to modify buffer content and length */
const uint32_t SrcTriggeringBuffer[BUFFER_SIZE_IN_WORDS] =
{
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80
};

/* SrcTriggeredBuffer is declared as const, so that it is placed in Flash. */
/* It is possible to modify buffer content and length */
const uint32_t SrcTriggeredBuffer[BUFFER_SIZE_IN_WORDS] =
{
  0xA1A2A3A4, 0xA5A6A7A8, 0xA9AAABAC, 0xADAEAFB0,
  0xB1B2B3B4, 0xB5B6B7B8, 0xB9BABBBC, 0xBDBEBFC0,
  0xC1C2C3C4, 0xC5C6C7C8, 0xC9CACBCC, 0xCDCECFD0,
  0xD1D2D3D4, 0xD5D6D7D8, 0xD9DADBDC, 0xDDDEDFE0,
  0xE1E2E3E4, 0xE5E6E7E8, 0xE9EAEBEC, 0xEDEEEFF0,
  0xF1F2F3F4, 0xF5F6F7F8, 0xF9FAFBFC, 0xFDFEFF00,
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20
};

/** Destination buffer */
__attribute__((section(".bss.non_cacheable_variables"), aligned(DMA_ALIGNMENT)))
uint32_t DstTriggeringBuffer[BUFFER_SIZE_IN_WORDS];
__attribute__((section(".bss.non_cacheable_variables"), aligned(DMA_ALIGNMENT)))
uint32_t DstTriggeredBuffer[BUFFER_SIZE_IN_WORDS];

volatile uint32_t BlkDataLength = 0U;
volatile uint32_t NumberOfBlocksToTransfer = 0U;
volatile uint8_t TriggeringTransferComplete;
volatile uint8_t TriggeringTransferError;
volatile uint8_t TriggeredTransferComplete;
volatile uint8_t TriggeredTransferError;

/* Private functions prototype -----------------------------------------------*/
static void TriggeringTransferCompleteCallback(hal_dma_handle_t *handle_dma);
static void TriggeringTransferErrorCallback(hal_dma_handle_t *handle_dma);
static void TriggeredTransferCompleteCallback(hal_dma_handle_t *handle_dma);
static void TriggeredTransferErrorCallback(hal_dma_handle_t *handle_dma);

/** ########## Step 1 ##########
  * The init of the DMA instance is triggered by the applicative code.
  * User's callbacks for DMA error and DMA complete are registered.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pDMA_Triggering = mx_dma_triggering_channel_init();
  pDMA_Triggered = mx_dma_triggered_channel_init();

  if ((pDMA_Triggering == NULL) || (pDMA_Triggered == NULL))
  {
    goto _app_init_exit;
  }

  /* Select Callbacks functions called after triggering and triggered DMA
   * Transfer complete and error
   * Callbacks are registered only for the triggering DMA channel.
   * The triggered DMA completion is monitored via polling.
   * This simplifies the example and focuses on the triggering mechanism.
   */
  if (HAL_DMA_RegisterXferCpltCallback(pDMA_Triggering,
                                       TriggeringTransferCompleteCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_DMA_RegisterXferErrorCallback(pDMA_Triggering,
                                        TriggeringTransferErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_DMA_RegisterXferCpltCallback(pDMA_Triggered,
                                       TriggeredTransferCompleteCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_DMA_RegisterXferErrorCallback(pDMA_Triggered,
                                        TriggeredTransferErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Start the DMA transfer on the triggered channel.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  uint32_t nbBlocks;
  uint32_t blkLength;

  /* Total bytes to transfer for triggering channel */
  NumberOfBlocksToTransfer = BUFFER_SIZE_IN_WORDS * 4;
  nbBlocks  = NumberOfBlocksToTransfer;

  /* Clear Triggered DMA flags */
  TriggeredTransferComplete = 0U;
  TriggeredTransferError    = 0U;

  /* BUFFER_SIZE_IN_WORDS is multiplied by 4,
    because the HAL parameter is given in bytes number and not words number */
  if (HAL_DMA_StartDirectXfer_IT(pDMA_Triggered,
                                 (uint32_t)&SrcTriggeredBuffer,
                                 (uint32_t)&DstTriggeredBuffer,
                                 BUFFER_SIZE_IN_WORDS * 4)  != HAL_OK)
  {
    goto _app_process_exit;
  }

  /** Check that the transfer has not started yet.
    * This ensures the triggered DMA is waiting for trigger events,
    * from the triggering DMA.
    */
  BlkDataLength = HAL_DMA_GetDirectXferRemainingDataByte(pDMA_Triggered);
  blkLength = BlkDataLength;

  if (nbBlocks != blkLength)
  {
    goto _app_process_exit;
  }

  /** ########## Step 3 ##########
    * Trigger the triggering channel to start multiple small transfers,
    * each transferring a block of data and triggering the triggered channel.
    * Wait for each transfer to complete or detect an error.
    */

  /** Each completion of the triggering DMA transfer causes one word to be transferred
    * on the triggered DMA channel, because of the single burst trigger configuration.
    * The application loops over the triggering DMA transfers to ensure the entire buffer
    * is processed by the triggered DMA.
    */
  for (uint32_t dataIdx = 0; dataIdx < BUFFER_SIZE_IN_WORDS; dataIdx++)
  {
    /* Clear Triggering DMA flags */
    TriggeringTransferComplete = 0U;
    TriggeringTransferError    = 0U;

    /* Configure source, destination and buffer size and Start DMA transfer */
    /* Enable All the DMA interrupts */
    if (HAL_DMA_StartDirectXfer_IT(pDMA_Triggering,
                                   (uint32_t)&SrcTriggeringBuffer[dataIdx],
                                   (uint32_t)&DstTriggeringBuffer[dataIdx],
                                   TRIGGERCHANNEL_BLOCK_SIZE) != HAL_OK)
    {
      goto _app_process_exit;
    }

    /* Update remaining bytes count for triggering DMA */
    NumberOfBlocksToTransfer -= TRIGGERCHANNEL_BLOCK_SIZE;
    nbBlocks  = NumberOfBlocksToTransfer;

    /** Verify that the triggered DMA transfer remains waiting for triggers.
      * The remaining data length on the triggered DMA matches the updated count
      */
    BlkDataLength = HAL_DMA_GetDirectXferRemainingDataByte(pDMA_Triggered);
    blkLength = BlkDataLength;
    if (nbBlocks != blkLength)
    {
      goto _app_process_exit;
    }

    /* Wait for the triggering DMA transfer to complete or an error to occur */
    while ((!TriggeringTransferComplete) && (TriggeringTransferError != 1));

    /* Check DMA error */
    if (TriggeringTransferError == 1)
    {
      goto _app_process_exit;
    }
  }


  /** ########## Step 4 ##########
    * After all small triggering DMA transfers complete,
    * wait for the triggered DMA channel to finish the entire buffer transfer.
    * This step ensures the triggered DMA has completed processing,
    * all triggered transfers,
    * Checks DMA transfers complete.
    */
  while ((!TriggeredTransferComplete) && (TriggeredTransferError != 1));

  /* Check of transfer DMA error */
  if (TriggeredTransferError == 1)
  {
    goto _app_process_exit;
  }

  /* Check if the triggering DMA buffer was correctly transferred */
  if (memcmp(SrcTriggeringBuffer, DstTriggeringBuffer, BUFFER_SIZE_IN_WORDS * 4U) != 0U)
  {
    goto _app_process_exit;
  }

  /* Check if the triggered DMA buffer was correctly transferred */
  if (memcmp(SrcTriggeredBuffer, DstTriggeredBuffer, BUFFER_SIZE_IN_WORDS * 4U) != 0U)
  {
    goto _app_process_exit;
  }
  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 5 ##########
  * Deinitialize the DMA instances before exiting the application scenario.
  */
app_status_t app_deinit(void)
{
  mx_dma_triggering_channel_deinit();
  mx_dma_triggered_channel_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** DMA transfer complete callback for triggering DMA channels */
static void TriggeringTransferCompleteCallback(hal_dma_handle_t *handle_dma)
{
  TriggeringTransferComplete = 1U; /* Complete flag for triggering channel */
}

/** DMA transfer error callback for triggering DMA channels*/
static void TriggeringTransferErrorCallback(hal_dma_handle_t *handle_dma)
{
  TriggeringTransferError = 1U; /* Error flag for triggering channel */
}

/** DMA transfer complete callback for triggered DMA channels */
static void TriggeredTransferCompleteCallback(hal_dma_handle_t *handle_dma)
{
  TriggeredTransferComplete = 1U; /* Complete flag for triggered channel */
}

/** DMA transfer error callback for triggered DMA channels */
static void TriggeredTransferErrorCallback(hal_dma_handle_t *handle_dma)
{
  TriggeredTransferError = 1U; /* Error flag for triggered channel */
}
