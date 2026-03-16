/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026 STMicroelectronics
 *
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 **************************************************************************/

#ifndef LX_USER_H
#define LX_USER_H



/* Defined, this option bypasses the NOR flash driver read routine in favor or reading
   the NOR memory directly, resulting in a significant performance increase.
  */
/*
#define LX_DIRECT_READ
*/


/* Defined, this causes the LevelX NOR instance open logic to verify free NOR
   sectors are all ones.
*/
/*
#define LX_FREE_SECTOR_DATA_VERIFY
*/

/* By default this value is 4, which represents a maximum of 4 blocks that
   can be allocated for metadata.
*/
/*
#define LX_NAND_FLASH_MAX_METADATA_BLOCKS 4
*/

/* Defined, this disabled the extended NOR cache.  */
/*
#define LX_NOR_DISABLE_EXTENDED_CACHE
*/

/* By default this value is 8, which represents a maximum of 8 sectors that
   can be cached in a NOR instance.
*/
/*
#define LX_NOR_EXTENDED_CACHE_SIZE   8
*/


/* By default this value is 16 and defines the logical sector mapping cache size.
   Large values improve performance, but cost memory. The minimum size is 8 and all
   values must be a power of 2.
*/
/*
#define LX_NOR_SECTOR_MAPPING_CACHE_SIZE   16
*/

/* Defined, this makes LevelX thread-safe by using an RTOS mutex object
   throughout the API.
*/

/* #define LX_THREAD_SAFE_ENABLE */


/* Defined, LevelX will be used in standalone mode (without RTOS) */

#define LX_STANDALONE_ENABLE

/* Define user extension for NOR flash control block. User extension is placed at the end of flash control block and it is not cleared on opening flash. */
/*
#define LX_NOR_FLASH_USER_EXTENSION    ????
*/

/* Define user extension for NAND flash control block. User extension is placed at the end of flash control block and it is not cleared on opening flash.  */
/*
#define LX_NAND_FLASH_USER_EXTENSION   ????
*/

/* Determine if logical sector mapping bitmap should be enabled in extended cache.
   Cache memory will be allocated to sector mapping bitmap first. One bit can be allocated for each physical sector.  */
/*
#define LX_NOR_ENABLE_MAPPING_BITMAP
*/

/* Determine if obsolete count cache should be enabled in extended cache.
   Cache memory will be allocated to obsolete count cache after the mapping bitmap if enabled,
   and the rest of the cache memory is allocated to sector cache.  */
/*
#define LX_NOR_ENABLE_OBSOLETE_COUNT_CACHE
*/

/* Defines obsolete count cache element size. If number of sectors per block is greater than 256, use USHORT instead of UCHAR.  */
/*
#define LX_NOR_OBSOLETE_COUNT_CACHE_TYPE            UCHAR
*/

/* Define the logical sector size for NOR flash. The sector size is in units of 32-bit words.
   This sector size should match the sector size used in file system.  */
/*
#define LX_NOR_SECTOR_SIZE                          (512/sizeof(ULONG))
*/
#define LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE
#endif /* LX_USER_H */

