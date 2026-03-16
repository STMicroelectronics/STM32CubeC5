/**
  ******************************************************************************
  * @file           : mx_crs.c
  * @brief          : CRS Peripheral initialization
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
#include "mx_crs.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/******************************************************************************/
/* Exported functions for CRS in LL layer (SW instance MyCRS_1) */
/******************************************************************************/
CRS_TypeDef *mx_crs_init(void)
{
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_CRS);

  /* CRS divider : Set the synchronization signal divider */
  /* LL_CRS_SetSyncDivider(CRS, LL_CRS_SYNC_DIV_1); */ /* Configuration matches register reset state at startup. */

  /* CRS source size: Set the synchronization signal source */
  LL_CRS_SetSyncSignalSource(CRS, LL_CRS_SYNC_SOURCE_LSE);

  /* CRS polarity: Set the polarity of the synchronization signal */
  LL_CRS_SetSyncPolarity(CRS, LL_CRS_SYNC_POLARITY_FALLING);

  /* CRS reload value: Set counter reload value */
  LL_CRS_SetReloadCounter(CRS, 0x5B7);

  /* CRS frequency error limit: Set frequency error limit value */
  /* LL_CRS_SetFreqErrorLimit(CRS, 0x22); */ /* Configuration matches register reset state at startup. */

  /* CRS trimming: Disable CRS hardware auto-trimming and set HSI144 trimming value */
  /* LL_CRS_ConfigTrimming(CRS, 0x30, LL_CRS_AUTO_TRIMMING_DISABLE); */ /* Configuration matches register reset state at startup. */

  /* Enable the interruption for CRS */
  NVIC_SetPriority(CRS_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(CRS_IRQn);

  return CRS;
}

void mx_crs_deinit(void)
{
  LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_CRS);
  LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_CRS);

  LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_CRS);

  /* Disable the interruption for CRS */
  NVIC_DisableIRQ(CRS_IRQn);
}
