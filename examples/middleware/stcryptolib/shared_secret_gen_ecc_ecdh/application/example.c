/**
  ******************************************************************************
  * file           : example.c
  * brief          : This example describes how to use the STM32 Cryptographic
  *                  library to establish and to verify a shared secret
  *                  using the ECDH algorithm over SECP256R1 curve.
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
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/** Extract from KAS_ECC_CDH_PrimitiveTest.txt
  * [P-256]
  *
  * COUNT = 0
  * QCAVSx = 700c48f77f56584c5cc632ca65640db91b6bacce3a4df6b42ce7cc838833d287
  * QCAVSy = db71e509e3fd9b060ddb20ba5c51dcc5948d46fbf640dfe0441782cab85fa4ac
  * dIUT = 7d7dc5f71eb29ddaf80d6214632eeae03d9058af1fb6d22ed80badb62bc1a534
  * QIUTx = ead218590119e8876b29146ff89ca61770c4edbbf97d38ce385ed281d8a6b230
  * QIUTy = 28af61281fd35e2fa7002523acc85a429cb06ee6648325389f59edfce1405141
  * ZIUT = 46fc62106420ff012e54a434fbdd2d25ccc5852060561e68040dd7778997bd7b
  * ---------------------------------------------------------------------
  */

const uint8_t Private_Key[] =
{
  0x7d, 0x7d, 0xc5, 0xf7, 0x1e, 0xb2, 0x9d, 0xda, 0xf8, 0x0d, 0x62, 0x14, 0x63, 0x2e, 0xea, 0xe0,
  0x3d, 0x90, 0x58, 0xaf, 0x1f, 0xb6, 0xd2, 0x2e, 0xd8, 0x0b, 0xad, 0xb6, 0x2b, 0xc1, 0xa5, 0x34
};
const uint8_t Remote_Public_Key[] =
{
  0x70, 0x0c, 0x48, 0xf7, 0x7f, 0x56, 0x58, 0x4c, 0x5c, 0xc6, 0x32, 0xca, 0x65, 0x64, 0x0d, 0xb9,
  0x1b, 0x6b, 0xac, 0xce, 0x3a, 0x4d, 0xf6, 0xb4, 0x2c, 0xe7, 0xcc, 0x83, 0x88, 0x33, 0xd2, 0x87,
  0xdb, 0x71, 0xe5, 0x09, 0xe3, 0xfd, 0x9b, 0x06, 0x0d, 0xdb, 0x20, 0xba, 0x5c, 0x51, 0xdc, 0xc5,
  0x94, 0x8d, 0x46, 0xfb, 0xf6, 0x40, 0xdf, 0xe0, 0x44, 0x17, 0x82, 0xca, 0xb8, 0x5f, 0xa4, 0xac
};
const uint8_t Expected_SecretX[] =
{
  0x46, 0xfc, 0x62, 0x10, 0x64, 0x20, 0xff, 0x01, 0x2e, 0x54, 0xa4, 0x34, 0xfb, 0xdd, 0x2d, 0x25,
  0xcc, 0xc5, 0x85, 0x20, 0x60, 0x56, 0x1e, 0x68, 0x04, 0x0d, 0xd7, 0x77, 0x89, 0x97, 0xbd, 0x7b
};

/* Computed data buffer */
uint8_t Computed_Secret[CMOX_ECC_SECP256R1_SECRET_LEN];

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes STM32 Cryptographic library.
  */
app_status_t app_init(void)
{
  app_status_t return_status;

  if (cmox_initialize(NULL) != CMOX_INIT_SUCCESS)
  {
    PRINTF("[ERROR] Step 1: STM32 Cryptographic library initialization ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
  }
  else
  {
    PRINTF("[INFO] Step 1: STM32 Cryptographic library initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Demonstrates how to establish and to verify the exchange of
  * shared secret using ECC algorithm configuration and operation.
  * This step is scheduled as follows:
  * Step 2.1: Establishes the shared secret using known remote public and known local Key.
  * Step 2.2: Compares the generated secret X coordinate with the expected one.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_OK;
  cmox_ecc_retval_t retval;
  size_t computed_size;
  /* Construct a ECC context, specifying mathematics implementation and working buffer for later processing */
  cmox_ecc_construct(&p_ecc_ctx, CMOX_ECC256_MATH_FUNCS, Working_Buffer, sizeof(Working_Buffer));

  /* Compute directly the shared secret passing all the needed parameters */
  retval = cmox_ecdh(&p_ecc_ctx,                                       /* ECC context */
                     CMOX_ECC_CURVE_SECP256R1,                         /* SECP256R1 ECC curve selected */
                     Private_Key, sizeof(Private_Key),                 /* Local Private key */
                     Remote_Public_Key, sizeof(Remote_Public_Key),     /* Remote Public key */
                     Computed_Secret, &computed_size);                 /* Data buffer to receive shared secret */

  /* Verify API returned value */
  if (retval != CMOX_ECC_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Computed_Secret))
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.1: Establish the shared secret using known remote public and known local Key COMPLETED.\n");

  /* Verify generated data are the expected ones */
  if (memcmp(Computed_Secret, Expected_SecretX, sizeof(Expected_SecretX)) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: Compare the generated secret X coordinate with the expected one COMPLETED.\n");

  /* Cleanup context */
  cmox_ecc_cleanup(&p_ecc_ctx);

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Deinitializes the STM32 Cryptographic library before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  /* No more need of cryptographic services, finalize cryptographic library */
  if (cmox_finalize(NULL) != CMOX_INIT_SUCCESS)
  {
    return EXEC_STATUS_ERROR;
  }

  PRINTF("[INFO] Step 3: STM32 Cryptographic library deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */
