/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : TAMP erase the backup register when an external tamper event occurs
  *                  using LL APIs.
  ******************************************************************************
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
void mx_tampx_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/**
  *  Disable the RTC domain write protection (RCC RTC domain control register RCC_BDCR, RTC registers,
  *  Tamper registers, backup registers and backup SRAM).
  */
inline void PWR_DisableRTCDomainWriteProtection(void)
{
  LL_PWR_DisableRTCDomainWriteProtection();
}


/**
  *  Program the value of the backup register.
  *  param: reg_idx    Backup register index.
  *  param: data_value Data value to be written in backup register.
  */
inline void TAMP_WriteBackupRegisterValue(uint32_t reg_idx, uint32_t data_value)
{
  LL_TAMP_BKP_SetRegister(reg_idx, data_value);
}


/**
  *  Enable the RTC domain write protection (RCC RTC domain control register RCC_BDCR, RTC registers,
  *  Tamper registers, backup registers and backup SRAM).
  */
inline void PWR_EnableRTCDomainWriteProtection(void)
{
  LL_PWR_EnableRTCDomainWriteProtection();
}


/**
  *  Retrieve the value of the backup register.
  *  param:  data_value Data value to be written in backup register.
  *  retval: uint32_t Value of the backup register
  */
inline uint32_t TAMP_ReadBackupRegisterValue(uint32_t reg_idx)
{
  return (LL_TAMP_BKP_GetRegister(reg_idx));
}


/**
  *  Start the MX_TAMPx passive tamper in interrupt mode.
  */
inline void TAMP_StartPassive_IT(void)
{
  LL_TAMP_EnableIT(MX_TAMP_X);

  LL_TAMP_Enable(MX_TAMP_X);
}


/**
  *  Handles the MX_TAMPx interrupts.
  */
void mx_tampx_irqhandler(void)
{
  if (LL_TAMP_READ_REG(SR) != 0UL)
  {
    LL_TAMP_ClearFlag_TAMP(MX_TAMP_X);

    TAMP_TamperEventCallback();
  }
}
