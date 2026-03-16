/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to use the PKA peripheral to determine if a point is on
  *                  a curve. This allows validating an external public key.
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
#include "pkv.h" /* reference buffers from NIST Cryptographic Algorithm */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* timeout value in milliseconds */
#define PKA_TIMEOUT 5000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_pka_point_check_config_t In;
hal_pka_montgomery_config_t MontgomeryConfig;
hal_pka_handle_t *pPKA;  /* PKA handle */
hal_rng_handle_t *pRNG;  /* RNG handle */
uint32_t EndOfProcess  = 0;
uint32_t ErrorCallback = 0;
uint8_t MontgomeryOut[32];

/* Private functions prototype -----------------------------------------------*/
static void PkaOperationCpltCallback(hal_pka_handle_t *pPKA);
static void PkaErrorCallback(hal_pka_handle_t *pPKA);

/** ########## Step 1 ##########
  * The applicative code initializes the RNG and PKA instances.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Init the RNG */
  pRNG = mx_example_rng_init();

  if (pRNG == NULL)
  {
    PRINTF("[ERROR] Step 1: RNG initialization ERROR.\n");
    goto _app_init_exit;
  }

  /* Init the PKA */
  pPKA = mx_example_pka_init();
  if (pPKA == NULL)
  {
    PRINTF("[ERROR] Step 1: PKA initialization ERROR.\n");
    goto _app_init_exit;
  }

  if (HAL_PKA_RegisterOperationCpltCallback(pPKA, PkaOperationCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_PKA_RegisterErrorCallback(pPKA, PkaErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;
  PRINTF("[INFO] Step 1: RNG and PKA initialization COMPLETED.\n");
_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Determines the Montgomery parameter.
  * This step is scheduled as follows:
  * Step 2.1: Sets Montgomery parameter Configuration.
  * Step 2.2: Computes montgomery R2 input parameter.
  * Step 2.3: Retrieves Montgomery parameter.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Sets Montgomery R2 input parameters */
  MontgomeryConfig.size_byte = Prime256v1PrimeLen;
  MontgomeryConfig.p_operand = Prime256v1Prime;
  HAL_PKA_SetConfigMontgomery(pPKA, &MontgomeryConfig);

  PRINTF("[INFO] Step 2.1: Set Montgomery parameter Configuration COMPLETED.\n");

  /* Computes montgomery R2 input parameter */
  HAL_PKA_Compute(pPKA, PKA_TIMEOUT);

  PRINTF("[INFO] Step 2.2: Compute of the montgomery R2 input parameter COMPLETED.\n");

  /* Gets Montgomery R2 parameters */
  HAL_PKA_GetResultMontgomery(pPKA, MontgomeryOut);

  PRINTF("[INFO] Step 2.3: Retrieve Montgomery parameter COMPLETED.\n");


  /** ########## Step 3 ##########
    * Determines if a point is on a curve. This allows validating an external public key.
    * This step is scheduled as follows:
    * Step 3.1: Configures the PKA peripheral to validate an external public key.
    * Step 3.2: Computes point P(x,y) on the curve primv256V1.
    * Step 3.3: Verifies if the calculated point is on the curve.
    * Step 3.4: Configures x and y coordinates.
    * Step 3.5: Computes point P(x,y) not on the curve primv256V1.
    * Step 3.6: Verifies if the calculated point is not on the curve.
    */
  /* Initializes the PKA configuration parameters */
  In.modulus_size_byte  = Prime256v1PrimeLen;
  In.coeff_sign         = Prime256v1_A_Sign;
  In.p_coeff_a          = Prime256v1AbsA;
  In.p_coeff_b          = Prime256v1_B;
  In.p_modulus          = Prime256v1Prime;
  In.p_montgomery_param = MontgomeryOut;

  /* Sets x and y coordinates */
  In.p_pt_x             = PKV1_Qx;
  In.p_pt_y             = PKV1_Qy;

  if (HAL_PKA_ECC_SetConfigPointCheck(pPKA, &In) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3.1: PKA peripheral configuration COMPLETED.\n");

  /* Computes point P(x,y) on the curve primv256V1 */
  if (HAL_PKA_Compute_IT(pPKA) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Waits for an IT computation process. */
  while ((EndOfProcess == 0) && (ErrorCallback == 0))
  {
    /* Puts the CPU in Wait For Interrupt state.*/
    __WFI();
  }

  PRINTF("[INFO] Step 3.2: Computation using IT COMPLETED.\n");
  EndOfProcess = 0;
  if (ErrorCallback == 1)
  {
    goto _app_process_exit;
  }

  /* Verifies if the calculated point is on the curve */
  if (HAL_PKA_ECC_IsPointCheckOnCurve(pPKA) != PKA_ECC_POINT_ON_CURVE)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3.3: Verification of the calculated point is on the curve COMPLETED.\n");
  /* Sets x and y coordinates */
  In.p_pt_x             = PKV2_Qx;
  In.p_pt_y             = PKV2_Qy;

  if (HAL_PKA_ECC_SetConfigPointCheck(pPKA, &In) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3.4: PKA peripheral configuration COMPLETED.\n");

  /* Computes point P(x,y) not on the curve primv256V1 */
  if (HAL_PKA_Compute_IT(pPKA) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Waits for an IT computation process. */
  while ((EndOfProcess == 0) && (ErrorCallback == 0))
  {
    /* Puts the CPU in Wait For Interrupt state.*/
    __WFI();
  }

  PRINTF("[INFO] Step 3.5: Computation using IT COMPLETED.\n");
  EndOfProcess = 0;
  if (ErrorCallback == 1)
  {
    goto _app_process_exit;
  }

  /* Verifies if the calculated point is not on the curve */
  if (HAL_PKA_ECC_IsPointCheckOnCurve(pPKA) != PKA_ECC_POINT_NOT_ON_CURVE)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3.6: Verification of the calculated point is not on the curve COMPLETED.\n");
  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 4 ##########
  * Deinitializes the PKA and the RNG peripherals before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_pka_deinit();
  mx_example_rng_deinit();

  PRINTF("[INFO] Step 4: PKA and RNG peripherals deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** PKA operation complete callback.
  * @user This implementation of the PKA operation complete callback can be customized.
  */
static void PkaOperationCpltCallback(hal_pka_handle_t *pPKA)
{
  EndOfProcess = 1;
}

/** PKA Error callback.
  * @user This implementation of the PKA Error callback can be customized.
  */
static void PkaErrorCallback(hal_pka_handle_t *pPKA)
{
  ErrorCallback = 1;
}
