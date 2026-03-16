/**
  ******************************************************************************
  * @file           : mx_cortex_nvic.c
  * @brief          : CORTEX_NVIC Peripheral initialization
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
#include "mx_cortex_nvic.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/******************************************************************************/
/* Exported functions for NVIC in HAL layer (SW instance MyCORTEX_NVIC_1) */
/******************************************************************************/
system_status_t mx_cortex_nvic_init(void)
{
  /* Configure the Priority grouping */
  HAL_CORTEX_NVIC_SetPriorityGrouping(HAL_CORTEX_NVIC_PRIORITY_GROUP_4);

  /* System service call via SWI instruction */
  HAL_CORTEX_NVIC_SetPriority(SVCall_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);

  /* Debug Monitor */
  HAL_CORTEX_NVIC_SetPriority(DebugMonitor_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);

  /* Pendable request for system service */
  HAL_CORTEX_NVIC_SetPriority(PendSV_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);

  return SYSTEM_OK;
}

/******************************************************************************/
/* Non maskable interrupt. The RCC Clock Security System (CSS) is linked to the NMI vector. */
/******************************************************************************/
void NMI_Handler(void)
{
  if (SYSTEM_OK == FLASH_NMI_IRQHandler())
  {
    return;
  }

  while (1);
}

/******************************************************************************/
/* System service call via SWI instruction is managed directly in user code.  */
/******************************************************************************/
/* void SVC_Handler(void)
{
}
  */

/******************************************************************************/
/*              Debug Monitor is managed directly in user code.               */
/******************************************************************************/
/* void DebugMonitor_Handler(void)
{
}
  */

/******************************************************************************/
/*   Pendable request for system service is managed directly in user code.    */
/******************************************************************************/
/* void PendSV_Handler(void)
{
}
  */
