/**
  ******************************************************************************
  * file           : example.c
  * brief          : This example describes how to use the STM32 Flexible Crypto
  *                  Framework (STFCF) to establish and verify a shared
  *                  secret using the ECDH algorithm over SECP256 curve.
  *
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
#include <string.h>
#include "example.h"
#include "crypto.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_rng_handle_t *pRNG;  /* pointer referencing the RNG handle from the generated code */
/* Key definition */
psa_key_attributes_t key_attributes;
psa_key_handle_t key_handle_private;      /* Private key */
psa_key_handle_t key_handle_public;       /* Public key */

/*******************************************************************************
  * Extract from KAS_ECC_CDH_PrimitiveTest.txt
  * [P-256]
  *
  * COUNT = 0
  * QCAVSx = 700c48f77f56584c5cc632ca65640db91b6bacce3a4df6b42ce7cc838833d287
  * QCAVSy = db71e509e3fd9b060ddb20ba5c51dcc5948d46fbf640dfe0441782cab85fa4ac
  * dIUT = 7d7dc5f71eb29ddaf80d6214632eeae03d9058af1fb6d22ed80badb62bc1a534
  * QIUTx = ead218590119e8876b29146ff89ca61770c4edbbf97d38ce385ed281d8a6b230
  * QIUTy = 28af61281fd35e2fa7002523acc85a429cb06ee6648325389f59edfce1405141
  * ZIUT = 46fc62106420ff012e54a434fbdd2d25ccc5852060561e68040dd7778997bd7b
  *
  ******************************************************************************
  */

const uint8_t Private_Key[] =
{
  0x7d, 0x7d, 0xc5, 0xf7, 0x1e, 0xb2, 0x9d, 0xda, 0xf8, 0x0d, 0x62, 0x14, 0x63, 0x2e, 0xea, 0xe0,
  0x3d, 0x90, 0x58, 0xaf, 0x1f, 0xb6, 0xd2, 0x2e, 0xd8, 0x0b, 0xad, 0xb6, 0x2b, 0xc1, 0xa5, 0x34
};
const uint8_t Remote_Public_Key[] =
{
  0x04, /* This octet is for a Weierstrass public key representation */
  0x70, 0x0c, 0x48, 0xf7, 0x7f, 0x56, 0x58, 0x4c, 0x5c, 0xc6, 0x32, 0xca, 0x65, 0x64, 0x0d, 0xb9,
  0x1b, 0x6b, 0xac, 0xce, 0x3a, 0x4d, 0xf6, 0xb4, 0x2c, 0xe7, 0xcc, 0x83, 0x88, 0x33, 0xd2, 0x87,
  0xdb, 0x71, 0xe5, 0x09, 0xe3, 0xfd, 0x9b, 0x06, 0x0d, 0xdb, 0x20, 0xba, 0x5c, 0x51, 0xdc, 0xc5,
  0x94, 0x8d, 0x46, 0xfb, 0xf6, 0x40, 0xdf, 0xe0, 0x44, 0x17, 0x82, 0xca, 0xb8, 0x5f, 0xa4, 0xac
};
const uint8_t Expected_SecretX[] =
{
  0x46, 0xfc, 0x62, 0x10, 0x64, 0x20, 0xff, 0x01, 0x2e, 0x54, 0xa4, 0x34, 0xfb, 0xdd, 0x2d, 0x25,
  0xcc, 0xc5, 0x85, 0x20, 0x60, 0x56, 0x1e, 0x68, 0x04, 0x0d, 0xd7, 0x77, 0x89, 0x97, 0xbd, 0x7b
};

/* Computed data buffer */
uint8_t Computed_Secret[sizeof(Expected_SecretX)];


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

  /* Setup the key policy for private key */
  psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_DERIVE);
  psa_set_key_algorithm(&key_attributes, PSA_ALG_ECDH);
  psa_set_key_type(&key_attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1));

  /* Import a private key */
  retval = psa_import_key(&key_attributes, Private_Key, sizeof(Private_Key), &key_handle_private);
  if (retval != PSA_SUCCESS)
  {
    goto _app_init_exit;
  }

  /* Reset the key attribute */
  psa_reset_key_attributes(&key_attributes);

  /* Setup the key policy for public key */
  psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_DERIVE);
  psa_set_key_algorithm(&key_attributes, PSA_ALG_ECDH);
  psa_set_key_type(&key_attributes, PSA_KEY_TYPE_DERIVE);
  /* Set up the key location using STFCF_KEY_LOCATION. If this latter is set to PSA_CRYPTO_KWE_DRIVER_LOCATION,
     the private key will be wrapped using STM32 Key Wrap Engine (KWE). If PSA_KEY_LOCATION_LOCAL_STORAGE is set,
     then the key is transparent*/
  psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_VOLATILE, \
                       STFCF_KEY_LOCATION));
  /* Import a public key */
  retval = psa_import_key(&key_attributes, Remote_Public_Key, sizeof(Remote_Public_Key), &key_handle_public);
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
  * Demonstrates how to use the STFCF to establish and verify the exchange of
  * shared secret using ECC algorithm configuration and operation in a single call method.
  * This step is scheduled as follows:
  * Step 2.1: Establishes a shared secret.
  * Step 2.2: Verifies the exchange of the shared secret.
  */
app_status_t app_process(void)
{
  psa_status_t retval;
  app_status_t return_status = EXEC_STATUS_ERROR;
  size_t computed_size;

  /* Derive a key */
  retval = psa_raw_key_agreement(PSA_ALG_ECDH,
                                 key_handle_private,        /* Private key to use to compute the secret */
                                 Remote_Public_Key,         /* Public key to use to compute the secret */
                                 sizeof(Remote_Public_Key), /* Size of public key */
                                 Computed_Secret,           /* Data buffer to receive shared secret */
                                 sizeof(Computed_Secret),   /* Size of buffer to receive shared secret */
                                 &computed_size);           /* Size of computed secret */

  PRINTF("[INFO] Step 2.1: Establish a shared secret COMPLETED.\n");

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Computed_Secret))
  {
    goto _app_process_exit;
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Computed_Secret, Expected_SecretX, sizeof(Expected_SecretX)) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: Verify the exchange of the shared secret COMPLETED.\n");

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
  psa_status_t retval;
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Destroy the public key */
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
