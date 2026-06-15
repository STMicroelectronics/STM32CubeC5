/**
  ******************************************************************************
  * @file           : mx_eeprom_emulation_app.h
  * @brief          : header for mx_eeprom_emulation_app.c
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_eeprom_emulation_license.md file
  * in the same directory as the generated code.
  * If no mx_eeprom_emulation_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_EEPROM_EMULATION_APP_H
#define MX_EEPROM_EMULATION_APP_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "eeprom_emul_core.h"
#include "mx_hal_def.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Exported defines --------------------------------------------------------- */

/******************************************************************************/
/* Exported defines for mx_eeprom_emulation in EEPROM Emulation utility      */
/******************************************************************************/
/* Flash handle */
#define EEPROM_EMULATION_FLASH_HANDLE      mx_flash_gethandle

/* CRC handle */
#define EEPROM_EMULATION_CRC_HANDLE        mx_crc_gethandle

/******************************************************************************/
/* Exported functions for mx_eeprom_emulation in EEPROM Emulation utility     */
/******************************************************************************/
/**
  * @brief EEPROM Emulation initialization function
  * @retval system_status_t Initialization status
  */
system_status_t mx_eeprom_emulation_init(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_EEPROM_EMULATION_APP_H */
