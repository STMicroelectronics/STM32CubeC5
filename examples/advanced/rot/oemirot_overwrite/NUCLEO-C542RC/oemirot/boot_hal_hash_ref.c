/**
  ******************************************************************************
  * @file    boot_hal_hash_ref.c
  * @brief   This file contains hash ref service implementation
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "boot_hal_hash_ref.h"
#include "low_level_flash.h"

#include "mx_hal_def.h"
#include "bootutil_priv.h"

static uint8_t ImageValidHashUpdate = 0U;
static uint8_t ImageValidHashRef[MCUBOOT_IMAGE_NUMBER * SHA_LEN] = {0U};

/* exported variables --------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* exported functions --------------------------------------------------------*/
/**
  * @brief This function load all hash references from flash.
  * @return 0 on success; nonzero on failure.
  */
int32_t boot_hash_ref_load(void)
{
  /* Read hash references */
  if (FLASH_DEV_NAME.ReadData(FLASH_HASH_REF_OFFSET, ImageValidHashRef,
                              (SHA_LEN * MCUBOOT_IMAGE_NUMBER)) != ARM_DRIVER_OK)
  {
    /* Erase hash invalid reference flash sector (i.e: Reset Double ECC error) */
    (void)FLASH_DEV_NAME.EraseSector(FLASH_HASH_REF_OFFSET);
    return BOOT_EFLASH;
  }

  return 0;
}

/**
  * @brief This function store all hash references in flash.
  * @return 0 on success; nonzero on failure.
  */
int32_t boot_hash_ref_store(void)
{
  if (ImageValidHashUpdate != 0)
  {
    /* Erase hash references flash sector */
    if (FLASH_DEV_NAME.EraseSector(FLASH_HASH_REF_OFFSET) != ARM_DRIVER_OK)
    {
      return BOOT_EFLASH;
    }

    /* Store new hash references in flash */
    if (FLASH_DEV_NAME.ProgramData(FLASH_HASH_REF_OFFSET, ImageValidHashRef,
                                   (SHA_LEN * MCUBOOT_IMAGE_NUMBER)) != ARM_DRIVER_OK)
    {
      return BOOT_EFLASH;
    }
  }

  return 0;
}

/**
  * @brief This function set one hash reference in ram.
  * @param hash_ref hash reference to update
  * @param size size of the hash references
  * @param image_index index of image corresponding to hash reference
  * @return 0 on success; nonzero on failure.
  */
int32_t boot_hash_ref_set(uint8_t *hash_ref, uint8_t size, uint8_t image_index)
{
  /* Check size */
  if (size != SHA_LEN)
  {
    return BOOT_EFLASH;
  }

  /* Check image index */
  if (image_index >= MCUBOOT_IMAGE_NUMBER)
  {
    return BOOT_EFLASH;
  }

  /* Set hash reference */
  (void)memcpy(ImageValidHashRef + (image_index * SHA_LEN), hash_ref, SHA_LEN);

  /* Memorize that hash references will have to be updated in flash (later) */
  ImageValidHashUpdate++;

  return 0;
}

/**
  * @brief This function get one hash reference from ram.
  * @param hash_ref hash reference to get
  * @param size size of the hash reference
  * @param image_index index of image corresponding to hash reference
  * @return 0 on success; nonzero on failure.
  */
int32_t boot_hash_ref_get(uint8_t *hash_ref, uint8_t size, uint8_t image_index)
{
  /* Check size */
  if (size != SHA_LEN)
  {
    return BOOT_EFLASH;
  }

  /* Check image index */
  if (image_index >= MCUBOOT_IMAGE_NUMBER)
  {
    return BOOT_EFLASH;
  }

  /* Get hash reference */
  (void)memcpy(hash_ref, ImageValidHashRef + (image_index * SHA_LEN), SHA_LEN);

  return 0;
}
