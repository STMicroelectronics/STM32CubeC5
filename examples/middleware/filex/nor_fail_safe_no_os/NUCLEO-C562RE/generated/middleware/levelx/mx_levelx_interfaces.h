/**
  ******************************************************************************
  * @file           : mx_levelx_interfaces.c
  * @brief          : header for the mx_levelx_interfaces.c
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_levelx_license.md file
  * in the same directory as the generated code.
  * If no mx_levelx_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_LEVELX_INTERFACES_H
#define MX_LEVELX_INTERFACES_H

/* Includes ------------------------------------------------------------------*/
#include "mx_w25q128j.h"
#include "stm32_lx_w25q128j_nor_flash_polling_driver.h"

/* Define the list of the LevelX generic functions to be called by the example */
#define mx_lx_nor_flash_driver_0_initialize  stm32_lx_w25q128j_nor_flash_polling_driver_initialize

/* Define the functions to get the levelx interface context */
VOID *mx_lx_nor_flash_driver_0_get_ctx(VOID);
    
#define LX_NOR_FLASH_0_SIZE   (W25Q128J_FLASH_SIZE - W25Q128J_BLOCK_SIZE)

#ifndef LX_NOR_DISABLE_EXTENDED_CACHE

#ifdef LX_NOR_ENABLE_MAPPING_BITMAP

#define LX_NOR_FLASH_0_BITMAP_CACHE_SIZE  (W25Q128J_FLASH_SIZE/LX_NOR_SECTOR_SIZE + 31) / 32
#else
#define LX_NOR_FLASH_0_BITMAP_CACHE_SIZE  0

#endif /* LX_NOR_ENABLE_MAPPING_BITMAP */

#ifdef LX_NOR_ENABLE_OBSOLETE_COUNT_CACHE

#define LX_NOR_FLASH_0_OBSOLETE_COUNT_CACHE_SIZE  (W25Q128J_FLASH_SIZE/W25Q128J_BLOCK_64K)*sizeof(LX_NOR_OBSOLETE_COUNT_CACHE_TYPE) / 4
#else
#define LX_NOR_FLASH_0_OBSOLETE_COUNT_CACHE_SIZE  0

#endif /* LX_NOR_ENABLE_OBSOLETE_COUNT_CACHE */
#endif /* LX_NOR_DISABLE_EXTENDED_CACHE */


#endif /* MX_LEVELX_INTERFACES_H */
