/**
  ******************************************************************************
  * file           : example.c
  * brief          : This example provides a short description of how to use the
  *                  Mbed TLS PSA opaque driver with STM32 Key Wrap Engine to wrap ECDSA
  *                  keys and use the wrapped key to export the public key.
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
#include "main.h"
#include "crypto.h"

/* Global variables ----------------------------------------------------------*/
hal_rng_handle_t *pRNG;  /* pointer referencing the RNG handle from the generated code */
app_status_t return_status = EXEC_STATUS_ERROR;
psa_status_t retval;
size_t computed_size;
/* Define key attributes */
psa_key_attributes_t key_attributes;
psa_key_handle_t key_handle_private;
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** Extract from SigGen.txt
  * [P-256,SHA-224]

Msg = ff624d0ba02c7b6370c1622eec3fa2186ea681d1659e0a845448e777b75a8e77a77bb26e5733179d58ef9bc8a4e8b69
71aef2539f77ab0963a3415bbd6258339bd1bf55de65db520c63f5b8eab3d55debd05e9494212170f5d65b3286b8b668705b1
e2b2b5568610617abb51d2dd0cb450ef59df4b907da90cfa7b268de8c4c2

d = 708309a7449e156b0db70e5b52e606c7e094ed676ce8953bf6c14757c826f590
Qx = 29578c7ab6ce0d11493c95d5ea05d299d536801ca9cbd50e9924e43b733b83ab
Qy = 08c8049879c6278b2273348474158515accaa38344106ef96803c5a05adc4800
k = 58f741771620bdc428e91a32d86d230873e9140336fcfb1e122892ee1d501bdc

R = 4a19274429e40522234b8785dc25fc524f179dcc95ff09b3c9770fc71f54ca0d
S = 58982b79a65b7320f5b92d13bdaecdd1259e760f0f718ba933fd098f6f75d4b7

  */

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

/* Computed data buffer */
uint8_t Computed_Pub_Key[sizeof(Public_Key)];
/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * The applicative code initializes the RNG instance and Wrap the private key
  * using key import operation and KWE driver location.
  * This step is scheduled as follows:
  * Step 1.1: Initializes the RNG instance.
  * Step 1.2: Wrap the private key using key import operation and KWE driver location.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  /* initializes the RNG instance */
  pRNG = mx_example_rng_init();

  if (pRNG == NULL)
  {
    goto _app_init_exit;
  }
  else
  {
    PRINTF("[INFO] Step 1.1: RNG initialization COMPLETED.\n");
  }

  /* Init the key attributes */
  key_attributes = psa_key_attributes_init();

  retval = psa_crypto_init();
  if (retval != PSA_SUCCESS)
  {
    goto _app_init_exit;
  }
  /* Setup the key policy for the private key */
  psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_SIGN_HASH);
  psa_set_key_algorithm(&key_attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_224));
  psa_set_key_type(&key_attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1));
  psa_set_key_bits(&key_attributes, 8U * sizeof(Private_Key));

  /* Set up the key location using PSA_KEY_LOCATION_LOCAL_STORAGE to wrap the private key */
  psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_VOLATILE,
                       PSA_KEY_LOCATION_LOCAL_STORAGE));

  /* Import the private key */
  retval = psa_import_key(&key_attributes, Private_Key, sizeof(Private_Key), &key_handle_private);
  if (retval != PSA_SUCCESS)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1.2: key configuration COMPLETED.\n");

  /* Reset the key attribute */
  psa_reset_key_attributes(&key_attributes);

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * This application demonstrates how to use the predefined curves parameters
  * to perform ECC operation in a single call method: the configuration of
  * the algorithm, and the operation is done in one single API call.
  * This step is scheduled as follows:
  * Step 2.1: Export the public key from the wrapped private key.
  * Step 2.2: Verify computed public key data size is the expected one.
  */
app_status_t app_process(void)
{
  /* Export the public key from the wrapped private key */
  retval = psa_export_public_key(key_handle_private, Computed_Pub_Key, sizeof(Computed_Pub_Key), &computed_size);
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: Export of the public key from the wrapped private key COMPLETED.\n");

  /* Verify computed public key data size is the expected one */
  if (computed_size != sizeof(Computed_Pub_Key))
  {
    goto _app_process_exit;
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Computed_Pub_Key, Public_Key, computed_size) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: Verifies computed public key data size COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Deinitializes the RNG instance and Destroy the key to free up resources.
  * This step is scheduled as follows:
  * Step 3.1: Destroy the PSA key and clear all data.
  * Step 3.2: Clear all data associated with the PSA layer.
  */
app_status_t app_deinit(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  /* Destroy the PSA key and clear all data */
  retval = psa_destroy_key(key_handle_private);
  if (retval != PSA_SUCCESS)
  {
    goto _app_deinit_exit;
  }

  /* Clear all data associated with the PSA layer */
  mbedtls_psa_crypto_free();

  PRINTF("[INFO] Step 3.1: Cleanup COMPLETED.\n");

  mx_example_rng_deinit();

  PRINTF("[INFO] Step 3.2: RNG deinitialization COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_deinit_exit:
  return return_status;
} /* end app_deinit */
