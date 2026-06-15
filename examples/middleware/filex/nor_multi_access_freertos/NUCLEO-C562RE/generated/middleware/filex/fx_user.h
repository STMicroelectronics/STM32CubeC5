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
  * This software is licensed under terms that can be found in the mx_filex_license.md file
  * in the same directory as the generated code.
  * If no mx_filex_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/**   FileX Component                                                    */
/**                                                                       */
/**   User Specific                                                       */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#ifndef FX_USER_H
#define FX_USER_H

/* Define various build options for the FileX port. The application should either make changes
   here by commenting or un-commenting the conditional compilation defined OR supply the defines
   through the compiler's equivalent of the -D option. */
/* Configure the FileX in Standalone mode */
/* #define FX_STANDALONE_ENABLE */

/* Determine if error checking is desired. If so, map API functions
   to the appropriate error checking front-ends. Otherwise, map API
   functions to the core functions that actually perform the work.
   Note: error checking is enabled by default. */

 /* #define FX_DISABLE_ERROR_CHECKING */

/* Defines the size in bytes of the bit map used to update the secondary FAT sectors.
   The larger the value the less unnecessary secondary FAT sector writes. */

/* #define FX_FAT_MAP_SIZE 128 */

/* Defines the number of entries in the FAT cache. */

/* #define FX_MAX_FAT_CACHE 16 */

/* Defines the maximum size of long file names supported by FileX.
   The minimum value is 13 et la valeur maximale est 256. */

/* #define FX_MAX_LONG_NAME_LEN 256 */

/* Defines the maximum number of logical sectors that can be cached by FileX. The cache memory
   supplied to FileX at fx_media_open determines how many sectors can actually be cached. */

/* #define FX_MAX_SECTOR_CACHE 256 */

/* Defines the number of seconds the time parameters are updated in FileX. */

/* #define FX_UPDATE_RATE_IN_SECONDS 10 */

/* Defines the number of FreeRTOS timer ticks required to achieve the update rate specified by
   FX_UPDATE_RATE_IN_SECONDS defined previously. */

/* #define FX_UPDATE_RATE_IN_TICKS 10000 */

/* Determine if cache is disabled. */

/* #define FX_DISABLE_CACHE */

/* When defined the local path support is disabled */

/* #define FX_NO_LOCAL_PATH */

/* Define FileX internal protection macros. If FX_SINGLE_THREAD is defined,
   these protection macros are effectively disabled. However, for multi-thread
   uses, the macros are setup to utilize a FreeRTOS mutex for multiple thread
   access control into an open media. */

/* #define FX_SINGLE_THREAD */

/* Include additional configurations from the 'more' section */

/* Defined, FileX does not update already opened files. */

/* #define FX_DONT_UPDATE_OPEN_FILES */

/* Direct read sector cache will be disabled if cache is disabled. */

 /* #define FX_DISABLE_DIRECT_DATA_READ_CACHE_FILL */

/* Defines the maximum size of long file names supported by FileX. */

/* #define FX_MAX_LAST_NAME_LEN 256 */

/* Defined, enables 64-bits sector addresses used in I/O driver. */

/* #define FX_DRIVER_USE_64BIT_LBA */

/* If defined, build options is disabled. */

/* #define FX_DISABLE_BUILD_OPTIONS */

/* If defined, single-line functions are disabled. */

/* #define FX_DISABLE_ONE_LINE_FUNCTION */

/* If defined, FAT entry refresh is disabled. */

/* #define FX_DISABLE_FAT_ENTRY_REFRESH */

/* If defined, consecutive sector detection is disabled. */

/* #define FX_DISABLE_CONSECUTIVE_DETECT */

/* Defined, gathering of media statistics is disabled. */

 /* #define FX_MEDIA_STATISTICS_DISABLE */

/* Defined, the file search cache optimization is disabled. */

/* #define FX_MEDIA_DISABLE_SEARCH_CACHE */

/* Defined, FileX is built without updating the time parameters. */

/* #define FX_NO_TIMER */

/* Defined, renaming inherits path information. */

/* #define FX_RENAME_PATH_INHERIT */

/* Defined, legacy single open logic for the same file is enabled. */

/* #define FX_SINGLE_OPEN_LEGACY */

/* If defined, file close is disabled. */

/* #define FX_DISABLE_FILE_CLOSE */

/* If defined, fast open is disabled. */

/* #define FX_DISABLE_FAST_OPEN */

/* If defined, force memory operations are disabled. */

/* #define FX_DISABLE_FORCE_MEMORY_OPERATION */
/* Defined, enables FileX fault tolerant service. */

/* #define FX_ENABLE_FAULT_TOLERANT */

/* Defined, data sector write requests are flushed immediately to the driver. */

/* #define FX_FAULT_TOLERANT */

/* Defined, data sector write requests are flushed immediately to the driver. */

/* #define FX_FAULT_TOLERANT_DATA */
#endif /* FX_USER_H */
