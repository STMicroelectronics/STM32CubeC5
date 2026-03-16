/**
  ******************************************************************************
  * file           : example.c
  * brief          : This example describes how to use the STM32 Flexible Crypto
                     Framework (STFCF) to perform encryption and decryption
  *                  using the AES CCM algorithm (Counter with CBC-MAC)
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
psa_key_handle_t key_handle;

/** Extract from NIST Special Publication 800-38C
  * VTT256.rsp
[Keylen = 256]
[IVlen = 104]
[PTlen = 192]
[AADlen = 256]
[Taglen = 4]

Count = 0
Key = 9074b1ae4ca3342fe5bf6f14bcf2f27904f0b15179d95a654f61e699692e6f71
IV = 2e1e0132468500d4bd47862563
Adata = 3c5f5404370abdcb1edde99de60d0682c600b034e063b7d3237723da70ab7552
PT = 239029f150bccbd67edbb67f8ae456b4ea066a4beee065f9
CT = 9c8d5dd227fd9f81237601830afee4f0115636c8e5d5fd74
Tag = 3cb9afed
  */

static const uint8_t Key[] =
{
  0x90, 0x74, 0xb1, 0xae, 0x4c, 0xa3, 0x34, 0x2f, 0xe5, 0xbf, 0x6f, 0x14, 0xbc, 0xf2, 0xf2, 0x79,
  0x04, 0xf0, 0xb1, 0x51, 0x79, 0xd9, 0x5a, 0x65, 0x4f, 0x61, 0xe6, 0x99, 0x69, 0x2e, 0x6f, 0x71
};
static const uint8_t IV[] =
{
  0x2e, 0x1e, 0x01, 0x32, 0x46, 0x85, 0x00, 0xd4, 0xbd, 0x47, 0x86, 0x25, 0x63
};
static const uint8_t plainText[] =
{
  0x23, 0x90, 0x29, 0xf1, 0x50, 0xbc, 0xcb, 0xd6, 0x7e, 0xdb, 0xb6, 0x7f, 0x8a, 0xe4, 0x56, 0xb4,
  0xea, 0x06, 0x6a, 0x4b, 0xee, 0xe0, 0x65, 0xf9
};
static const uint8_t addData[] =
{
  0x3c, 0x5f, 0x54, 0x04, 0x37, 0x0a, 0xbd, 0xcb, 0x1e, 0xdd, 0xe9, 0x9d, 0xe6, 0x0d, 0x06, 0x82,
  0xc6, 0x00, 0xb0, 0x34, 0xe0, 0x63, 0xb7, 0xd3, 0x23, 0x77, 0x23, 0xda, 0x70, 0xab, 0x75, 0x52
};
static const uint8_t expectedCipherText[] =
{
  /* Ciphertext */
  0x9c, 0x8d, 0x5d, 0xd2, 0x27, 0xfd, 0x9f, 0x81, 0x23, 0x76, 0x01, 0x83, 0x0a, 0xfe, 0xe4, 0xf0,
  0x11, 0x56, 0x36, 0xc8, 0xe5, 0xd5, 0xfd, 0x74
};
static const uint8_t expectedTag[] =
{
  0x3c, 0xb9, 0xaf, 0xed
};

static const uint8_t expectedCiphertextAndTag[] =
{
  0x9c, 0x8d, 0x5d, 0xd2, 0x27, 0xfd, 0x9f, 0x81, 0x23, 0x76, 0x01, 0x83, 0x0a, 0xfe, 0xe4, 0xf0,
  0x11, 0x56, 0x36, 0xc8, 0xe5, 0xd5, 0xfd, 0x74,
  0x3c, 0xb9, 0xaf, 0xed
};

/* Computed data buffer */
static uint8_t Computed_Ciphertext[sizeof(expectedCipherText) + sizeof(expectedTag)];
static uint8_t Computed_Plaintext[sizeof(plainText)];

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
  psa_set_key_algorithm(&key_attributes, PSA_ALG_AEAD_WITH_SHORTENED_TAG(PSA_ALG_CCM, 4));
  psa_set_key_type(&key_attributes, PSA_KEY_TYPE_AES);
  psa_set_key_bits(&key_attributes, 8U * sizeof(Key));
  /* Set up the key location using PSA_CRYPTO_KWE_DRIVER_LOCATION to wrap the private key
    using STM32 Key Wrap Engine (KWE) */
  psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_PERSISTENCE_VOLATILE, \
                       STFCF_KEY_LOCATION));
  /* Import the private key */
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
  * of a known ciphertext using the AES CCM algorithm.
  * This step is scheduled as follows:
  * Step 2.1: Performs an authenticated encryption of a known plaintext, followed by the verification
      of the generated ciphertext.
  * Step 2.2: Performs an authenticated decryption of a known ciphertext, followed by the verification
      of the generated plaintext.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  size_t computed_size;
  psa_status_t retval;

  /** ########## Step 2.1 ##########
    * Performs an authenticated encryption of a known plaintext,
    * followed by the verification of the generated ciphertext
    */

  /* Compute directly the ciphertext passing all the needed parameters */
  retval = psa_aead_encrypt(key_handle,                         /* The key id */
                            PSA_ALG_AEAD_WITH_SHORTENED_TAG(PSA_ALG_CCM, 4), /* Algorithm type */
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
                            PSA_ALG_AEAD_WITH_SHORTENED_TAG(PSA_ALG_CCM, 4), /* Algorithm type */
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

  /* Deinitialize RNG */
  mx_example_rng_deinit();

  PRINTF("[INFO] Step 3.2: RNG deinitialization COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_deinit_exit:
  return return_status;
} /* end app_deinit */
