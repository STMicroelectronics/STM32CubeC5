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
#include <stdlib.h> /* malloc */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* timeout value in milliseconds */
#define PKA_TIMEOUT 5000
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_pka_handle_t *pPKA;  /* PKA handle */
hal_rng_handle_t *pRNG;  /* RNG handle */

hal_pka_mod_exp_fast_config_t ExpFastConfig; /* PKA RSA Config handle */

const uint8_t Exponent[] =
{
  0x17, 0x24, 0x70, 0xF1, 0x2D, 0x60, 0x9A, 0x8B, 0x5F, 0xDC, 0xF8, 0x12,
  0x7C, 0xBE, 0xE9, 0xAD, 0xB9, 0x3C, 0xB7, 0x63, 0x59, 0x9D, 0xAE, 0x40
};
const uint32_t ExponentSize = 24;
const uint8_t Operand[] =
{
  0x55, 0xE8, 0x82, 0x73, 0x0E, 0xBC, 0x52, 0xEC, 0x42, 0xBE, 0x0C, 0x7B,
  0x6F, 0xFF, 0x0B, 0xFC, 0x07, 0xB0, 0x5A, 0x1C, 0x79, 0x36, 0x03, 0x7D
};
const uint32_t OperandSize = 24;
const uint8_t Modulus[] =
{
  0xAA, 0x7F, 0xE9, 0x0B, 0xDB, 0xFD, 0x3B, 0xD7, 0xDE, 0xA1, 0x84, 0x22,
  0xA7, 0xBF, 0x12, 0xE2, 0x7F, 0x00, 0x1A, 0x46, 0xBA, 0x64, 0x49, 0x11

};
const uint8_t MontgomeryParam[] =
{
  0xFB, 0x72, 0x1E, 0x55, 0x61, 0xDE, 0x0C, 0x22, 0x4A, 0x08, 0xD7, 0x05,
  0xB5, 0x4D, 0x1F, 0x16, 0xE4, 0x32, 0xC5, 0x42, 0xA8, 0xF4, 0x22, 0xA0
};
uint8_t Output[24] = {0};
uint8_t ExpectedOutput[] =
{
  0x48, 0x19, 0x19, 0xB6, 0xA3, 0x2E, 0xFD, 0x38, 0xCC, 0xB7, 0x6B, 0xA7,
  0x35, 0xC2, 0x69, 0xA9, 0xD3, 0x8F, 0xE1, 0xAC, 0xEF, 0x76, 0x56, 0x35
};

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the PKA and the RNG peripherals.
  * This step is scheduled as follows:
  * Step 1: RNG and PKA peripheral initialization.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Initializes the RNG and the PKA peripherals. */
  pRNG = mx_example_rng_init();
  pPKA = mx_example_pka_init();

  if ((pRNG == NULL) && (pPKA == NULL))
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: RNG and PKA initialization COMPLETED.\n");

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Executes the modular exponentiation fast mode and,
  * verifies the returned value with the expected output.
  * This step is scheduled as follows:
  * Step 2.1: Configures PKA for Modular exponentiation (fast) mode operation.
  * Step 2.2: Executes Modular exponentiation (fast) mode operation.
  * Step 2.3: Verifies the returned value of Modular exponentiation (fast) mode,
  *           operation with the expected one.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Set input parameters */
  ExpFastConfig.exponent_size_byte = ExponentSize;
  ExpFastConfig.operand_size_byte = OperandSize;
  ExpFastConfig.p_exponent = Exponent;
  ExpFastConfig.p_operand = Operand;
  ExpFastConfig.p_modulus = Modulus;
  ExpFastConfig.p_montgomery_param = MontgomeryParam;

  /* Configure the RSA CRT exponentiation operation. */
  if (HAL_PKA_SetConfigModExpFast(pPKA, &ExpFastConfig) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: PKA configuration for modular exponentiation (fast) mode operation COMPLETED.\n");

  /* Start the modular exponentiation fast operation */
  if (HAL_PKA_Compute(pPKA, PKA_TIMEOUT) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: Execution of modular exponentiation operation COMPLETED.\n");

  /* Copy the results to user specified space */
  HAL_PKA_GetResultModExpFast(pPKA, Output);

  /* Verify the computed hash message */
  if (memcmp(Output, ExpectedOutput, 24) != 0)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.3: Verification the result of modular exponentiation operation COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Deinitializes PKA and RNG instances before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_rng_deinit();
  mx_example_pka_deinit();
  PRINTF("[INFO] Step 3: PKA and RNG deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */

