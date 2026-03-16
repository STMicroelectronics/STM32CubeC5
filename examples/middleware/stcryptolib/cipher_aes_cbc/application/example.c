/**
  ******************************************************************************
  * file           : example.c
  * brief          : This example describes how to use the STM32 Cryptographic
  *                  library to perform encryption and decryption
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
#include "cmox_crypto.h"        /* Crypto services and defines */
#include <string.h>             /* Various functions for character manipulating. */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Chunk size (in bytes) when data to encrypt or decrypt are processed by chunk */
#define CHUNK_SIZE  48u

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* CBC context handle */
cmox_cbc_handle_t p_cbc_ctx;

/** Extract from NIST Special Publication 800-38A
  * F.2.1 CBC-AES128.Encrypt
    Key 2b7e151628aed2a6abf7158809cf4f3c
    IV 000102030405060708090a0b0c0d0e0f
    Block #1
    Plaintext 6bc1bee22e409f96e93d7e117393172a
    Input Block 6bc0bce12a459991e134741a7f9e1925
    Output Block 7649abac8119b246cee98e9b12e9197d
    Ciphertext 7649abac8119b246cee98e9b12e9197d
    Block #2
    Plaintext ae2d8a571e03ac9c9eb76fac45af8e51
    Input Block d86421fb9f1a1eda505ee1375746972c
    Output Block 5086cb9b507219ee95db113a917678b2
    Ciphertext 5086cb9b507219ee95db113a917678b2
    Block #3
    Plaintext 30c81c46a35ce411e5fbc1191a0a52ef
    Input Block 604ed7ddf32efdff7020d0238b7c2a5d
    Output Block 73bed6b8e3c1743b7116e69e22229516
    Ciphertext 73bed6b8e3c1743b7116e69e22229516
    Block #4
    Plaintext f69f2445df4f9b17ad2b417be66c3710
    Input Block 8521f2fd3c8eef2cdc3da7e5c44ea206
    Output Block 3ff1caa1681fac09120eca307586e1a7
    Ciphertext 3ff1caa1681fac09120eca307586e1a7
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
uint8_t expectedCipherText[] =
{
  0x76, 0x49, 0xab, 0xac, 0x81, 0x19, 0xb2, 0x46, 0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d,
  0x50, 0x86, 0xcb, 0x9b, 0x50, 0x72, 0x19, 0xee, 0x95, 0xdb, 0x11, 0x3a, 0x91, 0x76, 0x78, 0xb2,
  0x73, 0xbe, 0xd6, 0xb8, 0xe3, 0xc1, 0x74, 0x3b, 0x71, 0x16, 0xe6, 0x9e, 0x22, 0x22, 0x95, 0x16,
  0x3f, 0xf1, 0xca, 0xa1, 0x68, 0x1f, 0xac, 0x09, 0x12, 0x0e, 0xca, 0x30, 0x75, 0x86, 0xe1, 0xa7
};

/* Computed data buffer */
uint8_t Computed_Ciphertext[sizeof(expectedCipherText)];
uint8_t Computed_Plaintext[sizeof(plainText)];

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The init is triggered by the applicative code
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Initialize cryptographic library */
  if (cmox_initialize(NULL) != CMOX_INIT_SUCCESS)
  {
    PRINTF("[ERROR] Step 1.1: STM32 Cryptographic library initialization ERROR.\n");
    goto _app_init_exit;
  }
  else
  {
    PRINTF("[INFO] Step 1.1: STM32 Cryptographic library initialization COMPLETED.\n");
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Demonstrates encryption and decryption of a known ciphertext,
  * using the AES CBC algorithm in 'SINGLE CALL USAGE' way.
  */
app_status_t app_process(void)
{
  /* General cipher context */
  cmox_cipher_handle_t *p_cipher_ctx;
  app_status_t return_status = EXEC_STATUS_ERROR;
  cmox_cipher_retval_t retval;
  size_t computed_size;
  uint32_t index;

  /* Encryption of a known plainText, compute directly the ciphertext passing all the needed parameters */
  retval = cmox_cipher_encrypt(CMOX_AES_CBC_ENC_ALGO,                  /* Use AES CBC algorithm */
                               plainText, sizeof(plainText),           /* plainText to encrypt */
                               Key, sizeof(Key),                       /* AES key to use */
                               IV, sizeof(IV),                         /* Initialization vector */
                               Computed_Ciphertext, &computed_size);   /* Data buffer to receive generated ciphertext */

  /* Verify API returned value */
  if (retval != CMOX_CIPHER_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: Encryption of a known plainText COMPLETED.\n");

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(expectedCipherText))
  {
    goto _app_process_exit;
  }

  /* Verify generated data are the expected ones */
  if (memcmp(expectedCipherText, Computed_Ciphertext, computed_size) != 0)
  {
    goto _app_process_exit;
  }

  /* Decryption of a known ciphertext, compute directly the plaintext passing all the needed parameters */
  retval = cmox_cipher_decrypt(CMOX_AES_CBC_DEC_ALGO,                 /* Use AES CBC algorithm */
                               expectedCipherText, sizeof(expectedCipherText), /* Ciphertext to decrypt */
                               Key, sizeof(Key),                      /* AES key to use */
                               IV, sizeof(IV),                        /* Initialization vector */
                               Computed_Plaintext, &computed_size);   /* Data buffer to receive generated plaintext */

  /* Verify API returned value */
  if (retval != CMOX_CIPHER_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: Decryption of a known ciphertext COMPLETED.\n");

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

  /** ########## Step 3 ##########
    * Demonstrates encryption and decryption of a known ciphertext,
    * using the AES CBC algorithm, in 'MULTIPLE CALL USAG' method.
    */

  /* Construct a cipher context that is configured to perform AES CBC encryption operations */
  p_cipher_ctx = cmox_cbc_construct(&p_cbc_ctx, CMOX_AES_CBC_ENC);
  if (p_cipher_ctx == NULL)
  {
    goto _app_process_exit;
  }

  /* Initialize the cipher context */
  retval = cmox_cipher_init(p_cipher_ctx);
  if (retval != CMOX_CIPHER_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Setup of the encryption key into the context */
  retval = cmox_cipher_setKey(p_cipher_ctx, Key, sizeof(Key));  /* AES key to use */
  if (retval != CMOX_CIPHER_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Setup of the Initialization Vector (IV) into the context */
  retval = cmox_cipher_setIV(p_cipher_ctx, IV, sizeof(IV));     /* Initialization vector */
  if (retval != CMOX_CIPHER_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3.1: Construct, initialize and setup needed data into encryption cipher context.\n");

  /* Encrypt the plainText in multiple steps by appending chunks of CHUNK_SIZE bytes */
  for (index = 0; index < (sizeof(plainText) - CHUNK_SIZE); index += CHUNK_SIZE)
  {
    retval = cmox_cipher_append(p_cipher_ctx,
                                &plainText[index], CHUNK_SIZE,    /* Chunk of plainText to authenticate and encrypt */
                                Computed_Ciphertext, &computed_size);   /* Data buffer to receive generated chunk
                                                                           of ciphertext */

    /* Verify API returned value */
    if (retval != CMOX_CIPHER_SUCCESS)
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
    retval = cmox_cipher_append(p_cipher_ctx,
                                &plainText[index],
                                sizeof(plainText) - index,                      /* Last part of plainText to encrypt */
                                Computed_Ciphertext, &computed_size);           /* Data buffer to receive generated last
                                                                                   part of ciphertext */

    /* Verify API returned value */
    if (retval != CMOX_CIPHER_SUCCESS)
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

  PRINTF("[INFO] Step 3.2: Encryption of plainText in multiple steps COMPLETED.\n");

  /* Cleanup the context */
  retval = cmox_cipher_cleanup(p_cipher_ctx);
  if (retval != CMOX_CIPHER_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3.3: Clean-up encryption cipher context COMPLETED.\n");

  /* Construct a cipher context that is configured to perform AES CBC decryption operations */
  p_cipher_ctx = cmox_cbc_construct(&p_cbc_ctx, CMOX_AES_CBC_DEC);
  if (p_cipher_ctx == NULL)
  {
    goto _app_process_exit;
  }

  /* Initialize the cipher context */
  retval = cmox_cipher_init(p_cipher_ctx);
  if (retval != CMOX_CIPHER_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Setup of the decryption key into the context */
  retval = cmox_cipher_setKey(p_cipher_ctx, Key, sizeof(Key));  /* AES key to use */
  if (retval != CMOX_CIPHER_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Setup of the Initialization Vector (IV) into the context */
  retval = cmox_cipher_setIV(p_cipher_ctx, IV, sizeof(IV));     /* Initialization vector */
  if (retval != CMOX_CIPHER_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3.4: Construct, initialize and setup needed data into decryption cipher context.\n");

  /* Decrypt the plainText in multiple steps by appending chunks of CHUNK_SIZE bytes */
  for (index = 0; index < (sizeof(expectedCipherText) - CHUNK_SIZE); index += CHUNK_SIZE)
  {
    retval = cmox_cipher_append(p_cipher_ctx,
                                &expectedCipherText[index], CHUNK_SIZE,  /* Chunk of ciphertext to decrypt and verify */
                                Computed_Plaintext, &computed_size);     /* Data buffer to receive generated
                                                                            chunk of plainText */

    /* Verify API returned value */
    if (retval != CMOX_CIPHER_SUCCESS)
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
  /* Process with encryption of the last part if needed */
  if (index < sizeof(expectedCipherText))
  {
    retval = cmox_cipher_append(p_cipher_ctx,
                                &expectedCipherText[index],
                                sizeof(expectedCipherText) - index,    /* Last part of ciphertext to decrypt */
                                Computed_Plaintext, &computed_size);    /* Data buffer to receive generated last
                                                                           part of plaintext */

    /* Verify API returned value */
    if (retval != CMOX_CIPHER_SUCCESS)
    {
      goto _app_process_exit;
    }

    /* Verify generated data size is the expected one */
    if (computed_size != (sizeof(expectedCipherText) - index))
    {
      goto _app_process_exit;
    }

    /* Verify generated data are the expected ones */
    if (memcmp(&plainText[index], Computed_Plaintext, computed_size) != 0)
    {
      goto _app_process_exit;
    }
  }

  PRINTF("[INFO] Step 3.5: Decryption of plainText in multiple steps COMPLETED.\n");

  /* Cleanup the handle */
  retval = cmox_cipher_cleanup(p_cipher_ctx);
  if (retval != CMOX_CIPHER_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3.6: Clean-up decryption cipher context COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 4 ##########
  * Deinitialization before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  /* No more need of cryptographic services, finalize STM32 Cryptographic library */
  if (cmox_finalize(NULL) != CMOX_INIT_SUCCESS)
  {
    return EXEC_STATUS_ERROR;
  }

  PRINTF("[INFO] Step 4.1: Finalize STM32 Cryptographic library COMPLETED.\n \n");

  return EXEC_STATUS_OK;
} /* end app_deinit */
