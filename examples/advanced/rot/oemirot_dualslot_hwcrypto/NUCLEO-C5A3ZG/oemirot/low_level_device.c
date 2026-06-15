/**
  ******************************************************************************
  * @file    low_level_device.c
  * @brief   This file contains device definition for low_level_device
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
#include "flash_layout.h"
#include "low_level_flash.h"

static flash_range_t erase_vect[] =
{
#if defined(MCUBOOT_USE_HASH_REF)
  { FLASH_HASH_REF_OFFSET, FLASH_HASH_REF_OFFSET + FLASH_HASH_REF_SIZE - 1},
#endif /* MCUBOOT_USE_HASH_REF */
  { FLASH_AREAS_START_OFFSET, FLASH_AREAS_END_OFFSET}
};
static flash_range_t write_vect[] =
{
#if defined(MCUBOOT_USE_HASH_REF)
  { FLASH_HASH_REF_OFFSET, FLASH_HASH_REF_OFFSET + FLASH_HASH_REF_SIZE - 1},
#endif /* MCUBOOT_USE_HASH_REF */
  { FLASH_NVCNT_OFFSET, FLASH_NVCNT_OFFSET + FLASH_NVCNT_SIZE - 1},
  { FLASH_AREAS_START_OFFSET, FLASH_AREAS_END_OFFSET}
};

low_level_device_t FLASH0_DEV =
{
  .erase = { .nb = sizeof(erase_vect) / sizeof(flash_range_t), .range = erase_vect},
  .write = { .nb = sizeof(write_vect) / sizeof(flash_range_t), .range = write_vect},
  .read_error = 1
};
