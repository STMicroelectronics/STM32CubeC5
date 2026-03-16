/**
  ******************************************************************************
  * @file           : mx_tamp.c
  * @brief          : TAMP Peripheral initialization
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
#include "mx_tamp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/******************************************************************************/
/* Exported functions for TAMP in HAL layer (SW instance MyTAMP_1) */
/******************************************************************************/
system_status_t mx_tamp_init(void)
{
  hal_tamp_passive_config_t             p_passive_global_config;
  hal_tamp_passive_individual_config_t  p_passive_indiv_config;

  /* Disable RTC Domain Write Protection */
  HAL_PWR_DisableRTCDomainWriteProtection();

  HAL_RCC_RTCAPB_EnableClock();

  HAL_RCC_RTC_EnableKernelClock();

  p_passive_global_config.precharge = HAL_TAMP_PASSIVE_PULL_UP_PRECHARGE_DISABLE;
  p_passive_global_config.precharge_duration = HAL_TAMP_PASSIVE_PULL_UP_PRECHARGE_1_RTCCLK;
  p_passive_global_config.type_activation = HAL_TAMP_PASSIVE_FILTER_DISABLE;
  p_passive_global_config.sample_frequency = HAL_TAMP_PASSIVE_SAMPLE_FREQ_DIV_32768;
  if (HAL_TAMP_PASSIVE_SetConfig(&p_passive_global_config) != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  p_passive_indiv_config.trigger = HAL_TAMP_PASSIVE_TRIGGER_RISING;
  p_passive_indiv_config.erase_secrets = HAL_TAMP_PASSIVE_SECRETS_ERASE;
  p_passive_indiv_config.masked = HAL_TAMP_PASSIVE_UNMASKED;
  if (HAL_TAMP_PASSIVE_SetConfigTampers(HAL_TAMP_TAMPER_1, &p_passive_indiv_config) != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  /* Enable the interruption for TAMP */
  HAL_CORTEX_NVIC_SetPriority(TAMP_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(TAMP_IRQn);

  return SYSTEM_OK;
}

void mx_tamp_deinit(void)
{
  /* Disable the interuption for TAMP */
  HAL_CORTEX_NVIC_DisableIRQ(TAMP_IRQn);
}

/******************************************************************************/
/*                     Tamper global interrupts                    */
/******************************************************************************/
void TAMP_IRQHandler(void)
{
  HAL_TAMP_IRQHandler();
}
