/**
  ******************************************************************************
  * @file           : mx_system.c
  * @brief          : STM32 system program body
  *                   Applicative target system level initialization
  *                   (system clock, cache, TZ...) and system level peripherals
  *                   initialization. system_init() service is called by the main
  *                   before jumping the example entry point.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "mx_system.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
static int32_t system_clock_config(void);
static int32_t peripherals_init(void);
static int32_t mx_icache_init(void);

/* Exported functions --------------------------------------------------------*/
int32_t system_init(void)
{
  int32_t ret = -1;

  pre_system_init_hook();

  if (HAL_Init() == HAL_OK)
  {

    if (system_clock_config() == 0)
    {
      if (peripherals_init() == 0)
      {
        post_system_init_hook();
        ret = 0;
      }
    }
  }

  return ret;
}

/**
  * @brief  User hook function called before the HAL_Init() function
  */
__weak void pre_system_init_hook(void)
{
  /* NOTE : This function must not be modified, when the callback is needed,
            the pre_system_init_hook must be implemented in the user file
   */
}

/**
  * @brief  User hook function called after the HAL_Init() and Peripheral init functions
  */
__weak void post_system_init_hook(void)
{
  /* NOTE : This function must not be modified, when the callback is needed,
            the post_system_init_hook must be implemented in the user file
   */
}


/******************************************************************************/
/*                            Systick Handler                                 */
/******************************************************************************/
/**
  * @brief  This function handles SysTick Handler.
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/* Private functions definition ----------------------------------------------*/
/**
  * @brief  Peripherals initialization
  * @retval 0   Success
  * @retval -1  Initialization issue
  */
static int32_t peripherals_init(void)
{
  int32_t ret = 0;

  /*  ICACHE Initialization */
  if (mx_icache_init() != 0)
  {
    ret = -1;
  }

  /* gpio_cfg1_init */
  if (gpio_cfg1_init() != 0)
  {
    ret = -1;
  }

#ifdef USE_TRACE
  /* mx_gpio_trace_init */
  if (gpio_cfg2_init() != 0)
  {
    ret = -1;
  }
  /* mx_uart_trace_init */
  if (usart2_instance1_cfg1_init() == NULL)
  {
    ret = -1;
  }
#endif /* USE_TRACE */

  return ret;
}

/**
  * @brief  System clock configuration
  * @retval 0   Success
  * @retval -1  Power configuration issue.
  */
static int32_t system_clock_config(void)
{
  int32_t ret = 0;
  hal_rcc_bus_clk_config_t config_bus;
  HAL_RCC_HSIS_Enable();


  /** Frequency will be increased */
  HAL_FLASH_ITF_SetLatency(HAL_FLASH, HAL_FLASH_ITF_LATENCY_4);
  HAL_RCC_SetSYSCLKSource(HAL_RCC_SYSCLK_SRC_HSIS);


  /** Initializes the CPU, AHB and APB busses clocks */
  config_bus.hclk_prescaler  = HAL_RCC_HCLK_PRESCALER4;
  config_bus.pclk1_prescaler = HAL_RCC_PCLK_PRESCALER1;
  config_bus.pclk2_prescaler = HAL_RCC_PCLK_PRESCALER1;
  config_bus.pclk3_prescaler = HAL_RCC_PCLK_PRESCALER1;

  HAL_RCC_SetBusClockConfig(&config_bus);

  return ret;
}

/**
  * brief ICACHE Initialization Function
  * param None
  * retval Status
  */
static int32_t  mx_icache_init(void)
{
  int32_t ret = -1;
  hal_icache_handle_t gh_icache;

  if (HAL_ICACHE_Init(&gh_icache, HAL_ICACHE) == HAL_OK)
  {
    if (HAL_ICACHE_SetAssociativityMode(&gh_icache, HAL_ICACHE_ASSOCIATIVITY_1WAY) == HAL_OK)
    {
      if (HAL_ICACHE_Start(&gh_icache, HAL_ICACHE_IT_NONE) == HAL_OK)
      {
        ret = 0;
      }
    }
  }

  return ret;
}
