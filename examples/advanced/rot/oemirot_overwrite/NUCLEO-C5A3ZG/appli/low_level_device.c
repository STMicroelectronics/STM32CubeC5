/**
  ******************************************************************************
  * @file    low_level_device.c
  * @brief   This file contains device definition for low_level_device
  ******************************************************************************
  * @attention
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
#include "low_level_flash.h"
#include "appli_flash_layout.h"

static  flash_range_t ACCESS_VECT[] =
{
  { FLASH_SECONDARY_APP_SLOT_OFFSET, FLASH_SECONDARY_APP_SLOT_OFFSET + FLASH_SECONDARY_APP_SLOT_SIZE - 1},
#if (DATA_IMAGE_NUMBER == 1)
  { FLASH_SECONDARY_DATA_SLOT_OFFSET, FLASH_SECONDARY_DATA_SLOT_OFFSET + FLASH_SECONDARY_DATA_SLOT_SIZE - 1},
#endif /* (DATA_IMAGE_NUMBER == 1) */
};
#if defined(OVERWRITE_ONLY)
#define WRITE_VECT  ACCESS_VECT
#else
static  flash_range_t WRITE_VECT [] =
{
  /* Areas for writing candidate images */
  { FLASH_SECONDARY_APP_SLOT_OFFSET, FLASH_SECONDARY_APP_SLOT_OFFSET + FLASH_SECONDARY_APP_SLOT_SIZE - 1},
#if (DATA_IMAGE_NUMBER == 1)
  { FLASH_SECONDARY_DATA_SLOT_OFFSET, FLASH_SECONDARY_DATA_SLOT_OFFSET + FLASH_SECONDARY_DATA_SLOT_SIZE - 1},
#endif /* (DATA_IMAGE_NUMBER == 1) */

  /* Areas for writing confirm flag on in installed image */
  { FLASH_PRIMARY_APP_CONFIRM_OFFSET, FLASH_PRIMARY_APP_CONFIRM_OFFSET + FLASH_PROG_SIZE - 1},
#if (DATA_IMAGE_NUMBER == 1)
  { FLASH_PRIMARY_DATA_CONFIRM_OFFSET, FLASH_PRIMARY_DATA_CONFIRM_OFFSET + FLASH_PROG_SIZE - 1},
#endif /* (DATA_IMAGE_NUMBER == 1) */

};
#endif /* defined(OVERWRITE_ONLY) */
low_level_device_t flash0_dev =
{
  .erase = { .nb = sizeof(ACCESS_VECT) / sizeof(flash_range_t), .range = ACCESS_VECT},
  .write = { .nb = sizeof(WRITE_VECT) / sizeof(flash_range_t), .range = WRITE_VECT },
};
