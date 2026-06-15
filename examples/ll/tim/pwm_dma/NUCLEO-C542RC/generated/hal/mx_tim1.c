/**
  ******************************************************************************
  * @file           : mx_tim1.c
  * @brief          : Peripheral initialization
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
#include "mx_tim1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static uint32_t TIM1_CC1_DMA_LLi_reg[LL_DMA_NODE_REGISTER_NUM];

/* Exported function definition ----------------------------------------------*/
/******************************************************************************/
/* Exported functions for TIM1 in LL layer */
/******************************************************************************/

TIM_TypeDef *mx_tim1_init(void)
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);

  /* Timer configuration to reach the output frequency at 4 kHz */
  LL_TIM_SetPrescaler(TIM1, 143);
  /* LL_TIM_SetCounterMode(TIM1, LL_TIM_COUNTERMODE_UP); */ /* Configuration matches register reset state at startup. */
  LL_TIM_SetAutoReload(TIM1, 249);
  /* LL_TIM_SetClockDivision(TIM1, LL_TIM_CLOCKDIVISION_DIV1); */ /* Configuration matches register reset state at startup. */
  /* LL_TIM_SetClockDivision2(TIM1, LL_TIM_CLOCKDIVISION2_DIV1); */ /* Configuration matches register reset state at startup. */
  /* LL_TIM_SetClockSource(TIM1, LL_TIM_CLK_INTERNAL); */ /* Configuration matches register reset state at startup. */

  /* Channel 1 Configuration */
  /* LL_TIM_OC_SetPolarity(TIM1, LL_TIM_CHANNEL_CH1, LL_TIM_OCPOLARITY_HIGH); */ /* Configuration matches register reset state at startup. */
  /* LL_TIM_OC_SetIdleState(TIM1, LL_TIM_CHANNEL_CH1, LL_TIM_OCIDLESTATE_RESET); */ /* Configuration matches register reset state at startup. */
  /* LL_TIM_OC_SetOverrideState(TIM1, LL_TIM_CHANNEL_CH1, LL_TIM_OCOVERRIDE_RESET); */ /* Configuration matches register reset state at startup. */
  /* LL_TIM_OC_SetBreakMode(TIM1, LL_TIM_CHANNEL_CH1, LL_TIM_OCBREAKMODE_IMMEDIATE); */ /* Configuration matches register reset state at startup. */

  /* Compare Unit 1 Configuration */
  LL_TIM_OC_SetMode(TIM1, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);
  /* LL_TIM_OC_SetCompareValue(TIM1, LL_TIM_OC_COMPARE_UNIT_1, 0); */ /* Configuration matches register reset state at startup. */
  LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH1);

  /* Update Event Management */
  /* LL_TIM_SetUpdateSource(TIM1, LL_TIM_UPDATESOURCE_REGULAR); */ /* Configuration matches register reset state at startup. */
  /* LL_TIM_EnableUpdateEvent(TIM1); */ /* Configuration matches register reset state at startup. */

  /* Capture/Compare DMA request selection */
  /* LL_TIM_CC_SetDMAReqTrigger(TIM1, LL_TIM_CCDMAREQUEST_CC); */ /* Configuration matches register reset state at startup. */

  /* ### TIM1 GPIO Configuration ########################### */

  /* GPIO Clocks activation */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

  /**
    [GPIO Pin] ------> [Signal Name] ------> [Labels]

       PA8     ------>   TIM1_CH1   ------>  PA8
    **/

  /* Configure IO output speed (Low, Medium, High or Very-High) */
  /* LL_GPIO_SetPinSpeed(PA8_PORT, PA8_PIN, LL_GPIO_SPEED_FREQ_LOW); */ /* Configuration matches register reset state at startup. */

  /* Configure IO output type (Push-Pull or Open-Drain) */
  /* LL_GPIO_SetPinOutputType(PA8_PORT, PA8_PIN, LL_GPIO_OUTPUT_PUSHPULL); */ /* Configuration matches register reset state at startup. */

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(PA8_PORT, PA8_PIN, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /* Configure the Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(PA8_PORT, PA8_PIN, LL_GPIO_AF_1);

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  LL_GPIO_SetPinMode(PA8_PORT, PA8_PIN, LL_GPIO_MODE_ALTERNATE);

  /* ################################################## TIM1_cc1 DMA configuration */

  /* Enable LPDMA1 clock */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_LPDMA1);

  /* Configure DMA channel control parameters */
  LL_DMA_ConfigControl(LPDMA1_CH0, LL_DMA_PRIORITY_HIGH | LL_DMA_LINKEDLIST_EXECUTION_Q);

  /* Configure DMA channel transfer event mode */
  LL_DMA_SetTransferEventMode(LPDMA1_CH0, LL_DMA_LINKEDLIST_XFER_EVENT_Q);

  /* Configure DMA channel block data length */
  /* LL_DMA_SetBlkDataLength(LPDMA1_CH0, 0U); */ /* Configuration matches register reset state at startup. */

  /* Configure DMA channel linked-list base address */
  LL_DMA_SetLinkedListBaseAddr(LPDMA1_CH0, (uint32_t)&TIM1_CC1_DMA_LLi_reg);

  /* Configure DMA channel registers update and node address offset during the link transfer */
  LL_DMA_ConfigLinkUpdate(LPDMA1_CH0, LL_DMA_UPDATE_ALL, (uint32_t)&TIM1_CC1_DMA_LLi_reg);

  /* CTR1 register : Data width and address increment configuration */
  TIM1_CC1_DMA_LLi_reg[LL_DMA_NODE_CTR1_REG_OFFSET] = LL_DMA_SRC_ADDR_INCREMENTED | LL_DMA_SRC_DATA_WIDTH_WORD | \
                                                     LL_DMA_DEST_ADDR_FIXED | LL_DMA_DEST_DATA_WIDTH_WORD;

  /* CTR2 register : Request and direction configuration */
  TIM1_CC1_DMA_LLi_reg[LL_DMA_NODE_CTR2_REG_OFFSET] = LL_LPDMA1_REQUEST_TIM1_CC1 | LL_DMA_DIRECTION_MEMORY_TO_PERIPH;

  /* CLLR register */
  TIM1_CC1_DMA_LLi_reg[LL_DMA_NODE_CLLR_REG_OFFSET] = LL_DMA_UPDATE_ALL | ((uint32_t)&TIM1_CC1_DMA_LLi_reg & DMA_CLLR_LA);

  /* Enable the interruption for DMA */
  NVIC_SetPriority(LPDMA1_CH0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(LPDMA1_CH0_IRQn);

  return TIM1;
}

void mx_tim1_deinit(void)
{
  LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM1);

  LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM1);
  LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_TIM1);

  /* ### GPIO deinitialization of TIM1: TIM1_CH1 ########################### */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(PA8_PORT, PA8_PIN, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(PA8_PORT, PA8_PIN, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(PA8_PORT, PA8_PIN, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(PA8_PORT, PA8_PIN, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(PA8_PORT, PA8_PIN, LL_GPIO_PULL_NO);

  /* Reset the IO output state */
  LL_GPIO_WriteOutputPin(PA8_PORT, PA8_PIN, LL_GPIO_PIN_RESET);

  /* De-initialize the DMA channel */
  LL_DMA_DisableChannel(LPDMA1_CH0);
  LL_DMA_ClearFlag(LPDMA1_CH0, LL_DMA_FLAG_ALL);

  LL_DMA_WRITE_REG(LPDMA1_CH0, CCR, 0U);
  LL_DMA_WRITE_REG(LPDMA1_CH0, CTR1, 0U);
  LL_DMA_WRITE_REG(LPDMA1_CH0, CTR2, 0U);
  LL_DMA_WRITE_REG(LPDMA1_CH0, CBR1, 0U);
  LL_DMA_WRITE_REG(LPDMA1_CH0, CLLR, 0U);

  /* Disable the interruption for DMA */
  NVIC_DisableIRQ(LPDMA1_CH0_IRQn);
}

uint32_t *mx_tim1_cc1_dma_get_lli_reg(void)
{
  return TIM1_CC1_DMA_LLi_reg;
}

/******************************************************************************/
/*     LPDMA1 channel0 global interrupt is managed directly in user code.     */
/******************************************************************************/
/* void LPDMA1_CH0_IRQHandler(void)
{
}
  */
