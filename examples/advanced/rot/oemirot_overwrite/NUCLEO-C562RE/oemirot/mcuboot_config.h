/**
  * Copyright (c) 2018 Open Source Foundries Limited
  * Copyright (c) 2019 Arm Limited
  *
  * Copyright (c) 2023-2026 STMicroelectronics.
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#ifndef MCUBOOT_CONF_H
#define MCUBOOT_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Cryptography
 */
/* Supported crypto schemes (do not change values, as used in appli postbuild script) */
#define CRYPTO_SCHEME_EC384      0x1 /* ECDSA-384 signature,
                                        AES-CTR-256 encryption with sym key ECIES-P256 encrypted */
#define CRYPTO_SCHEME_EC256      0x2 /* ECDSA-256 signature,
                                        AES-CTR-128 encryption with sym key ECIES-P256 encrypted */

/* Crypto scheme configuration */
#define CRYPTO_SCHEME            CRYPTO_SCHEME_EC256

/* Crypto scheme settings */
#define NUM_ECC_BYTES 32
#define MCUBOOT_SIGN_EC256
#define MCUBOOT_ENCRYPT_EC256

/* Crypto implementation settings */
#define MCUBOOT_USE_MBED_TLS

/*
 * General config
 */
/* Image is revalidated in primary slot after installation */
#define MCUBOOT_VALIDATE_PRIMARY_SLOT

/* Flash having the same sector size can use this flag */
#define MCUBOOT_FLASH_HOMOGENOUS

/* Newer flash map api used */
#define MCUBOOT_USE_FLASH_AREA_GET_SECTORS

/* Use image hash reference to reduce boot time (signature check bypass) */
#define MCUBOOT_USE_HASH_REF

/* Version antirollback counters */
#define MCUBOOT_HW_ROLLBACK_PROT

/* Image encryption supported */
#define MCUBOOT_ENC_IMAGES

/* Flash programmation size granularity (in bytes) */
#define MCUBOOT_BOOT_MAX_ALIGN   16

/* Images measurements (boot record) saved in shared data for next stage */
/* #define MCUBOOT_MEASURED_BOOT */
/* #define MAX_BOOT_RECORD_SZ 0x80 */

/* Additional data saved in shared data for TFM next stage */
/* #define MCUBOOT_DATA_SHARING */
/* #define TFM_PARTITION_FIRMWARE_UPDATE */

/* Hash of public key provisioned */
#define MCUBOOT_HW_KEY

/* Raw private key provisioned */
#define MCUBOOT_RAW_ENC_KEY

/* Targeted primary slot identified in images */
#define MCUBOOT_ROM_FIXED

/*
 * FIH profile
 */
#define MCUBOOT_FIH_PROFILE_HIGH

/*
 * Logging
 */
#ifdef OEMIROT_DEV_MODE
#define MCUBOOT_HAVE_LOGGING
#endif /* OEMIROT_DEV_MODE */

/*
 * Watchdog feeding
 */
#define MCUBOOT_WATCHDOG_FEED()  \
  do {                           \
    /* Do nothing. */            \
  } while (0)

/*
 * Installation mode configuration
 */
#define MCUBOOT_OVERWRITE_ONLY

/*
 * Application image number:
 * - 1: without isolation
 */
#define MCUBOOT_APP_IMAGE_NUMBER              1U

/*
 * If application
 * - without isolation: Data image number (0 or 1)
 */
#define MCUBOOT_S_DATA_IMAGE_NUMBER           (0x0U)

/*
 * If application
 * - without isolation: 0
 */
#define MCUBOOT_NS_DATA_IMAGE_NUMBER          (0x0U)

/* Required definition for project without isolation */
#define MCUBOOT_DATA_IMAGE_NUMBER MCUBOOT_S_DATA_IMAGE_NUMBER

/*
 * Total number of images
 */
#define MCUBOOT_IMAGE_NUMBER (MCUBOOT_APP_IMAGE_NUMBER + MCUBOOT_S_DATA_IMAGE_NUMBER + MCUBOOT_NS_DATA_IMAGE_NUMBER)

/*
 * Inclusion of HAL dependencies
 */
#include "mx_hal_def.h"

#ifdef __cplusplus
}
#endif

#endif /* MCUBOOT_CONF_H */
