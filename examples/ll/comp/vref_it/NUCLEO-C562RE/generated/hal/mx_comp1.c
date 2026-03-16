/**
  ******************************************************************************
  * @file           : mx_comp1.c
  * @brief          : COMP1 Peripheral initialization
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
#include "mx_comp1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/******************************************************************************/
/* Exported functions for COMP1 in LL layer (SW instance MyCOMP_1) */
/******************************************************************************/
COMP_TypeDef *mx_comp1_init(void)
{
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_COMP12);

  LL_COMP_SetPowerMode(COMP1, LL_COMP_POWER_MODE_HIGH_SPEED);
  LL_COMP_ConfigInputs(COMP1, LL_COMP_INPUT_MINUS_VREFINT, LL_COMP_INPUT_PLUS_IO2);
  /* LL_COMP_SetInputHysteresis(COMP1, LL_COMP_HYSTERESIS_NONE); */ /* Configuration matches register reset state at startup. */
  /* LL_COMP_SetOutputPolarity(COMP1, LL_COMP_OUTPUTPOL_NONINVERTED); */ /* Configuration matches register reset state at startup. */

  /* GPIO Clocks activation */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

  /**
    COMP1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PB0     ------>   COMP1_INP2
    **/

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_0, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  /* LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG); */ /* Configuration matches register reset state at startup. */

  /* Configuration of EXTI line used by comparator to generate an interrupt */
  LL_EXTI_EnableRisingTrig_32_63(LL_EXTI_LINE_34);
  LL_EXTI_EnableFallingTrig_32_63(LL_EXTI_LINE_34);

  /* Enable comparator interruption through EXTI */
  NVIC_SetPriority(COMP1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(COMP1_IRQn);

  return COMP1;
}

void mx_comp1_deinit(void)
{
  LL_COMP_Disable(COMP1);

  LL_APB1_GRP2_DisableClock(LL_APB1_GRP2_PERIPH_COMP12);

  LL_APB1_GRP2_ForceReset(LL_APB1_GRP2_PERIPH_COMP12);
  LL_APB1_GRP2_ReleaseReset(LL_APB1_GRP2_PERIPH_COMP12);

  /* ### GPIO deinitialization of COMP1: COMP1_INP2 ########################### */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_0, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_0, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_0, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_0, LL_GPIO_PULL_NO);

  /* Reset the IO output state */
  LL_GPIO_WriteOutputPin(GPIOB, LL_GPIO_PIN_0, LL_GPIO_PIN_RESET);

  NVIC_DisableIRQ(COMP1_IRQn);
}
