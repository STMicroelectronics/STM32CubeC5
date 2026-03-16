/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to use the STM32 Cryptographic library to generate and to verify
  *                  a message authentication code (MAC) of a plaintext
  *                  using the SHA3 KMAC algorithm.
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
cmox_kmac_handle_t Kmac_Ctx; /* KMAC context handle */

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define CHUNK_SIZE  48u /* Chunk size (in bytes) when data to hash are processed by chunk */
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/** Extract from NIST KMAC_samples.pdf
  *
  Sample #3
Security Strength: 128-bits
Length of Key is 256-bits
Key is
40 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F
50 51 52 53 54 55 56 57 58 59 5A 5B 5C 5D 5E 5F
Length of data is 1600-bits
Data is
00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F
30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F
40 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F
50 51 52 53 54 55 56 57 58 59 5A 5B 5C 5D 5E 5F
60 61 62 63 64 65 66 67 68 69 6A 6B 6C 6D 6E 6F
70 71 72 73 74 75 76 77 78 79 7A 7B 7C 7D 7E 7F
80 81 82 83 84 85 86 87 88 89 8A 8B 8C 8D 8E 8F
90 91 92 93 94 95 96 97 98 99 9A 9B 9C 9D 9E 9F
A0 A1 A2 A3 A4 A5 A6 A7 A8 A9 AA AB AC AD AE AF
B0 B1 B2 B3 B4 B5 B6 B7 B8 B9 BA BB BC BD BE BF
C0 C1 C2 C3 C4 C5 C6 C7
Requested output length is 256-bits
S (as a character string) is
"My Tagged Application"

....

Outval is
1F 5B 4E 6C CA 02 20 9E 0D CB 5C A6 35 B8 9A 15
E2 71 EC C7 60 07 1D FD 80 5F AA 38 F9 72 92 30

  */

const uint8_t Key[] =
{
  0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F
};
/* Known plaintext to be signed and verified */
const uint8_t Message[] =
{
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
  0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
  0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
  0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
  0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
  0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
  0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
  0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7
};
const uint8_t Expected_Tag[] =
{
  0x1F, 0x5B, 0x4E, 0x6C, 0xCA, 0x02, 0x20, 0x9E, 0x0D, 0xCB, 0x5C, 0xA6, 0x35, 0xB8, 0x9A, 0x15,
  0xE2, 0x71, 0xEC, 0xC7, 0x60, 0x07, 0x1D, 0xFD, 0x80, 0x5F, 0xAA, 0x38, 0xF9, 0x72, 0x92, 0x30
};
const uint8_t Custom_Data[21] = "My Tagged Application";

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
  * using the KMAC algorithm in two ways.
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
  retval = cmox_mac_compute(CMOX_KMAC_128_ALGO,                /* Use KMAC 128 algorithm */
                            Message, sizeof(Message),          /* Message to authenticate */
                            Key, sizeof(Key),                  /* KMAC Key to use */
                            Custom_Data, sizeof(Custom_Data),  /* Custom data */
                            Computed_Tag,                      /* Data buffer to receive generated authnetication tag */
                            sizeof(Expected_Tag),              /* Expected authentication tag size */
                            &computed_size);                   /* Generated tag size */

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
  retval = cmox_mac_verify(CMOX_KMAC_128_ALGO,                /* Use KMAC 128 algorithm */
                           Message, sizeof(Message),          /* Message to authenticate */
                           Key, sizeof(Key),                  /* KMAC Key to use */
                           Custom_Data, sizeof(Custom_Data),  /* Custom data */
                           Expected_Tag,                      /* Authentication tag */
                           sizeof(Expected_Tag));             /* tag size */

  /* Verify API returned value */
  if (retval != CMOX_MAC_AUTH_SUCCESS)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.2: MAC verification using single API call method COMPLETED.\n");

  /** ########## Step 2.3 ##########
    * Generates and verifies a message authentication code (MAC) using multiple API calls method.
    */

  /* Construct a MAC context that is configured to perform KMAC 128 authentication operations */
  mac_ctx = cmox_kmac_construct(&Kmac_Ctx, CMOX_KMAC_128);
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

  /* Set the custom data */
  retval = cmox_mac_setCustomData(mac_ctx,
                                  Custom_Data, sizeof(Custom_Data)); /* Custom data */
  if (retval != CMOX_MAC_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Setup of the authentication key into the context */
  retval = cmox_mac_setKey(mac_ctx, Key, sizeof(Key));  /* KMAC Key to use */
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
  /* Construct a MAC context that is configured to perform KMAC 128 authentication operations */
  mac_ctx = cmox_kmac_construct(&Kmac_Ctx, CMOX_KMAC_128);
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

  /* Set the custom data */
  retval = cmox_mac_setCustomData(mac_ctx,
                                  Custom_Data, sizeof(Custom_Data)); /* Custom data */
  if (retval != CMOX_MAC_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Setup of the authentication key into the context */
  retval = cmox_mac_setKey(mac_ctx, Key, sizeof(Key));  /* KMAC Key to use */
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
