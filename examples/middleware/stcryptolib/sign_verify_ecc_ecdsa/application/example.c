/**
  ******************************************************************************
  * file           : example.c
  * brief          : This example describes how to use the STM32 Cryptographic Library
  *                  to sign and to verify a message using the ECDSA algorithm.
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
#include <string.h>
#include "example.h"
#include "cmox_crypto.h"

/* Global variables ----------------------------------------------------------*/
/* ECC context */
cmox_ecc_handle_t p_ecc_ctx;
/* ECC working buffer */
uint8_t Working_Buffer[2048] __ALIGNED(8);

/* Random data buffer */
uint32_t Computed_Random[8];
/* RNG peripheral handle */
hal_rng_handle_t *hrng;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define GENERATION_TIMEOUT_MS      1000U  /* The more random data to generate, the longer the timeout */

/** ---------------------------------------------------------------------
  * Extract from SigGen.txt
  * [P-256,SHA-224]
  *
  * Msg = ff624d0ba02c7b6370c1622eec3fa2186ea681d1659e0a845448e777b75a8e77a77bb26e5733179d58ef9bc8a4e8b69
  * 71aef2539f77ab0963a3415bbd6258339bd1bf55de65db520c63f5b8eab3d55debd05e9494212170f5d65b3286b8b668705b1
  * e2b2b5568610617abb51d2dd0cb450ef59df4b907da90cfa7b268de8c4c2
  * d = 708309a7449e156b0db70e5b52e606c7e094ed676ce8953bf6c14757c826f590
  * Qx = 29578c7ab6ce0d11493c95d5ea05d299d536801ca9cbd50e9924e43b733b83ab
  * Qy = 08c8049879c6278b2273348474158515accaa38344106ef96803c5a05adc4800
  * k = 58f741771620bdc428e91a32d86d230873e9140336fcfb1e122892ee1d501bdc
  * R = 4a19274429e40522234b8785dc25fc524f179dcc95ff09b3c9770fc71f54ca0d
  * S = 58982b79a65b7320f5b92d13bdaecdd1259e760f0f718ba933fd098f6f75d4b7
  *
  * ---------------------------------------------------------------------
  */
const uint8_t Message[] =
{
  0xff, 0x62, 0x4d, 0x0b, 0xa0, 0x2c, 0x7b, 0x63, 0x70, 0xc1, 0x62, 0x2e, 0xec, 0x3f, 0xa2, 0x18,
  0x6e, 0xa6, 0x81, 0xd1, 0x65, 0x9e, 0x0a, 0x84, 0x54, 0x48, 0xe7, 0x77, 0xb7, 0x5a, 0x8e, 0x77,
  0xa7, 0x7b, 0xb2, 0x6e, 0x57, 0x33, 0x17, 0x9d, 0x58, 0xef, 0x9b, 0xc8, 0xa4, 0xe8, 0xb6, 0x97,
  0x1a, 0xef, 0x25, 0x39, 0xf7, 0x7a, 0xb0, 0x96, 0x3a, 0x34, 0x15, 0xbb, 0xd6, 0x25, 0x83, 0x39,
  0xbd, 0x1b, 0xf5, 0x5d, 0xe6, 0x5d, 0xb5, 0x20, 0xc6, 0x3f, 0x5b, 0x8e, 0xab, 0x3d, 0x55, 0xde,
  0xbd, 0x05, 0xe9, 0x49, 0x42, 0x12, 0x17, 0x0f, 0x5d, 0x65, 0xb3, 0x28, 0x6b, 0x8b, 0x66, 0x87,
  0x05, 0xb1, 0xe2, 0xb2, 0xb5, 0x56, 0x86, 0x10, 0x61, 0x7a, 0xbb, 0x51, 0xd2, 0xdd, 0x0c, 0xb4,
  0x50, 0xef, 0x59, 0xdf, 0x4b, 0x90, 0x7d, 0xa9, 0x0c, 0xfa, 0x7b, 0x26, 0x8d, 0xe8, 0xc4, 0xc2
};
const uint8_t Private_Key[] =
{
  0x70, 0x83, 0x09, 0xa7, 0x44, 0x9e, 0x15, 0x6b, 0x0d, 0xb7, 0x0e, 0x5b, 0x52, 0xe6, 0x06, 0xc7,
  0xe0, 0x94, 0xed, 0x67, 0x6c, 0xe8, 0x95, 0x3b, 0xf6, 0xc1, 0x47, 0x57, 0xc8, 0x26, 0xf5, 0x90
};
const uint8_t Public_Key[] =
{
  0x29, 0x57, 0x8c, 0x7a, 0xb6, 0xce, 0x0d, 0x11, 0x49, 0x3c, 0x95, 0xd5, 0xea, 0x05, 0xd2, 0x99,
  0xd5, 0x36, 0x80, 0x1c, 0xa9, 0xcb, 0xd5, 0x0e, 0x99, 0x24, 0xe4, 0x3b, 0x73, 0x3b, 0x83, 0xab,
  0x08, 0xc8, 0x04, 0x98, 0x79, 0xc6, 0x27, 0x8b, 0x22, 0x73, 0x34, 0x84, 0x74, 0x15, 0x85, 0x15,
  0xac, 0xca, 0xa3, 0x83, 0x44, 0x10, 0x6e, 0xf9, 0x68, 0x03, 0xc5, 0xa0, 0x5a, 0xdc, 0x48, 0x00
};
const uint8_t Known_Random[] = /* = k - 1 */
{
  0x58, 0xf7, 0x41, 0x77, 0x16, 0x20, 0xbd, 0xc4, 0x28, 0xe9, 0x1a, 0x32, 0xd8, 0x6d, 0x23, 0x08,
  0x73, 0xe9, 0x14, 0x03, 0x36, 0xfc, 0xfb, 0x1e, 0x12, 0x28, 0x92, 0xee, 0x1d, 0x50, 0x1b, 0xdb
};
const uint8_t Known_Signature[] =
{
  0x4a, 0x19, 0x27, 0x44, 0x29, 0xe4, 0x05, 0x22, 0x23, 0x4b, 0x87, 0x85, 0xdc, 0x25, 0xfc, 0x52,
  0x4f, 0x17, 0x9d, 0xcc, 0x95, 0xff, 0x09, 0xb3, 0xc9, 0x77, 0x0f, 0xc7, 0x1f, 0x54, 0xca, 0x0d,
  0x58, 0x98, 0x2b, 0x79, 0xa6, 0x5b, 0x73, 0x20, 0xf5, 0xb9, 0x2d, 0x13, 0xbd, 0xae, 0xcd, 0xd1,
  0x25, 0x9e, 0x76, 0x0f, 0x0f, 0x71, 0x8b, 0xa9, 0x33, 0xfd, 0x09, 0x8f, 0x6f, 0x75, 0xd4, 0xb7
};

/* Computed data buffer */
uint8_t Computed_Hash[CMOX_SHA224_SIZE];
uint8_t Computed_Signature[CMOX_ECC_SECP256R1_SIG_LEN];

hal_rng_handle_t *pRNG;  /* pointer referencing the PPP handle from the generated code */

/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * The applicative code initializes STM32 Cryptographic library and the RNG instance.
  * As it is a description:
  * Step 1.1: Initializes the STM32 Cryptographic library.
  * Step 1.2: Initializes the RNG instance.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  /* Init the RNG */
  hrng = mx_example_rng_init();

  /** ########## Step 1.1 ##########
    * Initializes the STM32 Cryptographic library.
    */
  if (cmox_initialize(NULL) != CMOX_INIT_SUCCESS)
  {
    PRINTF("[ERROR] Step 1.1: STM32 Cryptographic library initialization ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
  }
  else
  {
    PRINTF("[INFO] Step 1.1: STM32 Cryptographic library initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }

  /** ########## Step 1.2 ##########
    * Initializes the RNG instance.
    */
  if (hrng != NULL)
  {
    PRINTF("[INFO] Step 1.2: RNG initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }
  else
  {
    PRINTF("[ERROR] Step 1.2: RNG initialization ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
  }

  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * This example describes how to use the STM32 Cryptographic Library
  * to sign and to verify a message with a known random then a true random
  * using the ECDSA algorithm.
  * As it is a description:
  * - Step 2.1: Computes and verifies the digest message.
  * - Step 2.2: Computes and verifies the signature of the known message digest using a known random.
  * - Step 2.3: Computes and verifies the signature of the known message digest using a true random.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  cmox_hash_retval_t hretval;
  cmox_ecc_retval_t retval;
  size_t computed_size;
  /* Fault check verification variable */
  uint32_t fault_check = CMOX_ECC_AUTH_FAIL;

  /** ########## Step 2.1 ##########
    * Computes and verifies the digest message.
    */

  /* Compute directly the digest passing all the needed parameters */
  hretval = cmox_hash_compute(CMOX_SHA224_ALGO,         /* Use SHA224 algorithm */
                              Message, sizeof(Message), /* Message to digest */
                              Computed_Hash,            /* Data buffer to receive digest data */
                              CMOX_SHA224_SIZE,         /* Expected digest size */
                              &computed_size);          /* Size of computed digest */

  /* Verify API returned value */
  if (hretval != CMOX_HASH_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != CMOX_SHA224_SIZE)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: Compute and verify the digest message COMPLETED.\n");

  /** ########## Step 2.2 ##########
    * Computes and verifies the signature of the known message digest using a known random.
    */

  /* Construct a ECC context, specifying mathematics implementation and working buffer for later processing */
  cmox_ecc_construct(&p_ecc_ctx, CMOX_ECC256_MATH_FUNCS, Working_Buffer, sizeof(Working_Buffer));

  /* Compute directly the signature passing all the needed parameters */
  retval = cmox_ecdsa_sign(&p_ecc_ctx,                               /* ECC context */
                           CMOX_ECC_CURVE_SECP256R1,                 /* SECP256R1 ECC curve selected */
                           Known_Random, sizeof(Known_Random),       /* Random data buffer */
                           Private_Key, sizeof(Private_Key),         /* Private key for signature */
                           Computed_Hash, CMOX_SHA224_SIZE,          /* Digest to sign */
                           Computed_Signature, &computed_size);      /* Data buffer to receive signature */

  /* Verify API returned value */
  if (retval != CMOX_ECC_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Known_Signature))
  {
    goto _app_process_exit;
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Computed_Signature, Known_Signature, computed_size) != 0)
  {
    goto _app_process_exit;
  }

  /* Cleanup context */
  cmox_ecc_cleanup(&p_ecc_ctx);

  /* Construct a ECC context, specifying mathematics implementation and working buffer for later processing */
  cmox_ecc_construct(&p_ecc_ctx, CMOX_ECC256_MATH_FUNCS, Working_Buffer, sizeof(Working_Buffer));

  /* Verify directly the signature passing all the needed parameters */
  retval = cmox_ecdsa_verify(&p_ecc_ctx,                                /* ECC context */
                             CMOX_ECC_CURVE_SECP256R1,                  /* SECP256R1 ECC curve selected */
                             Public_Key, sizeof(Public_Key),            /* Public key for verification */
                             Computed_Hash, CMOX_SHA224_SIZE,           /* Digest to verify */
                             Known_Signature, sizeof(Known_Signature),  /* Data buffer to receive signature */
                             &fault_check);                             /* Fault check variable:
                                                            to ensure no fault injection occurs during this API call */

  /* Verify API returned value */
  if (retval != CMOX_ECC_AUTH_SUCCESS)
  {
    goto _app_process_exit;
  }
  /* Verify Fault check variable value */
  if (fault_check != CMOX_ECC_AUTH_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Cleanup context */
  cmox_ecc_cleanup(&p_ecc_ctx);

  PRINTF("[INFO] Step 2.2: compute and verify the known message digest using a known random COMPLETED.\n");

  /** ########## Step 2.3 ##########
    * Computes and verifies the signature of the known message digest using a true random.
    */

  /* Construct a ECC context, specifying mathematics implementation and working buffer for later processing */
  cmox_ecc_construct(&p_ecc_ctx, CMOX_ECC256_MATH_FUNCS, Working_Buffer, sizeof(Working_Buffer));

  /** Note: The random value must satisfy some arithmetic constraints versus the selected curve and points
    * to minimize the statictical vulnerability.
    * In case this is not satisfied, cmox_ecdsa_sign returns CMOX_ECC_ERR_WRONG_RANDOM: new random has to be
    * generated and API call again.
    */
  if (HAL_RNG_GenerateRandomNumber(hrng, Computed_Random, sizeof(Computed_Random) / sizeof(uint32_t),
                                   GENERATION_TIMEOUT_MS) != HAL_OK)
  {
    /* Random number generation error */
    goto _app_process_exit;
  }

  /* Compute directly the signature passing all the needed parameters */
  retval = cmox_ecdsa_sign(&p_ecc_ctx,                                          /* ECC context */
                           CMOX_ECC_CURVE_SECP256R1,                            /* SECP256R1 ECC curve selected */
                           (uint8_t *)Computed_Random, sizeof(Computed_Random), /* Random data buffer */
                           Private_Key, sizeof(Private_Key),                    /* Private key for signature */
                           Computed_Hash, CMOX_SHA224_SIZE,                     /* Digest to sign */
                           Computed_Signature, &computed_size);                 /* Data buffer to receive signature */


  /* Verify API returned value */
  if (retval != CMOX_ECC_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Known_Signature))
  {
    goto _app_process_exit;
  }

  /* Cleanup context */
  cmox_ecc_cleanup(&p_ecc_ctx);

  /* Construct a ECC context, specifying mathematics implementation and working buffer for later processing */
  cmox_ecc_construct(&p_ecc_ctx, CMOX_ECC256_MATH_FUNCS, Working_Buffer, sizeof(Working_Buffer));

  /* Verify directly the signature passing all the needed parameters */
  retval = cmox_ecdsa_verify(&p_ecc_ctx,                                      /* ECC context */
                             CMOX_ECC_CURVE_SECP256R1,                        /* SECP256R1 ECC curve selected */
                             Public_Key, sizeof(Public_Key),                  /* Public key for verification */
                             Computed_Hash, CMOX_SHA224_SIZE,                 /* Digest to verify */
                             Computed_Signature, sizeof(Computed_Signature),  /* Data buffer to receive signature */
                             &fault_check);                                   /* Fault check variable:
                                                            to ensure no fault injection occurs during this API call */

  /* Verify API returned value */
  if (retval != CMOX_ECC_AUTH_SUCCESS)
  {
    goto _app_process_exit;
  }
  /* Verify Fault check variable value */
  if (fault_check != CMOX_ECC_AUTH_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Cleanup context */
  cmox_ecc_cleanup(&p_ecc_ctx);

  PRINTF("[INFO] Step 2.3: Compute and verify the signature using a true random COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Deinitializes STM32 Cryptographic library and the RNG instance before leaving the scenario.
  * As it is a description:
  * Step 3.1: Deinitializes the STM32 Cryptographic library.
  * Step 3.2: Deinitializes the RNG instance.
  */
app_status_t app_deinit(void)
{
  /** ########## Step 3.1 ##########
    * Deinitializes the STM32 Cryptographic library.
    */
  if (cmox_finalize(NULL) != CMOX_INIT_SUCCESS)
  {
    return EXEC_STATUS_ERROR;
  }
  PRINTF("[INFO] Step 3.1: STM32 Cryptographic library deinitialization COMPLETED.\n");

  /** ########## Step 3.2 ##########
    * Deinitializes the RNG instance.
    */
  mx_example_rng_deinit();
  PRINTF("[INFO] Step 3.2: RNG deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */
