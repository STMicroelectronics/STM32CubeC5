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

/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** FileX Component                                                       */
/**                                                                       */
/**   User Specific                                                       */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#ifndef FX_USER_H
#define FX_USER_H


/* Define various build options for the FileX port.  The application should either make changes
   here by commenting or un-commenting the conditional compilation defined OR supply the defines
   though the compiler's equivalent of the -D option.  */


/* Override various options with default values already assigned in fx_api.h or fx_port.h. Please
   also refer to fx_port.h for descriptions on each of these options.  */


/* Defines the maximum size of long file names supported by FileX. The default value is 33. The
   minimum value is 13 and the maximum value is 256.  */

/* #define FX_MAX_LONG_NAME_LEN            256   */
/* #define FX_MAX_LAST_NAME_LEN            256   */


/* Defines the maximum number of logical sectors that can be cached by FileX. The cache memory
   supplied to FileX at fx_media_open determines how many sectors can actually be cached.  */

/* #define FX_MAX_SECTOR_CACHE             256   */


/* Defines the size in bytes of the bit map used to update the secondary FAT sectors. The larger the value the
   less unnecessary secondary FAT sector writes.   */

/* #define FX_FAT_MAP_SIZE                 128  */


/* Defines the number of entries in the FAT cache.  */

/* #define FX_MAX_FAT_CACHE                16   */


/* Defines the number of seconds the time parameters are updated in FileX.  */

/* #define FX_UPDATE_RATE_IN_SECONDS       10   */


/* Defines the number of RTOS ticks required to achieve the update rate.  */

/* #define FX_UPDATE_RATE_IN_TICKS         1000 */


/* Defined, FileX is built without update to the time parameters.  */

/* #define FX_NO_TIMER  */


/* Defined, FileX does not update already opened files.  */

/* #define FX_DONT_UPDATE_OPEN_FILES   */


/* Defined, the file search cache optimization is disabled.  */

/* #define FX_MEDIA_DISABLE_SEARCH_CACHE  */


/* Defined, the direct read sector update of cache is disabled.  */

/* #define FX_DISABLE_DIRECT_DATA_READ_CACHE_FILL  */


/* Defined, gathering of media statistics is disabled.  */

/* #define FX_MEDIA_STATISTICS_DISABLE  */


/* Defined, legacy single open logic for the same file is enabled.  */

/* #define FX_SINGLE_OPEN_LEGACY   */


/* Defined, renaming inherits path information.  */

/* #define FX_RENAME_PATH_INHERIT   */


/* Defined, local path logic is disabled.  */

/* #define FX_NO_LOCAL_PATH   */


/* Eliminates RTOS protection logic from the FileX source  */

/* #define FX_SINGLE_THREAD   */


/* Configure the FileX in Standalone mode */

/* #define FX_STANDALONE_ENABLE */


/* Defined, data sector write requests are flushed immediately to the driver.  */

/* #define FX_FAULT_TOLERANT_DATA  */


/* Defined, system sector write requests (including FAT and directory entry requests)
   are flushed immediately to the driver.  */

/* #define FX_FAULT_TOLERANT   */


/* Defined, enables 64-bits sector addresses used in I/O driver.  */

/* #define FX_DRIVER_USE_64BIT_LBA   */


/* Defined, enables FileX fault tolerant service.  */

/* #define FX_ENABLE_FAULT_TOLERANT   */


/* Define byte offset in boot sector where the cluster number of the Fault Tolerant Log file is.
   Note that this field (byte 116 to 119) is marked as reserved by FAT 12/16/32 specification. */

/* #define FX_FAULT_TOLERANT_BOOT_INDEX      116 */

/* Below FX_DISABLE_XXX macros can be used for code size optimization required for memory
   critical applications */

/* Defined, error checking is disabled.  */

/* #define FX_DISABLE_ERROR_CHECKING   */


/* Defined, cache is disabled.  */

/* #define FX_DISABLE_CACHE   */


/* Defined, file close is disabled.  */

/* #define FX_DISABLE_FILE_CLOSE   */


/* Defined, fast open is disabled.  */

/* #define FX_DISABLE_FAST_OPEN   */


/* Defined, force memory operations are disabled.  */

/* #define FX_DISABLE_FORCE_MEMORY_OPERATION   */


/* Defined, build options is disabled.  */

/* #define FX_DISABLE_BUILD_OPTIONS   */


/* Defined, one line function is disabled.  */

/* #define FX_DISABLE_ONE_LINE_FUNCTION   */


/* Defined, FAT entry refresh is disabled.  */

/* #define FX_DIABLE_FAT_ENTRY_REFRESH   */


/* Defined, consecutive detect is disabled.  */

/* #define FX_DISABLE_CONSECUTIVE_DETECT   */

#endif /* FX_USER_H */
