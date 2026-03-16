/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to use the STM32 HAL API to perform encryption
  *                  and decryption with the SAES Hardware key using the AES CBC algorithm.
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
/* timeout value in milliseconds */
#define AES_TIMEOUT 100
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_aes_handle_t *pAES;  /* pointer referencing the AES handle from the generated code */
hal_aes_handle_t *pSAES; /* pointer referencing the SAES handle from the generated code */

/** Extract from NIST Special Publication 800-38A
  * F.2.1 CBC-AES128.Encrypt
  * Key 2b7e151628aed2a6abf7158809cf4f3c
  * IV 000102030405060708090a0b0c0d0e0f
  * Block #1
  * Plaintext 6bc1bee22e409f96e93d7e117393172a
  * Input Block 6bc0bce12a459991e134741a7f9e1925
  * Output Block 7649abac8119b246cee98e9b12e9197d
  * Ciphertext 7649abac 8119b246 cee98e9b 12e9197d
  * Block #2
  * Plaintext ae2d8a571e03ac9c9eb76fac45af8e51
  * Input Block d86421fb9f1a1eda505ee1375746972c
  * Output Block 5086cb9b507219ee95db113a917678b2
  * Ciphertext 5086cb9b 507219ee 95db113a 917678b2
  * Block #3
  * Plaintext 30c81c46a35ce411e5fbc1191a0a52ef
  * Input Block 604ed7ddf32efdff7020d0238b7c2a5d
  * Output Block 73bed6b8e3c1743b7116e69e22229516
  * Ciphertext 73bed6b8 e3c1743b 7116e69e 22229516
  * Block #4
  * Plaintext f69f2445df4f9b17ad2b417be66c3710
  * Input Block 8521f2fd3c8eef2cdc3da7e5c44ea206
  * Output Block 3ff1caa1681fac09120eca307586e1a7
  * Ciphertext 3ff1caa1 681fac09 120eca30 7586e1a7
  */
const uint32_t Key[4] =
{
  0x2b7e1516, 0x28aed2a6, 0xabf71588, 0x09cf4f3c
};

/* Initialization vector */
const uint32_t IV[4] =
{
  0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f
};

const uint32_t plainText[16] =
{
  0x6bc1bee2, 0x2e409f96, 0xe93d7e11, 0x7393172a,
  0xae2d8a57, 0x1e03ac9c, 0x9eb76fac, 0x45af8e51,
  0x30c81c46, 0xa35ce411, 0xe5fbc119, 0x1a0a52ef,
  0xf69f2445, 0xdf4f9b17, 0xad2b417b, 0xe66c3710
};

const uint32_t expectedCipherText[16] =
{
  0x7649abac, 0x8119b246, 0xcee98e9b, 0x12e9197d,
  0x5086cb9b, 0x507219ee, 0x95db113a, 0x917678b2,
  0x73bed6b8, 0xe3c1743b, 0x7116e69e, 0x22229516,
  0x3ff1caa1, 0x681fac09, 0x120eca30, 0x7586e1a7
};

/* Computed data buffers */
uint32_t computedCiphertext[16] = {0};
uint32_t computedPlaintext[16] = {0};
uint32_t encryptedKey128Bit[4] = {0};

/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * Initializes the AES/SAES instances.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_INIT_OK;

  pAES = mx_example_aes_init();
  if (pAES != NULL)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }
  else
  {
    PRINTF("[ERROR] Step 1: AES initialization ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
  }

  pSAES = mx_example_saes_init();

  if (pSAES != NULL)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }
  else
  {
    PRINTF("[ERROR] Step 1: SAES initialization ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
  }

  PRINTF("[INFO] Step 1: AES/SAES initialization COMPLETED.\n");

  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Uses the shared key to encrypt a known plaintext and to verify the the generated ciphertext.
  * Then decrypts a known ciphertext and verifies the the generated plaintext.
  * This step is scheduled as follows:
  * Step 2.1: Configures the AES Key with a HW key source and CBC algorithm.
  * Step 2.2: Encrypts an application key to be shared with AES peripheral, using SAES HW key.
  * Step 2.3: Configures the AES Key with a HW key source and CBC algorithm.
  * Step 2.4: Decrypts an application key to be shared with AES peripheral, using SAES HW key.
  * Step 2.5: Configures the AES peripheral with the CBC algorithm with encryption method.
  * Step 2.6: Encrypts of a known plaintext then verifies the generated ciphertext.
  * Step 2.7: Configures the AES peripheral with the CBC algorithm with decryption method.
  * Step 2.8: Decrypts of a known ciphertext then verifies the generated plaintext.
  * Step 2.9: Clears all data.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t retval;

  /** ########## Step 2.1 ##########
    * Configures the AES Key with a HW key source and CBC algorithm.
    */
  HAL_AES_CBC_SetConfig(pSAES, IV);
  HAL_AES_SetHWKey(pSAES, HAL_AES_KEY_SIZE_128BIT, HAL_AES_KEY_SELECT_DHUK, HAL_AES_KEY_MODE_SHARED);

  PRINTF("[INFO] Step 2.1: Configuration of the HW key source and CBC algorithm COMPLETED.\n");

  /** ########## Step 2.2 ##########
    * Encrypts an application key to be shared with AES peripheral, using SAES HW key.
    */
  HAL_AES_EncryptSharedKey(pSAES, Key, HAL_AES_KEY_SIZE_128BIT, encryptedKey128Bit, 0, AES_TIMEOUT);

  PRINTF("[INFO] Step 2.2: Encryption and verification of the key to be shared COMPLETED.\n");

  /** ########## Step 2.3 ##########
    * Configures the AES Key with a HW key source and CBC algorithm.
    */
  HAL_AES_CBC_SetConfig(pSAES, IV);
  HAL_AES_SetHWKey(pSAES, HAL_AES_KEY_SIZE_128BIT, HAL_AES_KEY_SELECT_DHUK, HAL_AES_KEY_MODE_SHARED);

  PRINTF("[INFO] Step 2.3: Configuration of the HW key source and CBC algorithm COMPLETED.\n");

  /** ########## Step 2.4 ##########
    * Decrypts an application key to be shared with AES peripheral, using SAES HW key.
    */
  HAL_AES_DecryptSharedKey(pSAES, encryptedKey128Bit, HAL_AES_KEY_SIZE_128BIT, 0, AES_TIMEOUT);

  PRINTF("[INFO] Step 2.4: Decryption and verification of the key to be shared COMPLETED.\n");

  /** ########## Step 2.5 ##########
    * Configures the AES peripheral with the CBC algorithm with encryption method.
    */
  HAL_AES_CBC_SetConfig(pAES, IV);
  HAL_AES_SetSharedKey(pAES, HAL_AES_KEY_SIZE_128BIT);
  HAL_AES_SetDataSwapping(pAES, HAL_AES_DATA_SWAPPING_NO);

  PRINTF("[INFO] Step 2.5: Configuration of the AES peripheral to work with CBC mode COMPLETED.\n");

  /** ########## Step 2.6 ##########
    * Encrypts a known plaintext then verifies the generated ciphertext.
    */
  retval = HAL_AES_Encrypt(pAES,                /* Pointer to a hal_aes_handle_t structure */
                           plainText,           /* Pointer to aligned **const void** plaintext */
                           64,                  /* Length of the plaintext buffer that must be in byte */
                           computedCiphertext,  /* Pointer to data buffer to be filled with the encrypted text */
                           AES_TIMEOUT);        /* Specify timeout value in milliseconds */

  /* Verify API returned value */
  if (retval != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (memcmp(computedCiphertext, expectedCipherText, 64) != 0)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.6: Encryption and verification of a known plaintext COMPLETED.\n");

  /** ########## Step 2.7 ##########
    * Configures the AES peripheral with the CBC algorithm with decryption method.
    */
  HAL_AES_CBC_SetConfig(pAES, IV);
  HAL_AES_SetSharedKey(pAES, HAL_AES_KEY_SIZE_128BIT);
  HAL_AES_SetDataSwapping(pAES, HAL_AES_DATA_SWAPPING_NO);

  PRINTF("[INFO] Step 2.7: Configuration of the AES peripheral to work with CBC mode COMPLETED.\n");

  /** ########## Step 2.8 ##########
    * Decrypts a known ciphertext then verifies the generated plaintext.
    */
  retval = HAL_AES_Decrypt(pAES,                /* Pointer to a hal_aes_handle_t structure */
                           computedCiphertext,  /* Pointer to aligned **const void** ciphertext */
                           64,                  /* Length of the ciphertext buffer that must be in byte */
                           computedPlaintext,   /* Pointer data buffer to be filled with the decrypted text */
                           AES_TIMEOUT);        /* Specify timeout value in milliseconds */

  /* Verify API returned value */
  if (retval != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* Verify generated data size is the expected one */
  if (memcmp(computedPlaintext, plainText, 64) != 0)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.8: Decryption and verification of a known ciphertext COMPLETED.\n");

  /** ########## Step 2.9 ##########
    * Clears all data.
    */
  memset((void *)computedCiphertext, 0U, sizeof(computedCiphertext));
  memset((void *)computedPlaintext, 0U, sizeof(computedPlaintext));
  memset((void *)encryptedKey128Bit, 0U, sizeof(encryptedKey128Bit));
  PRINTF("[INFO] Step 2.9: Data clearness COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Deinitializes the AES/SAES instances before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_aes_deinit();

  mx_example_saes_deinit();

  mx_example_rng_deinit();

  PRINTF("[INFO] Step 3: AES/SAES deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */
