
/**
  ******************************************************************************
  * @file           : mx_rcc.c
  * @brief          : STM32 RCC program body
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
#include "mx_rcc.h"

/* Private typedef -----------------------------------------------------------*//* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/

/******************************************************************************/
/* Exported functions for RCC in HAL layer */
/******************************************************************************/

/**
  * Configure the system core clock only and activate it using the HAL RCC unitary APIs (footprint optimization)
  *         The system Clock is configured as follow :
  *            System Clock source            = HSIS
  *            SYSCLK(Hz)                     = 144000000
  *            HCLK(Hz)                       = 144000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            APB3 Prescaler                 = 1
  *            Flash Latency(WS)              = 4
  */
system_status_t mx_rcc_init(void)
{
  if (HAL_RCC_HSIS_Enable() != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  /** Frequency will be increased */
  HAL_FLASH_ITF_SetLatency(HAL_FLASH, HAL_FLASH_ITF_LATENCY_4);
  HAL_RCC_SetSYSCLKSource(HAL_RCC_SYSCLK_SRC_HSIS);
  HAL_FLASH_ITF_SetProgrammingDelay(HAL_FLASH, HAL_FLASH_ITF_PROGRAM_DELAY_2);

  /** Initializes the CPU, AHB and APB busses clocks */
  hal_rcc_bus_clk_config_t config_bus;
  config_bus.hclk_prescaler  = HAL_RCC_HCLK_PRESCALER1;
  config_bus.pclk1_prescaler = HAL_RCC_PCLK_PRESCALER1;
  config_bus.pclk2_prescaler = HAL_RCC_PCLK_PRESCALER1;
  config_bus.pclk3_prescaler = HAL_RCC_PCLK_PRESCALER1;
  HAL_RCC_SetBusClockConfig(&config_bus);

  /** Systick Initialization */
  HAL_RCC_SetSysTickExternalClkSource(HAL_RCC_SYSTICK_CLK_SRC_HCLKDIV8);

  return SYSTEM_OK;
}

void mx_rcc_deinit(void)
{
  HAL_RCC_Reset();
}

/**
  * configures and activate the clocks used by all the peripherals selected within the project
  */
system_status_t mx_rcc_peripherals_clock_config(void)
{
  /* Enable SBS */
  HAL_RCC_SBS_EnableClock();

  /* Enable HSE to be used as clk source for PSI */
  if (HAL_RCC_HSE_Enable(HAL_RCC_HSE_ON) != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  /* Configure PSI to output 100MHz */
  hal_rcc_psi_config_t config_psi;
  config_psi.psi_source = HAL_RCC_PSI_SRC_HSE;
  config_psi.psi_ref = HAL_RCC_PSI_REF_48MHZ;
  config_psi.psi_out = HAL_RCC_PSI_OUT_100MHZ;
  HAL_RCC_PSI_SetConfig(&config_psi);

  /* Enable PSI */
  if (HAL_RCC_PSIS_Enable() != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  /** Ethernet Clock */
  /* PSIS Clk 100MHz --> ETH Clk 50MHz */
  HAL_RCC_ETH1_SetKernelClkSource(HAL_RCC_ETH1_CLK_SRC_PSIS);
  HAL_RCC_ETH1REF_SetKernelClkSource(HAL_RCC_ETH1REF_CLK_SRC_RMII);

  return SYSTEM_OK;
}
