/**
  ******************************************************************************
  * @file           : mx_dac1.c
  * @brief          : DAC1 Peripheral initialization
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
#include "mx_dac1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static uint32_t DAC1_CH1_DMA_LLi_reg[LL_DMA_NODE_REGISTER_NUM];

/******************************************************************************/
/* Exported functions for DAC1 in LL layer (SW instance MyDAC_1) */
/******************************************************************************/
DAC_TypeDef *mx_dac1_init(void)
{
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_DAC1);

  LL_RCC_SetADCDACClockSource(LL_RCC_ADCDAC_CLKSOURCE_PSIS);

  /****************************************************************************/
  /* Initialization of DAC instance                                           */
  /****************************************************************************/
  /* LL_DAC_SetHighFrequencyMode(DAC1, LL_DAC_HIGH_FREQ_MODE_DISABLED); */ /* Configuration matches register reset state at startup. */

  /****************************************************************************/
  /* Configuration of DAC channel                                             */
  /****************************************************************************/

  /* ========== Channel 1 ========== */
  LL_DAC_EnableTrigger(DAC1, LL_DAC_CHANNEL_1);
  LL_DAC_SetTriggerSource(DAC1, LL_DAC_CHANNEL_1, LL_DAC_TRIGGER_TIM6_TRGO);
  /* LL_DAC_ConfigOutput(DAC1, LL_DAC_CHANNEL_1, LL_DAC_OUTPUT_MODE_NORMAL, LL_DAC_OUTPUT_BUFFER_ENABLE, LL_DAC_OUTPUT_CONNECT_EXTERNAL); */ /* Configuration matches register reset state at startup. */
  /* LL_DAC_SetSignedFormat(DAC1, LL_DAC_CHANNEL_1, LL_DAC_SIGN_FORMAT_UNSIGNED); */ /* Configuration matches register reset state at startup. */

  /* Enable DAC DMA request for channel 1 */
  LL_DAC_EnableDMAReq(DAC1, LL_DAC_CHANNEL_1);

  /****************************************************************************/
  /* Configuration of GPIO                                                    */
  /****************************************************************************/

  /* GPIO Clocks activation */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

  /**
    DAC1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PA4     ------>   DAC1_OUT1
    **/

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_4, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  /* LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_ANALOG); */ /* Configuration matches register reset state at startup. */

  /****************************************************************************/
  /* Configuration of DMA                                                     */
  /****************************************************************************/

  /* ========== Channel 1 ========== */

  /* ################################################## DAC1_CH1_CH1 DMA configuration */

  /* Enable LPDMA1 clock */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_LPDMA1);
/**********************************************************************************************************************/
  /* LL Optimization Context (DMA) */
  /**********************************************************************************************************************/

  /* Configure DMA channel control parameters */
  LL_DMA_ConfigControl(LPDMA1_CH0, LL_DMA_PRIORITY_HIGH | LL_DMA_LINKEDLIST_EXECUTION_Q);

  /* Configure DMA channel transfer event mode */
  LL_DMA_SetTransferEventMode(LPDMA1_CH0, LL_DMA_LINKEDLIST_XFER_EVENT_Q);

  /* Configure DMA channel block data length */
  /* LL_DMA_SetBlkDataLength(LPDMA1_CH0, 0U); */ /* Configuration matches register reset state at startup. */

  /* Configure DMA channel linked-list base address */
  LL_DMA_SetLinkedListBaseAddr(LPDMA1_CH0, (uint32_t)&DAC1_CH1_DMA_LLi_reg);

  /* Configure DMA channel registers update and node address offset during the link transfer */
  LL_DMA_ConfigLinkUpdate(LPDMA1_CH0, LL_DMA_UPDATE_ALL, (uint32_t)&DAC1_CH1_DMA_LLi_reg);

  /* CTR1 register : Data width and address increment configuration */
  DAC1_CH1_DMA_LLi_reg[LL_DMA_NODE_CTR1_REG_OFFSET] = LL_DMA_SRC_ADDR_INCREMENTED | LL_DMA_SRC_DATA_WIDTH_HALFWORD | \
                                                     LL_DMA_DEST_ADDR_FIXED | LL_DMA_DEST_DATA_WIDTH_WORD;

  /* CTR2 register : Request and direction configuration */
  DAC1_CH1_DMA_LLi_reg[LL_DMA_NODE_CTR2_REG_OFFSET] = LL_LPDMA1_REQUEST_DAC1_CH1 | LL_DMA_DIRECTION_MEMORY_TO_PERIPH;

  /* CLLR register */
  DAC1_CH1_DMA_LLi_reg[LL_DMA_NODE_CLLR_REG_OFFSET] = LL_DMA_UPDATE_ALL | ((uint32_t)&DAC1_CH1_DMA_LLi_reg & DMA_CLLR_LA);

  /* Enable the interruption for DMA */
  NVIC_SetPriority(LPDMA1_CH0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(LPDMA1_CH0_IRQn);

  return DAC1;
}

void mx_dac1_deinit(void)
{
  LL_DAC_Disable(DAC1, LL_DAC_CHANNEL_1);

  LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_DAC1);

  LL_AHB2_GRP1_ForceReset(LL_AHB2_GRP1_PERIPH_DAC1);
  LL_AHB2_GRP1_ReleaseReset(LL_AHB2_GRP1_PERIPH_DAC1);

  /* ### GPIO deinitialization of DAC1: DAC1_OUT1 ########################### */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_4, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_4, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_4, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_4, LL_GPIO_PULL_NO);

  /* Reset the IO output state */
  LL_GPIO_WriteOutputPin(GPIOA, LL_GPIO_PIN_4, LL_GPIO_PIN_RESET);

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

  uint32_t *mx_dac1_ch1_dma_get_lli_reg(void)
  {
    return DAC1_CH1_DMA_LLi_reg;
  }
