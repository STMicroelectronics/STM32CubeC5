/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : Capturing external signals with the LPTIM using the LL API.
  ******************************************************************************
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LPTIM_GET_CAPTURE_COMPARE_REG_ADDR ((uint32_t)&MX_LPTIMx->CCR1)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
void mx_dmax_chy_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * brief: Start a MX_LPTIMx channel input capture in DMA mode.
  * param: value Pointer to the data buffer.
  */
inline void LPTIM_StartInputCaptureChannel_DMA(const void *value)
{
  /* Temporary enable the peripheral to modify DIER impact by EnableDMAReq. */
  uint32_t is_lptim_enabled = LL_LPTIM_IsEnabled(MX_LPTIMx);

  LL_DMA_ConfigAddresses(MX_DMAX_CHY_CH, LPTIM_GET_CAPTURE_COMPARE_REG_ADDR, (uint32_t)value);

  LL_DMA_SetBlkDataLength(MX_DMAX_CHY_CH, (CAPTURED_VALUE_BUFFER_SIZE * 2U)); /* Data size in bytes */

  LL_DMA_ClearFlag(MX_DMAX_CHY_CH, LL_DMA_FLAG_ALL);

  LL_DMA_DisableIT(MX_DMAX_CHY_CH, LL_DMA_IT_ALL);

#if defined (USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_DMA_EnableIT(MX_DMAX_CHY_CH, (LL_DMA_IT_TC | LL_DMA_IT_DTE | LL_DMA_IT_USE));
#else
  LL_DMA_EnableIT(MX_DMAX_CHY_CH, LL_DMA_IT_TC);
#endif /* USE_LL_APP_ERROR */

  LL_DMA_EnableChannel(MX_DMAX_CHY_CH);

  LL_LPTIM_Enable(MX_LPTIMx);

  LL_LPTIM_EnableDMAReq_CC1(MX_LPTIMx);

  if (is_lptim_enabled == 0U)
  {
    LL_LPTIM_Disable(MX_LPTIMx);
  }

  LL_LPTIM_CC_EnableChannel(MX_LPTIMx, MX_LPTIM_X_CHANNEL);
}


/**
  *  Start the MX_LPTIMx in polling mode.
  */
inline void LPTIM_Start(void)
{
  volatile uint32_t cfgr = (LL_LPTIM_READ_REG(MX_LPTIMx, CFGR) & ~(LL_LPTIM_OC_WAVEFORM_SETONCE | LPTIM_CFGR_TIMOUT));

  LL_LPTIM_WRITE_REG(MX_LPTIMx, CFGR, cfgr);

  LL_LPTIM_Enable(MX_LPTIMx);

  LL_LPTIM_StartCounter(MX_LPTIMx, LL_LPTIM_OPERATING_MODE_CONTINUOUS);
}


/**
  *  Handles the MX_DMAX_CHY_CH interrupts.
  */
void mx_dmax_chy_irqhandler(void)
{
  DMA_TypeDef *instance;
  uint32_t    flags;
  uint32_t    its;
  uint32_t    dma_channel;

  instance    = LL_DMA_GET_INSTANCE(MX_DMAX_CHY_CH);
  dma_channel = LL_DMA_GET_CHANNEL_IDX(MX_DMAX_CHY_CH);
  its         = LL_DMA_READ_REG((MX_DMAX_CHY_CH), CCR);

  /* Check DMA channel active interrupts */
  if (LL_DMA_IsActiveFlag_MIS(instance, dma_channel) == 0U)
  {
    return; /* the global interrupt flag for the current channel is down , nothing to do */
  }

  flags = LL_DMA_READ_REG((MX_DMAX_CHY_CH), CSR);

  /* Transfer Complete Interrupt management */
  if ((flags & its & LL_DMA_FLAG_TC) != 0U)
  {
    LL_DMA_ClearFlag_TC(MX_DMAX_CHY_CH);

    /* Check if there are remaining data */
    if (LL_DMA_IsActiveFlag_IDLE(MX_DMAX_CHY_CH) != 0U)
    {
      LL_DMA_ClearFlag_HT(MX_DMAX_CHY_CH);

      LL_DMA_DisableIT(MX_DMAX_CHY_CH, LL_DMA_IT_ALL);
    }

    return;
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  /* Error Interrupt management */
  LL_DMA_ClearFlag(MX_DMAX_CHY_CH, LL_DMA_FLAG_ALL);

  /* Check error flags */
  if (((flags & its) & (LL_DMA_FLAG_DTE | LL_DMA_FLAG_USE)) != 0U)
  {
    LL_DMA_ResetChannel(MX_DMAX_CHY_CH);

    LL_DMA_DisableIT(MX_DMAX_CHY_CH, LL_DMA_IT_ALL);
  }
#endif /* USE_LL_APP_ERROR */
}
