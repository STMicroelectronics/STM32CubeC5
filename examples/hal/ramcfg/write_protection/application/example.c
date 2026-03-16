/**
  ******************************************************************************
  * file           : example.c
  * brief          : example program body
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
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* @user: set the timeout period in milliseconds for erasing data */
#define TIMEOUT 1000U
#define BUFFER_SIZE             256U /* In Word */
#define SRAM_PAGE_SIZE          1024U /* In Byte */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_dma_handle_t *pDMA = NULL;  /* Pointer referencing the DMA handle from the generated code */
hal_ramcfg_t RamcfgInstance;

volatile uint32_t TransferCompleteDetected = 0U;
volatile uint32_t TransferErrorDetected = 0U;

/* SrcBuffer is declared as const, so that it is placed in Flash. */
/* @user: It is possible to modify buffer content and length, update BUFFER_SIZE if necessary */
const  uint32_t SrcBuffer[BUFFER_SIZE] =
{
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80,
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80,
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80,
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80,
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80,
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80,
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80,
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80
};

/* Private functions prototype -----------------------------------------------*/
static void TransferCompleteCallback(hal_dma_handle_t *pDMA);
static void TransferErrorCallback(hal_dma_handle_t *pDMA);
static app_status_t CheckSramPageContent(uint32_t PageIdx, uint32_t write_protection_state);
static app_status_t FillSramPageDma(uint32_t PageIdx);

/** ########## Step 1 ##########
  * The applicative code initializes the RAMCFG instance and triggers the DMA instance.
  * User's callbacks for DMA error and DMA complete are registered.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Getting Ramcfg Instance */
  RamcfgInstance = mx_example_ramcfg_init();

  pDMA = mx_example_dma_init();
  if (pDMA != NULL)
  {
    /* Select Callbacks functions called after Transfer Complete and Transfer Error */
    if (HAL_DMA_RegisterXferCpltCallback(pDMA, TransferCompleteCallback) != HAL_OK)
    {
      goto _app_init_exit;
    }
    if (HAL_DMA_RegisterXferErrorCallback(pDMA, TransferErrorCallback) != HAL_OK)
    {
      goto _app_init_exit;
    }
  }
  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * Erases the given SRAM memory.
    */
  if (HAL_RAMCFG_MassErase(RamcfgInstance, TIMEOUT) != HAL_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2: Memory erasing COMPLETED.\n");

  /** ########## Step 3 ##########
    * Enables write protection for the first half of SRAM memory.
    */
  if (HAL_RAMCFG_EnablePageWRP(RamcfgInstance, 0U, SRAM_PAGE_NUM_PROTECTED) != HAL_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 3: Write protection enabled for the first half of the SRAM pages.\n");

  /** ########## Step 4 ##########
    * Fills all pages of given SRAM.
    */
  for (uint32_t pageIdx = 0U; pageIdx < SRAM_PAGE_NUM; pageIdx++)
  {
    /* Reset DMA flags before each transfer */
    TransferErrorDetected = 0;
    TransferCompleteDetected = 0;
    if (FillSramPageDma(pageIdx) != EXEC_STATUS_OK)
    {
      goto _app_process_exit;
    }

    while (!TransferCompleteDetected && !TransferErrorDetected)
    {
      /** Put the CPU in Wait For Interrupt state. A DMA interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the DMA. It can be replaced by your own code.
        */
      __WFI();
    }

  }

  /* Check DMA error */
  if ((TransferCompleteDetected != 1) || (TransferErrorDetected == 1))
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 4: Memory filled with data.\n");

  /** ########## Step 5 ##########
    * Checks protected and unprotected SRAM pages content.
    */
  for (uint32_t pageIdx = 0U; pageIdx < SRAM_PAGE_NUM_PROTECTED; pageIdx++)
  {
    if (CheckSramPageContent(pageIdx, 1) != EXEC_STATUS_OK)
    {
      goto _app_process_exit;
    }
  }
  for (uint32_t pageIdx = SRAM_PAGE_NUM_PROTECTED; pageIdx < SRAM_PAGE_NUM; pageIdx++)
  {
    if (CheckSramPageContent(pageIdx, 0) != EXEC_STATUS_OK)
    {
      goto _app_process_exit;
    }
  }
  PRINTF("[INFO] Step 5: Protected and unprotected SRAM checked successfully.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 6 ##########
  * Deinitializes the DMA instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_dma_deinit();
  PRINTF("[INFO] Step 6: de-init.\n");
  return EXEC_STATUS_OK;
} /* end app_deinit */


/** DMA complete transfer detection callback
  * @user This implementation of the DMA complete transfer detection callback can be customized.
  */
static void TransferCompleteCallback(hal_dma_handle_t *pDMA)
{
  TransferCompleteDetected++;
}

/** DMA transfer error callback
  * @user This implementation of the DMA transfer error callback can be customized.
  */
static void TransferErrorCallback(hal_dma_handle_t *pDMA)
{
  TransferErrorDetected++;
}

static app_status_t FillSramPageDma(uint32_t PageIdx)
{
  /* Calculate the write address */
  uint32_t write_address = SRAM_BASE + (PageIdx * SRAM_PAGE_SIZE);

  /* Configure the source, destination and buffer size DMA fields and Start DMA Channel/Stream transfer */
  mx_example_dma_init();
  HAL_DMA_RegisterXferCpltCallback(pDMA, TransferCompleteCallback);
  HAL_DMA_RegisterXferErrorCallback(pDMA, TransferErrorCallback);

  /**  Starts the DMA transfer
    * It is mandatory to write the SRAM page by page
    */
  if (HAL_DMA_StartDirectXfer_IT(pDMA, (uint32_t)SrcBuffer, (uint32_t)write_address, (BUFFER_SIZE * 4U)) != HAL_OK)
  {
    return EXEC_STATUS_ERROR;
  }
  return EXEC_STATUS_OK;
}

static app_status_t CheckSramPageContent(uint32_t PageIdx, uint32_t write_protection_state)
{
  uint32_t base_address = SRAM_BASE + (PageIdx * SRAM_PAGE_SIZE);
  /* Total number of elements to check*/
  uint32_t total_elements = BUFFER_SIZE;
  for (uint32_t idx = 0U; idx < total_elements; idx++)
  {
    /* Calculate the address of the current element in SRAM*/
    uint32_t element_address = base_address + (idx * 4U);
    if (write_protection_state == 0)
    {
      /* Check if the content matches the source buffer */
      if (SrcBuffer[idx] != (*(uint32_t *)element_address))
      {
        return EXEC_STATUS_ERROR;
      }
    }
    else
    {
      if ((*(uint32_t *)element_address) != 0U)
      {
        return EXEC_STATUS_ERROR;
      }
    }
  }

  return EXEC_STATUS_OK;
}
