/**
  ******************************************************************************
  * @file           : mx_rcc.c
  * @brief          : STM32 RCC program body
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
#include "mx_rcc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/

/******************************************************************************/
/* Exported functions for RCC in HAL layer */
/******************************************************************************/

/**
  * Configure the system core clock only and activate it using the HAL RCC unitary APIs (footprint optimization)
  *         The system Clock is configured as follow :
  *            System Clock source            = PSIS
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

  hal_rcc_psi_config_t config_psi;
  config_psi.psi_source = HAL_RCC_PSI_SRC_HSI_8MHz;
  config_psi.psi_ref = HAL_RCC_PSI_REF_8MHZ;
  config_psi.psi_out = HAL_RCC_PSI_OUT_144MHZ;
  if (HAL_RCC_PSI_SetConfig(&config_psi) != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  if (HAL_RCC_PSIS_Enable() != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  /** Initializes the CPU, AHB and APB busses clocks */
  hal_rcc_bus_clk_config_t config_bus;
  config_bus.hclk_prescaler  = HAL_RCC_HCLK_PRESCALER1;
  config_bus.pclk1_prescaler = HAL_RCC_PCLK_PRESCALER1;
  config_bus.pclk2_prescaler = HAL_RCC_PCLK_PRESCALER1;
  config_bus.pclk3_prescaler = HAL_RCC_PCLK_PRESCALER1;
  if (HAL_RCC_SetBusClockConfig(&config_bus) != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  /** Frequency will be increased */
  HAL_FLASH_ITF_SetLatency(HAL_FLASH, HAL_FLASH_ITF_LATENCY_4);

  if (HAL_RCC_SetSYSCLKSource(HAL_RCC_SYSCLK_SRC_PSIS) != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  HAL_FLASH_ITF_SetProgrammingDelay(HAL_FLASH, HAL_FLASH_ITF_PROGRAM_DELAY_2);

  if (HAL_UpdateCoreClock() != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  /* No GPIO configuration required for RCC */

  return SYSTEM_OK;
}

void mx_rcc_deinit(void)
{
  /* Disable RTC Domain Write Protection */
  HAL_PWR_DisableRTCDomainWriteProtection();

  HAL_RCC_Reset();

  /* Enable RTC Domain Write Protection */
  HAL_PWR_EnableRTCDomainWriteProtection();
}

/**
  * configures and activate the clocks used by all the peripherals selected within the project
  */
system_status_t mx_rcc_peripherals_clock_config(void)
{
  /* Peripherals clocked with PCLK1 (144 MHz):
     USART2
  */
  /**
    * - Stop all Crypton S_HCLK using RCC
    * - Drive low pulse on RNG_HRESETn, HASH_HRESETn using RCC
    * - Start all crypton clocks RNG_HCLK, PKA_HCLK, CCB_HCLK except SAES_HCLK (PKA can start PKA RAM clear process)
    */
  /* -1- Disable the clock of all the Crypto IPs */
  HAL_RCC_RNG_DisableClock();
  HAL_RCC_HASH_DisableClock();

  /* -2- Reset all the Crypto IPs */
  HAL_RCC_RNG_Reset();
  HAL_RCC_HASH_Reset();

  /* -3- Enable the clock of the used Crypto IPs */
  /* Peripherals clocked with PSIDIV3 (48 MHz):
     RNG
  */
  if (HAL_RCC_PSIDIV3_Enable() != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  if (HAL_RCC_CK48_SetKernelClkSource(HAL_RCC_CK48_CLK_SRC_PSIDIV3) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_RNG_EnableClock();         /* RNG clock -> rng_hclk */
  HAL_RCC_HASH_EnableClock();

  return SYSTEM_OK;
}
