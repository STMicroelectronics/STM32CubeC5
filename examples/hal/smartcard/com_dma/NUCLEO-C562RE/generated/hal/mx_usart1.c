/**
  ******************************************************************************
  * @file           : mx_usart1.c
  * @brief          : USART1 Peripheral initialization
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
#include "mx_usart1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
/* Handle for SMARTCARD */
static hal_smartcard_handle_t hUSART1;
static hal_dma_handle_t hLPDMA1_CH0;
static hal_dma_handle_t hLPDMA1_CH1;
/* Exported function definition ----------------------------------------------*/
/******************************************************************************/
/* Exported functions for SMARTCARD in HAL layer */
/******************************************************************************/
hal_smartcard_handle_t *mx_usart1_smartcard_init(void)
{
  hal_smartcard_config_t smartcard_config;

  if (HAL_SMARTCARD_Init(&hUSART1, HAL_SMARTCARD1) != HAL_OK)
  {
    return NULL;
  }
  HAL_RCC_USART1_EnableClock();

  if (HAL_RCC_USART1_SetKernelClkSource(HAL_RCC_USART1_CLK_SRC_PCLK2) != HAL_OK)
  {
    return NULL;
  }

  smartcard_config.baud_rate = 10752U;
  smartcard_config.stop_bits = HAL_SMARTCARD_STOP_BIT_1_5;
  smartcard_config.first_bit = HAL_SMARTCARD_BIT_ORDER_LSB_FIRST;
  smartcard_config.parity = HAL_SMARTCARD_PARITY_EVEN;
  smartcard_config.nack = HAL_SMARTCARD_NACK_DISABLE;
  smartcard_config.clock_prescaler = HAL_SMARTCARD_CLOCK_PRESC_DIV1;
  smartcard_config.sclk_prescaler = HAL_SMARTCARD_SCLK_PRESC_DIV24;
  smartcard_config.clock_output = HAL_SMARTCARD_CLOCK_OUTPUT_ENABLE;
  smartcard_config.clock_polarity = HAL_SMARTCARD_CLOCK_POLARITY_LOW;
  smartcard_config.clock_phase = HAL_SMARTCARD_CLOCK_PHASE_1_EDGE;
  smartcard_config.guard_time_etu = 0U;
  smartcard_config.auto_retry_count = 3U;

  if (HAL_SMARTCARD_SetConfig(&hUSART1, &smartcard_config) != HAL_OK)
  {
    return NULL;
  }

  if (HAL_SMARTCARD_SetTxCpltIndication(&hUSART1, HAL_SMARTCARD_TX_CPLT_AFTER_GUARD_TIME) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_GPIOA_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    USART1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PA8     ------>   USART1_CK
       PA6     ------>   USART1_TX
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_LOW;
  gpio_config.alternate   = HAL_GPIO_AF_7;
  HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_8 | HAL_GPIO_PIN_6, &gpio_config);

  /* Enable interrupt */
  HAL_CORTEX_NVIC_SetPriority(USART1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(USART1_IRQn);
/* Configure the DMA TX */
      if (HAL_DMA_Init(&hLPDMA1_CH0, HAL_LPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_LPDMA1_EnableClock();

hal_dma_direct_xfer_config_t xfer_cfg_usart1_tx_dma;
  xfer_cfg_usart1_tx_dma.request         = HAL_LPDMA1_REQUEST_USART1_TX;
  xfer_cfg_usart1_tx_dma.direction       = HAL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  xfer_cfg_usart1_tx_dma.src_inc         = HAL_DMA_SRC_ADDR_INCREMENTED;
  xfer_cfg_usart1_tx_dma.dest_inc        = HAL_DMA_DEST_ADDR_FIXED;
  xfer_cfg_usart1_tx_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_BYTE;
  xfer_cfg_usart1_tx_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_BYTE;
  xfer_cfg_usart1_tx_dma.priority        = HAL_DMA_PRIORITY_HIGH;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hLPDMA1_CH0, &xfer_cfg_usart1_tx_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for LPDMA1_CH0 */
  HAL_CORTEX_NVIC_SetPriority(LPDMA1_CH0_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(LPDMA1_CH0_IRQn);

  /* Link the Transmit DMA handle to the SMARTCARD handle */
  if (HAL_SMARTCARD_SetTxDMA(&hUSART1, &hLPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

/* Configure the DMA RX */
      if (HAL_DMA_Init(&hLPDMA1_CH1, HAL_LPDMA1_CH1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_LPDMA1_EnableClock();

hal_dma_direct_xfer_config_t xfer_cfg_usart1_rx_dma;
  xfer_cfg_usart1_rx_dma.request         = HAL_LPDMA1_REQUEST_USART1_RX;
  xfer_cfg_usart1_rx_dma.direction       = HAL_DMA_DIRECTION_PERIPH_TO_MEMORY;
  xfer_cfg_usart1_rx_dma.src_inc         = HAL_DMA_SRC_ADDR_FIXED;
  xfer_cfg_usart1_rx_dma.dest_inc        = HAL_DMA_DEST_ADDR_INCREMENTED;
  xfer_cfg_usart1_rx_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_BYTE;
  xfer_cfg_usart1_rx_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_BYTE;
  xfer_cfg_usart1_rx_dma.priority        = HAL_DMA_PRIORITY_HIGH;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hLPDMA1_CH1, &xfer_cfg_usart1_rx_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for LPDMA1_CH1 */
  HAL_CORTEX_NVIC_SetPriority(LPDMA1_CH1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(LPDMA1_CH1_IRQn);

  /* Link the Receive DMA handle to the SMARTCARD handle */
  if (HAL_SMARTCARD_SetRxDMA(&hUSART1, &hLPDMA1_CH1) != HAL_OK)
  {
    return NULL;
  }

  return &hUSART1;
}

void mx_usart1_smartcard_deinit(void)
{
  /* De-initialize all GPIO pins associated with USART1 */
  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_6 | HAL_GPIO_PIN_8);

  /* Disable interrupt */
  HAL_CORTEX_NVIC_DisableIRQ(LPDMA1_CH1_IRQn);

  (void)HAL_SMARTCARD_DeInit(&hUSART1);

  HAL_RCC_USART1_Reset();

  HAL_RCC_USART1_DisableClock();

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hLPDMA1_CH0);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(LPDMA1_CH0_IRQn);

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hLPDMA1_CH1);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(LPDMA1_CH1_IRQn);
}

hal_smartcard_handle_t *mx_usart1_gethandle(void)
{
  return &hUSART1;
}

/******************************************************************************/
/*                          USART1 global interrupt                           */
/******************************************************************************/
void USART1_IRQHandler(void)
{
  HAL_SMARTCARD_IRQHandler(&hUSART1);
}

/******************************************************************************/
/*                      LPDMA1 channel0 global interrupt                      */
/******************************************************************************/
void LPDMA1_CH0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hLPDMA1_CH0);
}

/******************************************************************************/
/*                      LPDMA1 channel1 global interrupt                      */
/******************************************************************************/
void LPDMA1_CH1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hLPDMA1_CH1);
}
