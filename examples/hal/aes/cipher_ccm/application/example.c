/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to use the STM32 HAL API to perform encryption
  *                  and decryption in polling mode using the AES CCM algorithm.
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
hal_aes_handle_t *pAES; /* pointer referencing the AES handle from the generated code */
hal_aes_ccm_config_t p_ccm_config;

/** Extract from NIST Special Publication 800-38A
  * CCM Test Vectors SP 800-38C
  * Key 40414243 44454647 48494a4b 4c4d4e4f
  * Block
  * BlockB0 56101112 13141516 17000000 00000010
  * BlockB1 00100001 02030405 06070809
  * 0A0B0C0D 0E0F0000 00000000 00000000 00000000
  * plainText 20212223 24252627 28292A2B 2C2D2E2F
  *
  */
uint32_t AES128Key[4] = {0x40414243, 0x44454647, 0x48494a4b, 0x4c4d4e4f};
uint32_t Block_B0[4] = {0x56101112, 0x13141516, 0x17000000, 0x00000010};
uint32_t BlockB1[8] =
{
  0x00100001, 0x02030405, 0x06070809, 0x0a0b0c0d,
  0x0e0f0000, 0x00000000, 0x00000000, 0x00000000
};
uint32_t plainText[4] = {0x20212223, 0x24252627, 0x28292a2b, 0x2c2d2e2f};
uint32_t expectedCipherText[4] = {0xd2a1f0e0, 0x51ea5f62, 0x081a7792, 0x073d593d};

/* Used for storing the encrypted text */
uint32_t computedCiphertext[4] = {0};

/* Used for storing the decrypted text */
uint32_t computedPlaintext[4] = {0};


/* Private functions prototype -----------------------------------------------*/
/** ########## Step 1 ##########
  * Initializes and configures the AES instance.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pAES = mx_example_aes_init();
  if (pAES != NULL)
  {
    PRINTF("[INFO] Step 1: Initialization and configuration of the AES peripheral to work with CCM mode COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }
  else
  {
    PRINTF("[ERROR] Step 1: AES initialization ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
    goto _app_init_exit;
  }

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Encrypts a known plaintext and verifies the generated encrypted ciphertext,
  * then decrypts a known ciphertext and verifies the generated decrypted plaintext.
  * This step is scheduled as follows:
  * Step 2.1: Encrypts a known plaintext and verifies the generated ciphertext.
  * Step 2.2: Configures the AES peripheral with the CCM algorithm for decryption process.
  * Step 2.3: Decrypts a known ciphertext then verifies the generated plaintext.
  * Step 2.4: Clears all data.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  /* Set the AES CCM configuration parameters */

  /** ########## Step 2.1 ##########
    * Encrypts a known plaintext.
  */
  if (HAL_AES_Encrypt(pAES, plainText, 16, computedCiphertext, AES_TIMEOUT_MS) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* Verify the generated */
  if (memcmp(computedCiphertext, expectedCipherText, 16) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: Encryption and verification of a known plaintext COMPLETED.\n");

  /** ########## Step 2.2 ##########
    * Configures the AES peripheral with the CCM algorithm for decryption process.
    * Configuration is required before each encryption or decryption process
    */

  p_ccm_config.p_b0 = Block_B0;
  p_ccm_config.p_header             = BlockB1;
  p_ccm_config.header_size_byte     = 18;
  HAL_AES_CCM_SetConfig(pAES, &p_ccm_config);
  HAL_AES_SetNormalKey(pAES, HAL_AES_KEY_SIZE_128BIT, AES128Key);
  HAL_AES_SetDataSwapping(pAES, HAL_AES_DATA_SWAPPING_NO);

  PRINTF("[INFO] Step 2.2: Configuration of the AES peripheral to work with CCM in decryption mode COMPLETED.\n");

  /** ########## Step 2.3 ##########
    * Decrypts a known ciphertext then verifies the generated plaintext.
    */
  if (HAL_AES_Decrypt(pAES, computedCiphertext, 16, computedPlaintext, AES_TIMEOUT_MS) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* Verify generated generated plaintext */
  if (memcmp(computedPlaintext, plainText, 16) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.3: Decryption and verification of a known ciphertext COMPLETED.\n");

  /** ########## Step 2.4 ##########
    * Clears all data.
    */
  memset((void *)computedCiphertext, 0U, sizeof(computedCiphertext));
  memset((void *)computedPlaintext, 0U, sizeof(computedPlaintext));

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

  PRINTF("[INFO] Step 3: AES deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */
