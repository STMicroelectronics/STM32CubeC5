/**
  ******************************************************************************
  * @file    mx_lpdma1.c
  * @brief   Initialisation of all the peripherals used in this project
  *         File per peripheral instance that is handling the peripheral
  *         initialization and the IRQ handlers. Init parameters provided
  *         as a set of defines configurable/generated for a given target
  *         through STM32CubeSTUDIO.
  *
  ******************************************************************************
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_stm32c5xx_hal_drivers_license.md file
  * in the same directory as the generated code.
  * If no mx_stm32c5xx_hal_drivers_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mx_lpdma1.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables  --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported variables by reference -------------------------------------------*/
/* DMA handle */
static hal_dma_handle_t hLPDMA1_CH0;
static hal_dma_handle_t hLPDMA1_CH1;
/**
  * @brief DMA Initialization Function
  * @param None
  * @retval Pointer on the DMA Handle, NULL in case of error
  */
hal_dma_handle_t *dma1_channel0_cfg1_init(void)
{
  hal_dma_direct_xfer_config_t xfer_cfg_spi2_tx_dma;

 /* Configure the DMA TX */

  HAL_RCC_LPDMA1_EnableClock();

  if (HAL_DMA_Init(&hLPDMA1_CH0, HAL_LPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  xfer_cfg_spi2_tx_dma.request         = HAL_LPDMA1_REQUEST_SPI1_TX;
  xfer_cfg_spi2_tx_dma.direction       = HAL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  xfer_cfg_spi2_tx_dma.src_inc         = HAL_DMA_SRC_ADDR_INCREMENTED;
  xfer_cfg_spi2_tx_dma.dest_inc        = HAL_DMA_DEST_ADDR_FIXED;
  xfer_cfg_spi2_tx_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_BYTE;
  xfer_cfg_spi2_tx_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_BYTE;
  xfer_cfg_spi2_tx_dma.priority        = HAL_DMA_PRIORITY_LOW_WEIGHT_LOW;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hLPDMA1_CH0, &xfer_cfg_spi2_tx_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for LPDMA1_CH0 */
  HAL_CORTEX_NVIC_SetPriority(LPDMA1_CH0_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(LPDMA1_CH0_IRQn);

  /* Link the Transmit DMA handle to the SPI handle */
  if (HAL_SPI_SetTxDMA(mx_spi1_gethandle(), &hLPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  return &hLPDMA1_CH0;
}

/**
  * @brief DMA Initialization Function
  * @param None
  * @retval Pointer on the DMA Handle, NULL in case of error
  */
hal_dma_handle_t *dma1_channel1_cfg1_init(void)
{
  hal_dma_direct_xfer_config_t xfer_cfg_spi1_rx_dma;

  /* Configure the DMA RX */

  HAL_RCC_LPDMA1_EnableClock();

  if (HAL_DMA_Init(&hLPDMA1_CH1, HAL_LPDMA1_CH1) != HAL_OK)
  {
    return NULL;
  }

  xfer_cfg_spi1_rx_dma.request         = HAL_LPDMA1_REQUEST_SPI1_RX;
  xfer_cfg_spi1_rx_dma.direction       = HAL_DMA_DIRECTION_PERIPH_TO_MEMORY;
  xfer_cfg_spi1_rx_dma.src_inc         = HAL_DMA_SRC_ADDR_FIXED;
  xfer_cfg_spi1_rx_dma.dest_inc        = HAL_DMA_DEST_ADDR_INCREMENTED;
  xfer_cfg_spi1_rx_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_BYTE;
  xfer_cfg_spi1_rx_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_BYTE;
  xfer_cfg_spi1_rx_dma.priority        = HAL_DMA_PRIORITY_LOW_WEIGHT_LOW;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hLPDMA1_CH1, &xfer_cfg_spi1_rx_dma) != HAL_OK)
  {
    return NULL;
  }


  /* Enable the interruption for LPDMA1_CH1 */
  HAL_CORTEX_NVIC_SetPriority(LPDMA1_CH1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(LPDMA1_CH1_IRQn);


  /* Link the Receive DMA handle to the SPI handle */
  if (HAL_SPI_SetRxDMA(mx_spi1_gethandle(), &hLPDMA1_CH1) != HAL_OK)
  {
    return NULL;
  }

  return &hLPDMA1_CH1;
}


/**
  * @brief DMA peripheral deinitialization function
  * @param None
  * @retval None
  */
void dma1_channel0_deinit(void)
{
  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hLPDMA1_CH0);
  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(LPDMA1_CH0_IRQn);
}

/**
  * @brief DMA peripheral deinitialization function
  * @param None
  * @retval None
  */
void dma1_channel1_deinit(void)
{
  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hLPDMA1_CH1);
  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(LPDMA1_CH1_IRQn);
}

/**
  * @brief  Get the DMA1 object.
  * @param None
  * @retval Pointer on the DMA1 Handle
  */
hal_dma_handle_t *dma1_channel0_gethandle(void)
{
  return &hLPDMA1_CH0;
}

/**
  * @brief  Get the DMA1 object.
  * @param None
  * @retval Pointer on the DMA1 Handle
  */
hal_dma_handle_t *dma1_channel1_gethandle(void)
{
  return &hLPDMA1_CH1;
}


/******************************************************************************/
/*                     LPDMA1 channel0 global interrupt                    */
/******************************************************************************/
void LPDMA1_CH0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hLPDMA1_CH0);
}

/******************************************************************************/
/*                     LPDMA1 channel1 global interrupt                    */
/******************************************************************************/
void LPDMA1_CH1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hLPDMA1_CH1);
}
