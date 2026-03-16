/**
  ******************************************************************************
  * @file    eeprom_emul_conf.h
  * @author  MCD Application Team
  * @brief   EEPROM emulation configuration file.
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

/** @addtogroup EEPROM_Emulation
  * @{
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef EEPROM_EMUL_CONF_H
#define EEPROM_EMUL_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Private constants ---------------------------------------------------------*/

/*!<  Algo selection, the eeprom emulation has 2 algo one for FLITF flash interface and the second for NVM
      flash interface. So user can select one of them by enabling a switch EE_ALGO_FLITF or EE_ALGO_NVM  */
#define EE_ALGO_FLITF                 (1)

/*!< Number of bytes per data line in EEPROM is depends on the type of flash embedded in STM32 product;
     it can be either 8 or 16.*/
#define EE_FRAME_LINE_SIZE            (8)

/*!< Configuration of eeprom emulation in flash, can be custom */

/*!< Start address of the first page in flash, for EEPROM emulation */
#define EE_START_PAGE_ADDRESS      (FLASH_EDATA_BASE + FLASH_EDATA_BANK_SIZE)

/*!< Start address of the flash */
#define EE_FLASH_BASE_ADDRESS      (FLASH_EDATA_BASE)

#define EE_FLASH_PAGE_SIZE         (FLASH_EDATA_PAGE_SIZE)

#define EE_FLASH_BANK_SIZE         (FLASH_EDATA_BANK_SIZE * 2)

/*!< Number of 100Kcycles requested, minimum 1 for 10Kcycles (default),
     for instance 10 to reach 100Kcycles. This factor will increase
     pages number */
#define EE_CYCLES_NUMBER           (1U)

/*!< Number of guard pages avoiding frequent transfers */
#define EE_GUARD_PAGES_NUMBER      (1U)

/*!< Number of variables to handle in eeprom */
#define EE_NB_OF_VARIABLES         (1000U)

#ifdef __cplusplus
}
#endif

/**
  * @}
  */

#endif /* EEPROM_EMUL_CONF_H */
