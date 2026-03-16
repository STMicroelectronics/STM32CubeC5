/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to use the STM32 HAL API to perform an authenticated
  *                  encryption/decryption with associated data (AEAD/ADAD)
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
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define AES_TIMEOUT_MS  100

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_aes_gcm_config_t p_gcm_config;  /* pointer referencing the GCM handle from the generated code */
hal_aes_handle_t *pAES;

/** Extract from NIST Special Publication 800-38D
  * for more information about next variables please see:
  * https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38d.pdf
  * GCM test vector provided by NIST Test Patterns: gcmEncryptExtIV256.rsp
  * [Keylen = 256]
  * [IVlen = 96]
  * [PTlen = 408]
  * [AADlen = 384]
  * [Taglen = 128]
  *
  * Count = 0
  * Key = 463b412911767d57a0b33969e674ffe7845d313b88c6fe312f3d724be68e1fca
  * IV = 611ce6f9a6880750de7da6cb
  * PT = e7d1dcf668e2876861940e012fe52a98dacbd78ab63c08842cc9801ea581682ad54af0c34d0d7f6f59e8ee0bf4900e0fd85042
  * AAD = 0a682fbc6192e1b47a5e0868787ffdafe5a50cead3575849990cdd2ea9b3597749403efb4a56684f0c6bde352d4aeec5
  * CT = 8886e196010cb3849d9c1a182abe1eeab0a5f3ca423c3669a4a8703c0f146e8e956fb122e0d721b869d2b6fcd4216d7d4d3758
  * Tag = 2469cecd70fd98fec9264f71df1aee9a
  */
const uint32_t Key[8] =
{
  0x463b4129, 0x11767d57, 0xa0b33969, 0xe674ffe7,
  0x845d313b, 0x88c6fe31, 0x2f3d724b, 0xe68e1fca
};

/* Initialization vector */
const uint32_t IV[4] =
{
  0x611ce6f9, 0xa6880750, 0xde7da6cb, 0x00000002
};

uint32_t plainText[13] =
{
  0xe7d1dcf6, 0x68e28768, 0x61940e01, 0x2fe52a98,
  0xdacbd78a, 0xb63c0884, 0x2cc9801e, 0xa581682a,
  0xd54af0c3, 0x4d0d7f6f, 0x59e8ee0b, 0xf4900e0f,
  0xd8504200 /* padding last word with zeros */
};

/* Additional authenticated data */
uint32_t addData[12] =
{
  0x0a682fbc, 0x6192e1b4, 0x7a5e0868, 0x787ffdaf,
  0xe5a50cea, 0xd3575849, 0x990cdd2e, 0xa9b35977,
  0x49403efb, 0x4a56684f, 0x0c6bde35, 0x2d4aeec5
};

const uint32_t expectedCipherText[13] =
{
  0x8886e196, 0x010cb384, 0x9d9c1a18, 0x2abe1eea,
  0xb0a5f3ca, 0x423c3669, 0xa4a8703c, 0x0f146e8e,
  0x956fb122, 0xe0d721b8, 0x69d2b6fc, 0xd4216d7d,
  0x4d375800 /* padding last word with zeros */
};

const uint32_t expectedTag[4] =
{
  0x2469cecd, 0x70fd98fe, 0xc9264f71, 0xdf1aee9a
};

/* Computed data buffers */
uint32_t computedCiphertext[13] = {0};
uint32_t computedPlaintext[13] = {0};
uint32_t Tag[4] = {0};

/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * Initializes the AES instance.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_INIT_OK;

  pAES = mx_example_aes_init();
  if (pAES != NULL)
  {
    PRINTF("[INFO] Step 1: AES initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }
  else
  {
    PRINTF("[ERROR] Step 1: AES initialization ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
  }

  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Demonstrates how to perform an authenticated encryption/decryption
  * with associated data (AEAD) of a known plaintext using the AES GCM algorithm.
  * This step is scheduled as follows:
  * Step 2.1: Configures the AES peripheral with the GCM_GMAC algorithm
  * Step 2.2: Performs an authenticated encryption of a known plaintext,
  *           followed by the verification of the generated ciphertext.
  * Step 2.3: Performs a verified decryption of a known ciphertext,
  *           followed by the verification of the generated plaintext.
  * Step 2.4: Clears all data.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2.1 ##########
    * Configures the AES peripheral with the GCM_GMAC algorithm
    */
  p_gcm_config.p_init_vect = (uint32_t *) IV;
  p_gcm_config.p_header = addData;
  p_gcm_config.header_size_byte = 48;  /* header (Additional Authentication Data) in bytes */
  HAL_AES_GCM_GMAC_SetConfig(pAES, &p_gcm_config);

  HAL_AES_SetNormalKey(pAES, HAL_AES_KEY_SIZE_256BIT, Key);
  HAL_AES_SetDataSwapping(pAES, HAL_AES_DATA_SWAPPING_NO);

  PRINTF("[INFO] Step 2.1: Configuration of the AES peripheral to work with GCM mode COMPLETED.\n");

  /** ########## Step 2.2 ##########
    * Performs an authenticated encryption of a known plaintext,
    * followed by the verification of the generated ciphertext.
    */
  if (HAL_AES_Encrypt(pAES,               /* Pointer to a hal_aes_handle_t structure */
                      plainText,          /* Pointer to aligned **const void** plaintext */
                      51,                 /* Length of the plaintext buffer that must be in byte */
                      computedCiphertext, /* Pointer to data buffer to be filled with the encrypted text */
                      AES_TIMEOUT_MS      /* Specify timeout value in milliseconds */
                     ) != HAL_OK)
  {
    /* Processing error */
    goto _app_process_exit;
  }

  /* Ignore last byte*/
  computedCiphertext[12] = (computedCiphertext[12] & 0xFFFFFF00);
  if (memcmp(computedCiphertext, expectedCipherText, 51)  != 0)
  {
    goto _app_process_exit;
  }

  /* Compute the authentication Tag */
  HAL_AES_GCM_GenerateAuthTAG(pAES, Tag, AES_TIMEOUT_MS);
  if (memcmp(Tag, expectedTag, 16) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: Authenticated Encryption with associated data (AEAD) of a known plaintext COMPLETED.\n");

  /** ########## Step 2.3 ##########
    * Performs a verified decryption of a known ciphertext,
    * followed by the verification of the generated plaintext.
    */
  HAL_AES_GCM_GMAC_SetConfig(pAES, &p_gcm_config);
  HAL_AES_SetNormalKey(pAES, HAL_AES_KEY_SIZE_256BIT, Key);
  HAL_AES_SetDataSwapping(pAES, HAL_AES_DATA_SWAPPING_NO);

  if (HAL_AES_Decrypt(pAES,                /* Pointer to a hal_aes_handle_t structure */
                      computedCiphertext,  /* Pointer to aligned **const void** ciphertext */
                      51,                  /* Length of the ciphertext buffer that must be in byte */
                      computedPlaintext,   /* Pointer data buffer to be filled with the decrypted text */
                      AES_TIMEOUT_MS       /* Specify timeout value in milliseconds */
                     ) != HAL_OK)
  {
    /* Processing error */
    goto _app_process_exit;
  };

  /* Ignore last byte*/
  computedPlaintext[12] = (computedPlaintext[12] & 0xFFFFFF00);

  /* Compare results with expected ones*/
  if (memcmp(computedPlaintext, plainText, 51) != 0)
  {
    goto _app_process_exit;
  }

  /* Generate the authentication tag */
  HAL_AES_GCM_GenerateAuthTAG(pAES, Tag, AES_TIMEOUT_MS);
  if (memcmp(Tag, expectedTag, 16) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.3: Authenticated Decryption with associated data (AEAD) of a known ciphertext COMPLETED.\n");

  /** ########## Step 2.4 ##########
    * Clears all data.
    */
  memset((void *)computedCiphertext, 0U, sizeof(computedCiphertext));
  memset((void *)computedPlaintext, 0U, sizeof(computedPlaintext));
  memset((void *)Tag, 0U, sizeof(Tag));

  PRINTF("[INFO] Step 2.4: Data clearness COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Deinitializes the AES instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_aes_deinit();

  PRINTF("[INFO] Step 3: AES deinitialization before leaving the scenario COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */
