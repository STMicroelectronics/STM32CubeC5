/**
  ******************************************************************************
  * file           : example.c
  * brief          : HASH Integrity SHA256 with HAL API.
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
#define MESSAGE_BUFFER_SIZE 32
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_hash_config_t p_hashconfig;
hal_hash_handle_t *pHASH;

/** Extract from NIST Publication: SHA Test Vectors for Hashing Byte-Oriented Messages SHA256LongMsg.rsp
  *
  * Len = 1304
  * Msg = 451101250ec6f26652249d59dc974b7361d571a8101cdfd36aba3b5854d3ae0
  * 86b5fdd4597721b66e3c0dc5d8c606d9657d0e323283a5217d1f53f2f284f57b85c8a
  * 61ac8924711f895c5ed90ef17745ed2d728abd22a5f7a13479a462d71b56c19a74a40
  * b655c58edfe0a188ad2cf46cbf30524f65d423c837dd1ff2bf462ac4198007345bb44
  * dbb7b1c861298cdf61982a833afc728fae1eda2f87aa2c9480858bec
  * MD = 3c593aa539fdcdae516cdf2f15000f6634185c88f505b39775fb9ab137a10aa2
  */
/* Message to be computed */
uint8_t Message [4] __attribute__((aligned(128))) =
{
  0x86, 0x46, 0xc6, 0x0
};
/* The expected message of the compute process */
uint8_t ExpectedHash [MESSAGE_BUFFER_SIZE] =
{
  0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea, 0x41, 0x41, 0x40, 0xde, 0x5d, 0xae, 0x22, 0x23,
  0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c, 0xb4, 0x10, 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad
};
/* The expected message of the compute process */
uint8_t Msg65Input [65] __attribute__((aligned(128))) =
{
  0x52, 0x43, 0x31, 0x5f, 0x53, 0x6f, 0x75, 0x72, 0x63, 0x65, 0x61, 0x50, 0x72, 0x6f, 0x6a, 0x65,
  0x63, 0x74, 0x5f, 0x53, 0x46, 0x55, 0x61, 0x45, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x73, 0x61,
  0x53, 0x54, 0x4d, 0x33, 0x32, 0x32, 0x78, 0x47, 0x2d, 0x45, 0x56, 0x41, 0x4c, 0x61, 0x45, 0x57,
  0x41, 0x52, 0x4d, 0x61, 0x53, 0x54, 0x4d, 0x33, 0x32, 0x32, 0x78, 0x47, 0x5f, 0x45, 0x56, 0x41,
  0x4c
};
/* Message to be hashed by the accumulation process */
uint8_t Msg65Expected [MESSAGE_BUFFER_SIZE] =
{
  0xf6, 0x9c, 0x82, 0x20, 0x48, 0x97, 0x13, 0x5e, 0xbc, 0x18, 0xb3, 0xf3, 0x16,
  0x08, 0x91, 0x78, 0x79, 0x89, 0x34, 0x39, 0x80, 0x43, 0xa0, 0xb5,
  0xf8, 0x9e, 0x6a, 0x2e, 0x3e, 0x6b, 0x92, 0x1c
};

/* Computed HASH message */
uint8_t computed_hash_message[MESSAGE_BUFFER_SIZE] = {0};
uint32_t computed_size;
/* Output of accumulation message */
uint8_t SHA256_Output[MESSAGE_BUFFER_SIZE] = {0};
uint32_t SHA256_OutputSize;
/* Set to 1 if the computation/ update is correctly completed*/
uint32_t ComputeCpltCb;
/* Set to 1 if the computation/ update error is detected*/
uint32_t ComputeErrorCb;
/* Private functions prototype -----------------------------------------------*/
static void DigestCpltCallback(hal_hash_handle_t *pHASH);
static void ErrorCallback(hal_hash_handle_t *pHASH);
/** ########## Step 1 ##########
  * Initializes the HASH peripheral.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  /* Initialize the HASH peripheral*/
  pHASH = mx_example_hash_init();
  if (pHASH == NULL)
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
  PRINTF("[INFO] Step 1: HASH initialization COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Demonstrates how to digest a message, using SHA256 algorithm in interrupt mode,
  * for a single buffer to be hashed, HAL_HASH_Compute_IT() is called,
  * and for multi-buffer input, HAL_HASH_Update_IT()
  * and HAL_HASH_Finish() are called.
  * This step is scheduled as follows:
  * Step 2.1: Computes and verifies the HASH message in interrupt mode.
  * Step 2.2: Updates and verifies the HASH message in interrupt mode.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2.1 ##########
    * Computes and verifies the HASH message in IT mode.
    */

  /* Compute the HASH message in interrupt mode */
  if (HAL_HASH_Compute_IT(pHASH, Message, 3, computed_hash_message, MESSAGE_BUFFER_SIZE, &computed_size) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* Wait for one of these HASH interrupts: compute complete or compute error.*/
  while ((ComputeCpltCb == 0) && (ComputeErrorCb == 0))
  {
    /* Put the CPU in Wait For Interrupt state.*/
    __WFI();
  }
  /* Check if there is an error occurred while the computation*/
  if (ComputeErrorCb == 1)
  {
    goto _app_process_exit;
  }
  /* Verify the computed message */
  if (memcmp(computed_hash_message, ExpectedHash, 32) != 0)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.1: HASH message compute and verification COMPLETED.\n");
  ComputeCpltCb = 0;
  /* Clear all data */
  memset((void *)computed_hash_message, 0U, sizeof(computed_hash_message));

  /** ########## Step 2.2 ##########
    * Updates and verifies the HASH message in interrupt mode.
    */
  /* Set the HASH parameters */
  p_hashconfig.data_swapping  = HAL_HASH_DATA_SWAP_BYTE;
  p_hashconfig.algorithm = HAL_HASH_ALGO_SHA256;

  /*Configure the HASH peripheral */
  if (HAL_HASH_SetConfig(pHASH, &p_hashconfig) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* Start HASH update process in interrupt mode with the first input buffer */
  if (HAL_HASH_Update_IT(pHASH, Msg65Input, 8) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* HASH update process in interrupt mode with the second input buffer */
  if (HAL_HASH_Update_IT(pHASH, Msg65Input + 8, 20) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* HASH update process in interrupt mode with the third input buffer */
  if (HAL_HASH_Update_IT(pHASH, Msg65Input + 28, 4) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* HASH update process in interrupt mode with the third input buffer */
  if (HAL_HASH_Update_IT(pHASH, Msg65Input + 32, 33) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* Stop HASH update process */
  if (HAL_HASH_Finish(pHASH, SHA256_Output, MESSAGE_BUFFER_SIZE, &SHA256_OutputSize, 1000) != HAL_OK)
  {
    goto _app_process_exit;
  }
  if (ComputeErrorCb == 1)
  {
    goto _app_process_exit;
  }
  /* Verify the HASH message */
  if (memcmp(SHA256_Output, Msg65Expected, 32) != 0)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.2: HASH message update and verification COMPLETED.\n");
  ComputeCpltCb = 0;
  /* Clear all data */
  memset((void *)SHA256_Output, 0U, sizeof(SHA256_Output));

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/* Digest computation complete call back.*/
static void DigestCpltCallback(hal_hash_handle_t *pHASH)
{
  ComputeCpltCb = 1U;
}
/* HASH error callback.*/
static void ErrorCallback(hal_hash_handle_t *pHASH)
{
  ComputeErrorCb = 1U;
}

/** ########## Step 3 ##########
  * Deinitializes the HASH peripheral before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_hash_deinit();

  PRINTF("[INFO] Step 3: HASH peripheral deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */
