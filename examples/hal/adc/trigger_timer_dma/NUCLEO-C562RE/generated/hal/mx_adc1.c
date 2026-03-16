/**
  ******************************************************************************
  * @file           : mx_adc1.c
  * @brief          : ADC1 Peripheral initialization
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
#include "mx_adc1.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_adc_handle_t hADC1;
static hal_dma_handle_t hLPDMA1_CH1;

/******************************************************************************/
/* Exported functions for ADC1 in HAL layer (SW instance MyADC_1) */
/******************************************************************************/
hal_adc_handle_t *mx_adc1_init(void)
{
  HAL_RCC_ADC12_EnableClock();

  if (HAL_RCC_ADCDAC_SetKernelClkSource(HAL_RCC_ADCDAC_CLK_SRC_PSIS) != HAL_OK)
  {
    return NULL;
  }

  if (HAL_ADC_Init(&hADC1, HAL_ADC1) != HAL_OK)
  {
    return NULL;
  }

  hal_adc_config_t adc_config;
  hal_adc_channel_config_t adc_channel_config;

  adc_config.resolution          = HAL_ADC_RESOLUTION_12_BIT;
  adc_config.sampling_mode       = HAL_ADC_SAMPLING_MODE_NORMAL;
  HAL_ADC_SetConfig(&hADC1, &adc_config);

  /****************************************************************************/
  /* Configuration of basic features (mandatory)                              */
  /****************************************************************************/

/* ==================== Group Regular ====================*/
  hal_adc_reg_config_t reg_config;
  reg_config.trigger_src        = HAL_ADC_REG_TRIG_TIM2_TRGO;
  reg_config.trigger_edge       = HAL_ADC_REG_TRIG_EDGE_RISING;
  reg_config.sequencer_length   = 1;
  reg_config.sequencer_discont  = HAL_ADC_REG_SEQ_DISCONT_DISABLE;
  reg_config.continuous         = HAL_ADC_REG_CONV_SINGLE;
  reg_config.overrun            = HAL_ADC_REG_OVR_DATA_OVERWRITTEN;
  HAL_ADC_REG_SetConfig(&hADC1, &reg_config);

  adc_channel_config.group           = HAL_ADC_GROUP_REGULAR;
  adc_channel_config.sequencer_rank  = 1;
  adc_channel_config.sampling_time   = HAL_ADC_SAMPLING_TIME_289CYCLES;
  adc_channel_config.input_mode      = HAL_ADC_IN_SINGLE_ENDED;
  HAL_ADC_SetConfigChannel(&hADC1, HAL_ADC_CHANNEL_4, &adc_channel_config);

  /****************************************************************************/
  /* Configuration of additional features (optional)                          */
  /****************************************************************************/

  HAL_RCC_GPIOA_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    ADC1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PA4     ------>   ADC1_IN4
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ANALOG;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_4, &gpio_config);

  /* Enable the interruption for ADC */
  HAL_CORTEX_NVIC_SetPriority(ADC1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(ADC1_IRQn);
  /* Configure DMA */
      if (HAL_DMA_Init(&hLPDMA1_CH1, HAL_LPDMA1_CH1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_LPDMA1_EnableClock();

hal_dma_direct_xfer_config_t xfer_cfg_adc1_dma;
  xfer_cfg_adc1_dma.request         = HAL_LPDMA1_REQUEST_ADC1;
  xfer_cfg_adc1_dma.direction       = HAL_DMA_DIRECTION_PERIPH_TO_MEMORY;
  xfer_cfg_adc1_dma.src_inc         = HAL_DMA_SRC_ADDR_FIXED;
  xfer_cfg_adc1_dma.dest_inc        = HAL_DMA_DEST_ADDR_INCREMENTED;
  xfer_cfg_adc1_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_HALFWORD;
  xfer_cfg_adc1_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_HALFWORD;
  xfer_cfg_adc1_dma.priority        = HAL_DMA_PRIORITY_LOW_WEIGHT_LOW;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hLPDMA1_CH1, &xfer_cfg_adc1_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for LPDMA1_CH1 */
  HAL_CORTEX_NVIC_SetPriority(LPDMA1_CH1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(LPDMA1_CH1_IRQn);

  /* Link DMA handle to ADC handle */
  HAL_ADC_REG_SetDMA(&hADC1, &hLPDMA1_CH1);

  return &hADC1;
}

void mx_adc1_deinit(void)
{
  (void)HAL_ADC_DeInit(&hADC1);

  /* Disable the interruption for ADC */
  HAL_CORTEX_NVIC_DisableIRQ(ADC1_IRQn);

  /* De-initialize all GPIO pins associated with ADC1 */
  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_4);

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hLPDMA1_CH1);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(LPDMA1_CH1_IRQn);
}

hal_adc_handle_t *mx_adc1_gethandle(void)
{
  return &hADC1;
}

/******************************************************************************/
/*                           ADC1 global interrupt                            */
/******************************************************************************/
void ADC1_IRQHandler(void)
{
  HAL_ADC_IRQHandler(&hADC1);
}

/******************************************************************************/
/*                      LPDMA1 channel1 global interrupt                      */
/******************************************************************************/
void LPDMA1_CH1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hLPDMA1_CH1);
}
