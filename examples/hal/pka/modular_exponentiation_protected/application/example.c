/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to calculate (A^e mod n) with PKA in protected modular
  *                  exponentiation mode.
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* timeout value in milliseconds */
#define PKA_TIMEOUT 5000
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_pka_handle_t *pPKA;  /* PKA handle */
hal_rng_handle_t *pRNG;  /* RNG handle */

hal_pka_mod_exp_protect_config_t ModExpProtConfig; /* PKA RSA Config handle */

/* Input vectors */
uint32_t OperandSizeByte   = 32;
uint32_t ExponentSizeByte  = 32;

uint8_t pOperand[32] =
{
  0x45, 0xAB, 0x14, 0x95, 0x48, 0x22, 0x69, 0xC4, 0x8F, 0x1E, 0xCA, 0x23, 0x0C, 0x1F, 0x5A, 0xB4,
  0xBC, 0xE7, 0x12, 0xD8, 0x50, 0x09, 0x54, 0xBB, 0xE2, 0x1D, 0x2A, 0x39, 0x86, 0x3E, 0xDB, 0xFB
};

uint8_t pExponent[32] =
{
  0x12, 0x28, 0x49, 0x3A, 0x81, 0xFE, 0xCA, 0x62, 0x2B, 0x2D, 0x83, 0xCD, 0x97, 0x2C, 0x28, 0x23,
  0x39, 0x76, 0xF1, 0xF3, 0x71, 0xCE, 0x16, 0x84, 0xA0, 0x37, 0x98, 0xE0, 0xC7, 0x0B, 0xF4, 0x39
};

uint8_t pModulus[32] =
{
  0xE4, 0x88, 0xD8, 0x11, 0x76, 0xE5, 0x06, 0xFA, 0xB7, 0xC2, 0xFC, 0x5D, 0xF3, 0xCB, 0x75, 0x55,
  0x34, 0x3F, 0x45, 0xB4, 0x78, 0x52, 0xBA, 0x7E, 0xFB, 0xF5, 0xB1, 0x2A, 0xF4, 0x0B, 0xF4, 0xB3
};

uint8_t pPhi[32] =
{
  0xE4, 0x88, 0xD8, 0x11, 0x76, 0xE5, 0x06, 0xFA, 0xB7, 0xC2, 0xFC, 0x5D, 0xF3, 0xCB, 0x75, 0x53,
  0x50, 0x7D, 0xA2, 0xCD, 0x98, 0x24, 0x33, 0x76, 0x14, 0x8F, 0xCF, 0xA3, 0xF5, 0xCF, 0x4A, 0x88
};

uint8_t ExpectedOutput[32]  =
{
  0xD1, 0x2F, 0x36, 0x6C, 0x61, 0xB5, 0x66, 0x48, 0x61, 0x17, 0x8A, 0x1E, 0x6B, 0xD0, 0xE7, 0xBF,
  0x66, 0x0A, 0x2B, 0x07, 0x9D, 0x4D, 0x82, 0x68, 0x7E, 0xB1, 0x9D, 0x29, 0x94, 0x43, 0x6D, 0x39
};

uint8_t ExpectedOutputSize = 32;
uint8_t OutputBuffer[32];

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
  * Computes (A^e mod n) using PKA modular exponentiation (protected) operation.
  * This step is scheduled as follows:
  * Step 2.1: Configures PKA to process protected modular exponentiation operation
  * Step 2.2: Computes (A^e mod n) in (protected) modular exponentiation mode
  * Step 2.3: Compares to expected result
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Set input parameters */
  ModExpProtConfig.exponent_size_byte = ExponentSizeByte;
  ModExpProtConfig.operand_size_byte  = OperandSizeByte;
  ModExpProtConfig.p_exponent         = pExponent;
  ModExpProtConfig.p_modulus          = pModulus;
  ModExpProtConfig.p_operand          = pOperand;
  ModExpProtConfig.p_phi              = pPhi;

  /* Configures the modular exponentiation (protected mode). */
  if (HAL_PKA_SetConfigModExpProtect(pPKA, &ModExpProtConfig) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: PKA configuration to process protected modular exponentiation operation COMPLETED.\n");

  /* Starts the modular exponentiation (deciphering step) */
  if ((HAL_PKA_Compute(pPKA, PKA_TIMEOUT) != HAL_OK))
  {
    goto _app_process_exit;
  }

  /* Copy the results to user specified space */
  HAL_PKA_GetResultModExpProtect(pPKA, OutputBuffer);

  PRINTF("[INFO] Step 2.2: Computes (A^e mod n) using PKA protected modular exponentiation COMPLETED.\n");

  /* (Facultative) Compares the computed and expected result */
  if (memcmp(OutputBuffer, ExpectedOutput, ExpectedOutputSize) != 0)
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
