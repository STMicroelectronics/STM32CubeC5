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

/******************************************************************************/
/* Exported functions for DAC1 in LL layer (SW instance MyDAC_1) */
/******************************************************************************/
DAC_TypeDef *mx_dac1_init(void)
{
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_DAC1);

  LL_RCC_SetADCDACClockSource(LL_RCC_ADCDAC_CLKSOURCE_PSIS);
  LL_RCC_SetDACSHClockSource(LL_RCC_DAC1SH_CLKSOURCE_LSI);

  /****************************************************************************/
  /* Initialization of DAC instance                                           */
  /****************************************************************************/
  /* LL_DAC_SetHighFrequencyMode(DAC1, LL_DAC_HIGH_FREQ_MODE_DISABLED); */ /* Configuration matches register reset state at startup. */

  /****************************************************************************/
  /* Configuration of DAC channel                                             */
  /****************************************************************************/

  /* ========== Channel 1 ========== */
  /* LL_DAC_DisableTrigger(DAC1, LL_DAC_CHANNEL_1); */ /* Configuration matches register reset state at startup. */
  /* LL_DAC_SetTriggerSource(DAC1, LL_DAC_CHANNEL_1, LL_DAC_TRIGGER_SOFTWARE); */ /* Configuration matches register reset state at startup. */
  LL_DAC_ConfigOutput(DAC1, LL_DAC_CHANNEL_1, LL_DAC_OUTPUT_MODE_SAMPLE_AND_HOLD, LL_DAC_OUTPUT_BUFFER_DISABLE, LL_DAC_OUTPUT_CONNECT_EXTERNAL);
  /* LL_DAC_SetSignedFormat(DAC1, LL_DAC_CHANNEL_1, LL_DAC_SIGN_FORMAT_UNSIGNED); */ /* Configuration matches register reset state at startup. */

  /****************************************************************************/
  /* Configuration of DAC channel additional features (optional)              */
  /****************************************************************************/

  /* ========== Channel 1 ========== */
  /* Sample and Hold configuration */
  /* Note: Sample and hold mode activated with function "LL_DAC_ConfigOutput()" above */
  LL_DAC_SetSampleAndHoldSampleTime(DAC1, LL_DAC_CHANNEL_1, 379U);
  LL_DAC_SetSampleAndHoldHoldTime(DAC1, LL_DAC_CHANNEL_1, 938U);
  LL_DAC_SetSampleAndHoldRefreshTime(DAC1, LL_DAC_CHANNEL_1, 87U);

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
}
