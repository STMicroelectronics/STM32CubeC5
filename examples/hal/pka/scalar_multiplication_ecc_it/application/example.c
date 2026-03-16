/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to use the PKA peripheral to execute ECC scalar
  *                  multiplication in interruption mode.
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
#include "ecdsa_priv_prime256v1.h" /* reflect the content of ecdsa_priv_prime256v1.pem */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* timeout value in milliseconds */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

hal_pka_handle_t *pPKA;  /* PKA handle */
hal_rng_handle_t *pRNG;  /* RNG handle */
uint32_t EndOfProcess  = 0;
uint32_t ErrorCallback = 0;

/* Data buffers */
uint8_t BufferPtX[32 + 2] = {0};
uint8_t BufferPtY[32 + 2] = {0};

/* Private functions prototype -----------------------------------------------*/
static void pka_operation_cplt_callback(hal_pka_handle_t *pPKA);
static void pka_error_callback(hal_pka_handle_t *pPKA);


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

  if (HAL_PKA_RegisterOperationCpltCallback(pPKA, pka_operation_cplt_callback) != HAL_OK)
  {
    return_status = EXEC_STATUS_ERROR;
    goto _app_init_exit;
  }

  if (HAL_PKA_RegisterErrorCallback(pPKA, pka_error_callback) != HAL_OK)
  {
    return_status = EXEC_STATUS_ERROR;
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;
_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Generates a public key from a private key using ECC scalar multiplication.
  * Then determines if this public key is valid or not.
  * This step is scheduled as follows:
  * Step 2.1: Configures PKA for ECC scalar multiplication
  * Step 2.2: Performs ECC scalar multiplication in non-blocking mode with interrupt.
  * Step 2.3: Compares to expected result
  */
app_status_t app_process(void)
{
  hal_pka_ecc_mul_protect_config_t in_mul;
  hal_pka_ecc_mul_protect_result_t out;
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Reset the output buffer */
  memset(BufferPtX, 0, sizeof(BufferPtX));
  BufferPtX[0]       = 0xAA; /* magic number to detect overflow */
  BufferPtX[33]      = 0xAA; /* magic number to detect overflow */
  memset(BufferPtY, 0, sizeof(BufferPtY));
  BufferPtY[0]       = 0xAA; /* magic number to detect overflow */
  BufferPtY[33]      = 0xAA; /* magic number to detect overflow */

  /* Set output parameters */
  out.p_pt_x  = &BufferPtX[1];
  out.p_pt_y  = &BufferPtY[1];


  /* Set Multiplication input parameters */
  in_mul.coeff_sign            = Prime256v1_A_Sign;
  in_mul.p_coeff_a             = Prime256v1AbsA;
  in_mul.p_coeff_b             = Prime256v1_B;
  in_mul.p_modulus             = Prime256v1Prime;
  in_mul.modulus_size_byte     = Prime256v1PrimeLen;
  in_mul.p_pt_x                = Prime256v1GeneratorX;
  in_mul.p_pt_y                = Prime256v1GeneratorY;
  in_mul.p_scalar_mul          = ECDSA_PrivPrime256v1Priv;
  in_mul.scalar_mul_size_byte  = ECDSA_PrivPrime256v1PrivLen;
  in_mul.p_prime_order         = Prime256v1Order;
  in_mul.prime_order_size_byte = Prime256v1OrderLen;

  /* Set the ECC scalar multiplication configuration. */
  if (HAL_PKA_ECC_SetConfigMulProtect(pPKA, &in_mul) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: PKA configuration to process ECC scalar multiplication COMPLETED.\n");

  /* Launch ECC scalar multiplication computation in non-blocking mode with interrupt.*/
  if ((HAL_PKA_Compute_IT(pPKA) != HAL_OK))
  {
    goto _app_process_exit;
  }

  /* Wait for an IT computation process. */
  while ((EndOfProcess == 0) && (ErrorCallback == 0))
  {
    /* Put the CPU in Wait For Interrupt state.*/
    __WFI();
  }

  EndOfProcess = 0;
  if (ErrorCallback == 1)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: ECC scalar multiplication in interrupt mode COMPLETED.\n");

  /* Retrieve ECC scalar multiplication operation result */
  HAL_PKA_ECC_GetResultMulProtect(pPKA, &out);

  /* Compare the computed and expected result */
  if (memcmp(out.p_pt_x, ECDSA_PrivPrime256v1PubX, ECDSA_PrivPrime256v1PubXLen) != 0)
  {
    goto _app_process_exit;
  }

  if (memcmp(out.p_pt_y, ECDSA_PrivPrime256v1PubY, ECDSA_PrivPrime256v1PubYLen) != 0)
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


/** PKA operation complete callback.
  * @user This implementation of the PKA operation complete callback can be customized.
  */
static void pka_operation_cplt_callback(hal_pka_handle_t *pPKA)
{
  EndOfProcess = 1;
}

/** PKA Error callback.
  * @user This implementation of the PKA Error callback can be customized.
  */
static void pka_error_callback(hal_pka_handle_t *pPKA)
{
  ErrorCallback = 1;
}
