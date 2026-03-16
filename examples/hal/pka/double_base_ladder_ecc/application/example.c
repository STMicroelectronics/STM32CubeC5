/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to use the PKA to run ECC Double Base Ladder operation.
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
#define PKA_TIMEOUT 5000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* vector inputs and outputs*/
uint32_t Input1ModulusSize = 24;
uint32_t Input1OrderSize   = 24;
uint32_t Input1CoefSign    = 1;
uint8_t Input1DoubleBaseLadderInModulus[24] =
{
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
uint8_t Input1DoubleBaseLadderInACoeff[24] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03
};
uint8_t Input1DoubleBaseLadderInScalarK[24] =
{
  0x25, 0x29, 0x7D, 0xD0, 0xD3, 0x4D, 0x26, 0xAB,
  0x41, 0xC7, 0x18, 0xC5, 0x96, 0x4B, 0x41, 0xA7,
  0xAD, 0x8C, 0xB6, 0x59, 0xA2, 0x84, 0xD5, 0x17
};
uint8_t Input1DoubleBaseLadderInScalarM[24] =
{
  0xDC, 0xA9, 0x91, 0x69, 0x87, 0x7B, 0x16, 0xAF,
  0xD5, 0xC9, 0x53, 0xBE, 0x11, 0xA5, 0x13, 0xC3,
  0x14, 0x0F, 0xDC, 0xBC, 0x96, 0x40, 0x1A, 0xD0
};
uint8_t Input1DoubleBaseLadderInInitialPoint1X[24] =
{
  0xE7, 0xE5, 0x90, 0xDA, 0xC1, 0x2D, 0x4A, 0x2F,
  0x21, 0x5E, 0x6E, 0x16, 0x3B, 0x52, 0x27, 0xFB,
  0x48, 0x59, 0x81, 0xDF, 0x68, 0x02, 0xAA, 0xF7
};
uint8_t Input1DoubleBaseLadderInInitialPoint1Y[24] =
{
  0x65, 0x18, 0xE1, 0xF1, 0x67, 0xB1, 0x7A, 0xA8,
  0x90, 0x87, 0xD1, 0x53, 0x3B, 0xD1, 0x26, 0x76,
  0xC8, 0x12, 0x4A, 0xE8, 0x91, 0x9C, 0x71, 0x45
};
uint8_t Input1DoubleBaseLadderInInitialPoint1Z[24] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
};
uint8_t Input1DoubleBaseLadderInInitialPoint2X[24] =
{
  0xC8, 0xC1, 0x4C, 0x6E, 0x83, 0xDA, 0x2D, 0xAB,
  0x99, 0xC4, 0xDB, 0xE7, 0xDB, 0x39, 0x5E, 0x54,
  0x28, 0x55, 0x89, 0x45, 0x88, 0xBC, 0x74, 0x06
};
uint8_t Input1DoubleBaseLadderInInitialPoint2Y[24] =
{
  0x2B, 0x35, 0x8F, 0x4B, 0xE4, 0xA1, 0xA9, 0xEB,
  0x7F, 0x46, 0x21, 0xEE, 0x9F, 0xF9, 0xD7, 0xCB,
  0xE1, 0xEB, 0x42, 0xCE, 0x87, 0xC6, 0xD9, 0xE0
};
uint8_t Input1DoubleBaseLadderInInitialPoint2Z[24] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
};
uint8_t Output1X[24] =
{
  0x19, 0x51, 0xE5, 0x16, 0x72, 0x1D, 0x98, 0xB6,
  0x9E, 0xEC, 0x34, 0x0A, 0x2B, 0xA1, 0xC2, 0x08,
  0x03, 0x1C, 0xB6, 0xC7, 0xD3, 0x81, 0xCD, 0xEA
};
uint8_t Output1Y[24] =
{
  0x57, 0x1F, 0xA8, 0xFE, 0xCB, 0xA8, 0x5A, 0xD9,
  0xBA, 0xFD, 0x47, 0xA6, 0x60, 0xA9, 0x5F, 0xF5,
  0xE3, 0xCE, 0x22, 0x50, 0x42, 0xA9, 0x4D, 0x31
};

uint8_t BufferX[24];
uint8_t BufferY[24];
hal_pka_handle_t *pPKA;  /* PKA handle */
hal_rng_handle_t *pRNG;  /* RNG handle */
hal_pka_ecc_double_base_ladder_config_t In;
hal_pka_ecc_double_base_ladder_result_t Out;

/* Private functions prototype -----------------------------------------------*/


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
    goto _app_init_exit;
  }

  /* Init the PKA */
  pPKA = mx_example_pka_init();
  if (pPKA == NULL)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: RNG and PKA initialization COMPLETED.\n");

  return_status = EXEC_STATUS_INIT_OK;
_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Computes of k*P + m*Q, where (P,Q) are two points on an elliptic curve and (k,m) are two scalars.
  * This step is scheduled as follows:
  * Step 2.1: Configures the PKA configuration parameters.
  * Step 2.2: Starts the PKA ECC Double Base Ladder operation.
  * Step 2.3: Retrieves computation result.
  * Step 2.4: Checks retrieved result with expected result.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2.1 ##########
    * Configures the PKA configuration parameters.
    */
  /* Sets ECC double base ladder Configuration*/
  In.prime_order_size_byte = Input1OrderSize;
  In.modulus_size_byte     = Input1ModulusSize;
  In.coeff_sign            = Input1CoefSign;
  In.p_coeff_a             = Input1DoubleBaseLadderInACoeff;
  In.p_modulus             = Input1DoubleBaseLadderInModulus;
  In.p_integer_k           = Input1DoubleBaseLadderInScalarK;
  In.p_integer_m           = Input1DoubleBaseLadderInScalarM;
  In.p_base_pt_x_1         = Input1DoubleBaseLadderInInitialPoint1X;
  In.p_base_pt_y_1         = Input1DoubleBaseLadderInInitialPoint1Y;
  In.p_base_pt_z_1         = Input1DoubleBaseLadderInInitialPoint1Z;
  In.p_base_pt_x_2         = Input1DoubleBaseLadderInInitialPoint2X;
  In.p_base_pt_y_2         = Input1DoubleBaseLadderInInitialPoint2Y;
  In.p_base_pt_z_2         = Input1DoubleBaseLadderInInitialPoint2Z;

  /* Set output parameters */
  Out.p_pt_x = &BufferX[0];
  Out.p_pt_y = &BufferY[0];

  if ((HAL_PKA_ECC_SetConfigDoubleBaseLadder(pPKA, &In)) != HAL_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.1: Configuration parameters COMPLETED.\n");

  /** ########## Step 2.2 ##########
    * Starts the PKA ECC Double Base Ladder operation.
    */

  HAL_PKA_Compute(pPKA, PKA_TIMEOUT);

  PRINTF("[INFO] Step 2.2: Computation of the PKA ECC Double Base Ladder operation COMPLETED.\n");

  /** ########## Step 2.3 ##########
    * Retrieves computation result.
    */

  HAL_PKA_ECC_GetResultDoubleBaseLadder(pPKA, &Out);

  PRINTF("[INFO] Step 2.3: Retrieving the computation result COMPLETED.\n");

  /** ########## Step 2.4 ##########
    * Checks retrieved result with expected result.
    */

  if ((memcmp((uint8_t *)Out.p_pt_x, (uint8_t *)Output1X, 24) != 0)
      || (memcmp((uint8_t *)Out.p_pt_y, (uint8_t *)Output1Y, 24) != 0))
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.4: Verification of the computed result COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Deinitializes the PKA and the RNG peripherals before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_pka_deinit();
  mx_example_rng_deinit();

  PRINTF("[INFO] Step 3: PKA and RNG peripherals deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */
