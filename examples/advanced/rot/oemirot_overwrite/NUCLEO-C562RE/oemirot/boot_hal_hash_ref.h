/**
  ******************************************************************************
  * @file    boot_hal_hash_ref.h
  * @brief   Header for hash ref services in boot_hal_hash_ref.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BOOT_HAL_HASH_REF_H
#define BOOT_HAL_HASH_REF_H

/* Includes ------------------------------------------------------------------*/
#include "flash_layout.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
#define SHA256_LENGTH             (32U)
#define SHA384_LENGTH             (48U)

#define SHA_LEN     SHA256_LENGTH

/* Exported functions ------------------------------------------------------- */
int32_t boot_hash_ref_store(void);
int32_t boot_hash_ref_load(void);
int32_t boot_hash_ref_set(uint8_t *hash_ref, uint8_t size, uint8_t image_index);
int32_t boot_hash_ref_get(uint8_t *hash_ref, uint8_t size, uint8_t image_index);

#ifdef __cplusplus
}
#endif

#endif /* BOOT_HAL_HASH_REF_H */
