/**
  * Copyright (c) 2018 Arm Limited. All rights reserved.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef FLASH_LAYOUT_H
#define FLASH_LAYOUT_H

/* Includes ------------------------------------------------------------------*/
#include "mcuboot_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
/*************** Hardware constraints : Begin ********************************/
#define PAGE_SIZE                            (0x2000U) /* 8 KB */
#define BANK_SIZE                            (0x80000U) /* 512 KB */
#define FLASH_TOTAL_SIZE                     (0x100000U) /* 1024 KB */
#define FLASH_VTOR_ALIGNMENT                 (0x400U) /* 1 KB */

/*************** Hardware constraints : End **********************************/

/**************** Flash layout configuration : begin *************************/

/* ROT code area (for MPU config and OB check) */
#define FLASH_ROT_CODE_OFFSET                (0x0U)
#define FLASH_ROT_CODE_SIZE                  (0x12000U)

/**
  * Slots size per image type, in bytes.
  * In overwrite mode, primary slot size = secondary slot size, no additional sector required.
  * In swap with move mode, optimal flash layout requires that:
  * - primary slot is sized for image + 1 additional sector for trailer status + 1 additional sector for move-up
  * - secondary slot is sized for image + 1 additional sector for trailer status
  * Set slot size to 0 if associated image is disabled
  */
#define FLASH_PRIMARY_DATA_SLOT_SIZE         (0x0U)
#define FLASH_PRIMARY_APP_SLOT_SIZE          (0x74000U)
#define FLASH_SECONDARY_APP_SLOT_SIZE        (0x74000U)
#define FLASH_SECONDARY_DATA_SLOT_SIZE       (0x0U)

/**
  * Slots offset per image type, in bytes.
  * Constraint: Slots order below must not be modified
  */
#define FLASH_PRIMARY_DATA_SLOT_OFFSET       (0x18000U)
#define FLASH_PRIMARY_APP_SLOT_OFFSET        (0x18000U)
#define FLASH_SECONDARY_APP_SLOT_OFFSET      (0x8C000U)
#define FLASH_SECONDARY_DATA_SLOT_OFFSET     (0x100000U)

/* area for image HASH references */
#define FLASH_HASH_REF_OFFSET                (0x16000U)
#define FLASH_HASH_REF_SIZE                  (PAGE_SIZE)

/* area for authentication & encryption keys */
#define FLASH_KEYS_OFFSET                    (0x12000U)
#define FLASH_KEYS_SIZE                      (PAGE_SIZE)

/* area for anti roll back counter */
#define FLASH_NVCNT_OFFSET                   (0x14000U)
#define FLASH_NVCNT_SIZE                     (PAGE_SIZE)

/**
  * WRP start and end offset in bytes.
  * WRP protection required on ROT code and keys.
  */
#define FLASH_ROT_WRP_START_OFFSET           (0x0U)
#define FLASH_ROT_WRP_END_OFFSET             (0x13FFFU)

/**
  * HDP end offset in bytes.
  * HDP start offset is always 0.
  * HDP protection required on ROT code/data in flash.
  */
#define FLASH_ROT_HDP_END_OFFSET             (0x17FFFU)

/*************** Flash layout configuration : end ****************************/

/**
  * Note: Some STM32 series use 'Sector' or 'Page' to describe
  * erase unit and OB protection unit. Both names can be used here.
  */
#define FLASH_SECTORS_SIZE                   PAGE_SIZE

/* Check Images number configuration */
#if (MCUBOOT_APP_IMAGE_NUMBER == 1)
#if ((MCUBOOT_DATA_IMAGE_NUMBER != 0) && (MCUBOOT_DATA_IMAGE_NUMBER != 1))
#error "Data Images number configuration error"
#endif /* ((MCUBOOT_DATA_IMAGE_NUMBER != 0) && (MCUBOOT_DATA_IMAGE_NUMBER != 1)) */
#else
#error "App Images number configuration error"
#endif /* (MCUBOOT_APP_IMAGE_NUMBER == 1) */

/* Check areas are aligned on page size */
#define ASSERT_ALIGNED(VAL) \
  _Static_assert(((VAL) % (FLASH_SECTORS_SIZE)) == 0U, "Areas size not aligned on page size")

ASSERT_ALIGNED(FLASH_ROT_CODE_OFFSET);
ASSERT_ALIGNED(FLASH_ROT_CODE_SIZE);

ASSERT_ALIGNED(FLASH_PRIMARY_DATA_SLOT_SIZE);
ASSERT_ALIGNED(FLASH_PRIMARY_APP_SLOT_SIZE);
ASSERT_ALIGNED(FLASH_SECONDARY_APP_SLOT_SIZE);
ASSERT_ALIGNED(FLASH_SECONDARY_DATA_SLOT_SIZE);
ASSERT_ALIGNED(FLASH_HASH_REF_SIZE);
ASSERT_ALIGNED(FLASH_KEYS_SIZE);
ASSERT_ALIGNED(FLASH_NVCNT_SIZE);

ASSERT_ALIGNED(FLASH_PRIMARY_DATA_SLOT_OFFSET);
ASSERT_ALIGNED(FLASH_PRIMARY_APP_SLOT_OFFSET);
ASSERT_ALIGNED(FLASH_SECONDARY_APP_SLOT_OFFSET);
ASSERT_ALIGNED(FLASH_SECONDARY_DATA_SLOT_OFFSET);

ASSERT_ALIGNED(FLASH_HASH_REF_OFFSET);
ASSERT_ALIGNED(FLASH_KEYS_OFFSET);
ASSERT_ALIGNED(FLASH_NVCNT_OFFSET);

/* Check OB protections are alignmed on page size */
ASSERT_ALIGNED(FLASH_ROT_WRP_START_OFFSET);
ASSERT_ALIGNED(FLASH_ROT_WRP_END_OFFSET + 1U);
ASSERT_ALIGNED(FLASH_ROT_HDP_END_OFFSET + 1U);

/* Flash area IDs for Images */
#define FLASH_AREA_0_ID                      (1U)   /* App primary area */
#define FLASH_AREA_2_ID                      (2U)   /* App secondary area */
#if (MCUBOOT_DATA_IMAGE_NUMBER == 1)
#define FLASH_AREA_4_ID                      (5U)   /* Data primary area */
#define FLASH_AREA_6_ID                      (6U)   /* Data secondary area */
#endif /* (MCUBOOT_DATA_IMAGE_NUMBER == 1) */

/* Mapping flash areas to image types */
#define FLASH_AREA_4_SIZE                    (FLASH_PRIMARY_DATA_SLOT_SIZE)
#define FLASH_AREA_0_SIZE                    (FLASH_PRIMARY_APP_SLOT_SIZE)
#define FLASH_AREA_2_SIZE                    (FLASH_SECONDARY_APP_SLOT_SIZE)
#define FLASH_AREA_6_SIZE                    (FLASH_SECONDARY_DATA_SLOT_SIZE)

#define FLASH_AREA_1_SIZE                    (0U)
#define FLASH_AREA_3_SIZE                    (0U)
#define FLASH_AREA_5_SIZE                    (0U)
#define FLASH_AREA_7_SIZE                    (0U)

#define FLASH_AREA_4_OFFSET                  (FLASH_PRIMARY_DATA_SLOT_OFFSET)
#define FLASH_AREA_0_OFFSET                  (FLASH_PRIMARY_APP_SLOT_OFFSET)
#define FLASH_AREA_2_OFFSET                  (FLASH_SECONDARY_APP_SLOT_OFFSET)
#define FLASH_AREA_6_OFFSET                  (FLASH_SECONDARY_DATA_SLOT_OFFSET)

#if (MCUBOOT_DATA_IMAGE_NUMBER == 1)
/* Images areas begin */
#define FLASH_AREAS_START_OFFSET             (FLASH_AREA_4_OFFSET)
/* Images areas end */
#define FLASH_AREAS_END_OFFSET               (FLASH_AREA_6_OFFSET + FLASH_AREA_6_SIZE - 1U)
#elif (MCUBOOT_DATA_IMAGE_NUMBER == 0)
/* Images areas begin */
#define FLASH_AREAS_START_OFFSET             (FLASH_AREA_0_OFFSET)
/* Images areas end */
#define FLASH_AREAS_END_OFFSET               (FLASH_AREA_2_OFFSET + FLASH_AREA_2_SIZE - 1U)
#endif /* MCUBOOT_DATA_IMAGE_NUMBER == 1 */

/* Control Images areas end fits in flash size */
#if ((FLASH_AREAS_END_OFFSET + 1) > FLASH_TOTAL_SIZE)
#error "Images areas end overflow"
#endif /* ((FLASH_AREAS_END_OFFSET + 1) > FLASH_TOTAL_SIZE) */

/* Internal flash driver */
#define FLASH_DEV_NAME                       Driver_FLASH0

/* Maximum slot size for all images (number of sectors) */
#define MAX(a, b)                            ((a) > (b) ? (a) : (b))
#define MAX4(a, b, c, d)                     (MAX(MAX(a, b), MAX(c, d)))
#define MAX8(a, b, c, d, e, f, g, h)         (MAX(MAX4(a, b, c, d), MAX4(e, f, g, h)))
#define FLASH_MAX_SLOT_SIZE                  (MAX8(FLASH_AREA_0_SIZE, \
                                                   FLASH_AREA_1_SIZE, \
                                                   FLASH_AREA_2_SIZE, \
                                                   FLASH_AREA_3_SIZE, \
                                                   FLASH_AREA_4_SIZE, \
                                                   FLASH_AREA_5_SIZE, \
                                                   FLASH_AREA_6_SIZE, \
                                                   FLASH_AREA_7_SIZE))
#define MCUBOOT_MAX_IMG_SECTORS              (FLASH_MAX_SLOT_SIZE / FLASH_SECTORS_SIZE)

/* MCUboot Images structure */
#define APP_IMAGE_HEADER_SIZE                (FLASH_VTOR_ALIGNMENT) /* required for MPU */

/* App code */
#define APP_CODE_OFFSET                      (FLASH_PRIMARY_APP_SLOT_OFFSET + APP_IMAGE_HEADER_SIZE)
#define APP_CODE_SIZE                        (FLASH_PRIMARY_APP_SLOT_SIZE - APP_IMAGE_HEADER_SIZE)

/**
  * Note: Currently MPU enables execution on whole app area (including trailer).
  * It could be improved to limit execution on app firmware size only
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FLASH_LAYOUT_H */
