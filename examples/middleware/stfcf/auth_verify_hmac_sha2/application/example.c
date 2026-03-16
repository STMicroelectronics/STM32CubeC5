/**
  ******************************************************************************
  * file           : example.c
  * brief          : This example describes how to use the STM32 Flexible Crypto
  *                  Framework (STFCF) to perform the generation and the
  *                  verification of a known message authentication code (MAC) of
  *                  a plaintext using the HMAC SHA256 algorithm.
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
#include "example.h"
#include "crypto.h"             /* Crypto services and defines */
#include <string.h>             /* to use memcmp function */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CHUNK_SIZE  48u /* Chunk size (in bytes) when data to hash are processed by chunk */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

hal_rng_handle_t *pRNG;  /* pointer referencing the RNG handle from the generated code */
psa_status_t retval;
/* Key attributes */
psa_key_attributes_t key_attributes;
psa_key_handle_t key_handle;
/** Extract from HMAC.rsp
  *
  * Klen = 40
  * Tlen = 32
  * Key = cfd4a44910c9e567507abb6cede4fe601a7a2765c9755aa2cf6ba4814223811a26a8a1ef499cebd9
  * Msg = 3fb301cb4092f9623aa5ffd690d22d65d56e5a1c330b9c4a0d910c34e391c90a76d5401a2d3caa44b8c5d5aef3e928b
  * 90d2ee233e9f9a2cec4a32cd019d06a0dc1fcb1125f5746a4fbd32169ed7bf0e4fd065fa7c8ac97c366380484495f5c5b6850
  * dd1c9d8cd6694cf8686e46308ed0ed1f5bdf98cd831339771db63de5a7de
  * Mac = 20153bf8ea2953c48251ebcc4161f8b6e28499e5c76c24014cff4a9e2f62d25c
  */

const uint8_t Key[] =
{
  0xcf, 0xd4, 0xa4, 0x49, 0x10, 0xc9, 0xe5, 0x67, 0x50, 0x7a, 0xbb, 0x6c, 0xed, 0xe4, 0xfe, 0x60,
  0x1a, 0x7a, 0x27, 0x65, 0xc9, 0x75, 0x5a, 0xa2, 0xcf, 0x6b, 0xa4, 0x81, 0x42, 0x23, 0x81, 0x1a,
  0x26, 0xa8, 0xa1, 0xef, 0x49, 0x9c, 0xeb, 0xd9
};
/* Known plaintext to be signed and verified */
const uint8_t Message[] =
{
  0x3f, 0xb3, 0x01, 0xcb, 0x40, 0x92, 0xf9, 0x62, 0x3a, 0xa5, 0xff, 0xd6, 0x90, 0xd2, 0x2d, 0x65,
  0xd5, 0x6e, 0x5a, 0x1c, 0x33, 0x0b, 0x9c, 0x4a, 0x0d, 0x91, 0x0c, 0x34, 0xe3, 0x91, 0xc9, 0x0a,
  0x76, 0xd5, 0x40, 0x1a, 0x2d, 0x3c, 0xaa, 0x44, 0xb8, 0xc5, 0xd5, 0xae, 0xf3, 0xe9, 0x28, 0xb9,
  0x0d, 0x2e, 0xe2, 0x33, 0xe9, 0xf9, 0xa2, 0xce, 0xc4, 0xa3, 0x2c, 0xd0, 0x19, 0xd0, 0x6a, 0x0d,
  0xc1, 0xfc, 0xb1, 0x12, 0x5f, 0x57, 0x46, 0xa4, 0xfb, 0xd3, 0x21, 0x69, 0xed, 0x7b, 0xf0, 0xe4,
  0xfd, 0x06, 0x5f, 0xa7, 0xc8, 0xac, 0x97, 0xc3, 0x66, 0x38, 0x04, 0x84, 0x49, 0x5f, 0x5c, 0x5b,
  0x68, 0x50, 0xdd, 0x1c, 0x9d, 0x8c, 0xd6, 0x69, 0x4c, 0xf8, 0x68, 0x6e, 0x46, 0x30, 0x8e, 0xd0,
  0xed, 0x1f, 0x5b, 0xdf, 0x98, 0xcd, 0x83, 0x13, 0x39, 0x77, 0x1d, 0xb6, 0x3d, 0xe5, 0xa7, 0xde
};
const uint8_t Expected_Tag[] =
{
  0x20, 0x15, 0x3b, 0xf8, 0xea, 0x29, 0x53, 0xc4, 0x82, 0x51, 0xeb, 0xcc, 0x41, 0x61, 0xf8, 0xb6,
  0xe2, 0x84, 0x99, 0xe5, 0xc7, 0x6c, 0x24, 0x01, 0x4c, 0xff, 0x4a, 0x9e, 0x2f, 0x62, 0xd2, 0x5c
};

/* Computed data buffer */
uint8_t Computed_Tag[sizeof(Expected_Tag)];

/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * The applicative code initializes the RNG instance and PSA Crypto API.
  * This step is divided into sub-steps:
  * Step 1.1: Initializes the RNG instance.
  * Step 1.2: Initializes and configures the PSA Crypto API.
  */
app_status_t app_init(void)
{
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

  /* Setup the key policy */
  psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH);
  psa_set_key_algorithm(&key_attributes, PSA_ALG_HMAC(PSA_ALG_SHA_256));
  psa_set_key_type(&key_attributes, PSA_KEY_TYPE_HMAC);
  psa_set_key_bits(&key_attributes, 8U * sizeof(Key));
  /* Set up the key location using STFCF_KEY_LOCATION. If this latter is set to PSA_CRYPTO_KWE_DRIVER_LOCATION,
     the private key will be wrapped using STM32 Key Wrap Engine (KWE). If PSA_KEY_LOCATION_LOCAL_STORAGE is set,
     then the key is transparent*/
  psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_VOLATILE, \
                       STFCF_KEY_LOCATION));
  /* Import a key */
  retval = psa_import_key(&key_attributes, Key, sizeof(Key), &key_handle);
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
  * Demonstrates how to use the STFCF to perform the generation and the verification of a known message
  * authentication code (MAC) of a plaintext using the HMAC-SHA256 algorithm in two ways.
  * This step is scheduled as follows:
  * Step 2.1: Generates and verifies a message authentication code (MAC) using single API call method.
  * Step 2.2: Verifies the message with the given authentication tag using single API call method.
  * Step 2.3: Generates and verifies a message authentication code (MAC) using multiple API calls method.
  * Step 2.4: Verifies the message with the given authentication tag using multiple API calls method.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  size_t computed_size;
  /* General operation */
  psa_mac_operation_t operation;
  /* Index for piecemeal processing */
  uint32_t index;


  /** ########## Step 2.1 ##########
    * Generates and verifies the mac of a known message using single API call method.
    */
  /* Compute directly the authentication tag passing all the needed parameters */
  retval = psa_mac_compute(key_handle,                    /* The key id */
                           PSA_ALG_HMAC(PSA_ALG_SHA_256), /* Algorithm type */
                           Message, sizeof(Message),      /* Message to authenticate */
                           Computed_Tag,                  /* Data buffer to receive generated authentication tag */
                           sizeof(Computed_Tag),          /* Size of buffer to receive authentication tag */
                           &computed_size);               /* Size of computed authentication tag */

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Expected_Tag))
  {
    goto _app_process_exit;
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Expected_Tag, Computed_Tag, sizeof(Expected_Tag)) != 0)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.1: MAC generation and verification using single API call method COMPLETED.\n");

  /** ########## Step 2.2 ##########
    * Verifies the message passing all the needed parameters using single API call method.
    */
  retval = psa_mac_verify(key_handle,                    /* The key id */
                          PSA_ALG_HMAC(PSA_ALG_SHA_256), /* Algorithm type */
                          Message, sizeof(Message),      /* Message to authenticate */
                          Expected_Tag,                  /* Authentication tag */
                          sizeof(Expected_Tag));         /* tag size */

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: MAC verification using single API call method COMPLETED.\n");

  /** ########## Step 2.3 ##########
    * Generates and verifies a message authentication code (MAC) using multiple API calls method.
    */
  operation = psa_mac_operation_init();

  /* Setup the context for sign operation */
  retval = psa_mac_sign_setup(&operation,                     /* The current operation */
                              key_handle,                     /* The key id */
                              PSA_ALG_HMAC(PSA_ALG_SHA_256)); /* Algorithm type */

  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Process with the authentication of the message by chunk of CHUNK_SIZE Bytes */
  for (index = 0; index < (sizeof(Message) - CHUNK_SIZE); index += CHUNK_SIZE)
  {
    retval = psa_mac_update(&operation, &Message[index], CHUNK_SIZE); /* Chunk of data to authenticate */

    /* Verify API returned value */
    if (retval != PSA_SUCCESS)
    {
      goto _app_process_exit;
    }
  }
  /* Process with the authentication of the last part if needed */
  if (index < sizeof(Message))
  {
    /* Last part of data to authenticate */
    retval = psa_mac_update(&operation, &Message[index], sizeof(Message) - index);

    /* Verify API returned value */
    if (retval != PSA_SUCCESS)
    {
      goto _app_process_exit;
    }
  }

  /* Generate the authentication tag */
  retval = psa_mac_sign_finish(&operation,
                               Computed_Tag,          /* Data buffer to receive the computed tag */
                               sizeof(Computed_Tag),  /* Size of buffer to receive the computed tag */
                               &computed_size);       /* Size of computed tag */

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Expected_Tag))
  {
    goto _app_process_exit;
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Expected_Tag, Computed_Tag, computed_size) != 0)
  {
    goto _app_process_exit;
  }

  /* Free resources */
  retval = psa_mac_abort(&operation);
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.3: MAC generation and verification using multiple API calls method COMPLETED.\n");

  /** ########## Step 2.4 ##########
    * Verifies the message with the given authentication tag using multiple API calls method.
    */
  /* Setup the context for verify operation */
  retval = psa_mac_verify_setup(&operation,                     /* The current operation */
                                key_handle,                     /* The key id */
                                PSA_ALG_HMAC(PSA_ALG_SHA_256)); /* Algorithm type */

  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Process with the authentication of the message by chunk of CHUNK_SIZE Bytes */
  for (index = 0; index < (sizeof(Message) - CHUNK_SIZE); index += CHUNK_SIZE)
  {
    retval = psa_mac_update(&operation, &Message[index], CHUNK_SIZE); /* Chunk of data to authenticate */

    /* Verify API returned value */
    if (retval != PSA_SUCCESS)
    {
      goto _app_process_exit;
    }
  }
  /* Process with the authentication of the last part if needed */
  if (index < sizeof(Message))
  {
    /* Last part of data to authenticate */
    retval = psa_mac_update(&operation, &Message[index], sizeof(Message) - index);

    /* Verify API returned value */
    if (retval != PSA_SUCCESS)
    {
      goto _app_process_exit;
    }
  }

  /* Verify the authentication tag */
  retval = psa_mac_verify_finish(&operation, Expected_Tag, sizeof(Expected_Tag));

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Free resources */
  retval = psa_mac_abort(&operation);
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.4: MAC verification using multiple API calls method COMPLETED.\n");

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

  /* Destroy the PSA key and clear all data */
  retval = psa_destroy_key(key_handle);
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
