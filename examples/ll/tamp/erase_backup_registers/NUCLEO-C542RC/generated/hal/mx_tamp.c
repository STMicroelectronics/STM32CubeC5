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
/* Exported functions for TAMP in LL layer (SW instance MyTAMP_1) */
/******************************************************************************/
system_status_t mx_tamp_init(void)
{
  /* Disable RTC Domain Write Protection */
  LL_PWR_DisableRTCDomainWriteProtection();

  LL_APB3_GRP1_EnableClock(LL_APB3_GRP1_PERIPH_RTCAPB);
  LL_RCC_EnableRTC();

  LL_TAMP_SetConfig(LL_TAMP_PULL_UP_DISABLE,
                    LL_TAMP_DURATION_1RTCCLK,
                    LL_TAMP_FILTER_DISABLE,
                    LL_TAMP_SAMPLFREQDIV_32768);

  LL_TAMP_SetConfigTampers(MX_TAMP_X,
                           LL_TAMP_ACTIVELEVEL_DEACTIVATE_ALL,
                           LL_TAMP_ERASE_ACTIVATE_ALL,
                           LL_TAMP_MASK_NONE);

  /* Enable the interruption for TAMP */
  NVIC_SetPriority(TAMP_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(TAMP_IRQn);

  return SYSTEM_OK;
}

void mx_tamp_deinit(void)
{
  /* Disable the interuption for TAMP */
  NVIC_DisableIRQ(TAMP_IRQn);
}
