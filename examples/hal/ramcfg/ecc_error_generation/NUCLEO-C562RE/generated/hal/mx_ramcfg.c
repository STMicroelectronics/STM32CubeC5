/**
  ******************************************************************************
  * @file           : mx_ramcfg.c
  * @brief          : RAMCFG Peripheral initialization
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
#include "mx_ramcfg.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
/******************************************************************************/
/* Exported functions for RAMCFG_SRAM2 in HAL layer */
/******************************************************************************/
hal_ramcfg_t mx_ramcfg_sram2_init(void)
{
  HAL_RCC_RAMCFG_EnableClock();

  /* Enable the interruption for RAMCFG */
  HAL_CORTEX_NVIC_SetPriority(RAMCFG_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(RAMCFG_IRQn);

  HAL_RAMCFG_ECC_Enable_IT(HAL_RAMCFG_SRAM2, HAL_RAMCFG_IT_ECC_SINGLE);

  return HAL_RAMCFG_SRAM2;
}

void mx_ramcfg_sram2_deinit(void)
{
  HAL_RAMCFG_ECC_Disable(HAL_RAMCFG_SRAM2);
}

/******************************************************************************/
/* RAMCFG global interrupt (Grouping interrupts of all active RAMCFG instances)*/
/******************************************************************************/
void RAMCFG_IRQHandler(void)
{
  HAL_RAMCFG_IRQHandler(HAL_RAMCFG_SRAM2);
}

/******************************************************************************/
/* RAMCFG NMI interrupt */
/******************************************************************************/
__WEAK system_status_t RAMCFG_SRAM2_NMI_IRQHandler(void)
{
  /* NOTE : This function is a weak implementation for the RAMCFG NMI handler.
            User can override it to handle ECC NMI errors for RAMCFG.
  */
  /* Status to be updated to SYSTEM_OK when the user IRQhandler manages the NMI error */
  return SYSTEM_PERIPHERAL_ERROR;
}
