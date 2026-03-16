/**
  ******************************************************************************
  * file           : example.c
  * brief          : RNG configuration with HAL API
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define RNG_NUMBERS   8U      /* Number of 32-bit random numbers that will be generated */
#define RNG_GENERATION_TIMEOUT   10U            /* Timeout for generation in millisecond */

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
hal_rng_handle_t *pRNG;  /* pointer referencing the RNG handle from the generated code */
uint32_t RandomNumbers[RNG_NUMBERS] = {0}; /* array to store the generated random numbers */

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The init of the RNG instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pRNG = mx_example_rng_init();

  if (pRNG != NULL)
  {
    PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
}


/** ########## Step 2 ##########
  * Generates random numbers and stores them in RandomNumbers[].
  */
app_status_t app_process(void)
{
  if (HAL_RNG_GenerateRandomNumber(pRNG, RandomNumbers, RNG_NUMBERS, RNG_GENERATION_TIMEOUT) != HAL_OK)
  {
    if (HAL_RNG_GetLastErrorCodes(pRNG) != HAL_RNG_ERROR_SEED)
    {
      return EXEC_STATUS_ERROR;
    }

    if (HAL_RNG_RecoverSeedError(pRNG) != HAL_OK)
    {
      return EXEC_STATUS_ERROR;
    }
  }

  for (uint32_t i = 0; i < RNG_NUMBERS; i++)
  {
    PRINTF("[INFO] Step 2:%" PRId32 " - Generate 32-bit random number: 0x%" PRIx32 ".\n", i + 1, RandomNumbers[i]);
  }

  return EXEC_STATUS_OK;
}


/** ########## Step 3 ##########
  * Deinitializes the RNG instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  PRINTF("[INFO] Step 3: Device de-init.\n");
  mx_example_rng_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
