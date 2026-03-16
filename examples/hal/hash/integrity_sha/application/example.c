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
#define HASH_TIMEOUT_MS 1000
#define HASH_OUTPUT_BUFFER_SIZE 32
#define MESSAGE_SIZE 163

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
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
uint8_t Message[MESSAGE_SIZE] =
{
  0x45, 0x11, 0x01, 0x25, 0x0e, 0xc6, 0xf2, 0x66, 0x52, 0x24, 0x9d, 0x59, 0xdc, 0x97, 0x4b, 0x73,
  0x61, 0xd5, 0x71, 0xa8, 0x10, 0x1c, 0xdf, 0xd3, 0x6a, 0xba, 0x3b, 0x58, 0x54, 0xd3, 0xae, 0x08,
  0x6b, 0x5f, 0xdd, 0x45, 0x97, 0x72, 0x1b, 0x66, 0xe3, 0xc0, 0xdc, 0x5d, 0x8c, 0x60, 0x6d, 0x96,
  0x57, 0xd0, 0xe3, 0x23, 0x28, 0x3a, 0x52, 0x17, 0xd1, 0xf5, 0x3f, 0x2f, 0x28, 0x4f, 0x57, 0xb8,
  0x5c, 0x8a, 0x61, 0xac, 0x89, 0x24, 0x71, 0x1f, 0x89, 0x5c, 0x5e, 0xd9, 0x0e, 0xf1, 0x77, 0x45,
  0xed, 0x2d, 0x72, 0x8a, 0xbd, 0x22, 0xa5, 0xf7, 0xa1, 0x34, 0x79, 0xa4, 0x62, 0xd7, 0x1b, 0x56,
  0xc1, 0x9a, 0x74, 0xa4, 0x0b, 0x65, 0x5c, 0x58, 0xed, 0xfe, 0x0a, 0x18, 0x8a, 0xd2, 0xcf, 0x46,
  0xcb, 0xf3, 0x05, 0x24, 0xf6, 0x5d, 0x42, 0x3c, 0x83, 0x7d, 0xd1, 0xff, 0x2b, 0xf4, 0x62, 0xac,
  0x41, 0x98, 0x00, 0x73, 0x45, 0xbb, 0x44, 0xdb, 0xb7, 0xb1, 0xc8, 0x61, 0x29, 0x8c, 0xdf, 0x61,
  0x98, 0x2a, 0x83, 0x3a, 0xfc, 0x72, 0x8f, 0xae, 0x1e, 0xda, 0x2f, 0x87, 0xaa, 0x2c, 0x94, 0x80,
  0x85, 0x8b, 0xec
};
/* The expected message of the compute process */
uint8_t ExpectedHash[HASH_OUTPUT_BUFFER_SIZE] =
{
  0x3c, 0x59, 0x3a, 0xa5, 0x39, 0xfd, 0xcd, 0xae, 0x51, 0x6c, 0xdf, 0x2f, 0x15, 0x00, 0x0f, 0x66,
  0x34, 0x18, 0x5c, 0x88, 0xf5, 0x05, 0xb3, 0x97, 0x75, 0xfb, 0x9a, 0xb1, 0x37, 0xa1, 0x0a, 0xa2
};
/* Message to be hashed by the accumulation process */
uint8_t Msg65Input[65] __attribute__((aligned(128))) =
{
  0x52, 0x43, 0x31, 0x5f, 0x53, 0x6f, 0x75, 0x72, 0x63, 0x65, 0x61, 0x50, 0x72, 0x6f, 0x6a, 0x65,
  0x63, 0x74, 0x5f, 0x53, 0x46, 0x55, 0x61, 0x45, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x73, 0x61,
  0x53, 0x54, 0x4d, 0x33, 0x32, 0x32, 0x78, 0x47, 0x2d, 0x45, 0x56, 0x41, 0x4c, 0x61, 0x45, 0x57,
  0x41, 0x52, 0x4d, 0x61, 0x53, 0x54, 0x4d, 0x33, 0x32, 0x32, 0x78, 0x47, 0x5f, 0x45, 0x56, 0x41,
  0x4c
};
/* The expected message of the accumulation process */
uint8_t Msg65Expected [32] =
{
  0xf6, 0x9c, 0x82, 0x20, 0x48, 0x97, 0x13, 0x5e, 0xbc, 0x18, 0xb3, 0xf3, 0x16,
  0x08, 0x91, 0x78, 0x79, 0x89, 0x34, 0x39, 0x80, 0x43, 0xa0, 0xb5,
  0xf8, 0x9e, 0x6a, 0x2e, 0x3e, 0x6b, 0x92, 0x1c
};
/* Output of accumulation message */
uint8_t SHA256_Output[HASH_OUTPUT_BUFFER_SIZE] = {0};
uint32_t SHA256_OutputSize;
/* Computed HASH message */
uint8_t computed_hash_message[HASH_OUTPUT_BUFFER_SIZE] = {0};
uint32_t ComputedSize;
/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * Initializes the HASH instance.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pHASH = mx_example_hash_init();
  if (pHASH != NULL)
  {
    PRINTF("[INFO] Step 1: HASH initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }
  else
  {
    PRINTF("[ERROR] Step 1: HASH initialization ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
  }

  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Demonstrates how to digest a message, using SHA256 algorithm.
  * for a single buffer to be hashed, HAL_HASH_Compute() is called,
  * and for multi-buffer input, HAL_HASH_Update()
  * and HAL_HASH_Finish() are called.
  * This step is scheduled as follows:
  * Step 2.1: Computes and verifies the HASH message.
  * Step 2.2: Updates and verifies the HASH message of multi-buffer.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;


  /** ########## Step 2.1 ##########
    * Computes and verifies the HASH message.
    */

  /* Compute the message */
  if (HAL_HASH_Compute(pHASH, Message, MESSAGE_SIZE,
                       computed_hash_message, HASH_OUTPUT_BUFFER_SIZE, &ComputedSize,
                       HASH_TIMEOUT_MS) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Verify the computed hash message */
  if (memcmp(computed_hash_message, ExpectedHash, 32) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: HASH message compute and verification COMPLETED.\n");


  /*Clears all data */
  memset((void *)computed_hash_message, 0U, sizeof(computed_hash_message));

  /** ########## Step 2.2 ##########
    * Updates and verifies the HASH message.
    */

  /* Start HASH update process in polling mode with the first input buffer */
  if (HAL_HASH_Update(pHASH, Msg65Input, 8, HASH_TIMEOUT_MS) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* HASH update process in polling mode with the second input buffer */
  if (HAL_HASH_Update(pHASH, Msg65Input + 8, 20, HASH_TIMEOUT_MS) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* HASH update process in polling mode with the third input buffer */
  if (HAL_HASH_Update(pHASH, Msg65Input + 28, 4, HASH_TIMEOUT_MS) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* HASH update process in polling mode with the third input buffer */
  if (HAL_HASH_Update(pHASH, Msg65Input + 32, 33, HASH_TIMEOUT_MS) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* Finish HASH update process */
  if (HAL_HASH_Finish(pHASH, SHA256_Output, HASH_OUTPUT_BUFFER_SIZE, &SHA256_OutputSize, HASH_TIMEOUT_MS) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Verify the computed hash message */
  if (memcmp(SHA256_Output, Msg65Expected, 32) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: HASH message update and verification COMPLETED.\n");

  /* Clears all data */
  memset((void *)SHA256_Output, 0U, sizeof(SHA256_Output));

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Deinitializes the HASH peripheral before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_hash_init();

  PRINTF("[INFO] Step 3: HASH peripheral deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */
