/**
  ******************************************************************************
  * @file           : mx_lpdma1_ch0.c
  * @brief          : Provides LPDMA1_CH0 services
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

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "mx_lpdma1_ch0.h"

/* Exported variables by reference------------------------------------------------------------------------------------*/

static hal_dma_handle_t hLPDMA1_CH0;

/**********************************************************************************************************************/
/* Exported functions for LPDMA1_CH0 in HAL layer */
/**********************************************************************************************************************/
/**
  * @brief  Initialize and configure the lpdma1_ch0 (configuration 1) through HAL layer.
  * @retval Pointer to hal_dma_handle_t structure.
  */
hal_dma_handle_t *mx_lpdma1_ch0_init(void)
{
if (HAL_DMA_Init(&hLPDMA1_CH0, HAL_LPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_LPDMA1_EnableClock();

  hal_dma_direct_xfer_config_t xfer_cfg;

  xfer_cfg.request         = HAL_DMA_REQUEST_SW;
  xfer_cfg.direction       = HAL_DMA_DIRECTION_MEMORY_TO_MEMORY;
  xfer_cfg.src_inc         = HAL_DMA_SRC_ADDR_INCREMENTED;
  xfer_cfg.dest_inc        = HAL_DMA_DEST_ADDR_INCREMENTED;
  xfer_cfg.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_WORD;
  xfer_cfg.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_WORD;
  xfer_cfg.priority        = HAL_DMA_PRIORITY_LOW_WEIGHT_LOW;

  if (HAL_DMA_SetConfigDirectXfer(&hLPDMA1_CH0, &xfer_cfg) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for LPDMA1_CH0 */
  HAL_CORTEX_NVIC_SetPriority(LPDMA1_CH0_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(LPDMA1_CH0_IRQn);

  return &hLPDMA1_CH0;
}

/**
  * @brief  DeInitialize the lpdma1_ch0 (configuration 1) through HAL layer.
  */
void mx_lpdma1_ch0_deinit(void)
{
  (void)HAL_DMA_DeInit(&hLPDMA1_CH0);
}

/**
  * @brief  Get the LPDMA1_CH0 object.
  */
hal_dma_handle_t *mx_lpdma1_ch0_gethandle(void)
{
  return &hLPDMA1_CH0;
}
/******************************************************************************/
/* LPDMA1 channel0 global interrupt */
/******************************************************************************/
void LPDMA1_CH0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hLPDMA1_CH0);
}
