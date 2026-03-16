/**
  ******************************************************************************
  * file           : ll_example.h
  * brief          : Header for stm32ynxx_ll_example.c files.
  *                  This file contains example-specific declarations to interface with example.c functions.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LL_EXAMPLE_H
#define LL_EXAMPLE_H

/* Includes ------------------------------------------------------------------*/
#include "mx_hal_def.h"       /* aliases to the target-specific generated code */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** brief:  Disable the RTC domain write protection.
  */
void PWR_DisableRTCDomainWriteProtection(void);

/** brief:  Program the value of the backup register.
  * param:  reg_idx    Backup register index.
  * param:  data_value Data value to be written in backup register.
  */
void TAMP_WriteBackupRegisterValue(uint32_t reg_idx, uint32_t data_value);

/** brief:  Enable the RTC domain write protection.
  */
void PWR_EnableRTCDomainWriteProtection(void);

/** brief:  Retrieve the value of the backup register.
  * param:  data_value Data value to be written in backup register.
  * retval: uint32_t Data value of the backup register.
  */
uint32_t TAMP_ReadBackupRegisterValue(uint32_t reg_idx);

/** brief:  Start the MX_TAMPx passive tamper in interrupt mode.
  */
void TAMP_StartPassive_IT(void);

/** brief: MX_TAMPx event callback.
  */
void TAMP_TamperEventCallback(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_EXAMPLE_H */
