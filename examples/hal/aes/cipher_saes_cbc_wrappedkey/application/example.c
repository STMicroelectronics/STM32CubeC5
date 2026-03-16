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
const uint32_t Key[8] =
{
  0x603DEB10, 0x15CA71BE, 0x2B73AEF0, 0x857D7781,
  0x1F352C07, 0x3B6108D7, 0x2D9810A3, 0x0914DFF4
};

/* Initialization vector */
const uint32_t IV[4] =
{
  0x00010203, 0x04050607, 0x08090A0B, 0x0C0D0E0F
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
  0xF58C4C04, 0xD6E5F1BA, 0x779EABFB, 0x5F7BFBD6,
  0x9CFC4e96, 0x7EDB808D, 0x679F777B, 0xC6702C7D,
  0x39F23369, 0xA9D9BACF, 0xA530E263, 0x04231461,
  0xB2EB05E2, 0xC39BE9FC, 0xDA6C1907, 0x8C6A9D1B
};
/* Computed data buffers */
uint32_t computedCiphertext[16] = {0};
uint32_t computedPlaintext [16] = {0};
uint32_t encryptedKey256Bit[8] = {0};

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * Initializes the SAES instance and configures the HW key source.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_INIT_OK;

  pSAES = mx_example_saes_init();

  if (pSAES != NULL)
  {
    PRINTF("[INFO] Step 1: SAES initialization, configuration of the HW key source and CBC algorithm COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }
  else
  {
    PRINTF("[ERROR] Step 1: SAES initialization ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
  }


  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Encrypts a known plaintext and verifies the generated ciphertext
  * then decrypts a known ciphertext and verifies the generated plaintext using the a wrapped key.
  * This step is scheduled as follows:
  * Step 2.1: Encrypts an application key with SAES HW keys(wrapper key).
  * Step 2.2: Configures the AES Key with a HW key source and CBC algorithm.
  * Step 2.3: Decrypts an application key with SAES HW keys(wrapper key).
  * Step 2.4: Configures the AES peripheral with the CBC algorithm with encryption method.
  * Step 2.5: Encrypts of a known plaintext then verifies the generated ciphertext.
  * Step 2.6: Configures the AES peripheral with the CBC algorithm with decryption method.
  * Step 2.7: Decrypts of a known ciphertext then verifies the generated plaintext.
  * Step 2.8: Clears all data.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t retval;

  /** ########## Step 2.1 ##########
    * Encrypts an application key with SAES HW keys(wrapper key).
    */
  HAL_AES_WrapKey(pSAES, Key, HAL_AES_KEY_SIZE_256BIT, encryptedKey256Bit, AES_TIMEOUT);
  PRINTF("[INFO] Step 2.1: Encryption of an application key with SAES HW key(wrapper key) COMPLETED.\n");

  /** ########## Step 2.2 ##########
    * Configures the AES Key with a HW key source and CBC algorithm.
    */
  HAL_AES_CBC_SetConfig(pSAES, IV);
  HAL_AES_SetHWKey(pSAES, HAL_AES_KEY_SIZE_256BIT, HAL_AES_KEY_SELECT_DHUK, HAL_AES_KEY_MODE_WRAPPED);
  PRINTF("[INFO] Step 2.2: Configuration of the HW key source and CBC algorithm COMPLETED.\n");

  /** ########## Step 2.3 ##########
    * Decrypts an application key with SAES HW keys(wrapper key).
    */
  HAL_AES_UnwrapKey(pSAES, encryptedKey256Bit, HAL_AES_KEY_SIZE_256BIT, AES_TIMEOUT);
  PRINTF("[INFO] Step 2.3: Decryption of an application key with SAES HW key(wrapper key) COMPLETED.\n");

  /** ########## Step 2.4 ##########
    * Configures the AES peripheral with the CBC algorithm with encryption method.
    */
  HAL_AES_CBC_SetConfig(pSAES, IV);
  HAL_AES_SetDataSwapping(pSAES, HAL_AES_DATA_SWAPPING_NO);
  PRINTF("[INFO] Step 2.4: Configuration of the AES peripheral to work with CBC mode COMPLETED.\n");


  /** ########## Step 2.5 ##########
    * Encrypts of a known plaintext then verifies the generated ciphertext.
    */
  retval = HAL_AES_Encrypt(pSAES,                /* Pointer to a hal_aes_handle_t structure */
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
  PRINTF("[INFO] Step 2.5: Encryption and verification of a known plaintext COMPLETED.\n");

  /** ########## Step 2.6 ##########
    * Configures the AES peripheral with the CBC algorithm with decryption method.
    */
  HAL_AES_CBC_SetConfig(pSAES, IV);
  HAL_AES_SetDataSwapping(pSAES, HAL_AES_DATA_SWAPPING_NO);
  PRINTF("[INFO] Step 2.6: Configuration of the AES peripheral to work with CBC mode COMPLETED.\n");

  /** ########## Step 2.7 ##########
    * Decrypts of a known ciphertext then verifies the generated plaintext.
    */
  retval = HAL_AES_Decrypt(pSAES,                /* Pointer to a hal_aes_handle_t structure */
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
  PRINTF("[INFO] Step 2.7: Decryption and verification of a known ciphertext COMPLETED.\n");

  /** ########## Step 2.8 ##########
    * Clears all data.
    */
  memset((void *)computedCiphertext, 0U, sizeof(computedCiphertext));
  memset((void *)computedPlaintext, 0U, sizeof(computedPlaintext));
  memset((void *)encryptedKey256Bit, 0U, sizeof(encryptedKey256Bit));
  PRINTF("[INFO] Step 2.8: Data clearness COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Deinitializes the SAES instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_saes_deinit();
  PRINTF("[INFO] Step 3: SAES deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */
