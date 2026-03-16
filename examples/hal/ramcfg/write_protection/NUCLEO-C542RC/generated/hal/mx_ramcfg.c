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

  return HAL_RAMCFG_SRAM2;
}

void mx_ramcfg_sram2_deinit(void)
{
}

/******************************************************************************/
/* RAMCFG global interrupt (Grouping interrupts of all active RAMCFG instances)*/
/******************************************************************************/
/* Managed directly in user code.
void RAMCFG_IRQHandler(void)
{
}
*/

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
