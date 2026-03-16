/**
  ******************************************************************************
  * file           : example.c
  * brief          : This example describes how to use the STM32 Cryptographic
  *                  library to authenticate and to verify a message using the
  *                  AES CMAC algorithm.
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
#include <string.h>             /* Various functions for character manipulating. */
#include "cmox_crypto.h"        /* Crypto services and defines */


/* Global variables ----------------------------------------------------------*/
/* CMAC context handle */
cmox_cmac_handle_t p_cmac_ctx;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CHUNK_SIZE  48u   /* Chunk size (in bytes) when data to mac are processed by chunk */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/** Extract from NIST Special Publication 800-38A
  * Example #4
  *
  * Key is
  * 2B7E1516 28AED2A6 ABF71588 09CF4F3C
  * Mlen=64
  *
  * PT is
  * 6BC1BEE2 2E409F96 E93D7E11 7393172A
  * AE2D8A57 1E03AC9C 9EB76FAC 45AF8E51
  * 30C81C46 A35CE411 E5FBC119 1A0A52EF
  * F69F2445 DF4F9B17 AD2B417B E66C3710
  *
  *...
  *
  * Tag is
  * 51F0BEBF 7E3B9D92 FC497417 79363CFE
  * ---------------------------------------------------------------------
  */

const uint8_t Key[] =
{
  0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
};

const uint8_t Message[] =
{
  0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
  0xAE, 0x2D, 0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C, 0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 0x8E, 0x51,
  0x30, 0xC8, 0x1C, 0x46, 0xA3, 0x5C, 0xE4, 0x11, 0xE5, 0xFB, 0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF,
  0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F, 0x9B, 0x17, 0xAD, 0x2B, 0x41, 0x7B, 0xE6, 0x6C, 0x37, 0x10
};

const uint8_t Expected_Tag[] =
{
  0x51, 0xF0, 0xBE, 0xBF, 0x7E, 0x3B, 0x9D, 0x92, 0xFC, 0x49, 0x74, 0x17, 0x79, 0x36, 0x3C, 0xFE
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
  * Demonstrates how to use the STM32 Cryptographic library to authenticate and to verify
  * a message using the AES CMAC algorithm.
  * This step is scheduled as follows:
  * Step 2.1: Computes and verifies the message using single API call method.
  * Step 2.2: Computes and verifies the message using multiple API calls method.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  cmox_mac_retval_t retval;
  size_t computed_size;
  /* General mac context */
  cmox_mac_handle_t *mac_ctx;
  /* Index for piecemeal processing */
  uint32_t index;
  /* Fault check verification variable */
  uint32_t fault_check = CMOX_MAC_AUTH_FAIL;

  /** ########## Step 2.1 ##########
    * Computes and verifies the message using single API call method.
    */

  retval = cmox_mac_compute(CMOX_CMAC_AES_ALGO,        /* Use AES CMAC algorithm */
                            Message, sizeof(Message),  /* Message to authenticate */
                            Key, sizeof(Key),          /* AES key to use */
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

  /* Verify directly the message passing all the needed parameters */
  retval = cmox_mac_verify(CMOX_CMAC_AES_ALGO,        /* Use AES CMAC algorithm */
                           Message, sizeof(Message),  /* Message to authenticate */
                           Key, sizeof(Key),          /* AES key to use */
                           NULL, 0,                   /* Custom data */
                           Expected_Tag,              /* Authentication tag */
                           sizeof(Expected_Tag));     /* tag size */

  /* Verify API returned value */
  if (retval != CMOX_MAC_AUTH_SUCCESS)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.1: Compute and verify the message using single API call method COMPLETED.\n");

  /**
    * ########## Step 2.2 ##########
    * Computes and verifies the message using multiple API calls method.
    */

  mac_ctx = cmox_cmac_construct(&p_cmac_ctx, CMOX_CMAC_AES);
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
  retval = cmox_mac_setKey(mac_ctx, Key, sizeof(Key));  /* AES key to use */
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

  /* Construct a MAC context that is configured to perform AES CMAC authentication operations */
  /* Note: CMOX_CMAC_AES refer to the default AES implementation
    * selected in cmox_default_config.h. To use a specific implementation, user can
    * directly choose:
    * - CMOX_CMAC_AESFAST to select the AES fast implementation
    * - CMOX_CMAC_AESSMALL to select the AES small implementation
    */
  mac_ctx = cmox_cmac_construct(&p_cmac_ctx, CMOX_CMAC_AES);
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
  retval = cmox_mac_setKey(mac_ctx, Key, sizeof(Key));  /* AES key to use */
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
  PRINTF("[INFO] Step 2.2: Compute and verify the message using multiple API calls method COMPLETED.\n");

  /* Cleanup the context */
  retval = cmox_mac_cleanup(mac_ctx);
  if (retval != CMOX_MAC_SUCCESS)
  {
    goto _app_process_exit;
  }
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
