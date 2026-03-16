/**
  ******************************************************************************
  * @file           : mx_i3c1.c
  * @brief          : I3C1 Peripheral initialization
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
#include "mx_i3c1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_i3c_handle_t hI3C1;
static hal_dma_handle_t hLPDMA1_CH0;
static hal_dma_handle_t hLPDMA1_CH1;
static hal_dma_handle_t hLPDMA1_CH2;

/******************************************************************************/
/* Exported functions for I3C in HAL layer (SW instance MyI3C_1) */
/******************************************************************************/
hal_i3c_handle_t *mx_i3c1_init(void)
{
  if (HAL_I3C_Init(&hI3C1, HAL_I3C1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_I3C1_EnableClock();

  if (HAL_RCC_I3C1_SetKernelClkSource(HAL_RCC_I3C1_CLK_SRC_PCLK1) != HAL_OK)
  {
    return NULL;
  }

  /**
    * I3C1 timing_reg0 calculated by CubeMX2 with:
    * - SDA rise time = 350 ns
    * - Input frequency = 144 MHz
    * - Bus usage = UTILS_I3C_PURE_I3C_BUS
    * - I3C bus frequency = 12.5 MHz
    * - I3C duty cycle = 50 %
    * I3C1 timing_reg1 calculated by CubeMX2 with:
    * - Wait time = LL_I3C_OWN_ACTIVITY_STATE_0
    */
  hal_i3c_ctrl_config_t i3c_ctrl_config;
  i3c_ctrl_config.timing_reg0 = 0x330505UL;
  i3c_ctrl_config.timing_reg1 = 0x1D008EUL;
  if (HAL_I3C_CTRL_SetConfig(&hI3C1, &i3c_ctrl_config) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_GPIOB_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    I3C1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PB6     ------>   I3C1_SCL
       PB7     ------>   I3C1_SDA
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_HIGH;
  gpio_config.alternate   = HAL_GPIO_AF_3;
  HAL_GPIO_Init(HAL_GPIOB, HAL_GPIO_PIN_6 | HAL_GPIO_PIN_7, &gpio_config);

  /* Configure the Tx DMA for 1-byte transfers (FIFO threshold = 1_8) */
      if (HAL_DMA_Init(&hLPDMA1_CH0, HAL_LPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_LPDMA1_EnableClock();

hal_dma_direct_xfer_config_t xfer_cfg_i3c1_tx_dma;
  xfer_cfg_i3c1_tx_dma.request         = HAL_LPDMA1_REQUEST_I3C1_TX;
  xfer_cfg_i3c1_tx_dma.direction       = HAL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  xfer_cfg_i3c1_tx_dma.src_inc         = HAL_DMA_SRC_ADDR_INCREMENTED;
  xfer_cfg_i3c1_tx_dma.dest_inc        = HAL_DMA_DEST_ADDR_FIXED;
  xfer_cfg_i3c1_tx_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_BYTE;
  xfer_cfg_i3c1_tx_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_BYTE;
  xfer_cfg_i3c1_tx_dma.priority        = HAL_DMA_PRIORITY_LOW_WEIGHT_LOW;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hLPDMA1_CH0, &xfer_cfg_i3c1_tx_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for LPDMA1_CH0 */
  HAL_CORTEX_NVIC_SetPriority(LPDMA1_CH0_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(LPDMA1_CH0_IRQn);

  /* Link the transmit DMA handle to the I3C1 handle */
  if (HAL_I3C_SetTxDMA(&hI3C1, &hLPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  /* Configure the RX DMA for 1-byte transfers (FIFO threshold = 1_8) */
      if (HAL_DMA_Init(&hLPDMA1_CH1, HAL_LPDMA1_CH1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_LPDMA1_EnableClock();

hal_dma_direct_xfer_config_t xfer_cfg_i3c1_rx_dma;
  xfer_cfg_i3c1_rx_dma.request         = HAL_LPDMA1_REQUEST_I3C1_RX;
  xfer_cfg_i3c1_rx_dma.direction       = HAL_DMA_DIRECTION_PERIPH_TO_MEMORY;
  xfer_cfg_i3c1_rx_dma.src_inc         = HAL_DMA_SRC_ADDR_FIXED;
  xfer_cfg_i3c1_rx_dma.dest_inc        = HAL_DMA_DEST_ADDR_INCREMENTED;
  xfer_cfg_i3c1_rx_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_BYTE;
  xfer_cfg_i3c1_rx_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_BYTE;
  xfer_cfg_i3c1_rx_dma.priority        = HAL_DMA_PRIORITY_LOW_WEIGHT_LOW;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hLPDMA1_CH1, &xfer_cfg_i3c1_rx_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for LPDMA1_CH1 */
  HAL_CORTEX_NVIC_SetPriority(LPDMA1_CH1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(LPDMA1_CH1_IRQn);

  /* Link the receive DMA handle to the I3C1 handle */
  if (HAL_I3C_SetRxDMA(&hI3C1, &hLPDMA1_CH1) != HAL_OK)
  {
    return NULL;
  }

  /* Configure the Transmit Control DMA for 1 word (4 bytes) transfers */
      if (HAL_DMA_Init(&hLPDMA1_CH2, HAL_LPDMA1_CH2) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_LPDMA1_EnableClock();

hal_dma_direct_xfer_config_t xfer_cfg_i3c1_tc_dma;
  xfer_cfg_i3c1_tc_dma.request         = HAL_LPDMA1_REQUEST_I3C1_TC;
  xfer_cfg_i3c1_tc_dma.direction       = HAL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  xfer_cfg_i3c1_tc_dma.src_inc         = HAL_DMA_SRC_ADDR_INCREMENTED;
  xfer_cfg_i3c1_tc_dma.dest_inc        = HAL_DMA_DEST_ADDR_FIXED;
  xfer_cfg_i3c1_tc_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_WORD;
  xfer_cfg_i3c1_tc_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_WORD;
  xfer_cfg_i3c1_tc_dma.priority        = HAL_DMA_PRIORITY_LOW_WEIGHT_LOW;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hLPDMA1_CH2, &xfer_cfg_i3c1_tc_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for LPDMA1_CH2 */
  HAL_CORTEX_NVIC_SetPriority(LPDMA1_CH2_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(LPDMA1_CH2_IRQn);

  /* Link the transmit DMA handle to the I3C1 handle */
  if (HAL_I3C_SetTcDMA(&hI3C1, &hLPDMA1_CH2) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the Event interrupt for I3C1 */
  HAL_CORTEX_NVIC_SetPriority(I3C1_EV_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(I3C1_EV_IRQn);

  /* Enable the Error interrupt for I3C1 */
  HAL_CORTEX_NVIC_SetPriority(I3C1_ERR_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(I3C1_ERR_IRQn);

  return &hI3C1;
}

void mx_i3c1_deinit(void)
{
  /* Disable the Event interrupt for I3C1 */
  HAL_CORTEX_NVIC_DisableIRQ(I3C1_EV_IRQn);

  /* Disable the Error interrupt for I3C1 */
  HAL_CORTEX_NVIC_DisableIRQ(I3C1_ERR_IRQn);

  (void)HAL_I3C_DeInit(&hI3C1);

  HAL_RCC_I3C1_Reset();

  HAL_RCC_I3C1_DisableClock();

  /* De-initialize all GPIO pins associated with I3C1 */
  HAL_GPIO_DeInit(HAL_GPIOB, HAL_GPIO_PIN_6 | HAL_GPIO_PIN_7);

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hLPDMA1_CH0);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(LPDMA1_CH0_IRQn);

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hLPDMA1_CH1);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(LPDMA1_CH1_IRQn);

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hLPDMA1_CH2);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(LPDMA1_CH2_IRQn);
}

hal_i3c_handle_t *mx_i3c1_gethandle(void)
{
  return &hI3C1;
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

/******************************************************************************/
/*                      LPDMA1 channel2 global interrupt                      */
/******************************************************************************/
void LPDMA1_CH2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hLPDMA1_CH2);
}

/******************************************************************************/
/*                            I3C1 event interrupt                            */
/******************************************************************************/
void I3C1_EV_IRQHandler(void)
{
  HAL_I3C_EV_IRQHandler(&hI3C1);
}

/******************************************************************************/
/*                            I3C1 error interrupt                            */
/******************************************************************************/
void I3C1_ERR_IRQHandler(void)
{
  HAL_I3C_ERR_IRQHandler(&hI3C1);
}
