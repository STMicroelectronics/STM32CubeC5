/**
  ******************************************************************************
  * @file : mx_eeprom_emulation_app.c
  * @brief : EEPROM Emulation initialization source file
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


/* Includes ------------------------------------------------------------------*/
#include "mx_eeprom_emulation_app.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
static ee_object_t mx_ee_object;

/* Exported functions --------------------------------------------------------*/

/******************************************************************************/
/* Exported functions for mx_eeprom_emulation in EEPROM Emulation utility     */
/******************************************************************************/

system_status_t mx_eeprom_emulation_init(void)
{
  system_status_t return_status = SYSTEM_OK;

  /* Get EEPROM interface handles */
  mx_ee_object.f_object = EEPROM_EMULATION_FLASH_HANDLE();

  /* Get CRC interface handle */
  mx_ee_object.crc_object = EEPROM_EMULATION_CRC_HANDLE();

  /* Initialize EEPROM Emulation */
  if (EE_Init(&mx_ee_object, EE_CONDITIONAL_ERASE) != EE_OK)
  {
    return_status = SYSTEM_POSTSYSTEM_ERROR;
  }

  return return_status;
}

