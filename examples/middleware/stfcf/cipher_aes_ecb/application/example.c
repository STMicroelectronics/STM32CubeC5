/**
  ******************************************************************************
  * file           : example.c
  * brief          : This example describes how to use the STM32 Flexible Crypto
  *                  Framework (STFCF) to perform encryption and decryption
  *                  using the AES ECB algorithm.
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
/* Define key attributes */
psa_key_attributes_t key_attributes;
psa_key_handle_t key_handle_aes_ecb;
/*******************************************************************************
  * Extract from NIST Special Publication 800-38A
  * F.1.5 ECB-AES256.Encrypt
  * Key 603deb1015ca71be2b73aef0857d7781
  *     1f352c073b6108d72d9810a30914dff4
  * Block #1
  * Plaintext 6bc1bee22e409f96e93d7e117393172a
  * Input Block 6bc1bee22e409f96e93d7e117393172a
  * Output Block f3eed1bdb5d2a03c064b5a7e3db181f8
  * Ciphertext f3eed1bdb5d2a03c064b5a7e3db181f8
  * Block #2
  * Plaintext ae2d8a571e03ac9c9eb76fac45af8e51
  * Input Block ae2d8a571e03ac9c9eb76fac45af8e51
  * Output Block 591ccb10d410ed26dc5ba74a31362870
  * Ciphertext 591ccb10d410ed26dc5ba74a31362870
  * Block #3
  * Plaintext 30c81c46a35ce411e5fbc1191a0a52ef
  * Input Block 30c81c46a35ce411e5fbc1191a0a52ef
  * Output Block b6ed21b99ca6f4f9f153e7b1beafed1d
  * Ciphertext b6ed21b99ca6f4f9f153e7b1beafed1d
  * Block #4
  * Plaintext f69f2445df4f9b17ad2b417be66c3710
  * Input Block f69f2445df4f9b17ad2b417be66c3710
  * Output Block 23304b7a39f9f3ff067d8d8f9e24ecc7
  * Ciphertext 23304b7a39f9f3ff067d8d8f9e24ecc7
  *
  ******************************************************************************
  */
uint8_t Key[] =
{
  0x60, 0x3D, 0xEB, 0x10, 0x15, 0xCA, 0x71, 0xBE, 0x2B, 0x73, 0xAE, 0xF0, 0x85, 0x7D, 0x77, 0x81,
  0x1F, 0x35, 0x2C, 0x07, 0x3B, 0x61, 0x08, 0xD7, 0x2D, 0x98, 0x10, 0xA3, 0x09, 0x14, 0xDF, 0xF4
};

uint8_t PlainText[] =
{
  0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
  0xAE, 0x2D, 0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C, 0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 0x8E, 0x51,
  0x30, 0xC8, 0x1C, 0x46, 0xA3, 0x5C, 0xE4, 0x11, 0xE5, 0xFB, 0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF,
  0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F, 0x9B, 0x17, 0xAD, 0x2B, 0x41, 0x7B, 0xE6, 0x6C, 0x37, 0x10
};

uint8_t ExpectedCiphertext[] =
{
  0xF3, 0xEE, 0xD1, 0xBD, 0xB5, 0xD2, 0xA0, 0x3C, 0x06, 0x4B, 0x5A, 0x7E, 0x3D, 0xB1, 0x81, 0xF8,
  0x59, 0x1C, 0xCB, 0x10, 0xD4, 0x10, 0xED, 0x26, 0xDC, 0x5B, 0xA7, 0x4A, 0x31, 0x36, 0x28, 0x70,
  0xB6, 0xED, 0x21, 0xB9, 0x9C, 0xA6, 0xF4, 0xF9, 0xF1, 0x53, 0xE7, 0xB1, 0xBE, 0xAF, 0xED, 0x1D,
  0x23, 0x30, 0x4B, 0x7A, 0x39, 0xF9, 0xF3, 0xFF, 0x06, 0x7D, 0x8D, 0x8F, 0x9E, 0x24, 0xEC, 0xC7
};
/* Computed data buffer */
uint8_t Computed_Ciphertext[sizeof(ExpectedCiphertext)];
uint8_t Computed_Plaintext[sizeof(PlainText)];
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
  psa_status_t retval;
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
  /* Setup the key policy for the private key */
  psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
  psa_set_key_algorithm(&key_attributes, PSA_ALG_ECB_NO_PADDING);
  psa_set_key_type(&key_attributes, PSA_KEY_TYPE_AES);
  psa_set_key_bits(&key_attributes, 8U * sizeof(Key));
  /* Set up the key location using STFCF_KEY_LOCATION. If this latter is set to PSA_CRYPTO_KWE_DRIVER_LOCATION,
     the private key will be wrapped using STM32 Key Wrap Engine (KWE); If PSA_KEY_LOCATION_LOCAL_STORAGE is set,
     then the key is transparent*/
  psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_VOLATILE, \
                       STFCF_KEY_LOCATION));

  /* Import the private key */
  retval = psa_import_key(&key_attributes, Key, sizeof(Key), &key_handle_aes_ecb);
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
  * Demonstrates how to use the STFCF to perform an encryption and a decryption
  * of a known ciphertext using the AES ECB algorithm.
  * This step is scheduled as follows:
  * Step 2.1: Performs an encryption the plaintext, followed by verification with the expected ciphertext,
  *           passing all the needed parameters.
  * Step 2.2: Performs an encryption the ciphertext, followed by verification with the expected plaintext,
  *           passing all the needed parameters.
  */

app_status_t app_process(void)
{
  size_t computed_size;
  app_status_t return_status = EXEC_STATUS_ERROR;
  psa_status_t retval;
  /** ########## Step 2.1 ##########
    * Performs an encryption the plaintext, followed by
    * verification with the expected ciphertext,
    * passing all the needed parameters.
    */

  /* Encrypt the plaintext passing all the needed parameters */
  retval = psa_cipher_encrypt(key_handle_aes_ecb,            /* The key id */
                              PSA_ALG_ECB_NO_PADDING,        /* Algorithm type */
                              PlainText, sizeof(PlainText),  /* Plaintext to encrypt */
                              Computed_Ciphertext,           /* Data buffer to receive generated ciphertext */
                              sizeof(Computed_Ciphertext),   /* Size of buffer to receive ciphertext */
                              &computed_size);               /* Size of computed ciphertext */

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(ExpectedCiphertext))
  {
    goto _app_process_exit;
  }

  /* Verify generated data are the expected ones */
  if (memcmp(ExpectedCiphertext, Computed_Ciphertext, computed_size) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: Encryption and verification of ciphertext COMPLETED.\n");

  /** ########## Step 2.2 ##########
    * Performs an decryption the plaintext, followed by
    * verification with the expected plaintext,
    * passing all the needed parameters.
    */

  /* Decrypt directly ciphertext passing all the needed parameters */
  retval = psa_cipher_decrypt(key_handle_aes_ecb,          /* The key id */
                              PSA_ALG_ECB_NO_PADDING,      /* Algorithm type */
                              ExpectedCiphertext,          /* Ciphertext to decrypt */
                              sizeof(ExpectedCiphertext),  /* Size of IV + Ciphertext to decrypt */
                              Computed_Plaintext,          /* Data buffer to receive generated plaintext */
                              sizeof(Computed_Plaintext),  /* Size of buffer to receive plaintext */
                              &computed_size);             /* Size of computed plaintext */

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(PlainText))
  {
    goto _app_process_exit;
  }

  /* Verify generated data are the expected ones */
  if (memcmp(PlainText, Computed_Plaintext, computed_size) != 0)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.2: Decryption and verification of plaintext COMPLETED.\n");

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

  /* Destroy the private key */
  retval = psa_destroy_key(key_handle_aes_ecb);

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
