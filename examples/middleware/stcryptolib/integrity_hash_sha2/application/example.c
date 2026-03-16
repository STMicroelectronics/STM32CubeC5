/**
  ******************************************************************************
  * file           : example.c
  * brief          : This example describes how to use the STM32 Cryptographic
  *                  library digest a message using the SHA256 algorithm.
  *
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
#include "cmox_crypto.h"        /* Crypto services and defines */
#include <string.h>             /* Various functions for character manipulating. */

/* Global variables ----------------------------------------------------------*/
/* SHA256 context handle */
cmox_sha256_handle_t sha256_ctx;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Chunk size (in bytes) when data to hash are processed by chunk */
#define CHUNK_SIZE  48u
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/** Extract from SHA256LongMsg.rsp
  *
  * Len = 2096
  * Msg = 6b918fb1a5ad1f9c5e5dbdf10a93a9c8f6bca89f37e79c9fe12a57227941b173ac79d8d440cde8c64c4ebc84a4c803
  * d198a296f3de060900cc427f58ca6ec373084f95dd6c7c427ecfbf781f68be572a88dbcbb188581ab200bfb99a3a816407e7
  * dd6dd21003554d4f7a99c93ebfce5c302ff0e11f26f83fe669acefb0c1bbb8b1e909bd14aa48ba3445c88b0e1190eef765ad
  * 898ab8ca2fe507015f1578f10dce3c11a55fb9434ee6e9ad6cc0fdc4684447a9b3b156b908646360f24fec2d8fa69e2c93db
  * 78708fcd2eef743dcb9353819b8d667c48ed54cd436fb1476598c4a1d7028e6f2ff50751db36ab6bc32435152a00abd3d58d
  * 9a8770d9a3e52d5a3628ae3c9e0325
  * MD = 46500b6ae1ab40bde097ef168b0f3199049b55545a1588792d39d594f493dca7
  */

/* Known plaintext to be signed and verified */
const uint8_t Message[] =
{
  0x6b, 0x91, 0x8f, 0xb1, 0xa5, 0xad, 0x1f, 0x9c, 0x5e, 0x5d, 0xbd, 0xf1, 0x0a, 0x93, 0xa9, 0xc8,
  0xf6, 0xbc, 0xa8, 0x9f, 0x37, 0xe7, 0x9c, 0x9f, 0xe1, 0x2a, 0x57, 0x22, 0x79, 0x41, 0xb1, 0x73,
  0xac, 0x79, 0xd8, 0xd4, 0x40, 0xcd, 0xe8, 0xc6, 0x4c, 0x4e, 0xbc, 0x84, 0xa4, 0xc8, 0x03, 0xd1,
  0x98, 0xa2, 0x96, 0xf3, 0xde, 0x06, 0x09, 0x00, 0xcc, 0x42, 0x7f, 0x58, 0xca, 0x6e, 0xc3, 0x73,
  0x08, 0x4f, 0x95, 0xdd, 0x6c, 0x7c, 0x42, 0x7e, 0xcf, 0xbf, 0x78, 0x1f, 0x68, 0xbe, 0x57, 0x2a,
  0x88, 0xdb, 0xcb, 0xb1, 0x88, 0x58, 0x1a, 0xb2, 0x00, 0xbf, 0xb9, 0x9a, 0x3a, 0x81, 0x64, 0x07,
  0xe7, 0xdd, 0x6d, 0xd2, 0x10, 0x03, 0x55, 0x4d, 0x4f, 0x7a, 0x99, 0xc9, 0x3e, 0xbf, 0xce, 0x5c,
  0x30, 0x2f, 0xf0, 0xe1, 0x1f, 0x26, 0xf8, 0x3f, 0xe6, 0x69, 0xac, 0xef, 0xb0, 0xc1, 0xbb, 0xb8,
  0xb1, 0xe9, 0x09, 0xbd, 0x14, 0xaa, 0x48, 0xba, 0x34, 0x45, 0xc8, 0x8b, 0x0e, 0x11, 0x90, 0xee,
  0xf7, 0x65, 0xad, 0x89, 0x8a, 0xb8, 0xca, 0x2f, 0xe5, 0x07, 0x01, 0x5f, 0x15, 0x78, 0xf1, 0x0d,
  0xce, 0x3c, 0x11, 0xa5, 0x5f, 0xb9, 0x43, 0x4e, 0xe6, 0xe9, 0xad, 0x6c, 0xc0, 0xfd, 0xc4, 0x68,
  0x44, 0x47, 0xa9, 0xb3, 0xb1, 0x56, 0xb9, 0x08, 0x64, 0x63, 0x60, 0xf2, 0x4f, 0xec, 0x2d, 0x8f,
  0xa6, 0x9e, 0x2c, 0x93, 0xdb, 0x78, 0x70, 0x8f, 0xcd, 0x2e, 0xef, 0x74, 0x3d, 0xcb, 0x93, 0x53,
  0x81, 0x9b, 0x8d, 0x66, 0x7c, 0x48, 0xed, 0x54, 0xcd, 0x43, 0x6f, 0xb1, 0x47, 0x65, 0x98, 0xc4,
  0xa1, 0xd7, 0x02, 0x8e, 0x6f, 0x2f, 0xf5, 0x07, 0x51, 0xdb, 0x36, 0xab, 0x6b, 0xc3, 0x24, 0x35,
  0x15, 0x2a, 0x00, 0xab, 0xd3, 0xd5, 0x8d, 0x9a, 0x87, 0x70, 0xd9, 0xa3, 0xe5, 0x2d, 0x5a, 0x36,
  0x28, 0xae, 0x3c, 0x9e, 0x03, 0x25
};
const uint8_t Expected_Hash[] =
{
  0x46, 0x50, 0x0b, 0x6a, 0xe1, 0xab, 0x40, 0xbd, 0xe0, 0x97, 0xef, 0x16, 0x8b, 0x0f, 0x31, 0x99,
  0x04, 0x9b, 0x55, 0x54, 0x5a, 0x15, 0x88, 0x79, 0x2d, 0x39, 0xd5, 0x94, 0xf4, 0x93, 0xdc, 0xa7
};

/* Computed data buffer */
uint8_t computed_hash[CMOX_SHA256_SIZE]; /* Signature length is modulus size/8U */

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the STM32 Cryptographic library.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Initialize STM32 Cryptographic library */
  if (cmox_initialize(NULL) != CMOX_INIT_SUCCESS)
  {
    PRINTF("[ERROR] Step 1: STM32 Cryptographic library initialization ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
  }
  else
  {
    PRINTF("[INFO] Step 1: STM32 Cryptographic library initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Demonstrates how to use the STM32 Cryptographic library to digest
  * a message using the SHA256 compliant algorithm in two ways:
  *
  * This step is scheduled as follows:
  * Step 2.1: Computes the digest using single API call method:
  *           Algorithm configuration and execution is done via a single API call.
  * Step 2.2: Computes the digest using multiple API calls method:
  *           Algorithm configuration and execution is done in several API calls,
  *           allowing in particular a piecemeal injection of data to process.
  */

app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  cmox_hash_retval_t retval;
  size_t computed_size;
  /* General hash context */
  cmox_hash_handle_t *p_hash_ctx;
  /* Index for piecemeal processing */
  uint32_t index;

  /** ########## Step 2.1 ##########
    * Computes the digest using single API call method:
    * Algorithm configuration and execution is done via a single API call.
    */

  /* Compute directly the digest passing all the needed parameters */
  retval = cmox_hash_compute(CMOX_SHA256_ALGO,         /* Use SHA256 algorithm */
                             Message, sizeof(Message), /* Message to digest */
                             computed_hash,            /* Data buffer to receive digest data */
                             CMOX_SHA256_SIZE,         /* Expected digest size */
                             &computed_size);          /* Size of computed digest */
  /* Verify API returned value */
  if (retval != CMOX_HASH_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != CMOX_SHA256_SIZE)
  {
    goto _app_process_exit;
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Expected_Hash, computed_hash, computed_size) != 0)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.1: Compute the digest using single API call method COMPLETED.\n");

  /** ########## Step 2.2 ##########
    * Computes the digest using multiple API calls method:
    * Algorithm configuration and execution is done in several API calls,
    * allowing in particular a piecemeal injection of data to process.
    */

  /* Construct a hash context that is configured to perform SHA256 digest operations */
  p_hash_ctx = cmox_sha256_construct(&sha256_ctx);
  if (p_hash_ctx == NULL)
  {
    goto _app_process_exit;
  }

  /* Initialize the hash context */
  retval = cmox_hash_init(p_hash_ctx);
  if (retval != CMOX_HASH_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Set the desired size for the digest to compute: note that in the case
     where the size of the digest is the default for the algorithm, it is
     possible to skip this call. */
  retval = cmox_hash_setTagLen(p_hash_ctx, CMOX_SHA256_SIZE);
  if (retval != CMOX_HASH_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Append the message to be hashed by chunks of CHUNK_SIZE Bytes */
  for (index = 0; index < (sizeof(Message) - CHUNK_SIZE); index += CHUNK_SIZE)
  {
    retval = cmox_hash_append(p_hash_ctx, &Message[index], CHUNK_SIZE); /* Chunk of data to digest */

    /* Verify API returned value */
    if (retval != CMOX_HASH_SUCCESS)
    {
      goto _app_process_exit;
    }
  }
  /* Append the last part of the message if needed */
  if (index < sizeof(Message))
  {
    /* Last part of data to digest */
    retval = cmox_hash_append(p_hash_ctx, &Message[index], sizeof(Message) - index);

    /* Verify API returned value */
    if (retval != CMOX_HASH_SUCCESS)
    {
      goto _app_process_exit;
    }
  }

  /* Generate the digest data */
  retval = cmox_hash_generateTag(p_hash_ctx, computed_hash, &computed_size);

  /* Verify API returned value */
  if (retval != CMOX_HASH_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != CMOX_SHA256_SIZE)
  {
    goto _app_process_exit;
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Expected_Hash, computed_hash, computed_size) != 0)
  {
    goto _app_process_exit;
  }

  /* Cleanup the context */
  retval = cmox_hash_cleanup(p_hash_ctx);
  if (retval != CMOX_HASH_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: Compute the digest using multiple API calls method COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Deinitializes STM32 Cryptographic library before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  /* No more need of cryptographic services, finalize STM32 Cryptographic library */
  if (cmox_finalize(NULL) != CMOX_INIT_SUCCESS)
  {
    return EXEC_STATUS_ERROR;
  }

  PRINTF("[INFO] Step 3: STM32 Cryptographic library deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */
