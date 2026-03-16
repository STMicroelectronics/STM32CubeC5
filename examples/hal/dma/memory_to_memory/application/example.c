/**
  ******************************************************************************
  * file           : example.c
  * brief          : DMA transfer of a word data buffer from Flash memory to RAM.
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

/* @user: The maximum data bus width used by DMA in STM32 devices is 64 bits.
          Therefore, 8-byte alignment is the minimum recommended alignment for DMA buffers across STM32 devices. */
#define DMA_ALIGNMENT        (8U)

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE_IN_WORDS (32U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_dma_handle_t *pDMA;  /* pointer referencing the DMA handle from the generated code */

/* SrcBuffer is declared as const, so that it is placed in Flash. */
/* @user: It is possible to modify buffer content and length, update BUFFER_SIZE_IN_WORDS if necessary */
__attribute__((aligned(DMA_ALIGNMENT)))
const uint32_t SrcBuffer[BUFFER_SIZE_IN_WORDS] =
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

/** Destination buffer for CPU and DMA.
  * - Non-cacheable memory for data cache consistency.
  * - Aligned for DMA constraints.
  * - Mandatory with data cache enabled, harmless otherwise: portable across STM32 series.
  */
__attribute__((section(".bss.non_cacheable_area"), aligned(DMA_ALIGNMENT)))
uint32_t DstBuffer[BUFFER_SIZE_IN_WORDS];

volatile uint8_t TransferError;    /* Set to 1 if a transfer error is detected */
volatile uint8_t TransferComplete; /* Set to 1 if the transfer is correctly completed */

/* Private functions prototype -----------------------------------------------*/
static void TransferCompleteCallback(hal_dma_handle_t *handle_dma);
static void TransferErrorCallback(hal_dma_handle_t *handle_dma);


/** ########## Step 1 ##########
  * The init of the DMA instance is triggered by the applicative code.
  * User's callbacks for DMA error and DMA complete are registered.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pDMA = mx_example_dma_init();
  if (pDMA == NULL)
  {
    goto _app_init_exit;
  }

  /* Select Callbacks functions called after Transfer complete and Transfer error */
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


/** ########## Step 2 ##########
  * Starts the DMA transfer
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  TransferError = 0;
  TransferComplete = 0;

  /* BUFFER_SIZE_IN_WORDS is multiplied by 4, because the HAL parameter is given in bytes number and not words number */
  if (HAL_DMA_StartDirectXfer_IT(pDMA, (uint32_t)SrcBuffer, (uint32_t)DstBuffer, 4 * BUFFER_SIZE_IN_WORDS)  != HAL_OK)
  {
    goto _app_process_exit;
  }

  /** ########## Step 3 ##########
    * Waits for one of these DMA interrupts: transfer complete or transfer error.
    */
  while (!TransferComplete && !TransferError)
  {
    /** Put the CPU in Wait For Interrupt state. A DMA interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the DMA. It can be replaced by your own code.
      */
    __WFI();
  }

  if ((TransferComplete != 1) || (TransferError == 1))
  {
    goto _app_process_exit;
  }

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 4 ##########
  * Deinitializes the DMA instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_dma_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** DMA transfer complete callback
  * @user This implementation of the DMA complete callback can be customized.
  * This function is executed if the transfer complete interrupt is generated.
  */
static void TransferCompleteCallback(hal_dma_handle_t *handle_dma)
{
  /* async processing related to step 3 */
  TransferComplete = 1;
}


/** DMA transfer error callback
  * @user This implementation of the DMA error callback can be customized.
  * This function is executed if the transfer error interrupt is generated during the DMA transfer.
  */
static void TransferErrorCallback(hal_dma_handle_t *handle_dma)
{
  /* async processing related to step 3 */
  TransferError = 1;
}
