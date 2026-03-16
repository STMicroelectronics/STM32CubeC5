/**
  ******************************************************************************
  * file           : example.c
  * brief          : This example describes how to use the STM32 Flexible Crypto
  *                  Framework (STFCF) to perform encryption and decryption
  *                  using the AES CBC algorithm.
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
/* Key attributes */
psa_key_attributes_t key_attributes;
psa_key_handle_t key_handle;

/*******************************************************************************
  * Extract from NIST Special Publication 800-38A
  * F.2.1 CBC-AES128.Encrypt
  * Key 2b7e151628aed2a6abf7158809cf4f3c
  * IV 000102030405060708090a0b0c0d0e0f
  * Block #1
  * Plaintext 6bc1bee22e409f96e93d7e117393172a
  * Input Block 6bc0bce12a459991e134741a7f9e1925
  * Output Block 7649abac8119b246cee98e9b12e9197d
  * Ciphertext 7649abac8119b246cee98e9b12e9197d
  * Block #2
  * Plaintext ae2d8a571e03ac9c9eb76fac45af8e51
  * Input Block d86421fb9f1a1eda505ee1375746972c
  * Output Block 5086cb9b507219ee95db113a917678b2
  * Ciphertext 5086cb9b507219ee95db113a917678b2
  * Block #3
  * Plaintext 30c81c46a35ce411e5fbc1191a0a52ef
  * Input Block 604ed7ddf32efdff7020d0238b7c2a5d
  * Output Block 73bed6b8e3c1743b7116e69e22229516
  * Ciphertext 73bed6b8e3c1743b7116e69e22229516
  * Block #4
  * Plaintext f69f2445df4f9b17ad2b417be66c3710
  * Input Block 8521f2fd3c8eef2cdc3da7e5c44ea206
  * Output Block 3ff1caa1681fac09120eca307586e1a7
  * Ciphertext 3ff1caa1681fac09120eca307586e1a7
  *
  ******************************************************************************
  */
const uint8_t Key[] =
{
  0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};
const uint8_t IV[] =
{
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};
const uint8_t plainText[] =
{
  0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
  0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
  0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
  0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
};
const uint8_t expectedCipherText[] =
{
  0x76, 0x49, 0xab, 0xac, 0x81, 0x19, 0xb2, 0x46, 0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d,
  0x50, 0x86, 0xcb, 0x9b, 0x50, 0x72, 0x19, 0xee, 0x95, 0xdb, 0x11, 0x3a, 0x91, 0x76, 0x78, 0xb2,
  0x73, 0xbe, 0xd6, 0xb8, 0xe3, 0xc1, 0x74, 0x3b, 0x71, 0x16, 0xe6, 0x9e, 0x22, 0x22, 0x95, 0x16,
  0x3f, 0xf1, 0xca, 0xa1, 0x68, 0x1f, 0xac, 0x09, 0x12, 0x0e, 0xca, 0x30, 0x75, 0x86, 0xe1, 0xa7
};

const uint8_t IVandExpectedCiphertext[] =
{
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x76, 0x49, 0xab, 0xac, 0x81, 0x19, 0xb2, 0x46, 0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d,
  0x50, 0x86, 0xcb, 0x9b, 0x50, 0x72, 0x19, 0xee, 0x95, 0xdb, 0x11, 0x3a, 0x91, 0x76, 0x78, 0xb2,
  0x73, 0xbe, 0xd6, 0xb8, 0xe3, 0xc1, 0x74, 0x3b, 0x71, 0x16, 0xe6, 0x9e, 0x22, 0x22, 0x95, 0x16,
  0x3f, 0xf1, 0xca, 0xa1, 0x68, 0x1f, 0xac, 0x09, 0x12, 0x0e, 0xca, 0x30, 0x75, 0x86, 0xe1, 0xa7
};

/* Computed data buffer */
uint8_t Computed_Ciphertext[sizeof(expectedCipherText) + sizeof(IV)];
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
  psa_set_key_algorithm(&key_attributes, PSA_ALG_CBC_NO_PADDING);
  psa_set_key_type(&key_attributes, PSA_KEY_TYPE_AES);
  psa_set_key_bits(&key_attributes, 8U * sizeof(Key));
  /* Set up the key location using STFCF_KEY_LOCATION. If this latter is set to PSA_CRYPTO_KWE_DRIVER_LOCATION,
     the private key will be wrapped using STM32 Key Wrap Engine (KWE); If PSA_KEY_LOCATION_LOCAL_STORAGE is set,
     then the key is transparent*/
  psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_VOLATILE, \
                       STFCF_KEY_LOCATION));
  /* Import a volatile key for AES CBC in binary format to create an object that is equivalent to the public key */
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
  * of a known ciphertext using the AES CBC algorithm.
  * This step is scheduled as follows:
  * Step 2.2: Performs an encryption and a decryption using single API call method.
  * Step 2.3: Performs an encryption and a decryption using multiple API calls method.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  psa_cipher_operation_t operation;
  size_t computed_size;
  psa_status_t retval;
  /* Index for piecemeal processing */
  uint32_t index;

  /** ########## Step 2.1 ##########
    * Performs an encryption and a decryption of a known ciphertext
    * in a single API call method.
    */

  /* Compute directly the ciphertext passing all the needed parameters */
  retval = psa_cipher_encrypt(key_handle,                    /* The key id */
                              PSA_ALG_CBC_NO_PADDING,        /* Algorithm type */
                              plainText, sizeof(plainText),  /* plainText to encrypt */
                              Computed_Ciphertext,           /* Data buffer to receive generated ciphertext */
                              sizeof(Computed_Ciphertext),   /* Size of buffer to receive ciphertext */
                              &computed_size);               /* Size of computed ciphertext */

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one. We can only verify the size of the data.*/
  if (computed_size != sizeof(IVandExpectedCiphertext))
  {
    goto _app_process_exit;
  }

  /* Decrypt and verify directly ciphertext and tag passing all the needed parameters */
  retval = psa_cipher_decrypt(key_handle,                         /* The key id */
                              PSA_ALG_CBC_NO_PADDING,             /* Algorithm type */
                              IVandExpectedCiphertext,            /* IV + Ciphertext to decrypt */
                              sizeof(IVandExpectedCiphertext),    /* Size of IV + Ciphertext to decrypt */
                              Computed_Plaintext,                 /* Data buffer to receive generated plaintext */
                              sizeof(Computed_Plaintext),         /* Size of buffer to receive plaintext */
                              &computed_size);                    /* Size of computed plaintext */

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

  PRINTF("[INFO] Step 2.1: AES CBC encryption and decryption using single API call method COMPLETED.\n");

  /** ########## Step 2.2 ##########
    * Performs an encryption and a decryption of a known ciphertext
    * in a multiple API calls method.
    */
  operation = psa_cipher_operation_init();

  /* Setup the context for encrypt operation */
  retval = psa_cipher_encrypt_setup(&operation,                /* The current operation */
                                    key_handle,                /* The key id */
                                    PSA_ALG_CBC_NO_PADDING);   /* Algorithm type */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Setup of the Initialization Vector (IV) into the context */
  retval = psa_cipher_set_iv(&operation, IV, sizeof(IV));
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Process with encryption of the plaintext by chunk of CHUNK_SIZE bytes */
  for (index = 0; index < (sizeof(plainText) - CHUNK_SIZE); index += CHUNK_SIZE)
  {
    retval = psa_cipher_update(&operation,                    /* The current operation */
                               &plainText[index], CHUNK_SIZE, /* Chunk of plaintext to encrypt */
                               Computed_Ciphertext,           /* Data buffer to receive generated ciphertext */
                               sizeof(Computed_Ciphertext),   /* Size of buffer to receive ciphertext */
                               &computed_size);               /* Size of computed ciphertext */

    /* Verify API returned value */
    if (retval != PSA_SUCCESS)
    {
      goto _app_process_exit;
    }

    /* Verify generated data size is the expected one */
    if (computed_size != CHUNK_SIZE)
    {
      goto _app_process_exit;
    }

    /* Verify generated data are the expected ones */
    if (memcmp(&expectedCipherText[index], Computed_Ciphertext, computed_size) != 0)
    {
      goto _app_process_exit;
    }
  }

  /* Process with encryption of the last part if needed */
  if (index < sizeof(plainText))
  {
    retval = psa_cipher_update(&operation,                  /* The current operation */
                               &plainText[index],           /* Last part of plaintext to encrypt */
                               sizeof(plainText) - index,   /* Size of last part of plaintext to encrypt */
                               Computed_Ciphertext,         /* Data buffer to receive last part of ciphertext */
                               sizeof(Computed_Ciphertext), /* Size of buffer to receive ciphertext */
                               &computed_size);             /* Size of computed ciphertext */

    /* Verify API returned value */
    if (retval != PSA_SUCCESS)
    {
      goto _app_process_exit;
    }

    /* Verify generated data size is the expected one */
    if (computed_size != (sizeof(plainText) - index))
    {
      goto _app_process_exit;
    }

    /* Verify generated data are the expected ones */
    if (memcmp(&expectedCipherText[index], Computed_Ciphertext, computed_size) != 0)
    {
      goto _app_process_exit;
    }
  }

  /* Finish properly the encrypt */
  retval = psa_cipher_finish(&operation,                          /* The current operation */
                             Computed_Ciphertext,                 /* Data buffer to receive the ciphertext */
                             sizeof(Computed_Ciphertext),         /* Size of buffer to receive ciphertext */
                             &computed_size);                     /* Size of computed ciphertext */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Clean up cipher operation context */
  retval = psa_cipher_abort(&operation);
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Setup the context for decrypt operation */
  retval = psa_cipher_decrypt_setup(&operation,                  /* The current operation */
                                    key_handle,                  /* The key id */
                                    PSA_ALG_CBC_NO_PADDING);     /* Algorithm type */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Setup of the Initialization Vector (IV) into the context */
  retval = psa_cipher_set_iv(&operation, IV, sizeof(IV));
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Process with decryption of the plaintext by chunk of CHUNK_SIZE bytes */
  for (index = 0; index < (sizeof(expectedCipherText) - CHUNK_SIZE); index += CHUNK_SIZE)
  {
    retval = psa_cipher_update(&operation,                               /* The current operation */
                               &expectedCipherText[index], CHUNK_SIZE,   /* Chunk of ciphertext to decrypt */
                               Computed_Plaintext,                       /* Data buffer to receive chunk of plaintext */
                               sizeof(Computed_Plaintext),               /* Size of buffer to receive plaintext */
                               &computed_size);                          /* Size of computed plaintext */

    /* Verify API returned value */
    if (retval != PSA_SUCCESS)
    {
      goto _app_process_exit;
    }

    /* Verify generated data size is the expected one */
    if (computed_size != CHUNK_SIZE)
    {
      goto _app_process_exit;
    }

    /* Verify generated data are the expected ones */
    if (memcmp(&plainText[index], Computed_Plaintext, computed_size) != 0)
    {
      goto _app_process_exit;
    }
  }

  /* Process with decryption of the last part if needed */
  if (index < sizeof(expectedCipherText))
  {
    retval = psa_cipher_update(&operation,                           /* The current operation */
                               &expectedCipherText[index],           /* Last part of ciphertext to decrypt */
                               sizeof(expectedCipherText) - index,   /* Size of last part of ciphertext to decrypt */
                               Computed_Plaintext,                   /* Data buffer to receive last part of plaintext */
                               sizeof(Computed_Plaintext),           /* Size of buffer to receive plaintext */
                               &computed_size);                      /* Size of computed plaintext */

    /* Verify API returned value */
    if (retval != PSA_SUCCESS)
    {
      goto _app_process_exit;
    }

    /* Verify generated data size is the expected one */
    if (computed_size != (sizeof(plainText) - index))
    {
      goto _app_process_exit;
    }

    /* Verify generated data are the expected ones */
    if (memcmp(&plainText[index], Computed_Plaintext, computed_size) != 0)
    {
      goto _app_process_exit;
    }
  }

  /* Finish properly the decrypt */
  retval = psa_cipher_finish(&operation,                          /* The current operation */
                             Computed_Plaintext,                  /* Data buffer to receive the plaintext */
                             sizeof(Computed_Plaintext),          /* Size of buffer to receive plaintext */
                             &computed_size);                     /* Size of computed plaintext */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Clean up cipher operation context */
  retval = psa_cipher_abort(&operation);
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: AES CBC encryption and decryption using multiple API calls method COMPLETED.\n");

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

  /* Deinitialize RNG */
  mx_example_rng_deinit();

  PRINTF("[INFO] Step 3.2: RNG deinitialization COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_deinit_exit:
  return return_status;
} /* end app_deinit */
