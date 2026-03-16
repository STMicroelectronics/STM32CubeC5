/**
  ******************************************************************************
  * @file           : mx_cordic.c
  * @brief          : CORDIC Peripheral initialization
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "mx_cordic.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
/* Handle for CORDIC */
static hal_cordic_handle_t hCORDIC;
static hal_dma_handle_t hLPDMA1_CH0;
static hal_dma_handle_t hLPDMA1_CH1;


/* Exported function definition ----------------------------------------------*/
/******************************************************************************/
/* Exported functions for CORDIC in HAL layer */
/******************************************************************************/


hal_cordic_handle_t *mx_cordic_cfg1_hal_init(void)
{
  hal_cordic_config_t cordic_config;

  /* Basic configuration */
  if (HAL_CORDIC_Init(&hCORDIC, HAL_CORDIC) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_CORDIC_EnableClock();

  cordic_config.function       = HAL_CORDIC_FUNCTION_SINE;
  cordic_config.scaling_factor = HAL_CORDIC_SCALING_FACTOR_0;
  cordic_config.in_width       = HAL_CORDIC_IN_WIDTH_32_BIT;
  cordic_config.nb_arg         = HAL_CORDIC_NB_ARG_1;
  cordic_config.out_width      = HAL_CORDIC_OUT_WIDTH_32_BIT;
  cordic_config.nb_result      = HAL_CORDIC_NB_RESULT_2;
  cordic_config.scaling_factor = HAL_CORDIC_SCALING_FACTOR_0;
  cordic_config.precision      = HAL_CORDIC_PRECISION_6_CYCLE;

  if (HAL_CORDIC_SetConfig(&hCORDIC, &cordic_config) != HAL_OK)
  {
    return NULL;
  }

  /* Configure the DMA Write */
  HAL_RCC_LPDMA1_EnableClock();

  if (HAL_DMA_Init(&hLPDMA1_CH0, HAL_LPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  /* Set DMA channel direct xfer configuration */
  {
    hal_dma_direct_xfer_config_t xfer_cfg;

    xfer_cfg.request         = HAL_LPDMA1_REQUEST_CORDIC_WR;
    xfer_cfg.direction       = HAL_DMA_DIRECTION_MEMORY_TO_PERIPH;
    xfer_cfg.src_inc         = HAL_DMA_SRC_ADDR_INCREMENTED;
    xfer_cfg.dest_inc        = HAL_DMA_DEST_ADDR_FIXED;
    xfer_cfg.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_WORD;
    xfer_cfg.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_WORD;
    xfer_cfg.priority        = HAL_DMA_PRIORITY_LOW_WEIGHT_LOW;
    if (HAL_DMA_SetConfigPeriphDirectXfer(&hLPDMA1_CH0, &xfer_cfg) != HAL_OK)
    {
      return NULL;
    }
  }


  /* Enable the interruption for LPDMA1_CH0 */
  HAL_CORTEX_NVIC_SetPriority(LPDMA1_CH0_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(LPDMA1_CH0_IRQn);

  /* Link the Transmit DMA handle to the CORDIC handle */
  if (HAL_CORDIC_SetWriteDMA(&hCORDIC, &hLPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  /* Configure the DMA Read */
  HAL_RCC_LPDMA1_EnableClock();

  if (HAL_DMA_Init(&hLPDMA1_CH1, HAL_LPDMA1_CH1) != HAL_OK)
  {
    return NULL;
  }

  /* Set DMA channel direct xfer configuration */
  {
    hal_dma_direct_xfer_config_t xfer_cfg;

    xfer_cfg.request         = HAL_LPDMA1_REQUEST_CORDIC_RD;
    xfer_cfg.direction       = HAL_DMA_DIRECTION_PERIPH_TO_MEMORY;
    xfer_cfg.src_inc         = HAL_DMA_SRC_ADDR_FIXED;
    xfer_cfg.dest_inc        = HAL_DMA_DEST_ADDR_INCREMENTED;
    xfer_cfg.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_WORD;
    xfer_cfg.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_WORD;
    xfer_cfg.priority        = HAL_DMA_PRIORITY_LOW_WEIGHT_LOW;
    if (HAL_DMA_SetConfigPeriphDirectXfer(&hLPDMA1_CH1, &xfer_cfg) != HAL_OK)
    {
      return NULL;
    }
  }


  /* Enable the interruption for LPDMA1_CH1 */
  HAL_CORTEX_NVIC_SetPriority(LPDMA1_CH1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(LPDMA1_CH1_IRQn);

  /* Link the Transmit DMA handle to the PPP handle */
  if (HAL_CORDIC_SetReadDMA(&hCORDIC, &hLPDMA1_CH1) != HAL_OK)
  {
    return NULL;
  }
  HAL_CORTEX_NVIC_EnableIRQ(CORDIC_IRQn);

  return &hCORDIC;
}

void mx_cordic_cfg1_hal_deinit(void)
{
  /* Disable interrupt */
  HAL_CORTEX_NVIC_DisableIRQ(LPDMA1_CH0_IRQn);
  HAL_CORTEX_NVIC_DisableIRQ(LPDMA1_CH1_IRQn);
  HAL_CORTEX_NVIC_DisableIRQ(CORDIC_IRQn);
  (void)HAL_CORDIC_DeInit(&hCORDIC);

  HAL_RCC_CORDIC_Reset();
  HAL_RCC_CORDIC_DisableClock();

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hLPDMA1_CH0);

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hLPDMA1_CH1);

}

hal_cordic_handle_t *mx_cordic_hal_gethandle(void)
{
  return &hCORDIC;
}

/******************************************************************************/
/*                     CORDIC global interrupt                    */
/******************************************************************************/
void CORDIC_IRQHandler(void)
{
  HAL_CORDIC_IRQHandler(&hCORDIC);
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

