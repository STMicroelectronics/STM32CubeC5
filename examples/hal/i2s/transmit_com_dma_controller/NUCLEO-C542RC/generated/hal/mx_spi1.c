/**
  ******************************************************************************
  * @file           : mx_spi1.c
  * @brief          : SPI1 Peripheral initialization
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
#include "mx_spi1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_i2s_handle_t hSPI1;
static hal_dma_handle_t hLPDMA1_CH0;
/******************************************************************************/
/* Exported functions for I2S in HAL layer (SW instance MyI2S_1) */
/******************************************************************************/
hal_i2s_handle_t *mx_spi1_i2s_init(void)
{
  hal_i2s_master_config_t i2s_config;

  if (HAL_I2S_Init(&hSPI1, HAL_I2S1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_SPI1_EnableClock();

  if (HAL_RCC_SPI1_SetKernelClkSource(HAL_RCC_SPI1_CLK_SRC_PCLK2) != HAL_OK)
  {
    return NULL;
  }

  i2s_config.mode = HAL_I2S_MODE_MASTER_TX;
  i2s_config.standard = HAL_I2S_STANDARD_PCM_SHORT;
  i2s_config.data_format = HAL_I2S_DATA_FORMAT_16_BIT;
  i2s_config.audio_frequency = HAL_I2S_MASTER_AUDIO_FREQ_96_KHZ;
  i2s_config.clock_polarity = HAL_I2S_CLOCK_POLARITY_LOW;
  i2s_config.bit_order = HAL_I2S_MSB_FIRST;

  if (HAL_I2S_MASTER_SetConfig(&hSPI1, &i2s_config) != HAL_OK)
  {
    return NULL;
  }

  if (HAL_I2S_SetFifoThreshold(&hSPI1, HAL_I2S_FIFO_THRESHOLD_1_DATA) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_GPIOA_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    SPI1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PA5     ------>   I2S1_CK
       PA6     ------>   I2S1_SDI
       PA7     ------>   I2S1_SDO
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_LOW;
  gpio_config.alternate   = HAL_GPIO_AF_5;
  HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_5 | HAL_GPIO_PIN_6 | HAL_GPIO_PIN_7, &gpio_config);

  /**
    SPI1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PA15    ------>   I2S1_WS
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_UP;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_LOW;
  gpio_config.alternate   = HAL_GPIO_AF_5;
  HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_15, &gpio_config);

  /* Configure the DMA TX */
      if (HAL_DMA_Init(&hLPDMA1_CH0, HAL_LPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_LPDMA1_EnableClock();

hal_dma_direct_xfer_config_t xfer_cfg_spi1_tx_dma;
  xfer_cfg_spi1_tx_dma.request         = HAL_LPDMA1_REQUEST_SPI1_TX;
  xfer_cfg_spi1_tx_dma.direction       = HAL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  xfer_cfg_spi1_tx_dma.src_inc         = HAL_DMA_SRC_ADDR_INCREMENTED;
  xfer_cfg_spi1_tx_dma.dest_inc        = HAL_DMA_DEST_ADDR_FIXED;
  xfer_cfg_spi1_tx_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_HALFWORD;
  xfer_cfg_spi1_tx_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_HALFWORD;
  xfer_cfg_spi1_tx_dma.priority        = HAL_DMA_PRIORITY_LOW_WEIGHT_LOW;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hLPDMA1_CH0, &xfer_cfg_spi1_tx_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for LPDMA1_CH0 */
  HAL_CORTEX_NVIC_SetPriority(LPDMA1_CH0_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(LPDMA1_CH0_IRQn);

  /* Link the Transmit DMA handle to the I2S handle */
  if (HAL_I2S_SetTxDMA(&hSPI1, &hLPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

    if (HAL_RCC_SPI1_SetKernelClkSource(HAL_RCC_SPI1_CLK_SRC_PCLK2) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for I2S */
    HAL_CORTEX_NVIC_SetPriority(SPI1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(SPI1_IRQn);

  return &hSPI1;
}

void mx_spi1_i2s_deinit(void)
{
  /* Disable the interruption for I2S */
  HAL_CORTEX_NVIC_DisableIRQ(SPI1_IRQn);

  (void)HAL_I2S_DeInit(&hSPI1);

  HAL_RCC_SPI1_Reset();

  HAL_RCC_SPI1_DisableClock();

  /* De-initialize all GPIO pins associated with SPI1 */
  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_5 | HAL_GPIO_PIN_6 | HAL_GPIO_PIN_7 | HAL_GPIO_PIN_15);

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hLPDMA1_CH0);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(LPDMA1_CH0_IRQn);
 }

hal_i2s_handle_t *mx_spi1_i2s_gethandle(void)
{
  return &hSPI1;
}

/******************************************************************************/
/*                      LPDMA1 channel0 global interrupt                      */
/******************************************************************************/
void LPDMA1_CH0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hLPDMA1_CH0);
}

/******************************************************************************/
/*                           SPI1 global interrupt                            */
/******************************************************************************/
void SPI1_IRQHandler(void)
{
  HAL_I2S_IRQHandler(&hSPI1);
}
