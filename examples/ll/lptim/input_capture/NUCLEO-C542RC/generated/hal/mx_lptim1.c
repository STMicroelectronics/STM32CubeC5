/**
  ******************************************************************************
  * @file           : mx_lptim1.c
  * @brief          : LPTIM1 Peripheral initialization
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
#include "mx_lptim1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/

/* Exported variables by reference--------------------------------------------*/
/* Exported function definition ----------------------------------------------*/
/******************************************************************************/
/* Exported functions for SW instance in LL layer */
/******************************************************************************/
LPTIM_TypeDef *mx_lptim1_init(void)
{
  LL_RCC_SetLPTIMClockSource(LL_RCC_LPTIM1_CLKSOURCE_LSE);

  LL_APB3_GRP1_EnableClock(LL_APB3_GRP1_PERIPH_LPTIM1);
  /*Configure the low power timer's time-base unit.*/
  LL_LPTIM_Enable(LPTIM1);
  LL_LPTIM_WRITE_REG(LPTIM1, ICR, LL_LPTIM_FLAG_ALL);
  LL_LPTIM_SetRepetition(LPTIM1, 0x0000);
  while (LL_LPTIM_IsActiveFlag_REPOK(LPTIM1) == 0);
  LL_LPTIM_ClearFlag_REPOK(LPTIM1);
  LL_LPTIM_SetAutoReload(LPTIM1, 0xFFFF);
  while (LL_LPTIM_IsActiveFlag_ARROK(LPTIM1) == 0);
  LL_LPTIM_ClearFlag_ARROK(LPTIM1);
  LL_LPTIM_Disable(LPTIM1);
  /* LL_LPTIM_SetClockSource(LPTIM1, LL_LPTIM_CLK_SOURCE_INTERNAL); */ /* Configuration matches register reset state at startup. */
  /* LL_LPTIM_SetCounterMode(LPTIM1, LL_LPTIM_COUNTER_MODE_INTERNAL); */ /* Configuration matches register reset state at startup. */
  /* LL_LPTIM_SetPrescaler(LPTIM1, LL_LPTIM_PRESCALER_DIV1); */ /* Configuration matches register reset state at startup. */

  /*Configure the Input channel/Input Capture*/
  /* LL_LPTIM_SetRemap(LPTIM1, LL_LPTIM_LPTIM1_IC1_RMP_GPIO); */ /* Configuration matches register reset state at startup. */
  /* LL_LPTIM_IC_SetPolarity(LPTIM1, LL_LPTIM_CHANNEL_CH1, LL_LPTIM_ICPOLARITY_RISING); */ /* Configuration matches register reset state at startup. */
  /* LL_LPTIM_IC_SetFilter(LPTIM1, LL_LPTIM_CHANNEL_CH1, LL_LPTIM_ICFLT_CLOCK_DIV1); */ /* Configuration matches register reset state at startup. */
  /* LL_LPTIM_IC_SetPrescaler(LPTIM1, LL_LPTIM_CHANNEL_CH1, LL_LPTIM_ICPSC_DIV1); */ /* Configuration matches register reset state at startup. */
  LL_LPTIM_CC_SetChannelMode(LPTIM1, LL_LPTIM_CHANNEL_CH1, LL_LPTIM_CCMODE_INPUTCAPTURE);

  /* Init GPIO */
  /* GPIO Clocks activation */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

  /**
    LPTIM1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PB13    ------>   LPTIM1_CH1
    **/

  /* Configure IO output speed (Low, Medium, High or Very-High) */
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_13, LL_GPIO_SPEED_FREQ_MEDIUM);

  /* Configure IO output type (Push-Pull or Open-Drain) */
  /* LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_13, LL_GPIO_OUTPUT_PUSHPULL); */ /* Configuration matches register reset state at startup. */

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_13, LL_GPIO_PULL_UP);

  /* Configure the Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_13, LL_GPIO_AF_3);

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_13, LL_GPIO_MODE_ALTERNATE);

  /* ################################################## LPTIM1_IC1 DMA configuration */

  /* Enable LPDMA1 clock */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_LPDMA1);
/**********************************************************************************************************************/
  /* LL Optimization Context (DMA) */
  /**********************************************************************************************************************/

  /* Configure DMA channel transfer request */
  LL_DMA_SetPeriphRequest(LPDMA1_CH0, LL_LPDMA1_REQUEST_LPTIM1_IC1);

  /* Configure DMA channel transfer direction */
  /* LL_DMA_SetDataTransferDirection(LPDMA1_CH0, LL_DMA_DIRECTION_PERIPH_TO_MEMORY); */ /* Configuration matches register reset state at startup. */

  /* Configure DMA channel priority level */
  LL_DMA_SetChannelPriorityLevel(LPDMA1_CH0, LL_DMA_PRIORITY_HIGH);

  /* Configure DMA channel transfer register 1 */
  LL_DMA_ConfigTransfer(LPDMA1_CH0,
                        (LL_DMA_SRC_ADDR_FIXED | LL_DMA_SRC_DATA_WIDTH_HALFWORD | \
                         LL_DMA_DEST_ADDR_INCREMENTED | LL_DMA_DEST_DATA_WIDTH_HALFWORD));

  /* Enable the interruption for DMA */
  NVIC_SetPriority(LPDMA1_CH0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(LPDMA1_CH0_IRQn);
return LPTIM1;
}

void mx_lptim1_deinit(void)
{
  LL_APB3_GRP1_ForceReset(LL_APB3_GRP1_PERIPH_LPTIM1);
  LL_APB3_GRP1_ReleaseReset(LL_APB3_GRP1_PERIPH_LPTIM1);

  LL_APB3_GRP1_DisableClock(LL_APB3_GRP1_PERIPH_LPTIM1);
  /* deinit GPIO configuration */
  /* ### GPIO deinitialization of LPTIM1: LPTIM1_CH1 ########################### */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_13, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_13, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_13, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_13, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_13, LL_GPIO_PULL_NO);

  /* Reset the IO output state */
  LL_GPIO_WriteOutputPin(GPIOB, LL_GPIO_PIN_13, LL_GPIO_PIN_RESET);

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

/******************************************************************************/
/*     LPDMA1 channel0 global interrupt is managed directly in user code.     */
/******************************************************************************/
/* void LPDMA1_CH0_IRQHandler(void)
{
}
  */
