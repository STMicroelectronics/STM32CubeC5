/**
  ******************************************************************************
  * @file    mx_kwe_config.h
  * @brief   Config file for the PSA Key Wrap Engine driver
  *          This file must be copied to the application folder and included
  *          by mx_stfcf_config.h
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_stfcf_license.md file
  * in the same directory as the generated code.
  * If no mx_stfcf_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_KWE_CONFIG_H
#define MX_KWE_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/** @addtogroup KWE_MODULES
  * @{
  */

/** @addtogroup CONFIG
  * @brief
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Key Wrap Engine PSA Opaque Driver Location */
#define PSA_CRYPTO_KWE_DRIVER_LOCATION ((psa_key_location_t)0x00800001)
/* Exported variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define mx_stfcf_kwe_saes_gethandle mx_example_saes_gethandle
#define mx_stfcf_kwe_ccb_gethandle mx_example_ccb_gethandle

/* Exported functions ------------------------------------------------------- */

/**
  * \def KWE_DRIVER_ENABLED
  *
  * Enables STM32 KWE Driver core an implementation to use ST hardware crypto
  * accelerator that manages asymmetric and symmetric keys in wrapped forme.
  *
  * Uncomment a macro to enable the STM32 KWE PSA Driver core for ST key
  * wrap engine.
  *
  * Requires HAL_MODULE_ENABLED.
  */
#define KWE_DRIVER_ENABLED

/**
  * \def PSA_KWE_DRIVER_ENABLED
  *
  * Enables STM32 KWE PSA Driver Interface an implementation of PSA opaque
  * driver for the ST hardware crypto accelerator that manages
  * asymmetric and symmetric keys in wrapped forme.
  *
  * Uncomment a macro to enable the STM32 KWE PSA Driver Interface for ST
  * key wrap engine.
  *
  * Requires KWE_DRIVER_ENABLED.
  */
#define PSA_KWE_DRIVER_ENABLED

/**
  * \def KWE_ASYMMETRIC_KEY_WRAP_ENABLED
  *
  * Enables STM32 KWE PSA Driver that manages asymmetric keys in wrapped forme.
  *
  * Uncomment a macro to enable asymmetric key wrap operations on STM32 key
  * key wrap engine.
  *
  * Requires PSA_KWE_DRIVER_ENABLED.
  */
#define KWE_ASYMMETRIC_KEY_WRAP_ENABLED

/**
  * \def KWE_USE_CBC_TO_WRAP_SYMMETRIC_KEY
  *
  * Enables symmetric key wrap using AES-CBC algorithm, by default symmetric
  * keys are wrapped using AES-ECB algorithm.
  *
  * Uncomment a macro to enable symmetric key wrap operations using AES-CBC
  * algorithm.
  *
  * Requires PSA_KWE_DRIVER_ENABLED.
  */
/* #define KWE_USE_CBC_TO_WRAP_SYMMETRIC_KEY */

/**
  * \def KWE_ECP_SHORT_WEIERSTRASS_ENABLED
  *
  * Enables Short Weierstrass elliptic curve forme.
  *
  * Uncomment a macro to enable ECC public key export in Short Weierstrass
  * elliptic curve forme.
  *
  * Requires PSA_KWE_DRIVER_ENABLED.
  */
/* #define KWE_ECP_SHORT_WEIERSTRASS_ENABLED */

#ifdef __cplusplus
}
#endif

#endif /* MX_KWE_CONFIG_H */
/**
  * @}
  */

/**
  * @}
  */
