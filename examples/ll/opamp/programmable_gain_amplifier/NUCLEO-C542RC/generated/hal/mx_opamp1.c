/**
  ******************************************************************************
  * @file           : mx_opamp1.c
  * @brief          : OPAMP1 Peripheral initialization
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
#include "mx_opamp1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/******************************************************************************/
/* Exported functions for OPAMP1 in LL layer (SW instance MyOPAMP_1) */
/******************************************************************************/
OPAMP_TypeDef *mx_opamp1_init(void){
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_OPAMP1);

  /****************************************************************************/
  /* Configuration of basic features (mandatory)                              */
  /****************************************************************************/

  LL_OPAMP_SetConfig(OPAMP1, (LL_OPAMP_MODE_FUNCTIONAL
                              | LL_OPAMP_SPEED_MODE_HIGH
                              | LL_OPAMP_MODE_PGA
                              | LL_OPAMP_INPUT_NONINVERT_IO0
                              | LL_OPAMP_INPUT_INVERT_INT_PGA
                              | LL_OPAMP_OUTPUT_CONNECT_EXTERNAL)
                    );
  LL_OPAMP_SetPGAGain(OPAMP1, LL_OPAMP_PGA_GAIN_4);
  /* LL_OPAMP_SetPGAExternalMode(OPAMP1, LL_OPAMP_PGA_EXT_NONE); */ /* Configuration matches register reset state at startup. */

  /* GPIO Clocks activation */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

  /**
    OPAMP1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PA1     ------>   OPAMP1_VINP0
       PA6     ------>   OPAMP1_VOUT
    **/

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_1, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */
  /* LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_6, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  /* LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_ANALOG); */ /* Configuration matches register reset state at startup. */
  /* LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_ANALOG); */ /* Configuration matches register reset state at startup. */

return OPAMP1;
}

void mx_opamp1_deinit(void)
{
  LL_OPAMP_Disable(OPAMP1);

  LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_OPAMP1);
  LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_OPAMP1);

  LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_OPAMP1);

  /* ### GPIO deinitialization of OPAMP1: OPAMP1_VINP0,OPAMP1_VOUT ########################### */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_ANALOG);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_1, LL_GPIO_AF_0);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_6, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_1, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_1 | LL_GPIO_PIN_6, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_1, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_6, LL_GPIO_PULL_NO);

  /* Reset the IO output state */
  LL_GPIO_WriteOutputPin(GPIOA, LL_GPIO_PIN_1 | LL_GPIO_PIN_6, LL_GPIO_PIN_RESET);
}
