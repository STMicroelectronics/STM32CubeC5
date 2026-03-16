/**
  ******************************************************************************
  * @file           : mx_lpdma1_ch1.c
  * @brief          : Provides LPDMA1_CH1 services
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
#include "mx_lpdma1_ch1.h"

/* Exported variables by reference------------------------------------------------------------------------------------*/

static hal_dma_handle_t hLPDMA1_CH1;

/**********************************************************************************************************************/
/* Exported functions for LPDMA1_CH1 in HAL layer */
/**********************************************************************************************************************/
/**
  * @brief  Initialize and configure the lpdma1_ch1 (configuration 1) through HAL layer.
  * @retval Pointer to hal_dma_handle_t structure.
  */
hal_dma_handle_t *mx_lpdma1_ch1_init(void)
{
if (HAL_DMA_Init(&hLPDMA1_CH1, HAL_LPDMA1_CH1) != HAL_OK)
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
  xfer_cfg.priority        = HAL_DMA_PRIORITY_LOW_WEIGHT_HIGH;

  if (HAL_DMA_SetConfigDirectXfer(&hLPDMA1_CH1, &xfer_cfg) != HAL_OK)
  {
    return NULL;
  }

  /* Set DMA channel trigger xfer configuration */
  {
    hal_dma_trigger_config_t trigger_cfg;

    trigger_cfg.source    = HAL_LPDMA1_TRIGGER_LPDMA1_CH0_TC;
    trigger_cfg.polarity  = HAL_DMA_TRIGGER_POLARITY_RISING;
    trigger_cfg.mode      = HAL_DMA_TRIGGER_SINGLE_BURST_TRANSFER;
    if (HAL_DMA_SetConfigDirectXferTrigger(&hLPDMA1_CH1, &trigger_cfg) != HAL_OK)
    {
      return NULL;
    }
  }

  /* Enable the interruption for LPDMA1_CH1 */
  HAL_CORTEX_NVIC_SetPriority(LPDMA1_CH1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(LPDMA1_CH1_IRQn);

  return &hLPDMA1_CH1;
}

/**
  * @brief  DeInitialize the lpdma1_ch1 (configuration 1) through HAL layer.
  */
void mx_lpdma1_ch1_deinit(void)
{
  (void)HAL_DMA_DeInit(&hLPDMA1_CH1);
}

/**
  * @brief  Get the LPDMA1_CH1 object.
  */
hal_dma_handle_t *mx_lpdma1_ch1_gethandle(void)
{
  return &hLPDMA1_CH1;
}
/******************************************************************************/
/* LPDMA1 channel1 global interrupt */
/******************************************************************************/
void LPDMA1_CH1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hLPDMA1_CH1);
}
