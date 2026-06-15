/**
  ******************************************************************************
  * file           : mx_filex_interfaces.h
  * brief          : header for the mx_filex_interfaces.c
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

#ifndef MX_FILEX_INTERFACES_H
#define MX_FILEX_INTERFACES_H

/* Includes ------------------------------------------------------------------*/
#include "mx_levelx_interfaces.h"
#include "stm32_fx_nand_flash_driver.h"

typedef struct MX_FILEX_MEDIA_FORMAT_CONFIG_STRUCT
{
  CHAR   volume_name[11];
  UINT   number_of_fats;
  UINT   directory_entries;
  UINT   hidden_sectors;

  ULONG  total_sectors;
  UINT   bytes_per_sector;

  ULONG  sectors_per_cluster;
  UINT   heads;
  UINT   sectors_per_track;
} MX_FILEX_MEDIA_FORMAT_CONFIG;


#define mx_filex_driver_0  stm32_fx_nand_flash_driver

VOID *mx_filex_driver_0_get_ctx(VOID);

MX_FILEX_MEDIA_FORMAT_CONFIG *mx_filex_driver_0_get_media_format_config(VOID);
#endif /* MX_FILEX_INTERFACES_H */
