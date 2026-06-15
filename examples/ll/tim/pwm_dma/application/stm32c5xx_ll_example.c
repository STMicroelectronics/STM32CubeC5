/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : TIM PWM DMA with LL API
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
#define LL_DMA_OPT_IT_HT      (0x01UL << 9U)                        /* DMA channel half transfer interrupt enabled   */
#define LL_DMA_OPT_IT_TO      (0x01UL << 14U)                       /* DMA channel trigger overrun interrupt enabled */
#define LL_DMA_OPT_IT_DEFAULT (LL_DMA_OPT_IT_HT | LL_DMA_OPT_IT_TO) /* DMA channel all optional interrupts enabled   */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
void mx_lpdmax_chy_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/**
  *  Start the MX_TIMx Output Channel in DMA mode (default DMA interrupts).
  */
/* DMA linked-list node structure definition */
inline void TIM_OC_StartChannel_DMA(const uint8_t *p_data, uint32_t size_byte)
{
  /* Specifies the physical DMA channel node registers description */
  uint32_t *timx_chy_dma_lli_reg = mx_tim1_cc1_dma_get_lli_reg();

  /* Configure a DMA handle for a DMA transfer. */
  LL_TIM_EnableDMAReq(MX_TIMx, 0x200);

  uint32_t dest_addr = (uint32_t)(&MX_TIMx->CCR1) + LL_TIM_OFFSET_TAB_CCRx[0];
  /* Circular linked -list mode is activated */
  timx_chy_dma_lli_reg[LL_DMA_NODE_CBR1_REG_OFFSET] = size_byte;
  timx_chy_dma_lli_reg[LL_DMA_NODE_CSAR_REG_OFFSET] = (uint32_t)p_data;
  timx_chy_dma_lli_reg[LL_DMA_NODE_CDAR_REG_OFFSET] = dest_addr;

  /* Start the DMA channel linked-list transfer. */
  uint32_t update_bits = LL_DMA_UPDATE_CTR1 | LL_DMA_UPDATE_CTR2 | LL_DMA_UPDATE_CBR1 |
                         LL_DMA_UPDATE_CSAR | LL_DMA_UPDATE_CDAR | LL_DMA_UPDATE_CLLR;
  LL_DMA_SetLinkedListBaseAddr(MX_LPDMAX_CHY_CH, (uint32_t)timx_chy_dma_lli_reg);
  LL_DMA_ConfigLinkUpdate(MX_LPDMAX_CHY_CH, update_bits, ((uint32_t)timx_chy_dma_lli_reg & DMA_CLLR_LA));
  LL_DMA_ClearFlag((MX_LPDMAX_CHY_CH), LL_DMA_FLAG_ALL);
  LL_DMA_DisableIT((MX_LPDMAX_CHY_CH), LL_DMA_IT_ALL);

  LL_DMA_EnableIT((MX_LPDMAX_CHY_CH), (LL_DMA_IT_TC  | LL_DMA_IT_DTE | LL_DMA_IT_ULE \
                                       | LL_DMA_IT_USE | LL_DMA_OPT_IT_DEFAULT));
  LL_DMA_SetBlkDataLength((MX_LPDMAX_CHY_CH), 0U);
  LL_DMA_EnableChannel((MX_LPDMAX_CHY_CH));


  LL_TIM_CC_EnableChannel(MX_TIMx, MX_TIM_X_CHANNEL);
  if (IS_TIM_BREAK_INSTANCE(MX_TIMx))
  {
    LL_TIM_EnableAllOutputs(MX_TIMx);
  }

}


/**
  *  Start the MX_TIMx.
  */
void TIM_Start(void)
{
  LL_TIM_EnableCounter(MX_TIMx);
}


/**
  *  Handles the MX_LPDMAx_CHy interrupts.
  */
void mx_lpdmax_chy_irqhandler(void)
{
  /* Half Transfer Complete Interrupt management */
  LL_DMA_ClearFlag_HT(MX_LPDMAX_CHY_CH);
  /* Transfer Complete Interrupt management */
  LL_DMA_ClearFlag_TC(MX_LPDMAX_CHY_CH);

  /* Check if there are remaining data */
  if (LL_DMA_IsActiveFlag_IDLE(MX_LPDMAX_CHY_CH) != 0U)
  {
    LL_DMA_ClearFlag_HT(MX_LPDMAX_CHY_CH);

    LL_DMA_DisableIT(MX_LPDMAX_CHY_CH, LL_DMA_IT_ALL);
  }
}
