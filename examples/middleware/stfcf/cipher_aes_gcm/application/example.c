/**
  ******************************************************************************
  * file           : example.c
  * brief          : This example describes how to use the STM32 Flexible Crypto
  *                  Framework (STFCF) to perform authenticated encryption/decryption
  *                  with associated data (AEAD/ADAD)
  *                  using the AES GCM algorithm.
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_rng_handle_t *pRNG;  /* pointer referencing the RNG handle from the generated code */
/* Key attributes */
psa_key_attributes_t key_attributes;
psa_key_handle_t key_handle;

/*******************************************************************************
  * Extract from  NIST Special Publication 800-38D
  * for more information about next variables please see:
  * https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38d.pdf
  * gcmEncryptExtIV256.rsp
  * [Keylen = 256]
  * [IVlen = 96]
  * [PTlen = 408]
  * [AADlen = 384]
  * [Taglen = 128]

  * Count = 0
  * Key = 463b412911767d57a0b33969e674ffe7845d313b88c6fe312f3d724be68e1fca
  * IV = 611ce6f9a6880750de7da6cb
  * PT = e7d1dcf668e2876861940e012fe52a98dacbd78ab63c08842cc9801ea581682ad54af0c34d0d7f6f59e8ee0bf4900e0fd85042
  * AAD = 0a682fbc6192e1b47a5e0868787ffdafe5a50cead3575849990cdd2ea9b3597749403efb4a56684f0c6bde352d4aeec5
  * CT = 8886e196010cb3849d9c1a182abe1eeab0a5f3ca423c3669a4a8703c0f146e8e956fb122e0d721b869d2b6fcd4216d7d4d3758
  * Tag = 2469cecd70fd98fec9264f71df1aee9a
  *
  ******************************************************************************
  */
const uint8_t Key[] =
{
  0x46, 0x3b, 0x41, 0x29, 0x11, 0x76, 0x7d, 0x57, 0xa0, 0xb3, 0x39, 0x69, 0xe6, 0x74, 0xff, 0xe7,
  0x84, 0x5d, 0x31, 0x3b, 0x88, 0xc6, 0xfe, 0x31, 0x2f, 0x3d, 0x72, 0x4b, 0xe6, 0x8e, 0x1f, 0xca
};
const uint8_t IV[] =
{
  0x61, 0x1c, 0xe6, 0xf9, 0xa6, 0x88, 0x07, 0x50, 0xde, 0x7d, 0xa6, 0xcb
};
const uint8_t plainText[] =
{
  0xe7, 0xd1, 0xdc, 0xf6, 0x68, 0xe2, 0x87, 0x68, 0x61, 0x94, 0x0e, 0x01, 0x2f, 0xe5, 0x2a, 0x98,
  0xda, 0xcb, 0xd7, 0x8a, 0xb6, 0x3c, 0x08, 0x84, 0x2c, 0xc9, 0x80, 0x1e, 0xa5, 0x81, 0x68, 0x2a,
  0xd5, 0x4a, 0xf0, 0xc3, 0x4d, 0x0d, 0x7f, 0x6f, 0x59, 0xe8, 0xee, 0x0b, 0xf4, 0x90, 0x0e, 0x0f,
  0xd8, 0x50, 0x42
};
const uint8_t addData[] =
{
  0x0a, 0x68, 0x2f, 0xbc, 0x61, 0x92, 0xe1, 0xb4, 0x7a, 0x5e, 0x08, 0x68, 0x78, 0x7f, 0xfd, 0xaf,
  0xe5, 0xa5, 0x0c, 0xea, 0xd3, 0x57, 0x58, 0x49, 0x99, 0x0c, 0xdd, 0x2e, 0xa9, 0xb3, 0x59, 0x77,
  0x49, 0x40, 0x3e, 0xfb, 0x4a, 0x56, 0x68, 0x4f, 0x0c, 0x6b, 0xde, 0x35, 0x2d, 0x4a, 0xee, 0xc5
};
const uint8_t expectedCipherText[] =
{
  /* Ciphertext */
  0x88, 0x86, 0xe1, 0x96, 0x01, 0x0c, 0xb3, 0x84, 0x9d, 0x9c, 0x1a, 0x18, 0x2a, 0xbe, 0x1e, 0xea,
  0xb0, 0xa5, 0xf3, 0xca, 0x42, 0x3c, 0x36, 0x69, 0xa4, 0xa8, 0x70, 0x3c, 0x0f, 0x14, 0x6e, 0x8e,
  0x95, 0x6f, 0xb1, 0x22, 0xe0, 0xd7, 0x21, 0xb8, 0x69, 0xd2, 0xb6, 0xfc, 0xd4, 0x21, 0x6d, 0x7d,
  0x4d, 0x37, 0x58,
};
const uint8_t expectedTag[] =
{
  0x24, 0x69, 0xce, 0xcd, 0x70, 0xfd, 0x98, 0xfe, 0xc9, 0x26, 0x4f, 0x71, 0xdf, 0x1a, 0xee, 0x9a
};

const uint8_t expectedCiphertextAndTag[] =
{
  0x88, 0x86, 0xe1, 0x96, 0x01, 0x0c, 0xb3, 0x84, 0x9d, 0x9c, 0x1a, 0x18, 0x2a, 0xbe, 0x1e, 0xea,
  0xb0, 0xa5, 0xf3, 0xca, 0x42, 0x3c, 0x36, 0x69, 0xa4, 0xa8, 0x70, 0x3c, 0x0f, 0x14, 0x6e, 0x8e,
  0x95, 0x6f, 0xb1, 0x22, 0xe0, 0xd7, 0x21, 0xb8, 0x69, 0xd2, 0xb6, 0xfc, 0xd4, 0x21, 0x6d, 0x7d,
  0x4d, 0x37, 0x58,
  0x24, 0x69, 0xce, 0xcd, 0x70, 0xfd, 0x98, 0xfe, 0xc9, 0x26, 0x4f, 0x71, 0xdf, 0x1a, 0xee, 0x9a
};

/* Computed data buffer */
uint8_t Computed_Ciphertext[sizeof(expectedCipherText) + sizeof(expectedTag)];
uint8_t Computed_Plaintext[sizeof(plainText)];

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

  /* Setup the key policy */
  psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
  psa_set_key_algorithm(&key_attributes, PSA_ALG_GCM);
  psa_set_key_type(&key_attributes, PSA_KEY_TYPE_AES);
  psa_set_key_bits(&key_attributes, 8U * sizeof(Key));
  /* Set up the key location using STFCF_KEY_LOCATION. If this latter is set to PSA_CRYPTO_KWE_DRIVER_LOCATION,
     the private key will be wrapped using STM32 Key Wrap Engine (KWE). If PSA_KEY_LOCATION_LOCAL_STORAGE is set,
     then the key is transparent*/
  psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_VOLATILE, \
                       STFCF_KEY_LOCATION));
  /* Import a volatile key for AES GCM in binary format */
  /*  to create an object that is equivalent to the public key. */
  retval = psa_import_key(&key_attributes, Key, sizeof(Key), &key_handle);
  if (retval != PSA_SUCCESS)
  {
    goto _app_init_exit;
  }

  /* Reset the key attribute to frees any auxiliary resources that the structure might contain */
  psa_reset_key_attributes(&key_attributes);

  PRINTF("[INFO] Step 1.2: PSA key configuration COMPLETED.\n");

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Demonstrates how to use the STFCF to perform an encryption and a decryption
  * of a known ciphertext using the AES GCM algorithm.
  * This step is scheduled as follows:
  * Step 2.1: Performs authenticated encryption with associated data (AEAD).
  * Step 2.2: Performs authenticated decryption with associated data (ADAD)
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  psa_status_t retval;
  size_t computed_size;

  /** ########## Step 2.1 ##########
    * Performs an authenticated encryption of a known plaintext,
    * followed by the verification of the generated ciphertext
    */

  /* Compute directly the ciphertext passing all the needed parameters */
  retval = psa_aead_encrypt(key_handle,                         /* The key id */
                            PSA_ALG_GCM,                        /* Algorithm type */
                            IV, sizeof(IV),                     /* Initialization vector */
                            addData, sizeof(addData),           /* Additional authenticated data */
                            plainText, sizeof(plainText),       /* Plaintext to encrypt and authenticate */
                            Computed_Ciphertext,                /* Data buffer to receive ciphertext and auth tag */
                            sizeof(Computed_Ciphertext),        /* Size of buffer to receive ciphertext and tag */
                            &computed_size);                    /* Size of computed ciphertext */

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != (sizeof(expectedCipherText) + sizeof(expectedTag)))
  {
    goto _app_process_exit;
  }

  /* Verify generated data are the expected ones */
  if (memcmp(expectedCipherText, Computed_Ciphertext, sizeof(expectedCipherText)) != 0)
  {
    goto _app_process_exit;
  }

  /* Verify generated authentication tag is the expected one */
  if (memcmp(expectedTag, &Computed_Ciphertext[sizeof(expectedCipherText)], sizeof(expectedTag)) != 0)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.1: Authenticated encryption with associated data (AEAD) COMPLETED.\n");

  /** ########## Step 2.2 ##########
    * Performs an authenticated decryption of a known ciphertext,
    * followed by the verification of the generated plaintext.
    */

  /* Decrypt and verify directly ciphertext and tag passing all the needed parameters */
  retval = psa_aead_decrypt(key_handle,                          /* The key id */
                            PSA_ALG_GCM,                         /* Algorithm type */
                            IV, sizeof(IV),                      /* Initialization vector */
                            addData, sizeof(addData),            /* Additional authenticated data */
                            expectedCiphertextAndTag,            /* Ciphertext + tag to decrypt and verify */
                            sizeof(expectedCiphertextAndTag),    /* Sizeof Ciphertext + tag to decrypt and verify */
                            Computed_Plaintext,                  /* Data buffer to receive generated plaintext */
                            sizeof(Computed_Plaintext),          /* Size of data buff to receive generated plaintext */
                            &computed_size);                     /* Size of computed plaintext */

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(plainText))
  {
    goto _app_process_exit;
  }

  /* Verify generated data are the expected ones */
  if (memcmp(plainText, Computed_Plaintext, computed_size) != 0)
  {

    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.2: Authenticated decryption with associated data (ADAD) COMPLETED.\n");

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

  /* Destroy the key */
  retval = psa_destroy_key(key_handle);
  if (retval != PSA_SUCCESS)
  {
    goto _app_deinit_exit;
  }

  /* Clear all data associated with the PSA layer */
  mbedtls_psa_crypto_free();

  PRINTF("[INFO] Step 3.1: Cleanup COMPLETED.\n");

  /* Deinitialize the RNG */
  mx_example_rng_deinit();

  PRINTF("[INFO] Step 3.2: RNG deinitialization COMPLETED.\n");
  return_status = EXEC_STATUS_OK;

_app_deinit_exit:
  return return_status;
} /* end app_deinit */
