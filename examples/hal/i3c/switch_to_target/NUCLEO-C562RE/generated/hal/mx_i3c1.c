/**
  ******************************************************************************
  * @file           : mx_i3c1.c
  * @brief          : I3C1 Peripheral initialization
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
#include "mx_i3c1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_i3c_handle_t hI3C1;

/******************************************************************************/
/* Exported functions for I3C in HAL layer (SW instance MyI3C_1) */
/******************************************************************************/
hal_i3c_handle_t *mx_i3c1_init(void)
{
  if (HAL_I3C_Init(&hI3C1, HAL_I3C1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_I3C1_EnableClock();

  if (HAL_RCC_I3C1_SetKernelClkSource(HAL_RCC_I3C1_CLK_SRC_PCLK1) != HAL_OK)
  {
    return NULL;
  }

  /**
    * I3C1 timing_reg0 calculated by CubeMX2 with:
    * - SDA rise time = 350 ns
    * - Input frequency = 144 MHz
    * - Bus usage = UTILS_I3C_PURE_I3C_BUS
    * - I3C bus frequency = 12.5 MHz
    * - I3C duty cycle = 50 %
    * I3C1 timing_reg1 calculated by CubeMX2 with:
    * - Wait time = LL_I3C_OWN_ACTIVITY_STATE_0
    */
  hal_i3c_ctrl_config_t i3c_ctrl_config;
  i3c_ctrl_config.timing_reg0 = 0x330505UL;
  i3c_ctrl_config.timing_reg1 = 0x1D008EUL;
  if (HAL_I3C_CTRL_SetConfig(&hI3C1, &i3c_ctrl_config) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_GPIOB_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    I3C1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PB6     ------>   I3C1_SCL
       PB7     ------>   I3C1_SDA
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_HIGH;
  gpio_config.alternate   = HAL_GPIO_AF_3;
  HAL_GPIO_Init(HAL_GPIOB, HAL_GPIO_PIN_6 | HAL_GPIO_PIN_7, &gpio_config);

  /* Enable the Event interrupt for I3C1 */
  HAL_CORTEX_NVIC_SetPriority(I3C1_EV_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(I3C1_EV_IRQn);

  /* Enable the Error interrupt for I3C1 */
  HAL_CORTEX_NVIC_SetPriority(I3C1_ERR_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(I3C1_ERR_IRQn);

  return &hI3C1;
}

void mx_i3c1_deinit(void)
{
  /* Disable the Event interrupt for I3C1 */
  HAL_CORTEX_NVIC_DisableIRQ(I3C1_EV_IRQn);

  /* Disable the Error interrupt for I3C1 */
  HAL_CORTEX_NVIC_DisableIRQ(I3C1_ERR_IRQn);

  (void)HAL_I3C_DeInit(&hI3C1);

  HAL_RCC_I3C1_Reset();

  HAL_RCC_I3C1_DisableClock();

  /* De-initialize all GPIO pins associated with I3C1 */
  HAL_GPIO_DeInit(HAL_GPIOB, HAL_GPIO_PIN_6 | HAL_GPIO_PIN_7);
}

hal_i3c_handle_t *mx_i3c1_gethandle(void)
{
  return &hI3C1;
}

/******************************************************************************/
/*                            I3C1 event interrupt                            */
/******************************************************************************/
void I3C1_EV_IRQHandler(void)
{
  HAL_I3C_EV_IRQHandler(&hI3C1);
}

/******************************************************************************/
/*                            I3C1 error interrupt                            */
/******************************************************************************/
void I3C1_ERR_IRQHandler(void)
{
  HAL_I3C_ERR_IRQHandler(&hI3C1);
}
