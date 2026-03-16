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

/******************************************************************************/
/* Exported functions for ADC1 in LL layer (SW instance MyADC_1) */
/******************************************************************************/
ADC_TypeDef *mx_adc1_init(void)
{
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC12);
  LL_RCC_SetADCDACClockSource(LL_RCC_ADCDAC_CLKSOURCE_PSIS);

  /****************************************************************************/
  /* Configuration of basic features (mandatory)                              */
  /****************************************************************************/

  /* ==================== ADC common instance ==================== */
  LL_ADC_SetCommonPathInternalCh(ADC_COMMON_INSTANCE(ADC1),
                                 (LL_ADC_CHANNEL_TEMPSENSOR | LL_ADC_CHANNEL_VREFINT));

  /* ==================== ADC instance ==================== */
  /* LL_ADC_SetResolution(ADC1, LL_ADC_RESOLUTION_12B); */ /* Configuration matches register reset state at startup. */
  /* LL_ADC_SetSamplingMode(ADC1, LL_ADC_SAMPLING_MODE_NORMAL); */ /* Configuration matches register reset state at startup. */

  /* ==================== Group Regular ==================== */
  /* LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE); */ /* Configuration matches register reset state at startup. */
  /* LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE); */ /* Configuration matches register reset state at startup. */

  /* LL_ADC_REG_SetSequencerDiscont(ADC1, LL_ADC_REG_SEQ_DISCONT_DISABLE); */ /* Configuration matches register reset state at startup. */
  LL_ADC_REG_SetOverrun(ADC1, LL_ADC_REG_OVR_DATA_OVERWRITTEN);
  LL_ADC_REG_SetSequencerLength(ADC1,LL_ADC_REG_SEQ_SCAN_ENABLE_3RANKS);

  /* Configure the ADC regular group sequencer */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_4);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_4, LL_ADC_SAMPLINGTIME_289CYCLES);
  LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_4, LL_ADC_IN_SINGLE_ENDED);
  LL_ADC_SetChannelPreselection(ADC1, LL_ADC_CHANNEL_4);

  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_2, LL_ADC_CHANNEL_VREFINT);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_VREFINT, LL_ADC_SAMPLINGTIME_289CYCLES);
  LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_VREFINT, LL_ADC_IN_SINGLE_ENDED);
  LL_ADC_SetCommonPathInternalChAdd(ADC_COMMON_INSTANCE(ADC1), LL_ADC_CHANNEL_VREFINT);

  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_3, LL_ADC_CHANNEL_TEMPSENSOR);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_TEMPSENSOR, LL_ADC_SAMPLINGTIME_289CYCLES);
  LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_TEMPSENSOR, LL_ADC_IN_SINGLE_ENDED);
  LL_ADC_SetCommonPathInternalChAdd(ADC_COMMON_INSTANCE(ADC1), LL_ADC_CHANNEL_TEMPSENSOR);

  /* GPIO Clocks activation */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

  /**
    ADC1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PA4     ------>   ADC1_IN4
    **/

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_4, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  /* LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_ANALOG); */ /* Configuration matches register reset state at startup. */

  /* Enable the interruption for ADC */
  NVIC_SetPriority(ADC1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(ADC1_IRQn);
  /* Configure DMA */

  /* ################################################## ADC1_GLOBAL DMA configuration */

  /* Enable LPDMA1 clock */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_LPDMA1);
/**********************************************************************************************************************/
  /* LL Optimization Context (DMA) */
  /**********************************************************************************************************************/

  /* Configure DMA channel transfer request */
  LL_DMA_SetPeriphRequest(LPDMA1_CH0, LL_LPDMA1_REQUEST_ADC1);

  /* Configure DMA channel transfer direction */
  /* LL_DMA_SetDataTransferDirection(LPDMA1_CH0, LL_DMA_DIRECTION_PERIPH_TO_MEMORY); */ /* Configuration matches register reset state at startup. */

  /* Configure DMA channel priority level */
  /* LL_DMA_SetChannelPriorityLevel(LPDMA1_CH0, LL_DMA_PRIORITY_LOW_WEIGHT_LOW); */ /* Configuration matches register reset state at startup. */

  /* Configure DMA channel transfer register 1 */
  LL_DMA_ConfigTransfer(LPDMA1_CH0,
                        (LL_DMA_SRC_ADDR_FIXED | LL_DMA_SRC_DATA_WIDTH_HALFWORD | \
                         LL_DMA_DEST_ADDR_INCREMENTED | LL_DMA_DEST_DATA_WIDTH_HALFWORD));

  /* Enable the interruption for DMA */
  NVIC_SetPriority(LPDMA1_CH0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(LPDMA1_CH0_IRQn);

return ADC1;
}

void mx_adc1_deinit(void)
{
  LL_ADC_Disable(ADC1);

  NVIC_DisableIRQ(ADC1_IRQn);
  /* ### GPIO deinitialization of ADC1: ADC1_IN4 ########################### */

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

  /* Configure DMA */
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
