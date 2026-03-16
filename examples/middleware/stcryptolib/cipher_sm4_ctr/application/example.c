/**
  ******************************************************************************
  * file           : example.c
  * brief          : This example describes how to use the STM32 Cryptographic
  *                  library to perform encryption and decryption
  *                  using the SM4 CTR algorithm.
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

/* CTR context handle */
cmox_ctr_handle_t Ctr_Ctx;

/** Extract from IETF draft-ribose-cfrg-sm4-10
  * A.2.5.  SM4-CTR Examples
  * A.2.5.1.  Example 1
   Plaintext:

   AA AA AA AA AA AA AA AA BB BB BB BB BB BB BB BB
   CC CC CC CC CC CC CC CC DD DD DD DD DD DD DD DD
   EE EE EE EE EE EE EE EE FF FF FF FF FF FF FF FF
   AA AA AA AA AA AA AA AA BB BB BB BB BB BB BB BB

   Encryption Key:

   01 23 45 67 89 AB CD EF FE DC BA 98 76 54 32 10

   IV:

   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F

   Ciphertext:

   AC 32 36 CB 97 0C C2 07 91 36 4C 39 5A 13 42 D1
   A3 CB C1 87 8C 6F 30 CD 07 4C CE 38 5C DD 70 C7
   F2 34 BC 0E 24 C1 19 80 FD 12 86 31 0C E3 7B 92
   6E 02 FC D0 FA A0 BA F3 8B 29 33 85 1D 82 45 14
  */
const uint8_t Key[] =
{
  0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10
};
const uint8_t IV[] =
{
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};
const uint8_t plainText[] =
{
  0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB,
  0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD,
  0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB
};
const uint8_t expectedCipherText[] =
{
  0xAC, 0x32, 0x36, 0xCB, 0x97, 0x0C, 0xC2, 0x07, 0x91, 0x36, 0x4C, 0x39, 0x5A, 0x13, 0x42, 0xD1,
  0xA3, 0xCB, 0xC1, 0x87, 0x8C, 0x6F, 0x30, 0xCD, 0x07, 0x4C, 0xCE, 0x38, 0x5C, 0xDD, 0x70, 0xC7,
  0xF2, 0x34, 0xBC, 0x0E, 0x24, 0xC1, 0x19, 0x80, 0xFD, 0x12, 0x86, 0x31, 0x0C, 0xE3, 0x7B, 0x92,
  0x6E, 0x02, 0xFC, 0xD0, 0xFA, 0xA0, 0xBA, 0xF3, 0x8B, 0x29, 0x33, 0x85, 0x1D, 0x82, 0x45, 0x14
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
  * using the SM4 CTR algorithm in 'SINGLE CALL USAGE' way.
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
  retval = cmox_cipher_encrypt(CMOX_SM4_CTR_ENC_ALGO,                  /* Use SM4 CTR algorithm */
                               plainText, sizeof(plainText),           /* plainText to encrypt */
                               Key, sizeof(Key),                       /* AES key to use */
                               IV, sizeof(IV),                         /* Initialization vector */
                               Computed_Ciphertext, &computed_size);   /* Data buffer to receive generated
                                                                          ciphertext */

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
  retval = cmox_cipher_decrypt(CMOX_SM4_CTR_DEC_ALGO,                          /* Use SM4 CTR algorithm */
                               expectedCipherText, sizeof(expectedCipherText), /* Ciphertext to decrypt */
                               Key, sizeof(Key),                               /* AES key to use */
                               IV, sizeof(IV),                                 /* Initialization vector */
                               Computed_Plaintext, &computed_size);            /* Data buffer to receive generated
                                                                                  plaintext */

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
    * using the SM4 CTR algorithm, in 'MULTIPLE CALL USAG' method.
    */

  /* Construct a cipher context that is configured to perform SM4 CTR encryption operations */
  p_cipher_ctx = cmox_ctr_construct(&Ctr_Ctx, CMOX_SM4_CTR_ENC);
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
                                &plainText[index], CHUNK_SIZE,     /* Chunk of plainText to authenticate and encrypt */
                                Computed_Ciphertext, &computed_size);  /* Data buffer to receive generated chunk
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
                                sizeof(plainText) - index,              /* Last part of plainText to encrypt */
                                Computed_Ciphertext, &computed_size);   /* Data buffer to receive generated last
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

  /* Construct a cipher context that is configured to perform SM4 CTR decryption operations */
  p_cipher_ctx = cmox_ctr_construct(&Ctr_Ctx, CMOX_SM4_CTR_DEC);
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
                                Computed_Plaintext, &computed_size);   /* Data buffer to receive generated last
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
