/**
  ******************************************************************************
  * file           : mx_filex_interfaces.c
  * brief          : generate functions to initialize filex interfaces
  ******************************************************************************
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

#include "mx_filex_interfaces.h"

static LX_NOR_FLASH                       mx_stm32_fx_nor_flash_0;
static STM32_FX_NOR_FLASH_DRIVER_CONTEXT  mx_stm32_fx_nor_flash_ctx_0;

static MX_FILEX_MEDIA_FORMAT_CONFIG       media_0_format_cfg;

#ifndef LX_NOR_DISABLE_EXTENDED_CACHE
static ULONG extended_cache_memory[((LX_NOR_FLASH_0_BITMAP_CACHE_SIZE + LX_NOR_FLASH_0_OBSOLETE_COUNT_CACHE_SIZE)) / 4];
#endif

/* get the nor flash internal object */
VOID *mx_filex_driver_0_get_ctx(VOID)
{
  VOID *mx_lx_driver_0_ctx;

  mx_stm32_fx_nor_flash_ctx_0.stm32_fx_nor_flash_flags       = STM32_FX_NOR_FLAG_NONE;

  /* Get the NOR flash context */
  mx_lx_driver_0_ctx = mx_lx_nor_flash_driver_0_get_ctx();
  /* save it in the respective LX_NOR_FLASH control block */

  mx_stm32_fx_nor_flash_ctx_0.stm32_lx_nor_flash_handle         = &mx_stm32_fx_nor_flash_0;
  mx_stm32_fx_nor_flash_ctx_0.stm32_lx_nor_flash_driver_init    = mx_lx_nor_flash_driver_0_initialize;
  mx_stm32_fx_nor_flash_ctx_0.stm32_lx_nor_flash_driver_context = mx_lx_driver_0_ctx;


#ifndef LX_NOR_DISABLE_EXTENDED_CACHE
  mx_stm32_fx_nor_flash_ctx_0.extended_cache_memory          = (VOID *)extended_cache_memory;
  mx_stm32_fx_nor_flash_ctx_0.extended_cache_memory_size     = sizeof(extended_cache_memory);
#endif

  return (VOID *)&mx_stm32_fx_nor_flash_ctx_0;
}

/* get the fx_media_format() params */
MX_FILEX_MEDIA_FORMAT_CONFIG *mx_filex_driver_0_get_media_format_config()
{

  /* prepare the format_config */
  media_0_format_cfg.bytes_per_sector    = LX_NOR_SECTOR_SIZE * sizeof(ULONG);
  media_0_format_cfg.total_sectors       = LX_NOR_FLASH_0_SIZE / LX_NOR_SECTOR_SIZE * sizeof(ULONG);
  media_0_format_cfg.directory_entries   = 32U;
  media_0_format_cfg.hidden_sectors      = 0U;

  media_0_format_cfg.heads               = 1U;
  media_0_format_cfg.number_of_fats      = 1U;
  media_0_format_cfg.sectors_per_cluster = 8UL;
  media_0_format_cfg.sectors_per_track   = 1U;

  strcpy(media_0_format_cfg.volume_name, "MEDIA_0");

  return &media_0_format_cfg;
}
