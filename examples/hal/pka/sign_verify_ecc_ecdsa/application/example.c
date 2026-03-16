/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to use PKA ECDSA signature & verification.
  *
  ******************************************************************************
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "prime256v1.h" /* reference buffers containing the description of nist P-256 (ECDSA-256) */
#include "sig_gen.h" /* reference buffers from NIST */
#include "sig_ver.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* timeout value in milliseconds */
#define PKA_TIMEOUT 5000
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

hal_pka_handle_t *pPKA;  /* PKA handle */
hal_rng_handle_t *pRNG;  /* RNG handle */

hal_pka_ecdsa_signature_protect_config_t InSign;
hal_pka_ecdsa_verif_config_t InVerif;
hal_pka_ecdsa_signature_protect_result_t Out;
hal_pka_ecdsa_signature_result_ext_config_t OutExt;

uint32_t ResultSize;
/* Data buffers */
uint8_t rSignBuffer[32 + 2] = {0};
uint8_t sSignBuffer[32 + 2] = {0};
uint8_t BufferPtX[32 + 2] = {0};
uint8_t BufferPtY[32 + 2] = {0};
/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the RNG and PKA instances.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_INIT_OK;

  /* Init the RNG */
  pRNG = mx_example_rng_init();

  if (pRNG != NULL)
  {
    PRINTF("[INFO] Step 1.1: RNG initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }
  else
  {
    PRINTF("[ERROR] Step 1.1: RNG initialization ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
  }

  /* Init the PKA */
  pPKA = mx_example_pka_init();
  if (pPKA != NULL)
  {
    PRINTF("[INFO] Step 1.2: PKA initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }
  else
  {
    PRINTF("[ERROR] Step 1.2: PKA initialization ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
  }

  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Computes a signed message regarding the Elliptic curve digital signature
  * algorithm (ECDSA).
  * This step is scheduled as follows:
  * Step 2.1: Configures PKA for Signature Generation
  * Step 2.2: Performs Signature Generation
  * Step 2.3: Compares to expected result
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Reset the output buffer */
  memset(rSignBuffer, 0, sizeof(rSignBuffer));
  rSignBuffer[0]       = 0xAA; /* magic number to detect overflow */
  rSignBuffer[33]      = 0xAA; /* magic number to detect overflow */
  memset(sSignBuffer, 0, sizeof(sSignBuffer));
  sSignBuffer[0]       = 0xAA; /* magic number to detect overflow */
  sSignBuffer[33]      = 0xAA; /* magic number to detect overflow */
  memset(BufferPtX, 0, sizeof(BufferPtX));
  BufferPtX[0]  = 0xAA; /* magic number to detect overflow */
  BufferPtX[33] = 0xAA; /* magic number to detect overflow */
  memset(BufferPtY, 0, sizeof(BufferPtY));
  BufferPtY[0]  = 0xAA; /* magic number to detect overflow */
  BufferPtY[33] = 0xAA; /* magic number to detect overflow */
  /* Set output parameters */
  Out.p_r_sign  = &rSignBuffer[1];
  Out.p_s_sign  = &sSignBuffer[1];
  OutExt.p_pt_x = &BufferPtX[1];
  OutExt.p_pt_y = &BufferPtY[1];

  /* Set Sign input parameters */
  InSign.prime_order_size_byte = Prime256v1OrderLen;
  InSign.modulus_size_byte     = Prime256v1PrimeLen;
  InSign.coeff_sign            = Prime256v1_A_Sign;
  InSign.p_coeff               = Prime256v1AbsA;
  InSign.p_coeff_b             = Prime256v1_B;
  InSign.p_modulus             = Prime256v1Prime;
  InSign.p_base_pt_x           = Prime256v1GeneratorX;
  InSign.p_base_pt_y           = Prime256v1GeneratorY;
  InSign.p_prime_order         = Prime256v1Order;

  InSign.p_integer             = SigGen_K;
  InSign.p_hash                = SigGenHashMsg;
  InSign.p_private_key         = SigGen_D;

  /* Set signature configuration */
  if (HAL_PKA_ECDSA_SetConfigSignatureProtect(pPKA, &InSign) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: PKA configuration to process message signature using elliptic curves COMPLETED.\n");

  /* Launch signature computation */
  if ((HAL_PKA_Compute(pPKA, 5000) != HAL_OK))
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: Signature computation COMPLETED.\n");

  /* Retrieve ECDSA signature */
  ResultSize = HAL_PKA_ECDSA_GetResultSignatureProtect(pPKA, &Out, &OutExt);

  if (ResultSize != SigGen_R_Len)
  {
    goto _app_process_exit;
  }

  /* Compare to expected result */
  if (memcmp(Out.p_r_sign, SigGen_R, ResultSize) != 0)
  {
    goto _app_process_exit;
  }

  if (memcmp(Out.p_s_sign, SigGen_S, ResultSize) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.3: Compares to expected result COMPLETED.\n");

  /** ########## Step 3 ##########
    * Verifies if the generated signature is valid or not through "Signature Verification".
    * This step is scheduled as follows:
    * Step 3.1: Configures PKA for Signature Verification
    * Step 3.2: Performs Signature Verification
    * Step 3.3: Compares to expected result
    */
  /* Set Verification input parameters */
  InVerif.prime_order_size_byte = Prime256v1OrderLen;
  InVerif.modulus_size_byte     = Prime256v1PrimeLen;
  InVerif.coeff_sign            = Prime256v1_A_Sign;
  InVerif.p_coeff               = Prime256v1AbsA;
  InVerif.p_modulus             = Prime256v1Prime;
  InVerif.p_base_pt_x           = Prime256v1GeneratorX;
  InVerif.p_base_pt_y           = Prime256v1GeneratorY;
  InVerif.p_prime_order         = Prime256v1Order;

  InVerif.p_pub_key_curve_pt_x  = SigVer_Qx;
  InVerif.p_pub_key_curve_pt_y  = SigVer_Qy;
  InVerif.p_r_sign              = SigVer_R;
  InVerif.p_s_sign              = SigVer_S;
  InVerif.p_hash                = SigVerHashMsg;

  /* Set signature configuration */
  if (HAL_PKA_ECDSA_SetConfigVerifSignature(pPKA, &InVerif) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3.1: PKA configuration to process message verification using elliptic curves COMPLETED.\n");

  /* Launch signature computation */
  if ((HAL_PKA_Compute(pPKA, 5000) != HAL_OK))
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3.2: Signature computation COMPLETED.\n");

  /* Compare to expected result */
  if (HAL_PKA_ECDSA_IsValidVerifSignature(pPKA) != PKA_ECDSA_SIGNATURE_VALID)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3.3: Signature verification COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 4 ##########
  * Deinitializes PKA and RNG instances before leaving the scenario.
  */
app_status_t app_deinit(void)
{

  mx_example_pka_deinit();
  mx_example_rng_deinit();

  PRINTF("[INFO] Step 4: PKA and RNG deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */

