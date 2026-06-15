/**
  ******************************************************************************
  * @file           : mx_spi1.c
  * @brief          : SPI1 Peripheral initialization
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
#include "mx_spi1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
/******************************************************************************/
/* Exported functions for I2S in LL layer */
/******************************************************************************/
SPI_TypeDef *mx_spi1_i2s_init(void)
{
  /* Basic configuration */

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
  /* LL_RCC_SetSPIClockSource(LL_RCC_SPI1_CLKSOURCE_PCLK2); */ /* Configuration matches register reset state at startup. */

  LL_I2S_EnableI2SMode(SPI1);
  LL_I2S_SetTransferMode(SPI1, LL_I2S_MODE_MASTER_TX);
  LL_I2S_SetStandard(SPI1, LL_I2S_STANDARD_PCM_SHORT);
  /* LL_I2S_SetDataFormat(SPI1, LL_I2S_DATA_FORMAT_16_BIT); */ /* Configuration matches register reset state at startup. */
  /* LL_I2S_SetClockPolarity(SPI1, LL_I2S_CLOCK_POLARITY_LOW); */ /* Configuration matches register reset state at startup. */
  /* LL_I2S_SetTransferBitOrder(SPI1, LL_I2S_MSB_FIRST); */ /* Configuration matches register reset state at startup. */
  LL_I2S_SetPrescalerLinear(SPI1, 47);
  /* LL_I2S_SetPrescalerParity(SPI1, LL_I2S_PRESCALER_PARITY_EVEN); */ /* Configuration matches register reset state at startup. */

  /* LL_I2S_SetFIFOThreshold(SPI1, LL_I2S_FIFO_THRESHOLD_1_DATA); */ /* Configuration matches register reset state at startup. */

  /* ### SPI1 GPIO Configuration ########################### */

  /* GPIO Clocks activation */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

  /**
    [GPIO Pin] ------> [Signal Name] ------> [Labels]

       PA5     ------>   I2S1_CK   ------>  PA5
       PA7     ------>   I2S1_SDO   ------>  PA7
    **/

  /* Configure IO output speed (Low, Medium, High or Very-High) */
  /* LL_GPIO_SetPinSpeed(PA5_PORT, PA5_PIN, LL_GPIO_SPEED_FREQ_LOW); */ /* Configuration matches register reset state at startup. */
  /* LL_GPIO_SetPinSpeed(PA7_PORT, PA7_PIN, LL_GPIO_SPEED_FREQ_LOW); */ /* Configuration matches register reset state at startup. */

  /* Configure IO output type (Push-Pull or Open-Drain) */
  /* LL_GPIO_SetPinOutputType(GPIOA, PA5_PIN | PA7_PIN, LL_GPIO_OUTPUT_PUSHPULL); */ /* Configuration matches register reset state at startup. */

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(PA5_PORT, PA5_PIN, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */
  /* LL_GPIO_SetPinPull(PA7_PORT, PA7_PIN, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /* Configure the Alternate Function in current IO */
  LL_GPIO_SetAFPin_0_7(PA5_PORT, PA5_PIN, LL_GPIO_AF_5);
  LL_GPIO_SetAFPin_0_7(PA7_PORT, PA7_PIN, LL_GPIO_AF_5);

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  LL_GPIO_SetPinMode(PA5_PORT, PA5_PIN, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinMode(PA7_PORT, PA7_PIN, LL_GPIO_MODE_ALTERNATE);

  /**
    [GPIO Pin] ------> [Signal Name] ------> [Labels]

       PA15    ------>   I2S1_WS   ------>  NETR16_2
    **/

  /* Configure IO output speed (Low, Medium, High or Very-High) */
  /* LL_GPIO_SetPinSpeed(NETR16_2_PORT, NETR16_2_PIN, LL_GPIO_SPEED_FREQ_LOW); */ /* Configuration matches register reset state at startup. */

  /* Configure IO output type (Push-Pull or Open-Drain) */
  /* LL_GPIO_SetPinOutputType(NETR16_2_PORT, NETR16_2_PIN, LL_GPIO_OUTPUT_PUSHPULL); */ /* Configuration matches register reset state at startup. */

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(NETR16_2_PORT, NETR16_2_PIN, LL_GPIO_PULL_UP); */ /* Configuration matches register reset state at startup. */

  /* Configure the Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(NETR16_2_PORT, NETR16_2_PIN, LL_GPIO_AF_5);

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  /* LL_GPIO_SetPinMode(NETR16_2_PORT, NETR16_2_PIN, LL_GPIO_MODE_ALTERNATE); */ /* Configuration matches register reset state at startup. */

  /* Configure the DMA TX */

  /* ################################################## SPI1_TX DMA configuration */

  /* Enable LPDMA1 clock */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_LPDMA1);

  /* Configure DMA channel transfer request */
  LL_DMA_SetPeriphRequest(LPDMA1_CH0, LL_LPDMA1_REQUEST_SPI1_TX);

  /* Configure DMA channel transfer direction */
  LL_DMA_SetDataTransferDirection(LPDMA1_CH0, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  /* Configure DMA channel priority level */
  /* LL_DMA_SetChannelPriorityLevel(LPDMA1_CH0, LL_DMA_PRIORITY_LOW_WEIGHT_LOW); */ /* Configuration matches register reset state at startup. */

  /* Configure DMA channel transfer register 1 */
  LL_DMA_ConfigTransfer(LPDMA1_CH0,
                        (LL_DMA_SRC_ADDR_INCREMENTED | LL_DMA_SRC_DATA_WIDTH_HALFWORD | \
                         LL_DMA_DEST_ADDR_FIXED | LL_DMA_DEST_DATA_WIDTH_HALFWORD));

  /* Enable the interruption for DMA */
  NVIC_SetPriority(LPDMA1_CH0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(LPDMA1_CH0_IRQn);
  return SPI1;
}

void mx_spi1_i2s_deinit(void)
{
  LL_I2S_Disable(SPI1);

  /* ### GPIO deinitialization of SPI1: I2S1_CK,I2S1_SDO ########################### */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(PA5_PORT, PA5_PIN, LL_GPIO_MODE_ANALOG);
  LL_GPIO_SetPinMode(PA7_PORT, PA7_PIN, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_0_7(PA5_PORT, PA5_PIN, LL_GPIO_AF_0);
  LL_GPIO_SetAFPin_0_7(PA7_PORT, PA7_PIN, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(PA5_PORT, PA5_PIN, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinSpeed(PA7_PORT, PA7_PIN, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(GPIOA, PA5_PIN | PA7_PIN, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(PA5_PORT, PA5_PIN, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinPull(PA7_PORT, PA7_PIN, LL_GPIO_PULL_NO);

  /* Reset the IO output state */
  LL_GPIO_WriteOutputPin(PA5_PORT, PA5_PIN | PA7_PIN, LL_GPIO_PIN_RESET);

  /* ### GPIO deinitialization of SPI1: I2S1_WS ########################### */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(NETR16_2_PORT, NETR16_2_PIN, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(NETR16_2_PORT, NETR16_2_PIN, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(NETR16_2_PORT, NETR16_2_PIN, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(NETR16_2_PORT, NETR16_2_PIN, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(NETR16_2_PORT, NETR16_2_PIN, LL_GPIO_PULL_NO);

  /* Reset the IO output state */
  LL_GPIO_WriteOutputPin(NETR16_2_PORT, NETR16_2_PIN, LL_GPIO_PIN_RESET);

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
