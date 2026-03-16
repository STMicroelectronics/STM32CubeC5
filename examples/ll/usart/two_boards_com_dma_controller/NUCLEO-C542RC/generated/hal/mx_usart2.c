/**
  ******************************************************************************
  * @file           : mx_usart2.c
  * @brief          : USART2 Peripheral initialization
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
#include "mx_usart2.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
/* Exported function definition ----------------------------------------------*/
/******************************************************************************/
/* Exported functions for USART in LL layer */
/******************************************************************************/

USART_TypeDef *mx_usart2_usart_init(void)
{
  /* Basic configuration */

  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
  /* LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1); */ /* Configuration matches register reset state at startup. */

  LL_USART_ConfigSyncMasterMode(USART2);

  LL_USART_ConfigXfer(USART2, LL_USART_DATAWIDTH_8_BIT | LL_USART_PARITY_NONE
                              | LL_USART_DIRECTION_TX_RX | LL_USART_OVERSAMPLING_8,
                              LL_USART_STOP_BIT_1 | LL_USART_CLOCK_POLARITY_LOW
                              | LL_USART_CLOCK_PHASE_1_EDGE | LL_USART_LASTCLKPULSE_ENABLED);

  /* LL_USART_SetPrescaler(USART2, LL_USART_PRESCALER_DIV1); */ /* Configuration matches register reset state at startup. */

  LL_USART_SetBaudRate(USART2, 144000000, LL_USART_PRESCALER_DIV1, LL_USART_OVERSAMPLING_8,
                       115200);
  LL_USART_Enable(USART2);
  /* GPIO Clocks activation */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);

  /**
    USART2 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PC12    ------>   USART2_CK
       PC11    ------>   USART2_RX
       PC10    ------>   USART2_TX
    **/

  /* Configure IO output speed (Low, Medium, High or Very-High) */
  /* LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_12, LL_GPIO_SPEED_FREQ_LOW); */ /* Configuration matches register reset state at startup. */
  /* LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_11, LL_GPIO_SPEED_FREQ_LOW); */ /* Configuration matches register reset state at startup. */
  /* LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_10, LL_GPIO_SPEED_FREQ_LOW); */ /* Configuration matches register reset state at startup. */

  /* Configure IO output type (Push-Pull or Open-Drain) */
  /* LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_12 | LL_GPIO_PIN_11 | LL_GPIO_PIN_10, LL_GPIO_OUTPUT_PUSHPULL); */ /* Configuration matches register reset state at startup. */

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_12, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */
  /* LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_11, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */
  /* LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_10, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /* Configure the Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(GPIOC, LL_GPIO_PIN_12, LL_GPIO_AF_7);
  LL_GPIO_SetAFPin_8_15(GPIOC, LL_GPIO_PIN_11, LL_GPIO_AF_7);
  LL_GPIO_SetAFPin_8_15(GPIOC, LL_GPIO_PIN_10, LL_GPIO_AF_7);

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_12, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_11, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_10, LL_GPIO_MODE_ALTERNATE);

  /* Enable interrupt */
  NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(USART2_IRQn);

  /* ################################################## USART2_TX DMA configuration */

  /* Enable LPDMA1 clock */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_LPDMA1);
/**********************************************************************************************************************/
  /* LL Optimization Context (DMA) */
  /**********************************************************************************************************************/

  /* Configure DMA channel transfer request */
  LL_DMA_SetPeriphRequest(LPDMA1_CH0, LL_LPDMA1_REQUEST_USART2_TX);

  /* Configure DMA channel transfer direction */
  LL_DMA_SetDataTransferDirection(LPDMA1_CH0, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  /* Configure DMA channel priority level */
  /* LL_DMA_SetChannelPriorityLevel(LPDMA1_CH0, LL_DMA_PRIORITY_LOW_WEIGHT_LOW); */ /* Configuration matches register reset state at startup. */

  /* Configure DMA channel transfer register 1 */
  LL_DMA_ConfigTransfer(LPDMA1_CH0,
                        (LL_DMA_SRC_ADDR_INCREMENTED | LL_DMA_SRC_DATA_WIDTH_BYTE | \
                         LL_DMA_DEST_ADDR_FIXED | LL_DMA_DEST_DATA_WIDTH_BYTE));

  /* Enable the interruption for DMA */
  NVIC_SetPriority(LPDMA1_CH0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(LPDMA1_CH0_IRQn);

  /* ################################################## USART2_RX DMA configuration */

  /* Enable LPDMA1 clock */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_LPDMA1);
/**********************************************************************************************************************/
  /* LL Optimization Context (DMA) */
  /**********************************************************************************************************************/

  /* Configure DMA channel transfer request */
  LL_DMA_SetPeriphRequest(LPDMA1_CH1, LL_LPDMA1_REQUEST_USART2_RX);

  /* Configure DMA channel transfer direction */
  /* LL_DMA_SetDataTransferDirection(LPDMA1_CH1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY); */ /* Configuration matches register reset state at startup. */

  /* Configure DMA channel priority level */
  /* LL_DMA_SetChannelPriorityLevel(LPDMA1_CH1, LL_DMA_PRIORITY_LOW_WEIGHT_LOW); */ /* Configuration matches register reset state at startup. */

  /* Configure DMA channel transfer register 1 */
  LL_DMA_ConfigTransfer(LPDMA1_CH1,
                        (LL_DMA_SRC_ADDR_FIXED | LL_DMA_SRC_DATA_WIDTH_BYTE | \
                         LL_DMA_DEST_ADDR_INCREMENTED | LL_DMA_DEST_DATA_WIDTH_BYTE));

  /* Enable the interruption for DMA */
  NVIC_SetPriority(LPDMA1_CH1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(LPDMA1_CH1_IRQn);
return USART2;
}

void mx_usart2_usart_deinit(void)
{
  LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_USART2);
  LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_USART2);

  LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_USART2);
  /* ### GPIO deinitialization of USART2: USART2_CK,USART2_RX,USART2_TX ########################### */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_12, LL_GPIO_MODE_ANALOG);
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_11, LL_GPIO_MODE_ANALOG);
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_10, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(GPIOC, LL_GPIO_PIN_12, LL_GPIO_AF_0);
  LL_GPIO_SetAFPin_8_15(GPIOC, LL_GPIO_PIN_11, LL_GPIO_AF_0);
  LL_GPIO_SetAFPin_8_15(GPIOC, LL_GPIO_PIN_10, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_12, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_11, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_10, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_12 | LL_GPIO_PIN_11 | LL_GPIO_PIN_10, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_12, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_11, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_10, LL_GPIO_PULL_NO);

  /* Reset the IO output state */
  LL_GPIO_WriteOutputPin(GPIOC, LL_GPIO_PIN_12 | LL_GPIO_PIN_11 | LL_GPIO_PIN_10, LL_GPIO_PIN_RESET);

  /* Disable interrupt */
  NVIC_DisableIRQ(LPDMA1_CH1_IRQn);
LL_USART_Disable(USART2);

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

  /* De-initialize the DMA channel */
  LL_DMA_DisableChannel(LPDMA1_CH1);
  LL_DMA_ClearFlag(LPDMA1_CH1, LL_DMA_FLAG_ALL);

  LL_DMA_WRITE_REG(LPDMA1_CH1, CCR, 0U);
  LL_DMA_WRITE_REG(LPDMA1_CH1, CTR1, 0U);
  LL_DMA_WRITE_REG(LPDMA1_CH1, CTR2, 0U);
  LL_DMA_WRITE_REG(LPDMA1_CH1, CBR1, 0U);
  LL_DMA_WRITE_REG(LPDMA1_CH1, CLLR, 0U);

  /* Disable the interruption for DMA */
  NVIC_DisableIRQ(LPDMA1_CH1_IRQn);
}

/******************************************************************************/
/*         USART2 global interrupt is managed directly in user code.          */
/******************************************************************************/
/* void USART2_IRQHandler(void)
{
}
  */

/******************************************************************************/
/*     LPDMA1 channel0 global interrupt is managed directly in user code.     */
/******************************************************************************/
/* void LPDMA1_CH0_IRQHandler(void)
{
}
  */

/******************************************************************************/
/*     LPDMA1 channel1 global interrupt is managed directly in user code.     */
/******************************************************************************/
/* void LPDMA1_CH1_IRQHandler(void)
{
}
  */
