/**
  ******************************************************************************
  * file           : example.c
  * brief          : RandomGeneration DRBG using the STCryptoLib
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* DRBG context handle */
cmox_ctr_drbg_handle_t p_ctr_drbg_ctx;

/** Extract from CTR_DRBG.rsp
  * [AES-256 use df]
  * [PredictionResistance = False]
  * [EntropyInputLen = 256]
  * [NonceLen = 128]
  * [PersonalizationStringLen = 256]
  * [AdditionalInputLen = 0]
  * [ReturnedBitsLen = 512]
  *
  * COUNT = 0
  * EntropyInput = 4cfb218673346d9d50c922e49b0dfcd090adf04f5c3ba47327dfcd6fa63a785c
  * Nonce = 016962a7fd2787a24bf6be47ef3783f1
  * PersonalizationString = 88eeb8e0e83bf3294bdacd6099ebe4bf55ecd9113f71e5ebcb4575f3d6a68a6b
  * EntropyInputReseed = b7ec46072363834a1b0133f2c23891db4f11a68651f23e3a8b1fdc03b192c7e7
  * AdditionalInputReseed =
  * AdditionalInput =
  * AdditionalInput =
  * ReturnedBits = a55180a190bef3adaf28f6b795e9f1f3d6dfa1b27dd0467b0c75f5fa931e971475b27
  * cae03a29654e2f40966ea33643040d1400fe677873af8097c1fe9f00298
  */
const uint8_t Entropy[] =
{
  0x4c, 0xfb, 0x21, 0x86, 0x73, 0x34, 0x6d, 0x9d, 0x50, 0xc9, 0x22, 0xe4, 0x9b, 0x0d, 0xfc, 0xd0,
  0x90, 0xad, 0xf0, 0x4f, 0x5c, 0x3b, 0xa4, 0x73, 0x27, 0xdf, 0xcd, 0x6f, 0xa6, 0x3a, 0x78, 0x5c
};
const uint8_t Nonce[] =
{
  0x01, 0x69, 0x62, 0xa7, 0xfd, 0x27, 0x87, 0xa2, 0x4b, 0xf6, 0xbe, 0x47, 0xef, 0x37, 0x83, 0xf1
};
const uint8_t Personalization[] =
{
  0x88, 0xee, 0xb8, 0xe0, 0xe8, 0x3b, 0xf3, 0x29, 0x4b, 0xda, 0xcd, 0x60, 0x99, 0xeb, 0xe4, 0xbf,
  0x55, 0xec, 0xd9, 0x11, 0x3f, 0x71, 0xe5, 0xeb, 0xcb, 0x45, 0x75, 0xf3, 0xd6, 0xa6, 0x8a, 0x6b
};
const uint8_t EntropyInputReseed[] =
{
  0xb7, 0xec, 0x46, 0x07, 0x23, 0x63, 0x83, 0x4a, 0x1b, 0x01, 0x33, 0xf2, 0xc2, 0x38, 0x91, 0xdb,
  0x4f, 0x11, 0xa6, 0x86, 0x51, 0xf2, 0x3e, 0x3a, 0x8b, 0x1f, 0xdc, 0x03, 0xb1, 0x92, 0xc7, 0xe7
};
const uint8_t knownRandom[] =
{
  0xa5, 0x51, 0x80, 0xa1, 0x90, 0xbe, 0xf3, 0xad, 0xaf, 0x28, 0xf6, 0xb7, 0x95, 0xe9, 0xf1, 0xf3,
  0xd6, 0xdf, 0xa1, 0xb2, 0x7d, 0xd0, 0x46, 0x7b, 0x0c, 0x75, 0xf5, 0xfa, 0x93, 0x1e, 0x97, 0x14,
  0x75, 0xb2, 0x7c, 0xae, 0x03, 0xa2, 0x96, 0x54, 0xe2, 0xf4, 0x09, 0x66, 0xea, 0x33, 0x64, 0x30,
  0x40, 0xd1, 0x40, 0x0f, 0xe6, 0x77, 0x87, 0x3a, 0xf8, 0x09, 0x7c, 0x1f, 0xe9, 0xf0, 0x02, 0x98
};

/* Computed data buffer */
uint8_t Computed_Random[sizeof(knownRandom)];

/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * The applicative code initializes the STCryptoLib.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Initialize STCryptoLib */
  if (cmox_initialize(NULL) != CMOX_INIT_SUCCESS)
  {
    PRINTF("[ERROR] Step 1: STCryptoLib initialization ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
  }
  else
  {
    PRINTF("[INFO] Step 1: STCryptoLib initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Demonstrates how to use the STCryptoLib to generate a random numbers
  * based on known input entropy, nonce and personalization.
  *
  * This step is scheduled as follows:
  * Step 2.1: Initializes the DRBG context with entropy, nonce and personalization string parameters.
  * Step 2.2: Reseeds the DRBG with reseed parameters.
  * Step 2.3: Generates random data.
  * Step 2.4: Verifies generated data.
  * Step 2.5: Cleans the DRBG context.
  */
app_status_t app_process(void)
{
  app_status_t return_status;
  cmox_drbg_retval_t retval;
  /* General DRBG context */
  cmox_drbg_handle_t *p_drbg_ctx;

  /* Construct a drbg context that is configured to perform ctrDRBG with AES256 operations */
  p_drbg_ctx = cmox_ctr_drbg_construct(&p_ctr_drbg_ctx, CMOX_CTR_DRBG_AES256);
  if (p_drbg_ctx == NULL)
  {
    goto _app_process_exit;
  }

  /* Initialize the DRBG context with entropy, nonce and personalization string parameters */
  retval = cmox_drbg_init(p_drbg_ctx,                                     /* DRBG context */
                          Entropy, sizeof(Entropy),                     /* Entropy data */
                          Personalization, sizeof(Personalization),     /* Personalization string */
                          Nonce, sizeof(Nonce));                        /* Nonce data */
  if (retval != CMOX_DRBG_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: DRBG context initialization COMPLETED.\n");

  /* Reseed the DRBG with reseed parameters */
  retval = cmox_drbg_reseed(p_drbg_ctx,                                     /* DRBG context */
                            EntropyInputReseed, sizeof(EntropyInputReseed), /* Entropy reseed data */
                            NULL, 0);                                       /* No additional reseed data */
  if (retval != CMOX_DRBG_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: Reseed the DRBG with reseed parameters COMPLETED.\n");

  /* Generate 1st random data */
  retval = cmox_drbg_generate(p_drbg_ctx,                                  /* DRBG context */
                              NULL, 0,                                     /* No additional data */
                              Computed_Random, sizeof(Computed_Random));   /* Data buffer to receive generated random */

  /* Verify API returned value */
  if (retval != CMOX_DRBG_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Generate 2nd random data */
  retval = cmox_drbg_generate(p_drbg_ctx,                                  /* DRBG context */
                              NULL, 0,                                     /* No additional data */
                              Computed_Random, sizeof(Computed_Random));   /* Data buffer to receive generated random */

  /* Verify API returned value */
  if (retval != CMOX_DRBG_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.3: Generate random data COMPLETED.\n");

  /* Verify generated random data are conform to the expected known random.*/
  if (memcmp(Computed_Random, knownRandom, sizeof(knownRandom)) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.4: Verify generated data COMPLETED.\n");

  /* Cleanup the context */
  retval = cmox_drbg_cleanup(p_drbg_ctx);
  if (retval != CMOX_DRBG_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.5: Clean-up the DRBG context COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Deinitializes the STCryptoLib before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  /* No more need of cryptographic services, finalize STCryptoLib */
  if (cmox_finalize(NULL) != CMOX_INIT_SUCCESS)
  {
    return EXEC_STATUS_ERROR;
  }

  PRINTF("[INFO] Step 3: STCryptoLib deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */
