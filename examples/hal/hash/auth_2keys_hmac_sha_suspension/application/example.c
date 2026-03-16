/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to use the STM32 HAL API to hash data with HMAC sha256
                     algorithms in suspension mode, with different key
                     for each handle.
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
#define HASH_TIMEOUT_MS 1000
#define MESSAGE_SIZE 128
#define OUTPUT_BUFFER_SIZE 128
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_hash_handle_t *pHASH;
hal_hash_hmac_config_t p_hmac_config;
hal_hash_suspended_context_t Ctx;
hal_tim_handle_t *pTim;

/** Extract from HMAC.rsp
  *
  * Klen = 40
  * Tlen = 32
  * Key = cfd4a44910c9e567507abb6cede4fe601a7a2765c9755aa2cf6ba4814223811a26a8a1ef499cebd9
  * Msg = 3fb301cb4092f9623aa5ffd690d22d65d56e5a1c330b9c4a0d910c34e391c90a76d5401a2d3caa44b8c5d5aef3e928b
  * 90d2ee233e9f9a2cec4a32cd019d06a0dc1fcb1125f5746a4fbd32169ed7bf0e4fd065fa7c8ac97c366380484495f5c5b6850
  * dd1c9d8cd6694cf8686e46308ed0ed1f5bdf98cd831339771db63de5a7de
  * Mac = 20153bf8ea2953c48251ebcc4161f8b6e28499e5c76c24014cff4a9e2f62d25c
  *
  * Count = 31
  * Klen = 40
  * Tlen = 32
  * Key = 09675f2dcc4783b599f18fb765583668a0fd8ae4096f6fcdc60d4f35b4130fbefcd542ffe7459d2a
  * Msg = 0cf2198c31376f5c8915660137725f2bbc180a986e5a7bda27fa81593a4a339bab92cbc39fb2b8581108ee48c794812d845a7
  * 2ce8008c9e915d9e330bbb90e9136aa53ba0e6693dd4046d6b03362dfb9edfa04c887153cc5de677aab8c7839d517035879679c2972
  * 7e96c5426324a2575fbe678d6cc7fef5eb6cebd595cfddef
  * Mac = 6b142d4dfe217f1881aa0e6483b271dd5d43f70b85605953a0fef272ddde46ca
  */

uint8_t Key[] =
{
  0xcf, 0xd4, 0xa4, 0x49, 0x10, 0xc9, 0xe5, 0x67, 0x50, 0x7a, 0xbb, 0x6c, 0xed, 0xe4, 0xfe, 0x60,
  0x1a, 0x7a, 0x27, 0x65, 0xc9, 0x75, 0x5a, 0xa2, 0xcf, 0x6b, 0xa4, 0x81, 0x42, 0x23, 0x81, 0x1a,
  0x26, 0xa8, 0xa1, 0xef, 0x49, 0x9c, 0xeb, 0xd9
};
/* Known plaintext to be signed and verified */
const uint8_t Message[] =
{
  0x3f, 0xb3, 0x01, 0xcb, 0x40, 0x92, 0xf9, 0x62, 0x3a, 0xa5, 0xff, 0xd6, 0x90, 0xd2, 0x2d, 0x65,
  0xd5, 0x6e, 0x5a, 0x1c, 0x33, 0x0b, 0x9c, 0x4a, 0x0d, 0x91, 0x0c, 0x34, 0xe3, 0x91, 0xc9, 0x0a,
  0x76, 0xd5, 0x40, 0x1a, 0x2d, 0x3c, 0xaa, 0x44, 0xb8, 0xc5, 0xd5, 0xae, 0xf3, 0xe9, 0x28, 0xb9,
  0x0d, 0x2e, 0xe2, 0x33, 0xe9, 0xf9, 0xa2, 0xce, 0xc4, 0xa3, 0x2c, 0xd0, 0x19, 0xd0, 0x6a, 0x0d,
  0xc1, 0xfc, 0xb1, 0x12, 0x5f, 0x57, 0x46, 0xa4, 0xfb, 0xd3, 0x21, 0x69, 0xed, 0x7b, 0xf0, 0xe4,
  0xfd, 0x06, 0x5f, 0xa7, 0xc8, 0xac, 0x97, 0xc3, 0x66, 0x38, 0x04, 0x84, 0x49, 0x5f, 0x5c, 0x5b,
  0x68, 0x50, 0xdd, 0x1c, 0x9d, 0x8c, 0xd6, 0x69, 0x4c, 0xf8, 0x68, 0x6e, 0x46, 0x30, 0x8e, 0xd0,
  0xed, 0x1f, 0x5b, 0xdf, 0x98, 0xcd, 0x83, 0x13, 0x39, 0x77, 0x1d, 0xb6, 0x3d, 0xe5, 0xa7, 0xde
};
const uint8_t Expected_Tag[] =
{
  0x20, 0x15, 0x3b, 0xf8, 0xea, 0x29, 0x53, 0xc4, 0x82, 0x51, 0xeb, 0xcc, 0x41, 0x61, 0xf8, 0xb6,
  0xe2, 0x84, 0x99, 0xe5, 0xc7, 0x6c, 0x24, 0x01, 0x4c, 0xff, 0x4a, 0x9e, 0x2f, 0x62, 0xd2, 0x5c
};

uint8_t Key1[] =
{
  0x09, 0x67, 0x5f, 0x2d, 0xcc, 0x47, 0x83, 0xb5, 0x99, 0xf1, 0x8f, 0xb7, 0x65, 0x58, 0x36, 0x68,
  0xa0, 0xfd, 0x8a, 0xe4, 0x09, 0x6f, 0x6f, 0xcd, 0xc6, 0x0d, 0x4f, 0x35, 0xb4, 0x13, 0x0f, 0xbe,
  0xfc, 0xd5, 0x42, 0xff, 0xe7, 0x45, 0x9d, 0x2a
};

uint8_t Message1[] =
{
  0x0c, 0xf2, 0x19, 0x8c, 0x31, 0x37, 0x6f, 0x5c, 0x89, 0x15, 0x66, 0x01, 0x37, 0x72, 0x5f, 0x2b,
  0xbc, 0x18, 0x0a, 0x98, 0x6e, 0x5a, 0x7b, 0xda, 0x27, 0xfa, 0x81, 0x59, 0x3a, 0x4a, 0x33, 0x9b,
  0xab, 0x92, 0xcb, 0xc3, 0x9f, 0xb2, 0xb8, 0x58, 0x11, 0x08, 0xee, 0x48, 0xc7, 0x94, 0x81, 0x2d,
  0x84, 0x5a, 0x72, 0xce, 0x80, 0x08, 0xc9, 0xe9, 0x15, 0xd9, 0xe3, 0x30, 0xbb, 0xb9, 0x0e, 0x91,
  0x36, 0xaa, 0x53, 0xba, 0x0e, 0x66, 0x93, 0xdd, 0x40, 0x46, 0xd6, 0xb0, 0x33, 0x62, 0xdf, 0xb9,
  0xed, 0xfa, 0x04, 0xc8, 0x87, 0x15, 0x3c, 0xc5, 0xde, 0x67, 0x7a, 0xab, 0x8c, 0x78, 0x39, 0xd5,
  0x17, 0x03, 0x58, 0x79, 0x67, 0x9c, 0x29, 0x72, 0x7e, 0x96, 0xc5, 0x42, 0x63, 0x24, 0xa2, 0x57,
  0x5f, 0xbe, 0x67, 0x8d, 0x6c, 0xc7, 0xfe, 0xf5, 0xeb, 0x6c, 0xeb, 0xd5, 0x95, 0xcf, 0xdd, 0xef
};

uint8_t Expected_Tag1[] =
{
  0x6b, 0x14, 0x2d, 0x4d, 0xfe, 0x21, 0x7f, 0x18, 0x81, 0xaa, 0x0e, 0x64, 0x83, 0xb2, 0x71, 0xdd,
  0x5d, 0x43, 0xf7, 0x0b, 0x85, 0x60, 0x59, 0x53, 0xa0, 0xfe, 0xf2, 0x72, 0xdd, 0xde, 0x46, 0xca
};

/* Computed HASH message */
uint8_t Sha256Hmacoutput[OUTPUT_BUFFER_SIZE] = {0};
uint32_t ComputedSize;
uint8_t Sha256Hmacoutput1[OUTPUT_BUFFER_SIZE] = {0};
uint32_t ComputedSize1;
/* Set to 1 if the computation error is detected*/
uint32_t ComputeErrorCb;
/* Set to 1 if the computation is correctly completed*/
uint32_t ComputeCpltCb;
uint32_t ToSuspendCompute = 0;
/* Private functions prototype -----------------------------------------------*/
static void DigestCpltCallback(hal_hash_handle_t *pHASH);
static void ErrorCallback(hal_hash_handle_t *pHASH);
void HAL_TIM_UpdateCallback(hal_tim_handle_t *pTim);


/** ########## Step 1 ##########
  * Initializes the HASH and TIM instances.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pHASH = mx_example_hash_init();
  if (pHASH == NULL)
  {
    goto _app_init_exit;
  }

  pTim = mx_example_tim_init();
  if (pTim == NULL)
  {
    goto _app_init_exit;
  }
  /* Register the user hash digest Callback*/
  if (HAL_HASH_RegisterDigestComputationCpltCallback(pHASH, DigestCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  /* Register the user hash error Callback*/
  if (HAL_HASH_RegisterErrorCpltCallback(pHASH, ErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_TIM_RegisterUpdateCallback(pTim, HAL_TIM_UpdateCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_TIM_Start_IT(pTim) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: HASH and TIM initialization COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;
_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Hash data with HMAC sha256 algorithms in suspension mode, with different key for each handle.
  * This step is scheduled as follows:
  * Step 2.1: Suspends the IT Computation process.
  * Step 2.2: Generates and verifies a message authentication code (MAC) using SHA265 algorithm with different vectors.
  * Step 2.3: Resumes the IT Computation process.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Generates and verifies a message authentication code (MAC) using SHA265 algorithm.*/

  ToSuspendCompute = 1;
  /* Starts compute the message */
  if (HAL_HASH_HMAC_Compute_IT(pHASH,
                               Message,
                               128,
                               Sha256Hmacoutput,
                               OUTPUT_BUFFER_SIZE,
                               &ComputedSize) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /** ########## Step 2.1 ##########
    * Suspends the IT Computation process.
    */

  /* Save parameters of the suspended HASH Computation */
  HAL_HASH_SaveContext(pHASH, &Ctx);

  PRINTF("[INFO] Step 2.1: suspension of the IT Computation COMPLETED.\n");

  /** ########## Step 2.2 ##########
    * Starts another process:
    * Generates and verifies a message authentication code (MAC) using SHA265 algorithm with different vectors.
    */
  p_hmac_config.data_swapping = HAL_HASH_DATA_SWAP_BYTE;
  p_hmac_config.algorithm     = HAL_HASH_ALGO_SHA256;
  p_hmac_config.p_hmac_key    = Key1;
  p_hmac_config.key_size_byte = 40;

  if (HAL_HASH_HMAC_SetConfig(pHASH, &p_hmac_config) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* Compute the message */
  if (HAL_HASH_HMAC_Compute(pHASH,
                            Message1,
                            128,
                            Sha256Hmacoutput1,
                            OUTPUT_BUFFER_SIZE,
                            &ComputedSize1,
                            HASH_TIMEOUT_MS) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* Verify the computed hash message */
  if (memcmp(Sha256Hmacoutput1, Expected_Tag1, 32) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: MAC generation and verification using SHA256 in polling mode COMPLETED.\n");

  /*Clears all data */
  memset((void *)Sha256Hmacoutput1, 0U, sizeof(Sha256Hmacoutput1));

  /** ########## Step 2.3 ##########
    *  Resumes the IT Computation process.
    */
  /* Restore context*/
  HAL_HASH_RestoreContext(pHASH, &Ctx);

  /* Resume the IT Computation process */
  if (HAL_HASH_ResumeComputation(pHASH) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* Wait for an IT computation process suspension. */
  while ((ComputeCpltCb == 0) && (ComputeErrorCb == 0))
  {
    /* Put the CPU in Wait For Interrupt state.*/
    __WFI();
  }

  if (ComputeErrorCb == 1)
  {
    goto _app_process_exit;
  }
  /* Verify the computed hash message */
  if (memcmp(Sha256Hmacoutput, Expected_Tag, 32) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.3: Resume the IT process : MAC generation and verification in IT mode COMPLETED.\n");


  /*Clears all data */
  memset((void *)Sha256Hmacoutput, 0U, sizeof(Sha256Hmacoutput));

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** HASH digest computation complete callback.
  * @user This implementation of the HASH igest computation complete callback can be customized.
  */
static void DigestCpltCallback(hal_hash_handle_t *pHASH)
{
  ComputeCpltCb = 1U;
}
/** HASH error callback.
  * @user This implementation of the HASH error callback can be customized.
  */
static void ErrorCallback(hal_hash_handle_t *pHASH)
{
  ComputeErrorCb = 1U;
}

/** ########## Step 3 ##########
  * Deinitializes the HASH and DMA peripheral before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_hash_init();
  mx_example_tim_deinit();
  PRINTF("[INFO] Step 3: HASH and DMA peripherals deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** Update callback.
  * brief Function called when the timer update interrupt is generated.
  */

void HAL_TIM_UpdateCallback(hal_tim_handle_t *pTIM)
{
  if (ToSuspendCompute == 1)
  {
    HAL_HASH_RequestSuspendComputation(pHASH);
    ToSuspendCompute = 0;

  }
}
