/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to use the STM32 HAL API to suspend and resumes encryption,
  *                  in IT mode using the AES CBC algorithm, with modifying the context.
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_aes_handle_t *pAES; /* pointer referencing the AES handle from the generated code */
hal_tim_handle_t *pTim; /* pointer referencing the TIM handle from the generated code */
hal_aes_gcm_config_t p_gcm_config;
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
  * Extract from NIST Special Publication 800-38D
  * * for more information about next variables please see:
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
  * Tag = 2469cecd70fd98fec9264f71df1aee9
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
const uint32_t expectedCiphertext[16] =
{
  0x7649abac, 0x8119b246, 0xcee98e9b, 0x12e9197d,
  0x5086cb9b, 0x507219ee, 0x95db113a, 0x917678b2,
  0x73bed6b8, 0xe3c1743b, 0x7116e69e, 0x22229516,
  0x3ff1caa1, 0x681fac09, 0x120eca30, 0x7586e1a7
};
const uint32_t Key1[8] =
{
  0x463b4129, 0x11767d57, 0xa0b33969, 0xe674ffe7,
  0x845d313b, 0x88c6fe31, 0x2f3d724b, 0xe68e1fca
};
/* Initialization vector */
const uint32_t IV1[4] =
{
  0x611ce6f9, 0xa6880750, 0xde7da6cb, 0x00000002
};
uint32_t expectedPlaintext[13] =
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
const uint32_t decryptedText[13] =
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
uint32_t computedCiphertext[16] = {0};
uint32_t computedPlaintext[16] = {0};
uint32_t Tag[4] = {0};
/* Set to 1 if the output transfer is correctly completed */
uint32_t OutTransferCpltCb;
/* Set to 1 if a transfer error is detected */
uint32_t ErrorCb;
/* Set to 1 if a suspension is detected */
uint32_t SuspendCb;
uint32_t Suspendflag = 0;
/* Private functions prototype -----------------------------------------------*/
/* Functions allowing the user to configure dynamically the AES callbacks instead of weak functions */
static void OutTransfertCpltCallback(hal_aes_handle_t *pAES);
static void ErrorCallback(hal_aes_handle_t *pAES);
static void SuspendCallback(hal_aes_handle_t *pAES);
void HAL_TIM_UpdateCallback(hal_tim_handle_t *pTim);
/** ########## Step 1 ##########
  * Initializes the AES and the timer peripherals.
  * This step is scheduled as follows:
  * Step 1.1: Initializes the AES peripheral.
  * Step 1.2: Initializes the timer peripheral.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  /** ########## Step 1.1 ##########
    * Initializes the AES peripheral.
    */
  pAES = mx_example_aes_init();
  if (pAES == NULL)
  {
    goto _app_init_exit;
  }
  if (HAL_AES_RegisterSuspendCallback(pAES, SuspendCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  /* Register the AES callbacks */
  if (HAL_AES_RegisterOutTransferCpltCallback(pAES, OutTransfertCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_AES_RegisterErrorCallback(pAES, ErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  PRINTF("[INFO] Step 1.1: AES initialization COMPLETED.\n");
  /** ########## Step 1.2 ##########
    * Initializes the timer peripheral.
    */
  pTim = mx_example_tim_init();
  if (pTim == NULL)
  {
    goto _app_init_exit;
  }
  /* Register the user timer update Callback*/
  if (HAL_TIM_RegisterUpdateCallback(pTim, HAL_TIM_UpdateCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  PRINTF("[INFO] Step 1.2: Timer initialization COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;
_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Demonstrates how to suspend the AES CBC encryption IT process, using the timer
  * to request suspend encryption called in HAL_TIM_UpdateCallback by setting the Suspendflag.
  * Followed by a verified decryption and Authenticated Decryption with associated data
  * (AEAD) of a known text, then Resumes the IT encryption process.
  * This step is scheduled as follows:
  * Step 2.1: Suspends the IT encryption process.
  * Step 2.2: Configures the AES peripheral with the GCM algorithm.
  * Step 2.3: Performs a verified decryption of a known text, followed by the generation of the authentication tag.
  * Step 2.4: Resumes the IT encryption process.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_aes_save_context_t p_aes_context;

  HAL_TIM_Start_IT(pTim);

  /** ########## Step 2.1 ##########
    * Suspends the IT encryption process.
    */
  Suspendflag = 1;
  /* Start the encryption of plaintext in IT mode */
  if (HAL_AES_Encrypt_IT(pAES, plainText, 64, computedCiphertext) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* Wait for an IT computation process suspension. */
  while (SuspendCb == 0)
  {
    /* Put the CPU in Wait For Interrupt state.*/
    __WFI();
  }
  /* Save parameters of the suspended AES encryption */
  HAL_AES_SaveContext(pAES, &p_aes_context);

  PRINTF("[INFO] Step 2.1: Suspension of IT encryption process COMPLETED.\n");
  /** ########## Step 2.2 ##########
    * Configures the AES peripheral with the GCM algorithm.
    */
  /* Set the parameters of AES GCM structure */
  p_gcm_config.p_init_vect = (uint32_t *) IV1;
  p_gcm_config.p_header = addData;
  p_gcm_config.header_size_byte = 48;  /* header (Additional Authentication Data) in bytes */
  /* Configure the AES peripheral with the GCM_GMAC algorithm */
  HAL_AES_GCM_GMAC_SetConfig(pAES, &p_gcm_config);
  HAL_AES_SetNormalKey(pAES, HAL_AES_KEY_SIZE_256BIT, Key1);
  HAL_AES_SetDataSwapping(pAES, HAL_AES_DATA_SWAPPING_NO);

  PRINTF("[INFO] Step 2.2: Configuration of the AES peripheral to work with GCM algorithm COMPLETED.\n");

  /** ########## Step 2.3 ##########
    * Performs a verified decryption of a known text,
    * followed by the generation of the authentication tag.
    */

  /* Decrypt a message in polling mode.*/
  if (HAL_AES_Decrypt(pAES, decryptedText, 51, computedPlaintext, 1000) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* Ignore last byte*/
  computedPlaintext[12] = (computedPlaintext[12] & 0xFFFFFF00);
  /* Compare results with expected ones*/
  if (memcmp(computedPlaintext, expectedPlaintext, 51) != 0)
  {
    goto _app_process_exit;
  }
  /* Generate the authentication tag */
  HAL_AES_GCM_GenerateAuthTAG(pAES, Tag, 1000);
  /* Verify the generated tag */
  if (memcmp(Tag, expectedTag, 16) != 0)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.3: Authenticated Decryption with associated data (AEAD) of a known text COMPLETED.\n");
  /* Clear all data */
  memset((void *)computedPlaintext, 0U, sizeof(computedPlaintext));
  memset((void *)Tag, 0U, sizeof(Tag));
  /** ########## Step 2.4 ##########
    *  Resumes the IT encryption process.
    */
  /* Restore context*/
  if (HAL_AES_RestoreContext(pAES, &p_aes_context) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* Resume the IT encryption process */
  if (HAL_AES_Resume(pAES) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* Verify generated data size is the expected one */
  if (memcmp(computedCiphertext, expectedCiphertext, 64) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.4: Resumption the IT encryption process COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Deinitializes the AES and the timer peripherals before leaving the scenario.
  */

app_status_t app_deinit(void)
{
  mx_example_aes_deinit();
  mx_example_tim_deinit();

  PRINTF("[INFO] Step 3: AES and timer deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** Output FIFO transfer completed callback.
  * @user this implementation of the AES Output FIFO transfer completed callback can be customized.
  */
static void OutTransfertCpltCallback(hal_aes_handle_t *pAES)
{
  OutTransferCpltCb = 1U;
}
/** AES error callback
  * @user This implementation of the AES error callback can be customized.
  */
static void ErrorCallback(hal_aes_handle_t *pAES)
{
  ErrorCb = 1U;
}
/** AES suspend callback
  * @user This implementation of the AES suspend callback can be customized.
  */
static void SuspendCallback(hal_aes_handle_t *pAES)
{
  SuspendCb = 1U;
}
/** TIM update callback
  * @user This implementation of the TIM update callback can be customized.
  */
void HAL_TIM_UpdateCallback(hal_tim_handle_t *pTIM)
{
  if (Suspendflag == 1)
  {
    if (HAL_AES_GetState(pAES) == HAL_AES_STATE_ACTIVE)
    {
      HAL_AES_RequestSuspend(pAES);
      Suspendflag = 0;
    }
  }
}