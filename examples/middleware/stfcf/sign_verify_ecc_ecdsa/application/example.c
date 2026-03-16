/**
  ******************************************************************************
  * file           : example.c
  * brief          : This example describes how to use the STM32 Flexible Crypto
  *                  Framework (STFCF) to perform the signature and verification
  *                  of a known message using the ECDSA algorithm over SECP256 curve.
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

/* Includes ------------------------------------------------------------------*/
#include "example.h"
#include "crypto.h"             /* Crypto services and defines */
#include <string.h>             /* to use memcmp function */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_rng_handle_t *pRNG;  /* pointer referencing the RNG handle from the generated code */
/* Key attributes */
psa_key_attributes_t key_attributes;
psa_key_handle_t key_handle_public;       /* Public key */
psa_key_handle_t key_handle_private_det;  /* Private deterministic key */
psa_key_handle_t key_handle_private;      /* Private key */

/*******************************************************************************
  * Extract from SigGen.txt
  * [P-256,SHA-224]
  *
  * Msg = ff624d0ba02c7b6370c1622eec3fa2186ea681d1659e0a845448e777b75a8e77a77bb26e5733179d58ef9bc8a4e8b69
  * 71aef2539f77ab0963a3415bbd6258339bd1bf55de65db520c63f5b8eab3d55debd05e9494212170f5d65b3286b8b668705b1
  * e2b2b5568610617abb51d2dd0cb450ef59df4b907da90cfa7b268de8c4c2
  * d = 708309a7449e156b0db70e5b52e606c7e094ed676ce8953bf6c14757c826f590
  * Qx = 29578c7ab6ce0d11493c95d5ea05d299d536801ca9cbd50e9924e43b733b83ab
  * Qy = 08c8049879c6278b2273348474158515accaa38344106ef96803c5a05adc4800
  * k = 58f741771620bdc428e91a32d86d230873e9140336fcfb1e122892ee1d501bdc
  * R = 4a19274429e40522234b8785dc25fc524f179dcc95ff09b3c9770fc71f54ca0d
  * S = 58982b79a65b7320f5b92d13bdaecdd1259e760f0f718ba933fd098f6f75d4b7
  *
  ******************************************************************************
  */

/* Known plaintext to be signed and verified */
const uint8_t Message[] =
{
  0xff, 0x62, 0x4d, 0x0b, 0xa0, 0x2c, 0x7b, 0x63, 0x70, 0xc1, 0x62, 0x2e, 0xec, 0x3f, 0xa2, 0x18,
  0x6e, 0xa6, 0x81, 0xd1, 0x65, 0x9e, 0x0a, 0x84, 0x54, 0x48, 0xe7, 0x77, 0xb7, 0x5a, 0x8e, 0x77,
  0xa7, 0x7b, 0xb2, 0x6e, 0x57, 0x33, 0x17, 0x9d, 0x58, 0xef, 0x9b, 0xc8, 0xa4, 0xe8, 0xb6, 0x97,
  0x1a, 0xef, 0x25, 0x39, 0xf7, 0x7a, 0xb0, 0x96, 0x3a, 0x34, 0x15, 0xbb, 0xd6, 0x25, 0x83, 0x39,
  0xbd, 0x1b, 0xf5, 0x5d, 0xe6, 0x5d, 0xb5, 0x20, 0xc6, 0x3f, 0x5b, 0x8e, 0xab, 0x3d, 0x55, 0xde,
  0xbd, 0x05, 0xe9, 0x49, 0x42, 0x12, 0x17, 0x0f, 0x5d, 0x65, 0xb3, 0x28, 0x6b, 0x8b, 0x66, 0x87,
  0x05, 0xb1, 0xe2, 0xb2, 0xb5, 0x56, 0x86, 0x10, 0x61, 0x7a, 0xbb, 0x51, 0xd2, 0xdd, 0x0c, 0xb4,
  0x50, 0xef, 0x59, 0xdf, 0x4b, 0x90, 0x7d, 0xa9, 0x0c, 0xfa, 0x7b, 0x26, 0x8d, 0xe8, 0xc4, 0xc2
};
const uint8_t Private_Key[] =
{
  0x70, 0x83, 0x09, 0xa7, 0x44, 0x9e, 0x15, 0x6b, 0x0d, 0xb7, 0x0e, 0x5b, 0x52, 0xe6, 0x06, 0xc7,
  0xe0, 0x94, 0xed, 0x67, 0x6c, 0xe8, 0x95, 0x3b, 0xf6, 0xc1, 0x47, 0x57, 0xc8, 0x26, 0xf5, 0x90
};
const uint8_t Public_Key[] =
{
  0x04, /* This octet is for a Weierstrass public key representation */
  0x29, 0x57, 0x8c, 0x7a, 0xb6, 0xce, 0x0d, 0x11, 0x49, 0x3c, 0x95, 0xd5, 0xea, 0x05, 0xd2, 0x99,
  0xd5, 0x36, 0x80, 0x1c, 0xa9, 0xcb, 0xd5, 0x0e, 0x99, 0x24, 0xe4, 0x3b, 0x73, 0x3b, 0x83, 0xab,
  0x08, 0xc8, 0x04, 0x98, 0x79, 0xc6, 0x27, 0x8b, 0x22, 0x73, 0x34, 0x84, 0x74, 0x15, 0x85, 0x15,
  0xac, 0xca, 0xa3, 0x83, 0x44, 0x10, 0x6e, 0xf9, 0x68, 0x03, 0xc5, 0xa0, 0x5a, 0xdc, 0x48, 0x00
};

const uint8_t Known_Signature[] =
{
  0x24, 0xe0, 0x43, 0x67, 0xe6, 0x01, 0x42, 0xf0, 0xa4, 0xd4, 0xa4, 0xbd, 0x19, 0x38, 0x3a, 0xc8,
  0x82, 0xe4, 0x4a, 0xd2, 0x99, 0x57, 0xe7, 0x67, 0x77, 0x0b, 0xdf, 0xa1, 0xd8, 0xf0, 0xf7, 0x9c,
  0xed, 0x38, 0x82, 0xf1, 0xc6, 0x37, 0xd8, 0x26, 0x9b, 0xb6, 0xa1, 0xe7, 0x2d, 0x6b, 0x9d, 0x61,
  0xb8, 0x89, 0xfc, 0x24, 0x84, 0x3c, 0x51, 0xe3, 0xec, 0xe7, 0xe7, 0xd2, 0x9d, 0x28, 0x36, 0xa4
};

/* Computed data buffer */
uint8_t Computed_Hash[28]; /* SHA224 length */
uint8_t Computed_Signature[sizeof(Known_Signature)];


/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the RNG instance and PSA Crypto API.
  * This step is divided into sub-steps:
  * Step 1.1: Initializes the RNG instance.
  * Step 1.2: Initializes and configures the PSA Crypto API.
  */
app_status_t app_init(void)
{
  psa_status_t retval;
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Init the RNG */
  pRNG = mx_example_rng_init();

  if (pRNG == NULL)
  {
    goto _app_init_exit;
  }
  PRINTF("[INFO] Step 1.1: RNG initialization COMPLETED.\n");

  /* Init the key attributes */
  key_attributes = psa_key_attributes_init();

  /* Init the PSA */
  retval = psa_crypto_init();
  if (retval != PSA_SUCCESS)
  {
    goto _app_init_exit;
  }

  /* Setup the key policy for public key */
  psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_VERIFY_HASH);
  psa_set_key_algorithm(&key_attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_224));
  psa_set_key_type(&key_attributes, PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1));

  /* Import a public key */
  retval = psa_import_key(&key_attributes, Public_Key, sizeof(Public_Key), &key_handle_public);
  if (retval != PSA_SUCCESS)
  {
    goto _app_init_exit;
  }

  /* Reset the key attributes to free any auxiliary resources that the structure might contain */
  psa_reset_key_attributes(&key_attributes);

  /* Setup the key policy for deterministic private key */
  psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_SIGN_HASH);
  psa_set_key_algorithm(&key_attributes, PSA_ALG_DETERMINISTIC_ECDSA(PSA_ALG_SHA_224));
  psa_set_key_type(&key_attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1));
  psa_set_key_bits(&key_attributes, 8U * sizeof(Private_Key));

  /* Set up the key location using STFCF_KEY_LOCATION. If this latter is set to PSA_CRYPTO_KWE_DRIVER_LOCATION,
       the private key will be wrapped using STM32 Key Wrap Engine (KWE). If PSA_KEY_LOCATION_LOCAL_STORAGE is set,
       then the key is transparent*/
  psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_VOLATILE, \
                       STFCF_KEY_LOCATION));

  /* Import a deterministic private key */
  retval = psa_import_key(&key_attributes, Private_Key, sizeof(Private_Key), &key_handle_private_det);
  if (retval != PSA_SUCCESS)
  {
    goto _app_init_exit;
  }

  /* Reset the key attribute */
  psa_reset_key_attributes(&key_attributes);

  /* Setup the key policy for private key */
  psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_SIGN_HASH);
  psa_set_key_algorithm(&key_attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_224));
  psa_set_key_type(&key_attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1));
  psa_set_key_bits(&key_attributes, 8U * sizeof(Private_Key));

  /* Import a private key */
  retval = psa_import_key(&key_attributes, Private_Key, sizeof(Private_Key), &key_handle_private);
  if (retval != PSA_SUCCESS)
  {
    goto _app_init_exit;
  }

  /* Reset the key attribute */
  psa_reset_key_attributes(&key_attributes);

  PRINTF("[INFO] Step 1.2: PSA key configuration COMPLETED.\n");

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Demonstrates how to use the STFCF to perform the signature
  * and verification of a known message using the ECDSA algorithm over SECP256 curve.
  *
  * This step is scheduled as follows:
  * Step 2.1: Computes and verifies the digest
  * Step 2.2: Computes and verifies signature using a known random
  * Step 2.3: Computes and verifies signature using a true random
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  psa_status_t retval;
  size_t computed_size;

  /** ########## Step 2.1 ##########
    * Computes and verifies the digest
    */
  retval = psa_hash_compute(PSA_ALG_SHA_224,
                            Message, sizeof(Message), /* Message to digest */
                            Computed_Hash,            /* Data buffer to receive digest data */
                            sizeof(Computed_Hash),    /* Size of hash buffer */
                            &computed_size);          /* Size of computed digest */

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Computed_Hash))
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.1: Compute and verify digest COMPLETED.\n");

  /** ########## Step 2.2 ##########
    * Computes and verifies signature using a known random
    */
  retval = psa_sign_hash(key_handle_private_det,
                         PSA_ALG_DETERMINISTIC_ECDSA(PSA_ALG_SHA_224),   /* Algorithm type */
                         Computed_Hash, sizeof(Computed_Hash),           /* Digest to sign */
                         Computed_Signature, sizeof(Computed_Signature), /* Data buf to receive signature*/
                         &computed_size);                                /* Size of computed signature */

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Known_Signature))
  {
    goto _app_process_exit;
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Computed_Signature, Known_Signature, computed_size) != 0)
  {
    goto _app_process_exit;
  }

  /* Verify directly the signature passing all the needed parameters */
  retval = psa_verify_hash(key_handle_public,
                           PSA_ALG_ECDSA(PSA_ALG_SHA_224),            /* Algorithm type */
                           Computed_Hash, sizeof(Computed_Hash),      /* Digest to verify */
                           Known_Signature, sizeof(Known_Signature)); /* Signature to verify */

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.2: Compute and verify signature using a known random COMPLETED.\n");

  /** ########## Step 2.3 ##########
    * Computes and verifies signature using a true random
    */
  retval = psa_sign_hash(key_handle_private,
                         PSA_ALG_ECDSA(PSA_ALG_SHA_224),                 /* Algorithm type */
                         Computed_Hash, sizeof(Computed_Hash),           /* Digest to sign */
                         Computed_Signature, sizeof(Computed_Signature), /* Data buf to receive signature*/
                         &computed_size);                                /* Size of computed signature */

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Known_Signature))
  {
    goto _app_process_exit;
  }

  /* Verify directly the signature passing all the needed parameters */
  retval = psa_verify_hash(key_handle_public,
                           PSA_ALG_ECDSA(PSA_ALG_SHA_224),                  /* Algorithm type */
                           Computed_Hash, sizeof(Computed_Hash),            /* Digest to verify */
                           Computed_Signature, sizeof(Computed_Signature)); /* Signature to verify */

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.3: Compute and verify signature using a true random COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Deinitializes the RNG instance and Destroy the key to free up resources.
  * This step is scheduled as follows:
  * Step 3.1: Destroys the PSA key and clears all data.
  * Step 3.2: Deinitializes the RNG instance.
  */
app_status_t app_deinit(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  psa_status_t retval;

  retval = psa_destroy_key(key_handle_public);
  if (retval != PSA_SUCCESS)
  {
    goto _app_deinit_exit;
  }

  /* Destroy the private key */
  retval = psa_destroy_key(key_handle_private);
  if (retval != PSA_SUCCESS)
  {
    goto _app_deinit_exit;
  }

  /* Destroy the private deterministic key */
  retval = psa_destroy_key(key_handle_private_det);
  if (retval != PSA_SUCCESS)
  {
    goto _app_deinit_exit;
  }

  /* Clear all data associated with the PSA layer */
  mbedtls_psa_crypto_free();

  PRINTF("[INFO] Step 3.1: Cleanup COMPLETED.\n");

  /* Deinitialize RNG */
  mx_example_rng_deinit();

  PRINTF("[INFO] Step 3.2: RNG deinitialization COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_deinit_exit:
  return return_status;
} /* end app_deinit */
