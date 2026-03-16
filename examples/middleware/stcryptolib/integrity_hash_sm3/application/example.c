/**
  ******************************************************************************
  * file           : example.c
  * brief          : This example describes how to use the STM32 Cryptographic
  *                  library digest a message using the SM3 algorithm.
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
/* SM3 context handle */
cmox_sm3_handle_t sm3_ctx;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Chunk size (in bytes) when data to hash are processed by chunk */
#define CHUNK_SIZE  48u
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** Extract from IETF draft-oscca-cfrg-sm3-02
  * A.2.  Example 2, From GB/T 32905-2016

   This is example 2 provided by [GBT.32905-2016] to demonstrate hashing
   of a 512-bit plaintext.

A.2.1.  512-bit Input Message

 61626364 61626364 61626364 61626364 61626364 61626364 61626364 61626364
 61626364 61626364 61626364 61626364 61626364 61626364 61626364 61626364

...

A.2.3.  Hash Value

 debe9ff9 2275b8a1 38604889 c18e5a4d 6fdb70e5 387e5765 293dcba3 9c0c5732

  */
const uint8_t Message[] =
{
  0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,
  0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,
  0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64,
  0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64, 0x61, 0x62, 0x63, 0x64
};
const uint8_t Expected_Hash[] =
{
  0xde, 0xbe, 0x9f, 0xf9, 0x22, 0x75, 0xb8, 0xa1, 0x38, 0x60, 0x48, 0x89, 0xc1, 0x8e, 0x5a, 0x4d,
  0x6f, 0xdb, 0x70, 0xe5, 0x38, 0x7e, 0x57, 0x65, 0x29, 0x3d, 0xcb, 0xa3, 0x9c, 0x0c, 0x57, 0x32
};

/* Computed data buffer */
uint8_t computed_hash[CMOX_SM3_SIZE];


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
  * a message using the SM3 compliant algorithm in two ways:
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
  retval = cmox_hash_compute(CMOX_SM3_ALGO,            /* Use SM3 algorithm */
                             Message, sizeof(Message), /* Message to digest */
                             computed_hash,            /* Data buffer to receive digest data */
                             CMOX_SM3_SIZE,         /* Expected digest size */
                             &computed_size);          /* Size of computed digest */
  /* Verify API returned value */
  if (retval != CMOX_HASH_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != CMOX_SM3_SIZE)
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

  /* Construct a hash context that is configured to perform SM3 digest operations */
  p_hash_ctx = cmox_sm3_construct(&sm3_ctx);
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
  retval = cmox_hash_setTagLen(p_hash_ctx, CMOX_SM3_SIZE);
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
  if (computed_size != CMOX_SM3_SIZE)
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
