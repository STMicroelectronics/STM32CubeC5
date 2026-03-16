/**
  ******************************************************************************
  * @file           : mx_lpuart1.c
  * @brief          : LPUART1 Peripheral initialization
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
#include "mx_lpuart1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/* Exported function definition ----------------------------------------------*/

/******************************************************************************/
/* Exported functions for UART in LL layer */
/******************************************************************************/

USART_TypeDef *mx_lpuart1_uart_init(void)
{
  uint32_t reg_temp;

  /* Basic configuration */

  LL_APB3_GRP1_EnableClock(LL_APB3_GRP1_PERIPH_LPUART1);
  LL_RCC_SetLPUARTClockSource(LL_RCC_LPUART1_CLKSOURCE_HSIK);

  LL_USART_ConfigAsyncMode(LPUART1);

  reg_temp = (LL_USART_DATAWIDTH_8_BIT | LL_USART_PARITY_NONE
              | LL_USART_DIRECTION_TX_RX | LL_USART_OVERSAMPLING_16);
  LL_LPUART_ConfigXfer(LPUART1, reg_temp, LL_USART_STOP_BIT_1);

  /* LL_USART_SetHWFlowCtrl(LPUART1, LL_USART_HWCONTROL_NONE); */ /* Configuration matches register reset state at startup. */

  /* LL_USART_SetPrescaler(LPUART1, LL_USART_PRESCALER_DIV1); */ /* Configuration matches register reset state at startup. */

  LL_LPUART_SetBaudRate(LPUART1, 144000000, LL_USART_PRESCALER_DIV1, 115200);
  /* Wake up from low power mode configuration */
  LL_USART_EnableInStopMode(LPUART1);

  /* GPIO Clocks activation */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

  /**
    LPUART1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PB7     ------>   LPUART1_RX
       PB6     ------>   LPUART1_TX
    **/

  /* Configure IO output speed (Low, Medium, High or Very-High) */
  /* LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_LOW); */ /* Configuration matches register reset state at startup. */
  /* LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_LOW); */ /* Configuration matches register reset state at startup. */

  /* Configure IO output type (Push-Pull or Open-Drain) */
  /* LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_7 | LL_GPIO_PIN_6, LL_GPIO_OUTPUT_PUSHPULL); */ /* Configuration matches register reset state at startup. */

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_7, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */
  /* LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_6, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /* Configure the Alternate Function in current IO */
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_7, LL_GPIO_AF_8);
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_6, LL_GPIO_AF_8);

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_ALTERNATE);

  /* Enable interrupt */
  NVIC_SetPriority(LPUART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(LPUART1_IRQn);

  LL_USART_Enable(LPUART1);
  return LPUART1;
}

void mx_lpuart1_uart_deinit(void)
{
  LL_APB3_GRP1_ForceReset(LL_APB3_GRP1_PERIPH_LPUART1);
  LL_APB3_GRP1_ReleaseReset(LL_APB3_GRP1_PERIPH_LPUART1);

  LL_APB3_GRP1_DisableClock(LL_APB3_GRP1_PERIPH_LPUART1);

  /* Disable interrupt */
  NVIC_DisableIRQ(LPUART1_IRQn);
  LL_USART_Disable(LPUART1);

  /* ### GPIO deinitialization of LPUART1: LPUART1_RX,LPUART1_TX ########################### */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_ANALOG);
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_7, LL_GPIO_AF_0);
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_6, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_7 | LL_GPIO_PIN_6, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_7, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_6, LL_GPIO_PULL_NO);

  /* Reset the IO output state */
  LL_GPIO_WriteOutputPin(GPIOB, LL_GPIO_PIN_7 | LL_GPIO_PIN_6, LL_GPIO_PIN_RESET);
}

/******************************************************************************/
/*                LPUART1 Rx interrupt OR LPUART1 Tx interrupt                */
/*                     is managed directly in user code.                      */
/******************************************************************************/
/* void LPUART1_IRQHandler(void)
{
}
  */
