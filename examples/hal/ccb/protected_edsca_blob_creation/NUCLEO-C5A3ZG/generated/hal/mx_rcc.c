
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

typedef struct
{
  uint32_t hse: 4U;
  uint32_t hsidiv3: 4U;
  uint32_t psis: 4U;
  uint32_t psi_config: 1U;
} mx_rcc_status_t;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define RCC_CONSUME_RESOURCE(block, nb_resources)     (RCC_Status.block += (nb_resources))
#define RCC_RELEASE_RESOURCE(block)                   ((RCC_Status.block > 0U) ? (RCC_Status.block--) : \
                                                       (RCC_Status.block = 0U))
#define RCC_IS_RESOURCE_FREE(block)                   (RCC_Status.block == 0U)
#define RCC_IS_RESOURCE_TAKEN(block)                  (RCC_Status.block > 0U)

/* Private variables ---------------------------------------------------------*/
static mx_rcc_status_t RCC_Status;

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
  if (HAL_RCC_HSE_Enable(HAL_RCC_HSE_ON) != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  RCC_CONSUME_RESOURCE(hse, 1);
  hal_rcc_psi_config_t config_psi;
  config_psi.psi_source = HAL_RCC_PSI_SRC_HSE;
  config_psi.psi_ref = HAL_RCC_PSI_REF_48MHZ;
  config_psi.psi_out = HAL_RCC_PSI_OUT_144MHZ;
  if (HAL_RCC_PSI_SetConfig(&config_psi) != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  RCC_CONSUME_RESOURCE(psi_config, 1);

  if (HAL_RCC_PSIS_Enable() != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

    RCC_CONSUME_RESOURCE(psis, 1);

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
  RCC_Status = (mx_rcc_status_t){0};

  HAL_RCC_Reset();
}

/**
  * configure and enable clock for USART2
  */
system_status_t mx_rcc_usart2_clock_config(void)
{
  /* Peripherals using PCLK1 (144 MHz):
    USART2
  */

  if (HAL_RCC_USART2_SetKernelClkSource(HAL_RCC_USART2_CLK_SRC_PCLK1) != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  return SYSTEM_OK;
}

/**
  * Deactivate clock for USART2
  */
system_status_t mx_rcc_usart2_clock_deactivate(void)
{
  /* Nothing to do */

  return SYSTEM_OK;
}

/**
  * configure and enable clock for CCB
  */
system_status_t mx_rcc_ccb_clock_config(void)
{
  /* Peripherals using HSIDIV3 (48 MHz):
    CCB
  */
  if (mx_rcc_hsidiv3_clock_config(1) != SYSTEM_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  if (HAL_RCC_CK48_SetKernelClkSource(HAL_RCC_CK48_CLK_SRC_HSIDIV3) != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  return SYSTEM_OK;
}

/**
  * Deactivate clock for CCB
  */
system_status_t mx_rcc_ccb_clock_deactivate(void)
{
  if (mx_rcc_hsidiv3_clock_deactivate() != SYSTEM_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }
  return SYSTEM_OK;
}

/**
  * configure and enable HSE oscillator
  * @param nb_resources Number of resources which requests to configure the clock
  */
system_status_t mx_rcc_hse_clock_config(uint32_t nb_resources)
{
  if (RCC_IS_RESOURCE_FREE(hse))
  {
    if (HAL_RCC_HSE_Enable(HAL_RCC_HSE_ON) != HAL_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }
  }
  RCC_CONSUME_RESOURCE(hse, nb_resources);

  return SYSTEM_OK;
}

/**
  * Disable HSE oscillator
  */
system_status_t mx_rcc_hse_clock_deactivate(void)
{
  RCC_RELEASE_RESOURCE(hse);

  if (RCC_IS_RESOURCE_FREE(hse))
  {
    if (HAL_RCC_HSE_Disable() != HAL_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }
  }
  return SYSTEM_OK;
}

/**
  * configure and enable HSIDIV3 oscillator
  * @param nb_resources Number of resources which requests to configure the clock
  */
system_status_t mx_rcc_hsidiv3_clock_config(uint32_t nb_resources)
{
  if (RCC_IS_RESOURCE_FREE(hsidiv3))
  {
    if (HAL_RCC_HSIDIV3_Enable() != HAL_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }
  }
  RCC_CONSUME_RESOURCE(hsidiv3, nb_resources);

  return SYSTEM_OK;
}

/**
  * Disable HSIDIV3 oscillator
  */
system_status_t mx_rcc_hsidiv3_clock_deactivate(void)
{
  RCC_RELEASE_RESOURCE(hsidiv3);

  if (RCC_IS_RESOURCE_FREE(hsidiv3))
  {
    if (HAL_RCC_HSIDIV3_Disable() != HAL_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }
  }
  return SYSTEM_OK;
}

/**
  * configure and enable PSIS oscillator
  * @param nb_resources Number of resources which requests to configure the clock
  */
system_status_t mx_rcc_psis_clock_config(uint32_t nb_resources)
{
  if (RCC_IS_RESOURCE_FREE(psis))
  {
    if (mx_rcc_psi_config() != SYSTEM_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }
    if (HAL_RCC_PSIS_Enable() != HAL_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }
  }
  RCC_CONSUME_RESOURCE(psis, nb_resources);

  return SYSTEM_OK;
}

/**
  * Disable PSIS oscillator
  */
system_status_t mx_rcc_psis_clock_deactivate(void)
{
  RCC_RELEASE_RESOURCE(psis);

  if (RCC_IS_RESOURCE_FREE(psis))
  {
    if (HAL_RCC_PSIS_Disable() != HAL_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }

    if (mx_rcc_psi_deactivate() != SYSTEM_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }
  }
  return SYSTEM_OK;
}

/**
  * Enable the PSI source reference and configure the PSI oscillator.
  */
system_status_t mx_rcc_psi_config(void)
{
  if (RCC_IS_RESOURCE_FREE(psi_config))
  {
    /* Enable the oscillator used as PSI reference */
    if (mx_rcc_hse_clock_config(1) != SYSTEM_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }
    /* Configure the PSI oscillator */
    hal_rcc_psi_config_t config_psi;
    config_psi.psi_source = HAL_RCC_PSI_SRC_HSE;
    config_psi.psi_ref = HAL_RCC_PSI_REF_48MHZ;
    config_psi.psi_out = HAL_RCC_PSI_OUT_144MHZ;
    if (HAL_RCC_PSI_SetConfig(&config_psi) != HAL_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }

    RCC_CONSUME_RESOURCE(psi_config, 1);
  }

  return SYSTEM_OK;
}

/**
  * Disable the PSI source reference and the PSI oscillator.
  */
system_status_t mx_rcc_psi_deactivate(void)
{
  /* Deactivation is possible only if all PSI outputs are free */
  if (RCC_IS_RESOURCE_FREE(psis))
  {
    RCC_RELEASE_RESOURCE(psi_config);

    /* Deactivate the oscillator used as PSI reference */
    if (mx_rcc_hse_clock_deactivate() != SYSTEM_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }
  }

  return SYSTEM_OK;
}
