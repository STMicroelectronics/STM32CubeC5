/**
  **********************************************************************************************************************
  * @file  : mx_lpdma1_ch0.c
  * @brief : Provides LPDMA1_CH0 services
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in the root directory of this software
  * component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **********************************************************************************************************************
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
hal_dma_handle_t *mx_lpdma1_ch0_hal_init(void)
{

  HAL_RCC_LPDMA1_EnableClock();

  if (HAL_DMA_Init(&hLPDMA1_CH0, HAL_LPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  /* Set DMA channel linked list xfer configuration */
  {
    hal_dma_linkedlist_xfer_config_t linkedlist_xfer_cfg;

    linkedlist_xfer_cfg.priority        = HAL_DMA_PRIORITY_HIGH;
    linkedlist_xfer_cfg.xfer_event_mode = HAL_DMA_LINKEDLIST_XFER_EVENT_Q;
    if (HAL_DMA_SetConfigLinkedListXfer(&hLPDMA1_CH0, &linkedlist_xfer_cfg) != HAL_OK)
    {
      return NULL;
    }
  }

  /* Enable the interruption for LPDMA1_CH0 */
  HAL_CORTEX_NVIC_SetPriority(LPDMA1_CH0_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(LPDMA1_CH0_IRQn);

  return &hLPDMA1_CH0;
}

/**
  * @brief  DeInitialize the lpdma1_ch0 (configuration 1) through HAL layer.
  */
void mx_lpdma1_ch0_hal_deinit(void)
{
  /* Disable the interruption for LPDMA1_CH0 */
  HAL_CORTEX_NVIC_DisableIRQ(LPDMA1_CH0_IRQn);

  (void)HAL_DMA_DeInit(&hLPDMA1_CH0);
}

/**
  * @brief Retrieve the lpdma1_channel12 handle.
  */
hal_dma_handle_t *mx_lpdma1_ch0_hal_gethandle(void)
{
  return &hLPDMA1_CH0;
}
/******************************************************************************/
/*                     LPDMA1 channel1 global interrupt                    */
/******************************************************************************/
void LPDMA1_CH0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hLPDMA1_CH0);
}
