/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to use the PKA to run ECC Projective to Affine operation.
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

uint32_t Input1ModulusSize                                   = 24;
uint8_t Input1ConvToAffineInModulus[24]           =
{
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

uint8_t Input1ConvToAffineInInitialPointX[24]   =
{
  0x18, 0x0D, 0x8B, 0xA2, 0xAE, 0x50, 0x13, 0xF4, 0x98, 0xCA, 0x7E, 0xE6,
  0x89, 0x09, 0xD4, 0x16, 0x72, 0x22, 0xA3, 0x3A, 0xD8, 0x08, 0x8A, 0x37
};

uint8_t Input1ConvToAffineInInitialPointY[24]   =
{
  0x95, 0x5F, 0x3B, 0xDE, 0xAD, 0x2C, 0x03, 0xAA, 0x12, 0xF2, 0x4B, 0x6E,
  0xE5, 0x68, 0x35, 0x46, 0x3B, 0x2F, 0xD8, 0x33, 0x59, 0x42, 0x7D, 0x9B
};

uint8_t Input1ConvToAffineInInitialPointZ[24]   =
{
  0x16, 0xC8, 0x05, 0xA7, 0x19, 0xB0, 0xBE, 0xC3, 0x80, 0xCC, 0x78, 0x9F,
  0x8E, 0xED, 0x49, 0x08, 0x97, 0xAE, 0x78, 0xB7, 0x95, 0x58, 0x30, 0x2B
};

uint8_t Input1ConvToAffineInMontgomeryParam[24]  =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
};

uint8_t Output1X[24] =
{
  0xB3, 0x67, 0xA9, 0x1D, 0x79, 0xD2, 0xFF, 0x8A, 0x18, 0x6E, 0xF6, 0x08,
  0x0F, 0x0F, 0x71, 0xEB, 0xA7, 0x5A, 0x9F, 0x2A, 0x80, 0xED, 0xD9, 0x48
};

uint8_t Output1Y[24] =
{
  0x6A, 0x86, 0xD5, 0xAA, 0x2D, 0xC3, 0x88, 0x2F, 0xF1, 0x72, 0x69, 0x46,
  0x54, 0xC8, 0x93, 0x8B, 0x75, 0xF9, 0x1F, 0x8F, 0x76, 0x37, 0xA6, 0x8D
};

uint8_t BufferPtX[24 + 2] = {0};
uint8_t BufferPtY[24 + 2] = {0};

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
  * Demonstrates how to use the PKA to run ECC projective to affine operation computes
  * the conversion between the representation of a point P in homogeneous projective
  * coordinates and the representation of the point P in affine coordinates.
  * This step is scheduled as follows:
  * Step 2.1: Configures Projective to affine
  * Step 2.2: Computes the representation of the point P in affine coordinates
  * Step 2.3: Compares to expected result
  */
app_status_t app_process(void)
{
  hal_pka_ecc_projective_to_affine_config_t in;
  hal_pka_ecc_projective_to_affine_result_t out;

  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Reset the output buffer */
  memset(BufferPtX, 0, sizeof(BufferPtX));
  BufferPtX[0]  = 0xAA; /* magic number to detect overflow */
  BufferPtX[25] = 0xAA; /* magic number to detect overflow */
  memset(BufferPtY, 0, sizeof(BufferPtY));
  BufferPtY[0]  = 0xAA; /* magic number to detect overflow */
  BufferPtY[25] = 0xAA; /* magic number to detect overflow */

  /* Set output parameters */
  out.p_pt_x = &BufferPtX[1];
  out.p_pt_y = &BufferPtY[1];

  /* Set Sign input parameters */
  in.modulus_size_byte  = Input1ModulusSize;
  in.p_modulus          = Input1ConvToAffineInModulus;
  in.p_base_pt_x        = Input1ConvToAffineInInitialPointX;
  in.p_base_pt_y        = Input1ConvToAffineInInitialPointY;
  in.p_base_pt_z        = Input1ConvToAffineInInitialPointZ;
  in.p_montgomery_param = Input1ConvToAffineInMontgomeryParam;

  /* Set ECC projective to affine Configuration */
  if (HAL_PKA_ECC_SetConfigProjectiveToAffine(pPKA, &in) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: PKA configuration ECC projective to affine COMPLETED.\n");

  /* Computes the representation of the point P in affine coordinates */
  if ((HAL_PKA_Compute(pPKA, PKA_TIMEOUT) != HAL_OK))
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: Retrieve the point P in affine coordinates COMPLETED.\n");

  /* Retrieve ECC projective to affine operation result */
  HAL_PKA_ECC_GetResultProjectiveToAffine(pPKA, &out);

  /* Compare to expected result */
  if (memcmp(out.p_pt_x, Output1X, Input1ModulusSize) != 0)
  {
    goto _app_process_exit;
  }

  if (memcmp(out.p_pt_y, Output1Y, Input1ModulusSize) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.3: Compare to expected result COMPLETED.\n");

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
