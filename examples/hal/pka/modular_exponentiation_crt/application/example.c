/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to perform decryption using the Chinese Remainder Theorem
  *                 (CRT) optimization with the RSA algorithm.
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
#include <string.h>
#include "rsa_priv_2048.h"
#include "rsa_pub_2048.h"
#include "plaintext.h"
#include "ciphertext.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* timeout value in milliseconds */
#define PKA_TIMEOUT 5000
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_pka_handle_t *pPKA;  /* PKA handle */
hal_rng_handle_t *pRNG;  /* RNG handle */

hal_pka_rsa_crt_exp_config_t CRT_ExpConfig; /* PKA RSA Config handle */

uint8_t DecipheredBuffer[256];


/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * The applicative code initializes the RNG and PKA instances.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

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
    goto _app_init_exit;
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
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;
_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Computes a plaintext using PKA RSA CRT exponentiation operation.
  * Then determines if this plaintext is identical to the expected one or not.
  * This step is scheduled as follows:
  * Step 2.1: Configures PKA to process RSA CRT exponentiation operation
  * Step 2.2: Computes plaintext from a ciphertext (deciphering)
  * Step 2.3: Compares to expected result
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Set input parameters */
  CRT_ExpConfig.size_byte      = RSA_PRIV_2048_PRIVATEEXPONENT_LEN;
  CRT_ExpConfig.p_operand_dp   = RSA_Priv2048Exponent1;
  CRT_ExpConfig.p_operand_dq   = RSA_Priv2048Exponent2;
  CRT_ExpConfig.p_operand_qinv = RSA_Priv2048Coefficient;
  CRT_ExpConfig.p_prime_p      = RSA_Priv2048Prime1;
  CRT_ExpConfig.p_prime_q      = RSA_Priv2048Prime2;
  CRT_ExpConfig.p_operand_a    = CiphertextBin;

  /* Configures the RSA CRT exponentiation operation. */
  if (HAL_PKA_RSA_SetConfigCRTExp(pPKA, &CRT_ExpConfig) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: PKA configuration to process RSA CRT exponentiation operation COMPLETED.\n");

  /* Start the modular exponentiation (deciphering step) */
  if ((HAL_PKA_Compute(pPKA, PKA_TIMEOUT) != HAL_OK))
  {
    goto _app_process_exit;
  }

  /* Copy the results to user specified space */
  HAL_PKA_RSA_GetResultCRTExp(pPKA, DecipheredBuffer);

  PRINTF("[INFO] Step 2.2: Decipher ciphertext using PKA modular exponentiation COMPLETED.\n");

  /* Compare the computed plaintext and expected result */
  if (memcmp(DecipheredBuffer, PlaintextBin, PLAINTEXT_BIN_LEN) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.3: Compares to expected result COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Deinitializes PKA and RNG instances before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_pka_deinit();
  mx_example_rng_deinit();

  PRINTF("[INFO] Step 3: PKA and RNG deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */

