/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to use the STM32 Cryptographic library to generate and to verify
  *                  a message authentication code (MAC) of a plaintext
  *                  using the HMAC SHA256 algorithm.
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
cmox_hmac_handle_t p_hmac_ctx; /* HMAC context handle */

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define CHUNK_SIZE  48u /* Chunk size (in bytes) when data to hash are processed by chunk */
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/** Extract from HMAC.rsp
  *
  * Klen = 40
  * Tlen = 32
  * Key = cfd4a44910c9e567507abb6cede4fe601a7a2765c9755aa2cf6ba4814223811a26a8a1ef499cebd9
  * Msg = 3fb301cb4092f9623aa5ffd690d22d65d56e5a1c330b9c4a0d910c34e391c90a76d5401a2d3caa44b8c5d5aef3e928b
  * 90d2ee233e9f9a2cec4a32cd019d06a0dc1fcb1125f5746a4fbd32169ed7bf0e4fd065fa7c8ac97c366380484495f5c5b6850
  * dd1c9d8cd6694cf8686e46308ed0ed1f5bdf98cd831339771db63de5a7de
  * Mac = 20153bf8ea2953c48251ebcc4161f8b6e28499e5c76c24014cff4a9e2f62d25c
  */

const uint8_t Key[] =
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

/* Computed data buffer */
uint8_t Computed_Tag[sizeof(Expected_Tag)];

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
  * Generates and verifies a message authentication code (MAC) of a plaintext
  * using the HMAC-SHA256 algorithm in two ways.
  * This step is scheduled as follows:
  * Step 2.1: Generates and verifies a message authentication code (MAC) using single API call method.
  * Step 2.2: Verifies the message with the given authentication tag using single API call method.
  * Step 2.3: Generates and verifies a message authentication code (MAC) using multiple API calls method.
  * step 2.4: Verifies the message with the given authentication tag using multiple API calls method.
  */

app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  cmox_mac_retval_t  retval;
  size_t computed_size;
  /* General hash context */
  cmox_mac_handle_t *mac_ctx;
  /* Index for piecemeal processing */
  uint32_t index;
  /* Fault check verification variable */
  uint32_t fault_check = CMOX_MAC_AUTH_FAIL;

  /** ########## Step 2.1 ##########
    * Generates and verifies the mac of a known message using single API call method.
    */

  /* Compute directly the authentication tag passing all the needed parameters */
  retval = cmox_mac_compute(CMOX_HMAC_SHA256_ALGO,     /* Use HMAC SHA256 algorithm */
                            Message, sizeof(Message),  /* Message to authenticate */
                            Key, sizeof(Key),          /* HMAC Key to use */
                            NULL, 0,                   /* Custom data */
                            Computed_Tag,              /* Data buffer to receive generated authnetication tag */
                            sizeof(Expected_Tag),      /* Expected authentication tag size */
                            &computed_size);           /* Generated tag size */

  /* Verify API returned value */
  if (retval != CMOX_MAC_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Expected_Tag))
  {
    goto _app_process_exit;
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Expected_Tag, Computed_Tag, computed_size) != 0)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.1: MAC generation and verification using single API call method COMPLETED.\n");

  /** ########## Step 2.2 ##########
    * Verifies the message with the given authentication tag using single API call method.
    */
  /* Verify directly the message passing all the needed parameters */
  retval = cmox_mac_verify(CMOX_HMAC_SHA256_ALGO,     /* Use HMAC SHA256 algorithm */
                           Message, sizeof(Message),  /* Message to authenticate */
                           Key, sizeof(Key),          /* HMAC Key to use */
                           NULL, 0,                   /* Custom data */
                           Expected_Tag,              /* Authentication tag */
                           sizeof(Expected_Tag));     /* tag size */

  /* Verify API returned value */
  if (retval != CMOX_MAC_AUTH_SUCCESS)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.2: MAC verification using single API call method COMPLETED.\n");

  /** ########## Step 2.3 ##########
    * Generates and verifies a message authentication code (MAC) using multiple API calls method.
    */

  /* Construct a MAC context that is configured to perform HMAC SHA256 authentication operations */
  mac_ctx = cmox_hmac_construct(&p_hmac_ctx, CMOX_HMAC_SHA256);
  if (mac_ctx == NULL)
  {
    goto _app_process_exit;
  }

  /* Initialize the MAC context */
  retval = cmox_mac_init(mac_ctx);
  if (retval != CMOX_MAC_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Set the desired size for the authentication tag to compute: note that in the case
     where the size of the tag is the default for the algorithm, it is
     possible to skip this call. */
  retval = cmox_mac_setTagLen(mac_ctx, sizeof(Expected_Tag));
  if (retval != CMOX_MAC_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Setup of the authentication key into the context */
  retval = cmox_mac_setKey(mac_ctx, Key, sizeof(Key));  /* HMAC Key to use */
  if (retval != CMOX_MAC_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Append the message to be authenticated by chunks of CHUNK_SIZE Bytes */
  for (index = 0; index < (sizeof(Message) - CHUNK_SIZE); index += CHUNK_SIZE)
  {
    retval = cmox_mac_append(mac_ctx, &Message[index], CHUNK_SIZE); /* Chunk of data to authenticate */

    /* Verify API returned value */
    if (retval != CMOX_MAC_SUCCESS)
    {
      goto _app_process_exit;
    }
  }
  /* Append the last part of the message if needed */
  if (index < sizeof(Message))
  {
    retval = cmox_mac_append(mac_ctx, &Message[index], sizeof(Message) - index); /* Last part of data to authenticate */

    /* Verify API returned value */
    if (retval != CMOX_MAC_SUCCESS)
    {
      goto _app_process_exit;
    }
  }

  /* Generate the authentication tag */
  retval = cmox_mac_generateTag(mac_ctx, Computed_Tag, &computed_size);

  /* Verify API returned value */
  if (retval != CMOX_MAC_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Expected_Tag))
  {
    goto _app_process_exit;
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Expected_Tag, Computed_Tag, computed_size) != 0)
  {
    goto _app_process_exit;
  }

  /* Cleanup the context */
  retval = cmox_mac_cleanup(mac_ctx);
  if (retval != CMOX_MAC_SUCCESS)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.3: MAC generation and verification using multiple API calls method COMPLETED.\n");

  /** ########## Step 2.4 ##########
    * Verifies the message with the given authentication tag using multiple API calls method.
    */
  /* Construct a MAC context that is configured to perform HMAC SHA256 authentication operations */
  mac_ctx = cmox_hmac_construct(&p_hmac_ctx, CMOX_HMAC_SHA256);
  if (mac_ctx == NULL)
  {
    goto _app_process_exit;
  }

  /* Initialize the MAC context */
  retval = cmox_mac_init(mac_ctx);
  if (retval != CMOX_MAC_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Set the desired size for the authentication tag to compute: note that in the case
     where the size of the tag is the default for the algorithm, it is
     possible to skip this call. */
  retval = cmox_mac_setTagLen(mac_ctx, sizeof(Expected_Tag));
  if (retval != CMOX_MAC_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Setup of the authentication key into the context */
  retval = cmox_mac_setKey(mac_ctx, Key, sizeof(Key));  /* HMAC Key to use */
  if (retval != CMOX_MAC_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Append the message to be authenticated by chunks of CHUNK_SIZE Bytes */
  for (index = 0; index < (sizeof(Message) - CHUNK_SIZE); index += CHUNK_SIZE)
  {
    retval = cmox_mac_append(mac_ctx, &Message[index], CHUNK_SIZE); /* Chunk of data to authenticate */

    /* Verify API returned value */
    if (retval != CMOX_MAC_SUCCESS)
    {
      goto _app_process_exit;
    }
  }
  /* Append the last part of the message if needed */
  if (index < sizeof(Message))
  {
    retval = cmox_mac_append(mac_ctx, &Message[index], sizeof(Message) - index); /* Last part of data to authenticate */

    /* Verify API returned value */
    if (retval != CMOX_MAC_SUCCESS)
    {
      goto _app_process_exit;
    }
  }

  /* Verify the authentication tag */
  retval = cmox_mac_verifyTag(mac_ctx,
                              Expected_Tag,   /* Authentication tag used for verification */
                              &fault_check);  /* Fault check variable:
                                              to ensure no fault injection occurs during this API call */

  /* Verify API returned value */
  if (retval != CMOX_MAC_AUTH_SUCCESS)
  {
    goto _app_process_exit;
  }
  /* Verify Fault check variable value */
  if (fault_check != CMOX_MAC_AUTH_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Cleanup the context */
  retval = cmox_mac_cleanup(mac_ctx);
  if (retval != CMOX_MAC_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.4: MAC verification using multiple API calls method COMPLETED.\n");

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
