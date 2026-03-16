/**
  ******************************************************************************
  * @file           : mx_gpio_default.c
  * @brief          : gpio_default Peripheral initialization
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
#include "mx_gpio_default.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported variables by reference -------------------------------------------*/

/******************************************************************************/
/* Exported functions for GPIO in LL layer (SW instance MyGPIO_1) */
/******************************************************************************/
system_status_t mx_gpio_default_init(void)
{
  /* Enable peripherals and GPIO Clocks #################################*/
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA | LL_AHB2_GRP1_PERIPH_GPIOC);

  /* PA5   --------->  PA5 */
  /* PA5   --------->  MX_STATUS_LED */

  /* Initialize the output state */
  /* LL_GPIO_WriteOutputPin(GPIOA, LL_GPIO_PIN_5, LL_GPIO_PIN_RESET); */ /* Configuration matches register reset state at startup. */

  /* Configure IO output speed (Low, Medium, High or Very-High) */
  /* LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_LOW); */ /* Configuration matches register reset state at startup. */

  /* Configure IO output type (Push-Pull or Open-Drain) */
  /* LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_5, LL_GPIO_OUTPUT_PUSHPULL); */ /* Configuration matches register reset state at startup. */

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_5, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /* Configure IO direction mode (Input, Output or Analog) */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT);

  /* PC13  --------->  PC13 */

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_13, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /* Configure IO direction mode (Input, Output or Analog) */
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_INPUT);

  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_13);
  /* LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_13); */ /* Configuration matches register reset state at startup. */

  /* Compute the EXTI source register and configure the corresponding GPIO port */
  LL_EXTI_SetEXTISource(LL_EXTI_GPIO_PORTC, LL_EXTI_GPIO_LINE13);

  /* Set line 13 Interrupt priority */
  NVIC_SetPriority(EXTI13_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(EXTI13_IRQn);

  return SYSTEM_OK;
}

system_status_t mx_gpio_default_deinit(void)
{
  /* PA5   --------->  PA5 */
  /* PA5   --------->  MX_STATUS_LED */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_5, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_5, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_5, LL_GPIO_PULL_NO);

  /* Reset the current IO output state */
  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);

  /* PC13  --------->  PC13 */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(GPIOC, LL_GPIO_PIN_13, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_13, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_13, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_13, LL_GPIO_PULL_NO);

  /* Reset the current IO output state */
  LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13);

  LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_13);
  LL_EXTI_DisableEvent_0_31(LL_EXTI_LINE_13);
  LL_EXTI_DisableRisingTrig_0_31(LL_EXTI_LINE_13);
  LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_13);
  LL_EXTI_ClearRisingFlag_0_31(LL_EXTI_LINE_13);
  LL_EXTI_ClearFallingFlag_0_31(LL_EXTI_LINE_13);

  /* Reset the EXTI source */
  LL_EXTI_SetEXTISource(LL_EXTI_GPIO_PORTA, LL_EXTI_GPIO_LINE13);

  /* set line 13 Interrupt priority */
  NVIC_DisableIRQ(EXTI13_IRQn);

  return SYSTEM_OK;
}

/******************************************************************************/
/*          EXTI Line13 interrupt is managed directly in user code.           */
/******************************************************************************/
/* void EXTI13_IRQHandler(void)
{
}
  */
