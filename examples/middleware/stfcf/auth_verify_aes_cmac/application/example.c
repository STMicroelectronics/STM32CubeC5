/**
  ******************************************************************************
  * file           : example.c
  * brief          : This example describes how to use the STM32 Flexible Crypto
  *                  Framework (STFCF) to authenticate and to verify
  *                  a message using AES CMAC algorithm.
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
#define CHUNK_SIZE  48u
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_rng_handle_t *pRNG;  /* pointer referencing the RNG handle from the generated code */
psa_status_t retval;
/* Key attributes */
psa_key_attributes_t key_attributes;
psa_key_handle_t key_handle;

/** Extract from NIST Special Publication 800-38A
  * Example #4
  *
  * Key is
  * 2B7E1516 28AED2A6 ABF71588 09CF4F3C
  * Mlen=64
  *
  * PT is
  * 6BC1BEE2 2E409F96 E93D7E11 7393172A
  * AE2D8A57 1E03AC9C 9EB76FAC 45AF8E51
  * 30C81C46 A35CE411 E5FBC119 1A0A52EF
  * F69F2445 DF4F9B17 AD2B417B E66C3710
  *
  *...
  *
  * Tag is
  * 51F0BEBF 7E3B9D92 FC497417 79363CFE
  * ---------------------------------------------------------------------
  */

/* Known plaintext to be signed and verified */
const uint8_t Key[] =
{
  0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
};
const uint8_t Message[] =
{
  0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
  0xAE, 0x2D, 0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C, 0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 0x8E, 0x51,
  0x30, 0xC8, 0x1C, 0x46, 0xA3, 0x5C, 0xE4, 0x11, 0xE5, 0xFB, 0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF,
  0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F, 0x9B, 0x17, 0xAD, 0x2B, 0x41, 0x7B, 0xE6, 0x6C, 0x37, 0x10
};
const uint8_t Expected_Tag[] =
{
  0x51, 0xF0, 0xBE, 0xBF, 0x7E, 0x3B, 0x9D, 0x92, 0xFC, 0x49, 0x74, 0x17, 0x79, 0x36, 0x3C, 0xFE
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

  /* Setup the key policy for the key pair */
  psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH);
  psa_set_key_algorithm(&key_attributes, PSA_ALG_CMAC);
  psa_set_key_type(&key_attributes, PSA_KEY_TYPE_AES);
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
  * Demonstrates how to use  the STFCF to authenticate and to verify
  * a message using the AES CMAC algorithm.
  * This step is scheduled as follows:
  * Step 2.1: Computes and verifies the message using single API call method.
  * Step 2.2: Computes and verifies the message using multiple API calls method.
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
    * Computes and verifies the message using single API call method.
    */
  /* Compute directly the authentication tag passing all the needed parameters */
  retval = psa_mac_compute(key_handle,               /* The key id */
                           PSA_ALG_CMAC,             /* Algorithm type */
                           Message, sizeof(Message), /* Message to authenticate */
                           Computed_Tag,             /* Data buffer to receive generated authentication tag */
                           sizeof(Computed_Tag),     /* Size of buffer to receive authentication tag */
                           &computed_size);          /* Size of computed authentication tag */

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
  /* Verify directly the message passing all the needed parameters */
  retval = psa_mac_verify(key_handle,                /* The key id */
                          PSA_ALG_CMAC,              /* Algorithm type */
                          Message, sizeof(Message),  /* Message to authenticate */
                          Expected_Tag,              /* Authentication tag */
                          sizeof(Expected_Tag));     /* tag size */

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: Compute and verify the message using single API call method COMPLETED.\n");

  /** ########## Step 2.2 ##########
    * Computes and verifies the message using multiple API calls method.
    */

  operation = psa_mac_operation_init();

  /* Setup the context for sign operation */
  retval = psa_mac_sign_setup(&operation,           /* The current operation */
                              key_handle,           /* The key id */
                              PSA_ALG_CMAC);        /* Algorithm type */


  /* Verify API returned value */
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
    retval = psa_mac_update(&operation, &Message[index], sizeof(Message) - index); /* Last part of data to authen */

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

  /* Setup the context for verify operation */
  retval = psa_mac_verify_setup(&operation,           /* The current operation */
                                key_handle,           /* The key id */
                                PSA_ALG_CMAC);        /* Algorithm type */

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
    retval = psa_mac_update(&operation, &Message[index], sizeof(Message) - index);

    /* Verify API returned value */
    if (retval != PSA_SUCCESS)
    {
      goto _app_process_exit;
    }
  }

  /* Verify authentication tag */
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

  PRINTF("[INFO] Step 2.2: Compute and verify the message using multiple API calls method COMPLETED.\n");

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

  /* Destroy the key */
  retval = psa_destroy_key(key_handle);
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
