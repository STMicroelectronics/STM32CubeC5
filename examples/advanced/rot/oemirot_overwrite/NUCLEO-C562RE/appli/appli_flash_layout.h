/**
  ******************************************************************************
  * @file    appli_flash_layout.h
  * @brief   This file contains definitions of memory regions.
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

#ifndef MX_APPLI_FLASH_LAYOUT_H
#define MX_APPLI_FLASH_LAYOUT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************************************************************/
/* DEFINITIONS INHERITED FROM RoT CONFIGURATION.                              */
/* THEY ARE UPDATED AUTOMATICALLY BY APPLI PREBUILD: DO NOT MODIFY!           */
/******************************************************************************/

/* Images installation mode
   OVERWRITE_ONLY Defined: the FW installation uses overwrite method.
   OVERWRITE_ONLY UnDefined: The FW installation uses swap mode. */
#define OVERWRITE_ONLY


/* Data Images number
   0: no image
   1: data image */
#define DATA_IMAGE_NUMBER                                 (0x0U)

/* Data image primary slot offset */
#define FLASH_PRIMARY_DATA_SLOT_OFFSET                    (0x18000U)

/* Data image primary slot size */
#define FLASH_PRIMARY_DATA_SLOT_SIZE                      (0x0U)

/* App image primary slot offset */
#define FLASH_PRIMARY_APP_SLOT_OFFSET                     (0x18000U)

/* App image primary slot size */
#define FLASH_PRIMARY_APP_SLOT_SIZE                       (0x34000U)

/* App image secondary slot offset */
#define FLASH_SECONDARY_APP_SLOT_OFFSET                   (0x4C000U)

/* App image secondary slot size */
#define FLASH_SECONDARY_APP_SLOT_SIZE                     (0x34000U)

/* Data image secondary slot offset */
#define FLASH_SECONDARY_DATA_SLOT_OFFSET                  (0x80000U)

/* Data image secondary slot size */
#define FLASH_SECONDARY_DATA_SLOT_SIZE                    (0x0U)

/* App image header size */
#define APP_IMAGE_HEADER_SIZE                             0x400

/* Data image header size */
#define DATA_IMAGE_HEADER_SIZE                            0x20

/* App image secondary magic install request offset */
#define FLASH_SECONDARY_APP_INSTALL_REQ_OFFSET            (0x7FFF0U)

/* Data image secondary magic install request offset */
#define FLASH_SECONDARY_DATA_INSTALL_REQ_OFFSET           (0x0U)

#if !defined(OVERWRITE_ONLY)
/* App image primary confirmation flag offset */
#define FLASH_PRIMARY_APP_CONFIRM_OFFSET                  (0x4BFE0U)

/* Data image primary confirmation flag offset */
#define FLASH_PRIMARY_DATA_CONFIRM_OFFSET                 (0x0U)

#endif /* !defined (OVERWRITE_ONLY) */

/* App image primary version offset */
#define FLASH_PRIMARY_APP_VERSION_OFFSET                  (0x18014U)

/* Data image primary version offset */
#define FLASH_PRIMARY_DATA_VERSION_OFFSET                 (0x0U)

/* Flash Driver Used to Confirm Image(s) */
#define FLASH_PRIMARY_DEV_NAME                            Driver_FLASH0

/* Flash Driver Used to Download Image(s) */
#define FLASH_SECONDARY_DEV_NAME                          Driver_FLASH0


/******************************************************************************/
/* DEFINITIONS HW RELATED                                                     */
/******************************************************************************/

/* Flash total size */
#define FLASH_TOTAL_SIZE                                  0x100000

/* Flash base */
#ifndef FLASH_BASE
#define FLASH_BASE                                        0x08000000    /*!< Flash base address */
#endif /* FLASH_BASE */
/* Flash programmation granularity */
#define FLASH_PROG_SIZE                                   0x10

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_APPLI_FLASH_LAYOUT_H */
