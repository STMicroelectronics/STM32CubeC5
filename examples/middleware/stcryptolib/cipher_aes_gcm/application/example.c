/**
  ******************************************************************************
  * file           : example.c
  * brief          : Cipher AES GCM AuthEncrypt VerifDecrypt OSC CryptoAPI
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

/* GCM context handle */
cmox_gcm_handle_t p_gcm_ctx;

/** Extract from NIST Special Publication 800-38D
  * for more information about next variables please see:
  * https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38d.pdf
  * gcmEncryptExtIV256.rsp
[Keylen = 128]
[IVlen = 96]
[PTlen = 408]
[AADlen = 384]
[Taglen = 128]

Count = 2
Key = 463b412911767d57a0b33969e674ffe7845d313b88c6fe312f3d724be68e1fca
IV = 611ce6f9a6880750de7da6cb
PT = e7d1dcf668e2876861940e012fe52a98dacbd78ab63c08842cc9801ea581682ad54af0c34d0d7f6f59e8ee0bf4900e0fd85042
AAD = 0a682fbc6192e1b47a5e0868787ffdafe5a50cead3575849990cdd2ea9b3597749403efb4a56684f0c6bde352d4aeec5
CT = 8886e196010cb3849d9c1a182abe1eeab0a5f3ca423c3669a4a8703c0f146e8e956fb122e0d721b869d2b6fcd4216d7d4d3758
Tag = 2469cecd70fd98fec9264f71df1aee9a
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
const uint8_t PlainText[] =
{
  0xe7, 0xd1, 0xdc, 0xf6, 0x68, 0xe2, 0x87, 0x68, 0x61, 0x94, 0x0e, 0x01, 0x2f, 0xe5, 0x2a, 0x98,
  0xda, 0xcb, 0xd7, 0x8a, 0xb6, 0x3c, 0x08, 0x84, 0x2c, 0xc9, 0x80, 0x1e, 0xa5, 0x81, 0x68, 0x2a,
  0xd5, 0x4a, 0xf0, 0xc3, 0x4d, 0x0d, 0x7f, 0x6f, 0x59, 0xe8, 0xee, 0x0b, 0xf4, 0x90, 0x0e, 0x0f,
  0xd8, 0x50, 0x42
};
const uint8_t AddData[] =
{
  0x0a, 0x68, 0x2f, 0xbc, 0x61, 0x92, 0xe1, 0xb4, 0x7a, 0x5e, 0x08, 0x68, 0x78, 0x7f, 0xfd, 0xaf,
  0xe5, 0xa5, 0x0c, 0xea, 0xd3, 0x57, 0x58, 0x49, 0x99, 0x0c, 0xdd, 0x2e, 0xa9, 0xb3, 0x59, 0x77,
  0x49, 0x40, 0x3e, 0xfb, 0x4a, 0x56, 0x68, 0x4f, 0x0c, 0x6b, 0xde, 0x35, 0x2d, 0x4a, 0xee, 0xc5
};
const uint8_t ExpectedCipherText[] =
{
  /* Ciphertext */
  0x88, 0x86, 0xe1, 0x96, 0x01, 0x0c, 0xb3, 0x84, 0x9d, 0x9c, 0x1a, 0x18, 0x2a, 0xbe, 0x1e, 0xea,
  0xb0, 0xa5, 0xf3, 0xca, 0x42, 0x3c, 0x36, 0x69, 0xa4, 0xa8, 0x70, 0x3c, 0x0f, 0x14, 0x6e, 0x8e,
  0x95, 0x6f, 0xb1, 0x22, 0xe0, 0xd7, 0x21, 0xb8, 0x69, 0xd2, 0xb6, 0xfc, 0xd4, 0x21, 0x6d, 0x7d,
  0x4d, 0x37, 0x58,
};
const uint8_t ExpectedTag[] =
{
  0x24, 0x69, 0xce, 0xcd, 0x70, 0xfd, 0x98, 0xfe, 0xc9, 0x26, 0x4f, 0x71, 0xdf, 0x1a, 0xee, 0x9a
};

const uint8_t ExpectedCiphertextAndTag[] =
{
  0x88, 0x86, 0xe1, 0x96, 0x01, 0x0c, 0xb3, 0x84, 0x9d, 0x9c, 0x1a, 0x18, 0x2a, 0xbe, 0x1e, 0xea,
  0xb0, 0xa5, 0xf3, 0xca, 0x42, 0x3c, 0x36, 0x69, 0xa4, 0xa8, 0x70, 0x3c, 0x0f, 0x14, 0x6e, 0x8e,
  0x95, 0x6f, 0xb1, 0x22, 0xe0, 0xd7, 0x21, 0xb8, 0x69, 0xd2, 0xb6, 0xfc, 0xd4, 0x21, 0x6d, 0x7d,
  0x4d, 0x37, 0x58,
  0x24, 0x69, 0xce, 0xcd, 0x70, 0xfd, 0x98, 0xfe, 0xc9, 0x26, 0x4f, 0x71, 0xdf, 0x1a, 0xee, 0x9a
};

/* Computed data buffer */
uint8_t Computed_Ciphertext[sizeof(ExpectedCipherText) + sizeof(ExpectedTag)];
uint8_t Computed_Plaintext[sizeof(PlainText)];

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The initialization is triggered by the applicative code
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Initialize STM32 Cryptographic library */
  if (cmox_initialize(NULL) != CMOX_INIT_SUCCESS)
  {
    PRINTF("[ERROR] Step 1.0: STM32 Cryptographic library initialization ERROR.\n");
    goto _app_init_exit;
  }
  else
  {
    PRINTF("[INFO] Step 1.0: STM32 Cryptographic library initialization COMPLETED.\n");
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Demonstrate authenticated encryption and verified decryption
  * of a known ciphertext, using the AES GCM algorithm in 'SINGLE CALL USAGE' way.
  */
app_status_t app_process(void)
{
  /* General cipher context */
  cmox_cipher_handle_t *p_cipher_ctx;
  app_status_t return_status = EXEC_STATUS_ERROR;
  cmox_cipher_retval_t retval;
  size_t computed_size;
  uint32_t index;

  /* Fault check verification variable */
  uint32_t fault_check = CMOX_CIPHER_AUTH_FAIL;

  /* Encryption of a known plainText, compute directly the ciphertext passing all the needed parameters */
  retval = cmox_aead_encrypt(CMOX_AES_GCM_ENC_ALGO,                  /* Use AES GCM algorithm */
                             PlainText, sizeof(PlainText),           /* PlainText to encrypt */
                             sizeof(ExpectedTag),                    /* Authentication tag size */
                             Key, sizeof(Key),                       /* AES key to use */
                             IV, sizeof(IV),                         /* Initialization vector */
                             AddData, sizeof(AddData),               /* Additional authenticated data */
                             Computed_Ciphertext, &computed_size);   /* Data buffer to receive generated ciphertext
                                                                        and authentication tag */

  /* Verify API returned value */
  if (retval != CMOX_CIPHER_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: Authenticates Encryption of a known PlainText COMPLETED.\n");

  /* Verify generated data size is the expected one */
  if (computed_size != (sizeof(ExpectedCipherText) + sizeof(ExpectedTag)))
  {
    goto _app_process_exit;
  }

  /* Verify generated data are the expected ones */
  if (memcmp(ExpectedCipherText, Computed_Ciphertext, sizeof(ExpectedCipherText)) != 0)
  {
    goto _app_process_exit;
  }

  /* Verify generated authentication tag is the expected one */
  if (memcmp(ExpectedTag, &Computed_Ciphertext[sizeof(ExpectedCipherText)], sizeof(ExpectedTag)) != 0)
  {
    goto _app_process_exit;
  }

  /* Decryption of a known ciphertext, compute directly the plaintext passing all the needed parameters */
  retval = cmox_aead_decrypt(CMOX_AES_GCM_DEC_ALGO,                  /* Use AES GCM algorithm */
                             Computed_Ciphertext, computed_size,     /* Ciphertext + tag to decrypt and verify */
                             sizeof(ExpectedTag),                    /* Authentication tag size */
                             Key, sizeof(Key),                       /* AES key to use */
                             IV, sizeof(IV),                         /* Initialization vector */
                             AddData, sizeof(AddData),               /* Additional authenticated data */
                             Computed_Plaintext, &computed_size);    /* Data buffer to receive generated PlainText */

  /* Verify API returned value */
  if (retval != CMOX_CIPHER_AUTH_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: Verifies Decryption with a known ciphertext COMPLETED.\n");

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

  /** ########## Step 3 ##########
    * Demonstrate authenticated encryption and verified decryption of a known ciphertext,
    * using the AES GCM algorithm, in 'MULTIPLE CALL USAG' method.
    */

  /* Construct a cipher context that is configured to perform AES GCM encryption operations */
  p_cipher_ctx = cmox_gcm_construct(&p_gcm_ctx, CMOX_AES_GCM_ENC);
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

  /* Setup of the authentication tag length into the context */
  retval = cmox_cipher_setTagLen(p_cipher_ctx, sizeof(ExpectedTag));  /* Authentication tag size */
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

  /* Inject the additional authenticated data */
  retval = cmox_cipher_appendAD(p_cipher_ctx, AddData, sizeof(AddData));  /* Additional authenticated data */
  if (retval != CMOX_CIPHER_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3.1: Constructs, initializes and setups needed data into encryption cipher context COMPLETED.\n");

  /* Encrypt the PlainText in multiple steps by appending chunks of CHUNK_SIZE bytes */
  for (index = 0; index < (sizeof(PlainText) - CHUNK_SIZE); index += CHUNK_SIZE)
  {
    retval = cmox_cipher_append(p_cipher_ctx,
                                &PlainText[index], CHUNK_SIZE,    /* Chunk of PlainText to authenticate and encrypt */
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
    if (memcmp(&ExpectedCipherText[index], Computed_Ciphertext, computed_size) != 0)
    {
      goto _app_process_exit;
    }
  }

  /* Process with encryption of the last part if needed */
  if (index < sizeof(PlainText))
  {
    retval = cmox_cipher_append(p_cipher_ctx,
                                &PlainText[index], sizeof(PlainText) - index,   /* Last part of PlainText to encrypt */
                                Computed_Ciphertext, &computed_size);   /* Data buffer to receive generated last part
                                                                           of ciphertext */

    /* Verify API returned value */
    if (retval != CMOX_CIPHER_SUCCESS)
    {
      goto _app_process_exit;
    }

    /* Verify generated data size is the expected one */
    if (computed_size != (sizeof(PlainText) - index))
    {
      goto _app_process_exit;
    }

    /* Verify generated data are the expected ones */
    if (memcmp(&ExpectedCipherText[index], Computed_Ciphertext, computed_size) != 0)
    {
      goto _app_process_exit;
    }
  }

  PRINTF("[INFO] Step 3.2: Authenticates Encryption of plainText in multiple steps COMPLETED.\n");

  /* Generate the authentication tag */
  retval = cmox_cipher_generateTag(p_cipher_ctx,
                                   Computed_Ciphertext, &computed_size);  /* Data buffer to receive
                                                                             the authentication tag */

  /* Verify API returned value */
  if (retval != CMOX_CIPHER_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated authentication tag size is the expected one */
  if (computed_size != sizeof(ExpectedTag))
  {
    goto _app_process_exit;
  }

  /* Verify generated authentication tag is the expected one */
  if (memcmp(ExpectedTag, Computed_Ciphertext, computed_size) != 0)
  {
    goto _app_process_exit;
  }

  /* Cleanup the context */
  retval = cmox_cipher_cleanup(p_cipher_ctx);
  if (retval != CMOX_CIPHER_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3.3: Verifies authentication tag and cleans up encryption cipher context COMPLETED.\n");

  p_cipher_ctx = cmox_gcm_construct(&p_gcm_ctx, CMOX_AES_GCM_DEC);
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

  /* Setup of the authentication tag length into the context */
  retval = cmox_cipher_setTagLen(p_cipher_ctx, sizeof(ExpectedTag));  /* Authentication tag size */
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

  /* Inject the additional authenticated data */
  retval = cmox_cipher_appendAD(p_cipher_ctx, AddData, sizeof(AddData));  /* Additional authenticated data */
  if (retval != CMOX_CIPHER_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3.4: Constructs, initializes and setups needed data into decryption cipher context COMPLETED.\n");

  /* Decrypt the PlainText in multiple steps by appending chunks of CHUNK_SIZE bytes */
  for (index = 0; index < (sizeof(ExpectedCipherText) - CHUNK_SIZE); index += CHUNK_SIZE)
  {
    retval = cmox_cipher_append(p_cipher_ctx,
                                &ExpectedCipherText[index], CHUNK_SIZE,  /* Chunk of ciphertext to decrypt and verify */
                                Computed_Plaintext, &computed_size);     /* Data buffer to receive generated
                                                                            chunk of PlainText */

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
    if (memcmp(&PlainText[index], Computed_Plaintext, computed_size) != 0)
    {
      goto _app_process_exit;
    }
  }
  /* Process with encryption of the last part if needed */
  if (index < sizeof(ExpectedCipherText))
  {
    retval = cmox_cipher_append(p_cipher_ctx,
                                &ExpectedCipherText[index], /* Last part of ciphertext to decrypt and verify */
                                sizeof(ExpectedCipherText) - index,
                                Computed_Plaintext, /* Data buffer to receive generated last part of PlainText */
                                &computed_size);

    /* Verify API returned value */
    if (retval != CMOX_CIPHER_SUCCESS)
    {
      goto _app_process_exit;
    }

    /* Verify generated data size is the expected one */
    if (computed_size != (sizeof(ExpectedCipherText) - index))
    {
      goto _app_process_exit;
    }

    /* Verify generated data are the expected ones */
    if (memcmp(&PlainText[index], Computed_Plaintext, computed_size) != 0)
    {
      goto _app_process_exit;
    }
  }

  PRINTF("[INFO] Step 3.5: Verifies Decryption of PlainText in multiple steps COMPLETED.\n");

  /* Verify the authentication tag */
  retval = cmox_cipher_verifyTag(p_cipher_ctx,
                                 ExpectedTag,       /* Authentication tag used for verification */
                                 &fault_check);     /* Fault check variable:
                                                       to ensure no fault injection occurs during this API call */

  /* Verify API returned value */
  if (retval != CMOX_CIPHER_AUTH_SUCCESS)
  {
    goto _app_process_exit;
  }
  /* Verify fault_check variable value */
  if (fault_check != CMOX_CIPHER_AUTH_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Cleanup the handle */
  retval = cmox_cipher_cleanup(p_cipher_ctx);
  if (retval != CMOX_CIPHER_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3.6: Verifies authentication tag and cleans up decryption cipher context COMPLETED.\n");

  /* No more need of cryptographic services, finalize STM32 Cryptographic library */
  if (cmox_finalize(NULL) != CMOX_INIT_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 4.0: Finalizes STM32 Cryptographic library COMPLETED.\n \n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 4 ##########
  * Deinitialization before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  return EXEC_STATUS_OK;
} /* end app_deinit */

