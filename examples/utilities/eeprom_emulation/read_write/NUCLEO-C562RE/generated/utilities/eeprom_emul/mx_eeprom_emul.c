/**
  ******************************************************************************
  * @file : mx_eeprom_emul.c
  * @brief : eeprom_emulation mx file
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
/* Includes ------------------------------------------------------------------*/
#include "mx_eeprom_emul.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static ee_object_t mx_ee_object_example;
/* Private functions prototype------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/******************************************************************************/
/* Exported functions for mx_eeprom_emul in eeprom_emulation utility  */
/******************************************************************************/

system_status_t mx_eeprom_emulation_init(void)
{
  system_status_t return_status = SYSTEM_OK;

  /* get interface handle */
  mx_ee_object_example.f_object = MX_EEPROM_FLASH_HANDLE();
  mx_ee_object_example.crc_object = MX_EEPROM_CRC_HANDLE();
  /* Init of eeprom emulation */
  if (EE_Init(&mx_ee_object_example, EE_CONDITIONAL_ERASE) != EE_OK)
  {
    return_status = SYSTEM_POSTSYSTEM_ERROR;
  }

  return return_status;
}
