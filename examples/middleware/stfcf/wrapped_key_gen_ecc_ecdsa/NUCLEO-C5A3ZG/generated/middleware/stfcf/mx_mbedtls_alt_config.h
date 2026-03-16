/**
  ******************************************************************************
  * @file    mx_mbedtls_alt_config.h
  * @brief   Configuration file for mbedtls_alt middleware
  *
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
#ifndef MX_MBEDTLS_ALT_CONFIG_H
#define MX_MBEDTLS_ALT_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mx_hal_def.h"

/* Exported macros -----------------------------------------------------------*/
#define mx_stfcf_mbedtls_alt_hal_aes_gethandle mx_example_saes_gethandle
#define mx_stfcf_mbedtls_alt_hal_hash_gethandle mx_example_hash_gethandle
#define mx_stfcf_mbedtls_alt_hal_pka_gethandle mx_example_pka_gethandle
#define mx_stfcf_mbedtls_alt_hal_rng_gethandle mx_example_rng_gethandle

/* Exported constants ------------------------------------------------------- */

/** @defgroup MBEDTLS_HAL_XXX_ALT
  * @brief    Alternative cryptography implementation based on STM32 hardware
  *           cryptographic accelerator
  * @{
  */

/**
  * @brief MBEDTLS_HAL_AES_ALT Enables ST AES alternative module to replace mbed
  *        TLS AES module by ST AES alternative implementation based on STM32
  *        AES hardware accelerator.
  *
  *        Uncomment a macro to enable ST AES hardware alternative module.
  *        Requires: MBEDTLS_AES_C, MBEDTLS_AES_ALT.
  */
#define MBEDTLS_HAL_AES_ALT

/**
  * @brief MBEDTLS_HAL_CCM_ALT Enables ST CCM alternative module to replace mbed
  *        TLS CCM module by ST CCM alternative implementation based on STM32
  *        AES hardware accelerator.
  *
  *        Uncomment a macro to enable ST CCM hardware alternative module.
  *        Requires: MBEDTLS_AES_C, MBEDTLS_CCM_C, MBEDTLS_CCM_ALT.
  */
/* #define MBEDTLS_HAL_CCM_ALT */

/**
  * @brief MBEDTLS_HAL_GCM_ALT Enables ST GCM alternative module to replace mbed
  *        TLS GCM module by ST GCM alternative implementation based on STM32
  *        AES hardware accelerator.
  *
  *        Uncomment a macro to enable ST GCM hardware alternative module.
  *        Requires: MBEDTLS_AES_C, MBEDTLS_GCM_C, MBEDTLS_GCM_ALT.
  */
/* #define MBEDTLS_HAL_GCM_ALT */

/**
  * @brief MBEDTLS_HAL_SHA1_ALT Enables ST SHA-1 alternative module to replace
  *        mbed TLS SHA-1 module by ST SHA-1 alternative implementation based
  *        on STM32 HASH hardware accelerator.
  *
  *        Uncomment a macro to enable ST SHA1 hardware alternative module.
  *        Requires: MBEDTLS_SHA1_C, MBEDTLS_SHA1_ALT.
  */
/* #define MBEDTLS_HAL_SHA1_ALT */

/**
  * @brief MBEDTLS_HAL_SHA256_ALT Enables ST SHA-224 and SHA-256 alternative
  *        modules to replace mbed TLS SHA-224 and SHA-256 modules by ST SHA-224
  *        and SHA-256 alternative implementation based on STM32 HASH hardware
  *        accelerator.
  *
  *        Uncomment a macro to enable ST SHA256 hardware alternative module.
  *        Requires: MBEDTLS_SHA256_C, MBEDTLS_SHA256_ALT.
  */
#define MBEDTLS_HAL_SHA256_ALT

/**
  * @brief MBEDTLS_HAL_SHA512_ALT Enables ST SHA-384 and SHA-512 alternative
  *        modules to replace mbed TLS SHA-384 and SHA-512 modules by ST SHA-384
  *        and SHA-512 alternative implementation based on STM32 HASH hardware
  *        accelerator.
  *
  *        Uncomment a macro to enable ST SHA512 hardware alternative module.
  *        Requires: MBEDTLS_SHA512_C, MBEDTLS_SHA512_ALT.
  */
/* #define MBEDTLS_HAL_SHA512_ALT */

/**
  * @brief MBEDTLS_HAL_ECDSA_ALT Enables ST ECDSA alternative module to replace
  *        mbed TLS ECDSA sign and  verify functions by ST ECDSA alternative
  *        implementation based on STM32 PKA hardware accelerator.
  *
  *        Uncomment a macro to enable ST ECDSA hardware alternative module.
  *        Requires: MBEDTLS_ECDSA_C, MBEDTLS_ECDSA_SIGN_ALT,
  *                  MBEDTLS_ECDSA_VERIFY_ALT, MBEDTLS_ECP_ALT.
  */
/* #define MBEDTLS_HAL_ECDSA_ALT */

/**
  * @brief MBEDTLS_HAL_ECDH_ALT Enables ST ECDH alternative module to replace
  *        mbed TLS Compute shared secret module by ST Compute shared secret
  *        alternative implementation based on STM32 PKA hardware accelerator.
  *
  *        Uncomment a macro to enable ST ECDH hardware alternative module.
  *        Requires: MBEDTLS_ECDH_C, MBEDTLS_ECDH_COMPUTE_SHARED_ALT,
  *                  MBEDTLS_ECP_ALT.
  */
/* #define MBEDTLS_HAL_ECDH_ALT */

/**
  * @brief MBEDTLS_HAL_ECP_ALT Enables ST ECP alternative module to replace
  *        mbed TLS ECP module by ST ECP alternative implementation based on
  *        STM32 PKA hardware accelerator.
  *
  *        Uncomment a macro to enable ST ECP hardware alternative module.
  *        Requires: MBEDTLS_ECP_C, MBEDTLS_ECP_ALT.
  */
#define MBEDTLS_HAL_ECP_ALT

/**
  * @brief MBEDTLS_HAL_RSA_ALT Enables ST RSA alternative module to replace
  *        mbed TLS RSA module by ST RSA alternative implementation based on
  *        STM32 PKA hardware accelerator.
  *
  *        Uncomment a macro to enable ST RSA hardware alternative module.
  *        Requires: MBEDTLS_RSA_C, MBEDTLS_RSA_ALT.
  */
/* #define MBEDTLS_HAL_RSA_ALT */

/**
  * @brief MBEDTLS_HAL_ENTROPY_HARDWARE_ALT Enables ST entropy source module
  *        to replace mbed TLS entropy module by ST entropy implementation
  *        based on STM32 RNG hardware accelerator.
  *
  *        Uncomment a macro to enable ST entropy hardware alternative module.
  *        Requires: MBEDTLS_ENTROPY_C, MBEDTLS_ENTROPY_HARDWARE_ALT.
  */
#define MBEDTLS_HAL_ENTROPY_HARDWARE_ALT

/**
  * @}
  */

/** @defgroup MBEDTLS_STCRYPTOLIB_XXX_ALT
  * @brief    Alternative cryptography implementation based on STM32
  *           Cryptographic library
  * @{
  */

/**
  * @brief MBEDTLS_STCRYPTOLIB_AES_ALT Enables ST AES alternative module to replace mbed
  *        TLS AES module by ST AES alternative implementation based on STM32
  *        Cryptographic library.
  *
  *        Uncomment a macro to enable ST AES software alternative module.
  *        Requires: MBEDTLS_AES_C, MBEDTLS_AES_ALT.
  */
/* #define MBEDTLS_STCRYPTOLIB_AES_ALT */

/**
  * @brief MBEDTLS_STCRYPTOLIB_CCM_ALT Enables ST CCM alternative module to replace mbed
  *        TLS CCM module by ST CCM alternative implementation based on STM32
  *        AES hardware accelerator.
  *
  *        Uncomment a macro to enable ST CCM hardware alternative module.
  *        Requires: MBEDTLS_AES_C, MBEDTLS_CCM_C, MBEDTLS_CCM_ALT.
  */
/* #define MBEDTLS_STCRYPTOLIB_CCM_ALT */

/**
  * @brief MBEDTLS_STCRYPTOLIB_CHACHAPOLY_ALT Enables ST CHACHAPOLY alternative module
  *        to replace mbedTLS CHACHAPOLY module by ST CHACHAPOLY alternative
  *        implementation based on STM32 Cryptographic library.
  *
  *        Uncomment a macro to enable ST CHACHAPOLY software alternative module.
  *        Requires: MBEDTLS_CHACHAPOLY_C, MBEDTLS_CHACHAPOLY_ALT.
  */
/* #define MBEDTLS_STCRYPTOLIB_CHACHAPOLY_ALT */

/**
  * @brief MBEDTLS_STCRYPTOLIB_GCM_ALT Enables ST GCM alternative module to replace mbed
  *        TLS GCM module by ST GCM alternative implementation based on STM32
  *        Cryptographic library.
  *
  *        Uncomment a macro to enable ST GCM software alternative module.
  *        Requires: MBEDTLS_AES_C, MBEDTLS_GCM_C, MBEDTLS_GCM_ALT.
  */
/* #define MBEDTLS_STCRYPTOLIB_GCM_ALT */

/**
  * @brief MBEDTLS_STCRYPTOLIB_CMAC_ALT Enables ST CMAC alternative module to replace
  *        mbed TLS CMAC module by ST CMAC alternative implementation based on
  *        STM32 Cryptographic library.
  *
  *        Uncomment a macro to enable ST CMAC software alternative module.
  *        Requires: MBEDTLS_CMAC_C, MBEDTLS_CMAC_ALT.
  */
/* #define MBEDTLS_STCRYPTOLIB_CMAC_ALT */

/**
  * @brief MBEDTLS_STCRYPTOLIB_SHA256_ALT Enables ST SHA-224 and SHA-256 alternative
  *        modules to replace mbed TLS SHA-224 and SHA-256 modules by ST SHA-224
  *        and SHA-256 alternative implementation based on STM32 Cryptographic
  *        library.
  *
  *        Uncomment a macro to enable ST SHA256 software alternative module.
  *        Requires: MBEDTLS_SHA256_C, MBEDTLS_SHA256_ALT.
  */
/* #define MBEDTLS_STCRYPTOLIB_SHA256_ALT */

/**
  * @brief MBEDTLS_STCRYPTOLIB_SHA512_ALT Enables ST SHA-384 and SHA-512 alternative
  *        modules to replace mbed TLS SHA-384 and SHA-512 modules by ST SHA-384
  *        and SHA-512 alternative implementation based on STM32 Cryptographic
  *        library.
  *
  *        Uncomment a macro to enable ST SHA512 software alternative module.
  *        Requires: MBEDTLS_SHA512_C, MBEDTLS_SHA512_ALT.
  */
/* #define MBEDTLS_STCRYPTOLIB_SHA512_ALT */

/**
  * @brief MBEDTLS_STCRYPTOLIB_ECDSA_ALT Enables ST ECDSA alternative module to replace
  *        mbed TLS ECDSA sign and  verify modules by ST ECDSA alternative
  *        implementation based on STM32 Cryptographic library.
  *
  *        Uncomment a macro to enable ST ECDSA software alternative module.
  *        Requires: MBEDTLS_ECDSA_C, MBEDTLS_ECDSA_SIGN_ALT,
  *                  MBEDTLS_ECDSA_VERIFY_ALT.
  */
/* #define MBEDTLS_STCRYPTOLIB_ECDSA_ALT */

/**
  * @brief MBEDTLS_STCRYPTOLIB_ECDH_ALT Enables ST ECDH alternative module to replace
  *        mbed TLS Compute shared secret module by ST Compute shared secret
  *        alternative implementation based on STM32 Cryptographic library.
  *
  *        Uncomment a macro to enable ST ECDH software alternative module.
  *        Requires: MBEDTLS_ECDH_C, MBEDTLS_ECDH_COMPUTE_SHARED_ALT.
  */
/* #define MBEDTLS_STCRYPTOLIB_ECDH_ALT */

/**
  * @brief MBEDTLS_STCRYPTOLIB_RSA_ALT Enables ST RSA alternative modules to replace
  *        mbed TLS RSA module by ST RSA alternative implementation based on
  *        STM32 Cryptographic library.
  *
  *        Uncomment a macro to enable ST RSA software alternative module.
  *        Requires: MBEDTLS_RSA_C, MBEDTLS_RSA_ALT.
  */
/* #define MBEDTLS_STCRYPTOLIB_RSA_ALT */

/**
  * @brief MBEDTLS_STCRYPTOLIB_DRBG_ALT Enables ST DRBG module to replace mbed TLS
  *        DRBG module by an external implementation based on STM32
  *        Cryptographic library.
  *
  *        Uncomment a macro to enable ST DRBG alternative module.
  *        Requires: MBEDTLS_PSA_CRYPTO_C, MBEDTLS_PSA_CRYPTO_EXTERNAL_RNG.
  */
/* #define MBEDTLS_STCRYPTOLIB_DRBG_ALT */
/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* MX_MBEDTLS_ALT_CONFIG_H */
