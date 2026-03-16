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

static STM32_FX_NAND_FLASH_DRIVER_CONTEXT mx_stm32_fx_nand_flash_ctx_0;
static LX_NAND_FLASH mx_stm32_fx_nand_flash_0;
static MX_FILEX_MEDIA_FORMAT_CONFIG media_0_format_cfg;

ULONG mx_lx_nand_driver_buffer[8192];
/* get the nand flash internal object for media_0*/
VOID *mx_filex_driver_0_get_ctx()
{
  VOID * mx_lx_driver_0_ctx;

  mx_stm32_fx_nand_flash_ctx_0.stm32_fx_nand_flash_flags       = STM32_FX_NAND_FLAG_NONE;
  /* Get the NAND flash context */
  mx_lx_driver_0_ctx = mx_lx_nand_flash_driver_0_get_ctx();
  /* save it in the respective LX_NAND_FLASH control block */
  mx_stm32_fx_nand_flash_0.lx_nand_flash_driver_info_ptr = mx_lx_driver_0_ctx;

  mx_stm32_fx_nand_flash_ctx_0.stm32_lx_nand_flash_handle      = &mx_stm32_fx_nand_flash_0;
  mx_stm32_fx_nand_flash_ctx_0.stm32_lx_nand_flash_driver_init = mx_lx_nand_flash_driver_0_initialize;
  mx_stm32_fx_nand_flash_ctx_0.stm32_lx_nand_flash_driver_context = mx_lx_driver_0_ctx;
  mx_stm32_fx_nand_flash_ctx_0.stm32_lx_nand_driver_buffer = mx_lx_nand_driver_buffer;
  mx_stm32_fx_nand_flash_ctx_0.stm32_lx_nand_driver_buffer_size = sizeof(mx_lx_nand_driver_buffer);
  return (VOID *)&mx_stm32_fx_nand_flash_ctx_0;
}

/* get the fx_media_format() params for media_0 */
MX_FILEX_MEDIA_FORMAT_CONFIG *mx_filex_driver_0_get_media_format_config()
{

  /* prepare the format_config */
  media_0_format_cfg.bytes_per_sector    = LX_NAND_FLASH_0_SECTOR_SIZE; /* Sector size must match the NAND page size defined by LX_NAND_FLASH_0_SECTOR_SIZE. */
  media_0_format_cfg.total_sectors       = LX_NAND_FLASH_0_SIZE / LX_NAND_FLASH_0_SECTOR_SIZE;
  media_0_format_cfg.directory_entries   = 32U;
  media_0_format_cfg.hidden_sectors      = 0U;

  media_0_format_cfg.heads               = 1U;
  media_0_format_cfg.number_of_fats      = 1U;
  media_0_format_cfg.sectors_per_cluster = 8UL;
  media_0_format_cfg.sectors_per_track   = 1U;

  strcpy(media_0_format_cfg.volume_name, "MEDIA_0");

  return &media_0_format_cfg;
}
