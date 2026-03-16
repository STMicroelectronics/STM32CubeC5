/**
  ******************************************************************************
  * @file           : mx_eeprom_emul.h
  * @brief          : header for mx_eeprom_emul.c
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_EEPROM_EMUL_H
#define MX_EEPROM_EMUL_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "eeprom_emul_core.h"
#include "mx_def.h"
#include "mx_hal_def.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function ----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


/******************************************************************************/
/* Exported functions for mx_eeprom_emul in eeprom_emulation utility  */
/******************************************************************************/
/**
  * @brief seq initialization function
  * This function call the seq init function
  */
system_status_t mx_eeprom_emulation_init(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* MX_EEPROM_EMUL_H */
