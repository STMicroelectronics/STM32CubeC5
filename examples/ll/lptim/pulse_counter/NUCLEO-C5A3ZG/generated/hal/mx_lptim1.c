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
  LL_RCC_SetLPTIMClockSource(LL_RCC_LPTIM1_CLKSOURCE_LSI);

  LL_APB3_GRP1_EnableClock(LL_APB3_GRP1_PERIPH_LPTIM1);
  /*Configure the low power timer's time-base unit.*/
  LL_LPTIM_Enable(LPTIM1);
  LL_LPTIM_WRITE_REG(LPTIM1, ICR, LL_LPTIM_FLAG_ALL);
  LL_LPTIM_SetRepetition(LPTIM1, 0x0000);
  while (LL_LPTIM_IsActiveFlag_REPOK(LPTIM1) == 0);
  LL_LPTIM_ClearFlag_REPOK(LPTIM1);
  LL_LPTIM_SetAutoReload(LPTIM1, 0x3E8);
  while (LL_LPTIM_IsActiveFlag_ARROK(LPTIM1) == 0);
  LL_LPTIM_ClearFlag_ARROK(LPTIM1);
  LL_LPTIM_Disable(LPTIM1);
  /* LL_LPTIM_SetClockSource(LPTIM1, LL_LPTIM_CLK_SOURCE_INTERNAL); */ /* Configuration matches register reset state at startup. */
  LL_LPTIM_SetCounterMode(LPTIM1, LL_LPTIM_COUNTER_MODE_EXTERNAL);
  /* LL_LPTIM_SetPrescaler(LPTIM1, LL_LPTIM_PRESCALER_DIV1); */ /* Configuration matches register reset state at startup. */

  /*Configure the LPTIM Input1.*/
  /* LL_LPTIM_SetInput1Source(LPTIM1, LL_LPTIM_INPUT1_SRC_GPIO); */ /* Configuration matches register reset state at startup. */
  /* LL_LPTIM_ConfigClock(LPTIM1, LL_LPTIM_CLK_FILTER_NONE, LL_LPTIM_CLK_POLARITY_RISING); */ /* Configuration matches register reset state at startup. */

  /* Init GPIO */
  /* GPIO Clocks activation */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

  /**
    LPTIM1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PB10    ------>   LPTIM1_IN1
    **/

  /* Configure IO output speed (Low, Medium, High or Very-High) */
  /* LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_10, LL_GPIO_SPEED_FREQ_LOW); */ /* Configuration matches register reset state at startup. */

  /* Configure IO output type (Push-Pull or Open-Drain) */
  /* LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_10, LL_GPIO_OUTPUT_PUSHPULL); */ /* Configuration matches register reset state at startup. */

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_10, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /* Configure the Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_10, LL_GPIO_AF_3);

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_10, LL_GPIO_MODE_ALTERNATE);

  /* Enable the LPTIM global interruption */
  NVIC_SetPriority(LPTIM1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(LPTIM1_IRQn);

return LPTIM1;
}

void mx_lptim1_deinit(void)
{
  LL_APB3_GRP1_ForceReset(LL_APB3_GRP1_PERIPH_LPTIM1);
  LL_APB3_GRP1_ReleaseReset(LL_APB3_GRP1_PERIPH_LPTIM1);

  LL_APB3_GRP1_DisableClock(LL_APB3_GRP1_PERIPH_LPTIM1);
  /* deinit GPIO configuration */
  /* ### GPIO deinitialization of LPTIM1: LPTIM1_IN1 ########################### */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_10, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_10, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_10, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_10, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_10, LL_GPIO_PULL_NO);

  /* Reset the IO output state */
  LL_GPIO_WriteOutputPin(GPIOB, LL_GPIO_PIN_10, LL_GPIO_PIN_RESET);

  /* Disable the LPTIM global interruption */
  NVIC_DisableIRQ(LPTIM1_IRQn);
}

/******************************************************************************/
/*         LPTIM1 global interrupt is managed directly in user code.          */
/******************************************************************************/
/* void LPTIM1_IRQHandler(void)
{
}
  */
