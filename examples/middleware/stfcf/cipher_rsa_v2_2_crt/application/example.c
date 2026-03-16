/**
  ******************************************************************************
  * file           : example.c
  * brief          : This example describes how to use the STM32 Flexible Crypto
  *                  Framework (STFCF) to perform an encryption
  *                  and a decryption of a message using the RSA PKCS#1 v2.2
  *                  compliant algorithm.
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

/** Extract from psa_arch_tests\api-tests\dev_apis\crypto\common\test_crypto_common.c
  *
  * # Public key
  * # ----------
  *
  * # Modulus:
  * af 05 7d 39 6e e8 4f b7 5f db b5 c2 b1 3c 7f e5
  * a6 54 aa 8a a2 47 0b 54 1e e1 fe b0 b1 2d 25 c7
  * 97 11 53 12 49 e1 12 96 28 04 2d bb b6 c1 20 d1
  * 44 35 24 ef 4c 0e 6e 1d 89 56 ee b2 07 7a f1 23
  * 49 dd ee e5 44 83 bc 06 c2 c6 19 48 cd 02 b2 02
  * e7 96 ae bd 94 d3 a7 cb f8 59 c2 c1 81 9c 32 4c
  * b8 2b 9c d3 4e de 26 3a 2a bf fe 47 33 f0 77 86
  * 9e 86 60 f7 d6 83 4d a5 3d 69 0e f7 98 5f 6b c3
  *
  * # Exponent:
  * 01 00 01
  *
  * # Private key
  * # -----------
  *
  * # Modulus:
  * af 05 7d 39 6e e8 4f b7 5f db b5 c2 b1 3c 7f e5
  * a6 54 aa 8a a2 47 0b 54 1e e1 fe b0 b1 2d 25 c7
  * 97 11 53 12 49 e1 12 96 28 04 2d bb b6 c1 20 d1
  * 44 35 24 ef 4c 0e 6e 1d 89 56 ee b2 07 7a f1 23
  * 49 dd ee e5 44 83 bc 06 c2 c6 19 48 cd 02 b2 02
  * e7 96 ae bd 94 d3 a7 cb f8 59 c2 c1 81 9c 32 4c
  * b8 2b 9c d3 4e de 26 3a 2a bf fe 47 33 f0 77 86
  * 9e 86 60 f7 d6 83 4d a5 3d 69 0e f7 98 5f 6b c3
  *
  * # Public exponent:
  * 01 00 01
  *
  * # Prime1
  * e2 ab 76 08 41 bb 9d 30 a8 1d 22 2d e1 eb 73 81
  * d8 22 14 40 7f 1b 97 5c bb fe 4e 1a 94 67 fd 98
  * ad bd 78 f6 07 83 6c a5 be 19 28 b9 d1 60 d9 7f
  * d4 5c 12 d6 b5 2e 2c 98 71 a1 74 c6 6b 48 81 13
  *
  * # Prime2
  * c5 ab 27 60 21 59 ae 7d 6f 20 c3 c2 ee 85 1e 46
  * dc 11 2e 68 9e 28 d5 fc bb f9 90 a9 9e f8 a9 0b
  * 8b b4 4f d3 64 67 e7 fc 17 89 ce b6 63 ab da 33
  * 86 52 c3 c7 3f 11 17 74 90 2e 84 05 65 92 70 91
  *
  * # Exponent1
  * b6 cd bd 35 4f 7d f5 79 a6 3b 48 b3 64 3e 35 3b
  * 84 89 87 77 b4 8b 15 f9 4e 0b fc 05 67 a6 ae 59
  * 11 d5 7a d6 40 9c f7 64 7b f9 62 64 e9 bd 87 eb
  * 95 e2 63 b7 11 0b 9a 1f 9f 94 ac ce d0 fa fa 4d
  *
  * # Exponent2
  * 71 19 5e ec 37 e8 d2 57 de cf c6 72 b0 7a e6 39
  * f1 0c bb 9b 0c 73 9d 0c 80 99 68 d6 44 a9 4e 3f
  * d6 ed 92 87 07 7a 14 58 3f 37 90 58 f7 6a 8a ec
  * d4 3c 62 dc 8c 0f 41 76 66 50 d7 25 27 5a c4 a1
  *
  * # Coefficient
  * bb 32 d1 33 ed c2 e0 48 d4 63 38 8b 7b e9 cb 4b
  * e2 9f 4b 62 50 be 60 3e 70 e3 64 75 01 c9 7d dd
  * e2 0a 4e 71 be 95 fd 5e 71 78 4e 25 ac a4 ba f2
  * 5b e5 73 8a ae 59 bb fe 1c 99 77 81 44 7a 2b 24
  *
  * # OAEP Example 10.4
  * # ------------------
  *
  * # Message:
  * 45 35 d1 2b 43 77 92 8a 7c 0a 61 c9 f8 25 a4 86
  * 71 ea 05 91 07 48 c8 ef
  *
  * # Seed:
  * 0x53 0x65 0x65 0x64
  *
  * ---------------------------------------------------------------------
  */

/* Known plaintext to be encrypted and decrypted */
const uint8_t Message[] =
{
  0x45, 0x35, 0xd1, 0x2b, 0x43, 0x77, 0x92, 0x8a, 0x7c, 0x0a, 0x61, 0xc9, 0xf8, 0x25, 0xa4, 0x86,
  0x71, 0xea, 0x05, 0x91, 0x07, 0x48, 0xc8, 0xef
};

const uint8_t Seed[] =
{
  0x53, 0x65, 0x65, 0x64
};

const uint8_t rsa_128_key_pair[] =
{
  0x30, 0x82, 0x02, 0x5e, 0x02, 0x01, 0x00, 0x02, 0x81, 0x81, 0x00, 0xaf, 0x05,
  0x7d, 0x39, 0x6e, 0xe8, 0x4f, 0xb7, 0x5f, 0xdb, 0xb5, 0xc2, 0xb1, 0x3c, 0x7f,
  0xe5, 0xa6, 0x54, 0xaa, 0x8a, 0xa2, 0x47, 0x0b, 0x54, 0x1e, 0xe1, 0xfe, 0xb0,
  0xb1, 0x2d, 0x25, 0xc7, 0x97, 0x11, 0x53, 0x12, 0x49, 0xe1, 0x12, 0x96, 0x28,
  0x04, 0x2d, 0xbb, 0xb6, 0xc1, 0x20, 0xd1, 0x44, 0x35, 0x24, 0xef, 0x4c, 0x0e,
  0x6e, 0x1d, 0x89, 0x56, 0xee, 0xb2, 0x07, 0x7a, 0xf1, 0x23, 0x49, 0xdd, 0xee,
  0xe5, 0x44, 0x83, 0xbc, 0x06, 0xc2, 0xc6, 0x19, 0x48, 0xcd, 0x02, 0xb2, 0x02,
  0xe7, 0x96, 0xae, 0xbd, 0x94, 0xd3, 0xa7, 0xcb, 0xf8, 0x59, 0xc2, 0xc1, 0x81,
  0x9c, 0x32, 0x4c, 0xb8, 0x2b, 0x9c, 0xd3, 0x4e, 0xde, 0x26, 0x3a, 0x2a, 0xbf,
  0xfe, 0x47, 0x33, 0xf0, 0x77, 0x86, 0x9e, 0x86, 0x60, 0xf7, 0xd6, 0x83, 0x4d,
  0xa5, 0x3d, 0x69, 0x0e, 0xf7, 0x98, 0x5f, 0x6b, 0xc3, 0x02, 0x03, 0x01, 0x00,
  0x01, 0x02, 0x81, 0x81, 0x00, 0x87, 0x4b, 0xf0, 0xff, 0xc2, 0xf2, 0xa7, 0x1d,
  0x14, 0x67, 0x1d, 0xdd, 0x01, 0x71, 0xc9, 0x54, 0xd7, 0xfd, 0xbf, 0x50, 0x28,
  0x1e, 0x4f, 0x6d, 0x99, 0xea, 0x0e, 0x1e, 0xbc, 0xf8, 0x2f, 0xaa, 0x58, 0xe7,
  0xb5, 0x95, 0xff, 0xb2, 0x93, 0xd1, 0xab, 0xe1, 0x7f, 0x11, 0x0b, 0x37, 0xc4,
  0x8c, 0xc0, 0xf3, 0x6c, 0x37, 0xe8, 0x4d, 0x87, 0x66, 0x21, 0xd3, 0x27, 0xf6,
  0x4b, 0xbe, 0x08, 0x45, 0x7d, 0x3e, 0xc4, 0x09, 0x8b, 0xa2, 0xfa, 0x0a, 0x31,
  0x9f, 0xba, 0x41, 0x1c, 0x28, 0x41, 0xed, 0x7b, 0xe8, 0x31, 0x96, 0xa8, 0xcd,
  0xf9, 0xda, 0xa5, 0xd0, 0x06, 0x94, 0xbc, 0x33, 0x5f, 0xc4, 0xc3, 0x22, 0x17,
  0xfe, 0x04, 0x88, 0xbc, 0xe9, 0xcb, 0x72, 0x02, 0xe5, 0x94, 0x68, 0xb1, 0xea,
  0xd1, 0x19, 0x00, 0x04, 0x77, 0xdb, 0x2c, 0xa7, 0x97, 0xfa, 0xc1, 0x9e, 0xda,
  0x3f, 0x58, 0xc1, 0x02, 0x41, 0x00, 0xe2, 0xab, 0x76, 0x08, 0x41, 0xbb, 0x9d,
  0x30, 0xa8, 0x1d, 0x22, 0x2d, 0xe1, 0xeb, 0x73, 0x81, 0xd8, 0x22, 0x14, 0x40,
  0x7f, 0x1b, 0x97, 0x5c, 0xbb, 0xfe, 0x4e, 0x1a, 0x94, 0x67, 0xfd, 0x98, 0xad,
  0xbd, 0x78, 0xf6, 0x07, 0x83, 0x6c, 0xa5, 0xbe, 0x19, 0x28, 0xb9, 0xd1, 0x60,
  0xd9, 0x7f, 0xd4, 0x5c, 0x12, 0xd6, 0xb5, 0x2e, 0x2c, 0x98, 0x71, 0xa1, 0x74,
  0xc6, 0x6b, 0x48, 0x81, 0x13, 0x02, 0x41, 0x00, 0xc5, 0xab, 0x27, 0x60, 0x21,
  0x59, 0xae, 0x7d, 0x6f, 0x20, 0xc3, 0xc2, 0xee, 0x85, 0x1e, 0x46, 0xdc, 0x11,
  0x2e, 0x68, 0x9e, 0x28, 0xd5, 0xfc, 0xbb, 0xf9, 0x90, 0xa9, 0x9e, 0xf8, 0xa9,
  0x0b, 0x8b, 0xb4, 0x4f, 0xd3, 0x64, 0x67, 0xe7, 0xfc, 0x17, 0x89, 0xce, 0xb6,
  0x63, 0xab, 0xda, 0x33, 0x86, 0x52, 0xc3, 0xc7, 0x3f, 0x11, 0x17, 0x74, 0x90,
  0x2e, 0x84, 0x05, 0x65, 0x92, 0x70, 0x91, 0x02, 0x41, 0x00, 0xb6, 0xcd, 0xbd,
  0x35, 0x4f, 0x7d, 0xf5, 0x79, 0xa6, 0x3b, 0x48, 0xb3, 0x64, 0x3e, 0x35, 0x3b,
  0x84, 0x89, 0x87, 0x77, 0xb4, 0x8b, 0x15, 0xf9, 0x4e, 0x0b, 0xfc, 0x05, 0x67,
  0xa6, 0xae, 0x59, 0x11, 0xd5, 0x7a, 0xd6, 0x40, 0x9c, 0xf7, 0x64, 0x7b, 0xf9,
  0x62, 0x64, 0xe9, 0xbd, 0x87, 0xeb, 0x95, 0xe2, 0x63, 0xb7, 0x11, 0x0b, 0x9a,
  0x1f, 0x9f, 0x94, 0xac, 0xce, 0xd0, 0xfa, 0xfa, 0x4d, 0x02, 0x40, 0x71, 0x19,
  0x5e, 0xec, 0x37, 0xe8, 0xd2, 0x57, 0xde, 0xcf, 0xc6, 0x72, 0xb0, 0x7a, 0xe6,
  0x39, 0xf1, 0x0c, 0xbb, 0x9b, 0x0c, 0x73, 0x9d, 0x0c, 0x80, 0x99, 0x68, 0xd6,
  0x44, 0xa9, 0x4e, 0x3f, 0xd6, 0xed, 0x92, 0x87, 0x07, 0x7a, 0x14, 0x58, 0x3f,
  0x37, 0x90, 0x58, 0xf7, 0x6a, 0x8a, 0xec, 0xd4, 0x3c, 0x62, 0xdc, 0x8c, 0x0f,
  0x41, 0x76, 0x66, 0x50, 0xd7, 0x25, 0x27, 0x5a, 0xc4, 0xa1, 0x02, 0x41, 0x00,
  0xbb, 0x32, 0xd1, 0x33, 0xed, 0xc2, 0xe0, 0x48, 0xd4, 0x63, 0x38, 0x8b, 0x7b,
  0xe9, 0xcb, 0x4b, 0xe2, 0x9f, 0x4b, 0x62, 0x50, 0xbe, 0x60, 0x3e, 0x70, 0xe3,
  0x64, 0x75, 0x01, 0xc9, 0x7d, 0xdd, 0xe2, 0x0a, 0x4e, 0x71, 0xbe, 0x95, 0xfd,
  0x5e, 0x71, 0x78, 0x4e, 0x25, 0xac, 0xa4, 0xba, 0xf2, 0x5b, 0xe5, 0x73, 0x8a,
  0xae, 0x59, 0xbb, 0xfe, 0x1c, 0x99, 0x77, 0x81, 0x44, 0x7a, 0x2b, 0x24
};

/* Computed data buffer */
uint8_t Computed_Encryption[1024 / 8U]; /* Ciphertext size = modulus size */
uint8_t Computed_Text[sizeof(Message)];


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

  retval = psa_crypto_init();
  if (retval != PSA_SUCCESS)
  {
    goto _app_init_exit;
  }

  /* Setup the key policy for the key pair */
  psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_DECRYPT | PSA_KEY_USAGE_ENCRYPT);
  psa_set_key_algorithm(&key_attributes, PSA_ALG_RSA_OAEP(PSA_ALG_SHA_256));
  psa_set_key_type(&key_attributes, PSA_KEY_TYPE_RSA_KEY_PAIR);
  psa_set_key_bits(&key_attributes, 1024); /* Key length = modulus size */
  /* Set up the key location using STFCF_KEY_LOCATION. If this latter is set to PSA_CRYPTO_KWE_DRIVER_LOCATION,
   the private key will be wrapped using STM32 Key Wrap Engine (KWE). If PSA_KEY_LOCATION_LOCAL_STORAGE is set,
   then the key is transparent*/
  psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_VOLATILE, \
                       STFCF_KEY_LOCATION));
  /* Import a key pair */
  retval = psa_import_key(&key_attributes, rsa_128_key_pair, sizeof(rsa_128_key_pair), &key_handle);
  if (retval != PSA_SUCCESS)
  {
    goto _app_init_exit;
  }

  /* Reset the key attribute to free any auxiliary resources that the structure might contain */
  psa_reset_key_attributes(&key_attributes);

  PRINTF("[INFO] Step 1.2: PSA key configuration COMPLETED.\n");

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Demonstrates how to use the STFCF to perform an encryption and a decryption
  * the computed ciphertext using the RSA PKCS#1 v2.2 compliant algorithm.
  *
  * This step is scheduled as follows:
  * Step 2.1: Encrypts a known message using CRT private key representation.
  * Step 2.2: Decrypts the computed ciphertext using CRT private key representation.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  psa_status_t retval;
  size_t computed_size;

  /** ########## Step 2.1 ##########
    * Encrypts a known message using CRT private key representation.
    */
  /* Compute directly the encrypted message passing all the needed parameters */
  retval = psa_asymmetric_encrypt(key_handle,
                                  PSA_ALG_RSA_OAEP(PSA_ALG_SHA_256),                /* Algorithm type */
                                  Message, sizeof(Message),                         /* Message to encrypt */
                                  Seed, sizeof(Seed),                               /* Salt parameters */
                                  Computed_Encryption, sizeof(Computed_Encryption), /* Data buf to receive ciphertext */
                                  &computed_size);                                  /* Size of computed ciphertext */

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Computed_Encryption))
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: Encrypt a known message using CRT private key representation COMPLETED.\n");


  /** ########## Step 2.2 ##########
    * Decrypts the computed ciphertext using CRT private key representation.
    */
  /* Compute directly the clear message passing all the needed parameters */
  retval = psa_asymmetric_decrypt(key_handle,
                                  PSA_ALG_RSA_OAEP(PSA_ALG_SHA_256),                 /* Algorithm type */
                                  Computed_Encryption, sizeof(Computed_Encryption),  /* Ciphertext */
                                  Seed, sizeof(Seed),                                /* Salt parameters */
                                  Computed_Text, sizeof(Computed_Text),              /* Data buf to receive plaintext */
                                  &computed_size);                                   /* Size of computed plaintext */

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Message))
  {
    goto _app_process_exit;
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Computed_Text, Message, computed_size) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: Decrypt the computed ciphertext using CRT private key representation COMPLETED.\n");

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
