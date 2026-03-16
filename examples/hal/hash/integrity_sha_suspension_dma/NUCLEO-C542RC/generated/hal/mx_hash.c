/**
  ******************************************************************************
  * @file           : mx_hash.c
  * @brief          : HASH Peripheral initialization
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_stm32c5xx_hal_drivers_license.md file
  * in the same directory as the generated code.
  * If no mx_stm32c5xx_hal_drivers_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mx_hash.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_hash_handle_t hHASH;

static hal_dma_handle_t hLPDMA1_CH0;

/******************************************************************************/
/* Exported functions for HASH in HAL layer (SW instance MyHASH_1) */
/******************************************************************************/
hal_hash_handle_t *mx_hash_init(void)
{
  hal_hash_config_t config;

  if (HAL_HASH_Init(&hHASH, HAL_HASH) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_HASH_EnableClock();

  config.data_swapping = HAL_HASH_DATA_SWAP_BYTE;
  config.algorithm     = HAL_HASH_ALGO_SHA256;
  if (HAL_HASH_SetConfig(&hHASH, &config) != HAL_OK)
  {
    return NULL;
  }

  /* DMA Configuration ********************************************************/
    if (HAL_DMA_Init(&hLPDMA1_CH0, HAL_LPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_LPDMA1_EnableClock();

hal_dma_direct_xfer_config_t xfer_cfg_hash_in_dma;
  xfer_cfg_hash_in_dma.request         = HAL_LPDMA1_REQUEST_HASH_IN;
  xfer_cfg_hash_in_dma.direction       = HAL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  xfer_cfg_hash_in_dma.src_inc         = HAL_DMA_SRC_ADDR_INCREMENTED;
  xfer_cfg_hash_in_dma.dest_inc        = HAL_DMA_DEST_ADDR_FIXED;
  xfer_cfg_hash_in_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_WORD;
  xfer_cfg_hash_in_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_WORD;
  xfer_cfg_hash_in_dma.priority        = HAL_DMA_PRIORITY_LOW_WEIGHT_LOW;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hLPDMA1_CH0, &xfer_cfg_hash_in_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for LPDMA1_CH0 */
  HAL_CORTEX_NVIC_SetPriority(LPDMA1_CH0_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(LPDMA1_CH0_IRQn);

  /* Link the DMA handle to the HASH handle */
  if (HAL_HASH_SetInDMA(&hHASH, &hLPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }
  return &hHASH;
}

void mx_hash_deinit(void)
{
  (void)HAL_HASH_DeInit(&hHASH);

  HAL_RCC_HASH_Reset();

  HAL_RCC_HASH_DisableClock();

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hLPDMA1_CH0);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(LPDMA1_CH0_IRQn);
}

hal_hash_handle_t *mx_hash_gethandle(void)
{
  return &hHASH;
}

/******************************************************************************/
/*                      LPDMA1 channel0 global interrupt                      */
/******************************************************************************/
void LPDMA1_CH0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hLPDMA1_CH0);
}
