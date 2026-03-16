/**
  ******************************************************************************
  * file           : example.c
  * brief          : RandomGeneration DRBG using the  STM32 Flexible Crypto
  *                  Framework (STFCF)
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

/* Crypto services and defines */
#include "crypto.h"

/* to use memcmp function */
#include <string.h>

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Data buffer size */
#define RANDOM_SIZE  48u

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
hal_rng_handle_t *pRNG; /* Pointer referencing the RNG handle from the generated code */

/* Data buffer to receive generated random */
uint8_t ComputedRandom [RANDOM_SIZE] = {0};
uint8_t ComputedRandom2[RANDOM_SIZE] = {0};

/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * The applicative code initializes the RNG instance and PSA Crypto API.
  * This step is divided into sub-steps:
  * Step 1.1: Initializes the RNG instance.
  * Step 1.2: Initializes the PSA Crypto API.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  psa_status_t retval;

  /* Initializes the RNG instance */
  pRNG = mx_example_rng_init();

  if (pRNG == NULL)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1.1: RNG initialization COMPLETED.\n");

  /* Initializes the PSA Crypto API */
  retval = psa_crypto_init();

  if (retval != PSA_SUCCESS)
  {
    goto _app_init_exit;
  }
  PRINTF("[INFO] Step 1.2: PSA Crypto API initialization COMPLETED.\n");

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Demonstrates how to use the STM32 Flexible Crypto
  * Framework (STFCF) to generate a random numbers using the RandomGeneration DRBG.
  *
  * This step is scheduled as follows:
  * Step 2.1: Generates the first random data.
  * Step 2.2: Generates the second random data.
  * Step 2.3: Verifies the generated data are different.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  psa_status_t retval;

  /** ########## Step 2.1 ##########
    * Generates the first random data.
    */
  retval = psa_generate_random(ComputedRandom, sizeof(ComputedRandom));

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: First random data generation COMPLETED.\n");

  /** ########## Step 2.2 ##########
    * Generates the second random data.
    */
  retval = psa_generate_random(ComputedRandom2, sizeof(ComputedRandom2));

  /* Verify API returned value */
  if (retval != PSA_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: Second random data generation COMPLETED.\n");

  /** ########## Step 2.3 ##########
    *  Verifies the generated data are different.
    */
  if (memcmp(ComputedRandom, ComputedRandom2, sizeof(ComputedRandom)) == 0)
  {
    /* Random are identical ! */
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.3: Random data verification COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * clears data of the PSA layer, and deinitializes the RNG
  * instance before leaving the scenario.
  * This step is divided into sub-steps:
  * Step 3.1: Clears the data of the PSA layer.
  * Step 3.2: Deinitializes the RNG instance.
  */
app_status_t app_deinit(void)
{
  mbedtls_psa_crypto_free();

  PRINTF("[INFO] Step 3.1: Clear data of the PSA layer COMPLETED.\n");

  mx_example_rng_deinit();

  PRINTF("[INFO] Step 3.2: RNG deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */
