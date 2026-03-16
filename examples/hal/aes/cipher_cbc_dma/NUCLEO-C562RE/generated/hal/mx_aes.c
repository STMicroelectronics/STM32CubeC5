/**
  ******************************************************************************
  * @file           : mx_aes.c
  * @brief          : AES Peripheral initialization
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
#include "mx_aes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_aes_handle_t hAES;
static hal_dma_handle_t hLPDMA1_CH2;
static hal_dma_handle_t hLPDMA1_CH0;

const uint32_t AESKey[4] =
  {
    0x2B7E1516, 0x28AED2A6, 0xABF71588, 0x09CF4F3C
  };

uint32_t AESIV[4] =
  {
    0x00010203, 0x04050607, 0x08090A0B, 0x0C0D0E0F
  };

/******************************************************************************/
/* Exported functions for AES in HAL layer (SW instance MyAES_1) */
/******************************************************************************/
hal_aes_handle_t *mx_aes_init(void)
{
  if (HAL_AES_Init(&hAES, HAL_AES) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_AES_EnableClock();

  if (HAL_AES_CBC_SetConfig(&hAES, AESIV) != HAL_OK)
  {
    return NULL;
  }

  HAL_AES_SetNormalKey(&hAES, HAL_AES_KEY_SIZE_128BIT, AESKey);
  /* Configure the DMA OUT */
      if (HAL_DMA_Init(&hLPDMA1_CH0, HAL_LPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_LPDMA1_EnableClock();

hal_dma_direct_xfer_config_t xfer_cfg_aes_out_dma;
  xfer_cfg_aes_out_dma.request         = HAL_LPDMA1_REQUEST_AES_OUT;
  xfer_cfg_aes_out_dma.direction       = HAL_DMA_DIRECTION_PERIPH_TO_MEMORY;
  xfer_cfg_aes_out_dma.src_inc         = HAL_DMA_SRC_ADDR_FIXED;
  xfer_cfg_aes_out_dma.dest_inc        = HAL_DMA_DEST_ADDR_INCREMENTED;
  xfer_cfg_aes_out_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_WORD;
  xfer_cfg_aes_out_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_WORD;
  xfer_cfg_aes_out_dma.priority        = HAL_DMA_PRIORITY_HIGH;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hLPDMA1_CH0, &xfer_cfg_aes_out_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for LPDMA1_CH0 */
  HAL_CORTEX_NVIC_SetPriority(LPDMA1_CH0_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(LPDMA1_CH0_IRQn);

  /* Link the Output DMA handle to the AES handle */
  if (HAL_AES_SetOutDMA(&hAES, &hLPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  /* Configure the DMA IN */
      if (HAL_DMA_Init(&hLPDMA1_CH2, HAL_LPDMA1_CH2) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_LPDMA1_EnableClock();

hal_dma_direct_xfer_config_t xfer_cfg_aes_in_dma;
  xfer_cfg_aes_in_dma.request         = HAL_LPDMA1_REQUEST_AES_IN;
  xfer_cfg_aes_in_dma.direction       = HAL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  xfer_cfg_aes_in_dma.src_inc         = HAL_DMA_SRC_ADDR_INCREMENTED;
  xfer_cfg_aes_in_dma.dest_inc        = HAL_DMA_DEST_ADDR_FIXED;
  xfer_cfg_aes_in_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_WORD;
  xfer_cfg_aes_in_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_WORD;
  xfer_cfg_aes_in_dma.priority        = HAL_DMA_PRIORITY_HIGH;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hLPDMA1_CH2, &xfer_cfg_aes_in_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for LPDMA1_CH2 */
  HAL_CORTEX_NVIC_SetPriority(LPDMA1_CH2_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(LPDMA1_CH2_IRQn);

  /* Link the Input DMA handle to the AES handle */
  if (HAL_AES_SetInDMA(&hAES, &hLPDMA1_CH2) != HAL_OK)
  {
    return NULL;
  }

  return &hAES;
}

void mx_aes_deinit(void)
{
  (void)HAL_AES_DeInit(&hAES);

  HAL_RCC_AES_Reset();

  HAL_RCC_AES_DisableClock();

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hLPDMA1_CH2);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(LPDMA1_CH2_IRQn);

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hLPDMA1_CH0);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(LPDMA1_CH0_IRQn);
}

hal_aes_handle_t *mx_aes_gethandle(void)
{
  return &hAES;
}

/******************************************************************************/
/*                      LPDMA1 channel0 global interrupt                      */
/******************************************************************************/
void LPDMA1_CH0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hLPDMA1_CH0);
}

/******************************************************************************/
/*                      LPDMA1 channel2 global interrupt                      */
/******************************************************************************/
void LPDMA1_CH2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hLPDMA1_CH2);
}
