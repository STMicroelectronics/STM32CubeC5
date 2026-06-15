

/***************************************************************************
  * Copyright (c) 2024 Microsoft Corporation
  *
  * This program and the accompanying materials are made available under the
  * terms of the MIT License which is available at
  * https://opensource.org/licenses/MIT.
  *
  * SPDX-License-Identifier: MIT
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

/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/**   LevelX Component                                                    */
/**                                                                       */
/**   User Specific                                                       */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#ifndef LX_USER_H
#define LX_USER_H

/* Defined, this makes LevelX thread-safe by using an RTOS mutex object
throughout the API.
*/
#define LX_THREAD_SAFE_ENABLE

/* Defined, LevelX will be used in standalone mode (without RTOS) */
/*
#define LX_STANDALONE_ENABLE
*/


/* Define the logical sector size for NOR flash. The sector size is in units of 32-bit words.
This sector size should match the sector size used in file system. */
/*
#define LX_NOR_SECTOR_SIZE 128
*/

/* Defined, this causes the LevelX NOR instance open logic to verify free NOR
sectors are all ones.
*/
/*
#define LX_FREE_SECTOR_DATA_VERIFY
*/

/* Defined, this disabled the extended NOR cache. */
/*
#define LX_NOR_DISABLE_EXTENDED_CACHE
*/

/* By default this value is 8, which represents a maximum of 8 sectors that
can be cached in a NOR instance.
*/
/*
#define LX_NOR_EXTENDED_CACHE_SIZE 8
*/

/* By default this value is 16 and defines the logical sector mapping cache size.
Large values improve performance, but cost memory. The minimum size is 8 and all
values must be a power of 2.
*/
/*
#define LX_NOR_SECTOR_MAPPING_CACHE_SIZE 16
*/

/* Determine if logical sector mapping bitmap should be enabled in extended cache.
Cache memory will be allocated to sector mapping bitmap first. One bit can be allocated for each physical sector. */
/*
#define LX_NOR_ENABLE_MAPPING_BITMAP
*/

/* Determine if obsolete count cache should be enabled in extended cache.
Cache memory will be allocated to obsolete count cache after the mapping bitmap if enabled,
and the rest of the cache memory is allocated to sector cache. */
/*
#define LX_NOR_ENABLE_OBSOLETE_COUNT_CACHE
*/

/* Defines obsolete count cache element size. If number of sectors per block is greater than 256, use USHORT instead of
UCHAR. */
/*
#define LX_NOR_OBSOLETE_COUNT_CACHE_TYPE UCHAR
*/


/* By default this value is 4, which represents a maximum of 4 blocks that
can be allocated for metadata.
*/
/*
#define LX_NAND_FLASH_MAX_METADATA_BLOCKS 4
*/
      
#define LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE
#define LX_NOR_FLASH_USER_EXTENSION void *lx_nor_flash_info_ptr;

#endif /* LX_USER_H */
