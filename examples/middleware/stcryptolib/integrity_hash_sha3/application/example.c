/**
  ******************************************************************************
  * file           : example.c
  * brief          : This example describes how to use the STM32 Cryptographic
  *                  library digest a message using the SHA3 algorithm.
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
/* SHA3 context handle */
cmox_sha3_handle_t sha3_ctx;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Chunk size (in bytes) when data to hash are processed by chunk */
#define CHUNK_SIZE  48u
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/** Extract from SHA3_512LongMsg.rsp
  *
Len = 2328
Msg = 22e1df25c30d6e7806cae35cd4317e5f94db028741a76838bfb7d5576fbccab001749a95897122c8d51bb49cfef854
563e2b27d9013b28833f161d520856ca4b61c2641c4e184800300aede3518617c7be3a4e6655588f181e9641f8df7a6a42ea
d423003a8c4ae6be9d767af5623078bb116074638505c10540299219b0155f45b1c18a74548e4328de37a911140531deb643
4c534af2449c1abe67e18030681a61240225f87ede15d519b7ce2500bccf33e1364e2fbe6a8a2fe6c15d73242610ed36b074
0080812e8902ee531c88e0359020797cbdd1fb78848ae6b5105961d05cdddb8af5fef21b02db94c9810464b8d3ea5f047b94
bf0d23931f12df37e102b603cd8e5f5ffa83488df257ddde110106262e0ef16d7ef213e7b49c69276d4d048f
MD = a6375ff04af0a18fb4c8175f671181b4cf79653a3d70847c6d99694b3f5d41601f1dbef809675c63cac4ec83153b1c7
8131a7b61024ce36244f320ab8740cb7e

  */
const uint8_t Message[] =
{
  0x22, 0xe1, 0xdf, 0x25, 0xc3, 0x0d, 0x6e, 0x78, 0x06, 0xca, 0xe3, 0x5c, 0xd4, 0x31, 0x7e, 0x5f,
  0x94, 0xdb, 0x02, 0x87, 0x41, 0xa7, 0x68, 0x38, 0xbf, 0xb7, 0xd5, 0x57, 0x6f, 0xbc, 0xca, 0xb0,
  0x01, 0x74, 0x9a, 0x95, 0x89, 0x71, 0x22, 0xc8, 0xd5, 0x1b, 0xb4, 0x9c, 0xfe, 0xf8, 0x54, 0x56,
  0x3e, 0x2b, 0x27, 0xd9, 0x01, 0x3b, 0x28, 0x83, 0x3f, 0x16, 0x1d, 0x52, 0x08, 0x56, 0xca, 0x4b,
  0x61, 0xc2, 0x64, 0x1c, 0x4e, 0x18, 0x48, 0x00, 0x30, 0x0a, 0xed, 0xe3, 0x51, 0x86, 0x17, 0xc7,
  0xbe, 0x3a, 0x4e, 0x66, 0x55, 0x58, 0x8f, 0x18, 0x1e, 0x96, 0x41, 0xf8, 0xdf, 0x7a, 0x6a, 0x42,
  0xea, 0xd4, 0x23, 0x00, 0x3a, 0x8c, 0x4a, 0xe6, 0xbe, 0x9d, 0x76, 0x7a, 0xf5, 0x62, 0x30, 0x78,
  0xbb, 0x11, 0x60, 0x74, 0x63, 0x85, 0x05, 0xc1, 0x05, 0x40, 0x29, 0x92, 0x19, 0xb0, 0x15, 0x5f,
  0x45, 0xb1, 0xc1, 0x8a, 0x74, 0x54, 0x8e, 0x43, 0x28, 0xde, 0x37, 0xa9, 0x11, 0x14, 0x05, 0x31,
  0xde, 0xb6, 0x43, 0x4c, 0x53, 0x4a, 0xf2, 0x44, 0x9c, 0x1a, 0xbe, 0x67, 0xe1, 0x80, 0x30, 0x68,
  0x1a, 0x61, 0x24, 0x02, 0x25, 0xf8, 0x7e, 0xde, 0x15, 0xd5, 0x19, 0xb7, 0xce, 0x25, 0x00, 0xbc,
  0xcf, 0x33, 0xe1, 0x36, 0x4e, 0x2f, 0xbe, 0x6a, 0x8a, 0x2f, 0xe6, 0xc1, 0x5d, 0x73, 0x24, 0x26,
  0x10, 0xed, 0x36, 0xb0, 0x74, 0x00, 0x80, 0x81, 0x2e, 0x89, 0x02, 0xee, 0x53, 0x1c, 0x88, 0xe0,
  0x35, 0x90, 0x20, 0x79, 0x7c, 0xbd, 0xd1, 0xfb, 0x78, 0x84, 0x8a, 0xe6, 0xb5, 0x10, 0x59, 0x61,
  0xd0, 0x5c, 0xdd, 0xdb, 0x8a, 0xf5, 0xfe, 0xf2, 0x1b, 0x02, 0xdb, 0x94, 0xc9, 0x81, 0x04, 0x64,
  0xb8, 0xd3, 0xea, 0x5f, 0x04, 0x7b, 0x94, 0xbf, 0x0d, 0x23, 0x93, 0x1f, 0x12, 0xdf, 0x37, 0xe1,
  0x02, 0xb6, 0x03, 0xcd, 0x8e, 0x5f, 0x5f, 0xfa, 0x83, 0x48, 0x8d, 0xf2, 0x57, 0xdd, 0xde, 0x11,
  0x01, 0x06, 0x26, 0x2e, 0x0e, 0xf1, 0x6d, 0x7e, 0xf2, 0x13, 0xe7, 0xb4, 0x9c, 0x69, 0x27, 0x6d,
  0x4d, 0x04, 0x8f
};
const uint8_t Expected_Hash[] =
{
  0xa6, 0x37, 0x5f, 0xf0, 0x4a, 0xf0, 0xa1, 0x8f, 0xb4, 0xc8, 0x17, 0x5f, 0x67, 0x11, 0x81, 0xb4,
  0xcf, 0x79, 0x65, 0x3a, 0x3d, 0x70, 0x84, 0x7c, 0x6d, 0x99, 0x69, 0x4b, 0x3f, 0x5d, 0x41, 0x60,
  0x1f, 0x1d, 0xbe, 0xf8, 0x09, 0x67, 0x5c, 0x63, 0xca, 0xc4, 0xec, 0x83, 0x15, 0x3b, 0x1c, 0x78,
  0x13, 0x1a, 0x7b, 0x61, 0x02, 0x4c, 0xe3, 0x62, 0x44, 0xf3, 0x20, 0xab, 0x87, 0x40, 0xcb, 0x7e
};

/* Computed data buffer */
uint8_t computed_hash[CMOX_SHA3_512_SIZE]; /* Signature length is modulus size/8U */

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
  * a message using the SHA3 compliant algorithm in two ways:
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
  retval = cmox_hash_compute(CMOX_SHA3_512_ALGO,         /* Use SHA3-512 algorithm */
                             Message, sizeof(Message), /* Message to digest */
                             computed_hash,            /* Data buffer to receive digest data */
                             CMOX_SHA3_512_SIZE,         /* Expected digest size */
                             &computed_size);          /* Size of computed digest */
  /* Verify API returned value */
  if (retval != CMOX_HASH_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != CMOX_SHA3_512_SIZE)
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

  /* Construct a hash context that is configured to perform SHA3-512 digest operations */
  p_hash_ctx = cmox_sha3_512_construct(&sha3_ctx);
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
  retval = cmox_hash_setTagLen(p_hash_ctx, CMOX_SHA3_512_SIZE);
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
  if (computed_size != CMOX_SHA3_512_SIZE)
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
