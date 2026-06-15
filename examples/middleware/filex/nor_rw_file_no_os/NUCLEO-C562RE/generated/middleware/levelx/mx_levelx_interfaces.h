/**
  ******************************************************************************
  * file           : mx_levelx_interfaces.h
  * brief          : header for the mx_levelx_interfaces.c
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

#ifndef MX_LEVELX_INTERFACES_H
#define MX_LEVELX_INTERFACES_H

/* Includes ------------------------------------------------------------------*/
#include "mx_w25q128j.h"
#include "stm32_lx_w25q128j_nor_flash_polling_driver.h"

#define LX_NOR_FLASH_0_SIZE   (W25Q128J_FLASH_SIZE - W25Q128J_BLOCK_SIZE)

#ifndef LX_NOR_DISABLE_EXTENDED_CACHE

#ifdef LX_NOR_ENABLE_MAPPING_BITMAP

#define FX_NOR_FLASH_0_BITMAP_CACHE_SIZE  (W25Q128J_FLASH_SIZE/LX_NOR_SECTOR_SIZE + 31) / 32
#else
#define FX_NOR_FLASH_0_BITMAP_CACHE_SIZE  0

#endif /* LX_NOR_ENABLE_MAPPING_BITMAP */

#ifdef LX_NOR_ENABLE_OBSOLETE_COUNT_CACHE

#define FX_NOR_FLASH_0_OBSOLETE_COUNT_CACHE_SIZE  (W25Q128J_FLASH_SIZE/W25Q128J_BLOCK_SIZE)*sizeof(LX_NOR_OBSOLETE_COUNT_CACHE_TYPE) / 4
#else
#define FX_NOR_FLASH_0_OBSOLETE_COUNT_CACHE_SIZE  0

#endif /* LX_NOR_ENABLE_OBSOLETE_COUNT_CACHE */

#endif /* LX_NOR_DISABLE_EXTENDED_CACHE */

/* define the list of the levelx generic functions to be called by the example */
#define mx_lx_nor_flash_driver_0_initialize   stm32_lx_w25q128j_nor_flash_polling_driver_initialize

/* define the functions to get the levelx interface context */

VOID *mx_lx_nor_flash_driver_0_get_ctx(VOID);

#endif /* MX_LEVELX_INTERFACES_H */
