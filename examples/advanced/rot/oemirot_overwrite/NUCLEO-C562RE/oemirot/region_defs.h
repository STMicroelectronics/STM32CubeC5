/**
  * Copyright (c) 2017-2018 ARM Limited
  *
  * Copyright (c) 2018-2026 STMicroelectronics.
  * All rights reserved.
  *
  * Licensed under the Apache License, Version 2.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  *     http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  */

#ifndef REGION_DEFS_H
#define REGION_DEFS_H

#include "flash_layout.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*********************** Hardware constraints ********************************/

/* OEMiRoT SRAM
   SRAM2 is selected because ECC and Erase on reset capable.
   SRAM_ROT_xxxx definitions are aligned with RAM_ROT_xxxx definitions of the linker file.
*/
#define SRAM_ROT_BASE                       (0x20010000U)
#define SRAM_ROT_SIZE                       (0x00005000U)
/* SRAM usable at start of appli: SRAM1 selected */
#define SRAM_BASE                           (0x20000000U) /* SRAM1 base address */
#define SRAM_SIZE                           (0x00010000U) /* SRAM1 size */

/* FLASH base address */
#define ROM_BASE                            (FLASH_BASE) /* User FLASH base address */

/* BL descriptor memory */
#define EXITHDPLIB_BASE                     (EXITHDPLIB_SYS_FLASH_PFUNC_START)
#define EXITHDPLIB_SIZE                     (0x200U)

/* Peripherals */
/* #define PERIPH_BASE dfp definition is used */
#define PERIPH_SIZE                         (0x10000000U)

/* System memory */
#define RO_AND_SYSTEM_MEM_BASE              (0x08FFE000U)
#define RO_AND_SYSTEM_MEM_SIZE              (0x01E00U)

/*********************** Generic definitions *********************************/

/* Alias definitions for secure and non-secure ROM areas */
#define ROM_ALIAS_BASE                      (ROM_BASE)
#define ROM_ALIAS(x)                        (ROM_ALIAS_BASE + (x))

/* Alias for flash_map source code compatibility (in mw) */
#define FLASH_BASE_ADDRESS                  (ROM_BASE)

/* RoT code region */
#define ROT_CODE_START                      (ROM_ALIAS(FLASH_ROT_CODE_OFFSET))
#define ROT_CODE_SIZE                       (FLASH_ROT_CODE_SIZE)
#define ROT_CODE_END                        (ROT_CODE_START + ROT_CODE_SIZE - 1U)

/* RoT VTOR address */
#define ROT_VTOR_ADDR                       ((uint32_t) ROT_CODE_START)

/* RoT slot areas regions */
#define ROT_AREAS_START                     (ROM_ALIAS(FLASH_AREAS_START_OFFSET))
#define ROT_AREAS_END                       (ROM_ALIAS(FLASH_AREAS_END_OFFSET))

/* SRAM region */
#define BOOT_SHARED_DATA_SIZE               (0x400U) /* 1KB */
#define ROT_DATA_START                      (SRAM_ROT_BASE)
#define ROT_DATA_SIZE                       (SRAM_ROT_SIZE - BOOT_SHARED_DATA_SIZE)
#define BOOT_SHARED_DATA_BASE               (ROT_DATA_START + ROT_DATA_SIZE)

/* app regions */
#define CODE_START                          (ROM_ALIAS(APP_CODE_OFFSET))
#define CODE_SIZE                           (APP_CODE_SIZE)
#define CODE_END                            (CODE_START + CODE_SIZE - 1U)

/* MPU SRAM region */
#define SRAM_START                          (SRAM_BASE)
#define SRAM_END                            (SRAM_BASE + SRAM_SIZE + SRAM_ROT_SIZE - 1U)

/* MPU Peripherals region */
#define PERIPH_START                        (PERIPH_BASE)
#define PERIPH_END                          (PERIPH_BASE + PERIPH_SIZE - 1U)

/* Hash ref region */
#define HASH_REF_START                      (ROM_ALIAS(FLASH_HASH_REF_OFFSET))
#define HASH_REF_END                        (HASH_REF_START + FLASH_HASH_REF_SIZE - 1U)

/* MPU RO and system memory region (for access to BL function pointers and device memory size) */
#define RO_AND_SYSTEM_MEM_START             (RO_AND_SYSTEM_MEM_BASE)
#define RO_AND_SYSTEM_MEM_END               (RO_AND_SYSTEM_MEM_BASE + RO_AND_SYSTEM_MEM_SIZE - 1U)

/* MPU RO memory region (for access to EXITHDPLIB function pointers) */
#define EXITHDPLIB_START                    (EXITHDPLIB_BASE)
#define EXITHDPLIB_END                      (EXITHDPLIB_BASE + EXITHDPLIB_SIZE - 1U)

/* non volatile counter region */
#define KEYS_START                          (ROM_ALIAS(FLASH_KEYS_OFFSET))
#define KEYS_END                            (KEYS_START + FLASH_KEYS_SIZE - 1U)

/* MPU region for hash ref + nv counters + keys */
#define ROT_BOOT_DATA_START                 (KEYS_START)
#define ROT_BOOT_DATA_END                   (HASH_REF_END)

/* Mapping order checks */
#if (MAX(FLASH_ROT_CODE_OFFSET, FLASH_KEYS_OFFSET) != FLASH_KEYS_OFFSET)
#error FLASH_ROT_CODE_OFFSET > FLASH_KEYS_OFFSET
#endif /* (MAX(FLASH_ROT_CODE_OFFSET, FLASH_KEYS_OFFSET) != FLASH_KEYS_OFFSET) */
#if (MAX(FLASH_KEYS_OFFSET, FLASH_NVCNT_OFFSET) != FLASH_NVCNT_OFFSET)
#error FLASH_KEYS_OFFSET > FLASH_NVCNT_OFFSET
#endif /* (MAX(FLASH_KEYS_OFFSET, FLASH_NVCNT_OFFSET) != FLASH_NVCNT_OFFSET) */
#if (MAX(FLASH_NVCNT_OFFSET, FLASH_HASH_REF_OFFSET) != FLASH_HASH_REF_OFFSET)
#error FLASH_NVCNT_OFFSET > FLASH_HASH_REF_OFFSET
#endif /* (MAX(FLASH_NVCNT_OFFSET, FLASH_HASH_REF_OFFSET) != FLASH_HASH_REF_OFFSET) */
#if (MAX(FLASH_HASH_REF_OFFSET, FLASH_PRIMARY_APP_SLOT_OFFSET) != FLASH_PRIMARY_APP_SLOT_OFFSET)
#error FLASH_HASH_REF_OFFSET > FLASH_PRIMARY_APP_SLOT_OFFSET
#endif /* (MAX(FLASH_HASH_REF_OFFSET, FLASH_PRIMARY_APP_SLOT_OFFSET) != FLASH_PRIMARY_APP_SLOT_OFFSET) */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* REGION_DEFS_H */
