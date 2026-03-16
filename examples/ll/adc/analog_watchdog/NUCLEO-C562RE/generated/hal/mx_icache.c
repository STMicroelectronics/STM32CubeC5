
/**
  ******************************************************************************
  * @file           : mx_icache.c
  * @brief          : ICACHE Peripheral initialization
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
#include "mx_icache.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/******************************************************************************/
/* Exported functions for ICACHE in LL layer (SW instance MyICACHE_1) */
/******************************************************************************/
ICACHE_TypeDef *mx_icache_init(void)
{
  //LL_ICACHE_SetMode(ICACHE, LL_ICACHE_2WAYS);

  return ICACHE;
}

void mx_icache_deinit(void)
  {
  /* interrupts disabled */
  LL_ICACHE_DisableIT(ICACHE, LL_ICACHE_IER_ALL);

  /* Clear any pending flags */
  LL_ICACHE_ClearFlag(ICACHE, LL_ICACHE_FCR_ALL);

  /* Reset monitors */
  LL_ICACHE_ResetMonitors(ICACHE, LL_ICACHE_MONITOR_ALL);

  /* Disable monitors and ICACHE, 2_ways associative mode is set */
  LL_ICACHE_WRITE_REG(ICACHE, CR, LL_ICACHE_2WAYS);

  /* Reset and disable remapped remap_regions */
  LL_ICACHE_WRITE_REG(ICACHE, CRR0, 0x00000200U);
  LL_ICACHE_WRITE_REG(ICACHE, CRR1, 0x00000200U);
  LL_ICACHE_WRITE_REG(ICACHE, CRR2, 0x00000200U);
  LL_ICACHE_WRITE_REG(ICACHE, CRR3, 0x00000200U);

  NVIC_DisableIRQ(ADC1_IRQn);
}
