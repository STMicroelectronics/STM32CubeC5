/**
  ******************************************************************************
  * file           : example.c
  * brief          : This example provides a short description of how to use the
  *                  Mbed TLS PSA opaque driver with STM32 Key Wrap Engine to generate
  *                  ECDSA wrapped key.
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
#include "kwe_psa_driver_interface.h"

/* Global variables ----------------------------------------------------------*/
hal_rng_handle_t *pRNG;  /* pointer referencing the RNG handle from the generated code */
psa_status_t retval;
size_t computed_size;
/* Define key attributes */
psa_key_attributes_t key_attributes;
psa_key_handle_t key_handle_private;
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define PSA_ECC_KEY_ID_USER  ((psa_key_id_t)0x1fff0001)
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the RNG instance and configure the key.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1.1 ##########
    * Initialize the RNG instance
    */

  pRNG = mx_example_rng_init();
  if (pRNG == NULL)
  {
    goto _app_init_exit;
  }
  PRINTF("[INFO] Step 1.1: RNG initialization COMPLETED.\n");

  /** ########## Step 1.2 ##########
    * Creates the PSA keys
    */

  /* Init the PSA */
  retval = psa_crypto_init();
  if (retval != PSA_SUCCESS)
  {
    goto _app_init_exit;
  }

  /* Init the key attributes */
  key_attributes = psa_key_attributes_init();

  /* Setup the key policy for the key pair */
  psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_SIGN_HASH);
  psa_set_key_algorithm(&key_attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_224));
  psa_set_key_type(&key_attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1));
  psa_set_key_bits(&key_attributes, 32 * 8U);

  /* Set up the key location using PSA_CRYPTO_KWE_DRIVER_LOCATION to wrap the private key
       using STM32 Key Wrap Engine (KWE) */
  psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_VOLATILE,
                       STFCF_KEY_LOCATION));

  PRINTF("[INFO] Step 1.2: PSA Initialization COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;
_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Demonstrates how to generate the ECDSA private key based on Key Wrap Engine
  *
  * This step is scheduled as follows:
  * Step 2: Generate a private key
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  psa_status_t retval;

  /* Generate a private key */
  retval = psa_generate_key(&key_attributes, &key_handle_private);
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2: Private key generation COMPLETED.\n");

  return_status = EXEC_STATUS_OK;
_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Destroys the PSA key before leaving the scenario and deinitializes the RNG instance.
  *
  * This step is scheduled as follows:
  * Step 3.1: Deinitializes the RNG instance.
  * Step 3.2: Destroy the PSA key and clear all data.
  */
app_status_t app_deinit(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 3.1 ##########
    * Destroys the PSA keys and clears all data
    */

  /* Reset the key attribute */
  psa_reset_key_attributes(&key_attributes);

  /* Destroy the PSA key */
  retval = psa_destroy_key(key_handle_private);
  if (retval != PSA_SUCCESS)
  {
    goto _app_deinit_exit;
  }

  /* Clear all data associated with the PSA layer */
  mbedtls_psa_crypto_free();

  PRINTF("[INFO] Step 3.1: PSA keys Destruction and all data clearance COMPLETED.\n");

  /** ########## Step 3.2 ##########
    * Deinitializes the RNG instance
    */

  mx_example_rng_deinit();

  PRINTF("[INFO] Step 3.2: RNG deinitialization COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_deinit_exit:
  return return_status;
} /* end app_deinit */
