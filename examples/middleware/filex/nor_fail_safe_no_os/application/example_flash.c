/**
  ******************************************************************************
  * file           : example_flash.c
  * brief          : example program body for families using FLASH.
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
#include "mx_filex_app.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint32_t reset_count;
static hal_tim_handle_t *pTIM;
hal_flash_handle_t *pFLASH;
static hal_rng_handle_t *pRNG;

static void TimerExpirationCallback(hal_tim_handle_t *htim);
/** ########## Step 1 ##########
  * Initializes FileX.
  */
/* The init of the sd instance is triggered by the applicative code */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_INIT_OK;

  /* Initialize the platform handles used by this example.
   * Any failure is latched into return_status; the remaining initializations are still attempted
   * to keep the behavior deterministic and easier to debug.
   */
  pFLASH = mx_example_flash_init();
  if (pFLASH == NULL)
  {
    return_status = EXEC_STATUS_ERROR;
  }

  pTIM = mx_example_tim_init();
  if (pTIM == NULL)
  {
    return_status = EXEC_STATUS_ERROR;
  }
  pRNG = mx_example_rng_init();
  if (pRNG == NULL)
  {
    return_status = EXEC_STATUS_ERROR;
  }

  reset_count = *(__IO uint32_t *)FLASH_USER_ADDR;

  if (reset_count == 0xFFFFFFFFU)
  {
    /* First run: initialize counter to 0. */
    reset_count = 0U;
  }

  if (reset_count < NVIC_RESET_COUNT)
  {
    /* Increment a reset counter stored at FLASH_USER_ADDR.
    * The board is reset until the counter reaches NVIC_RESET_COUNT.
    */

    reset_count++;

    HAL_FLASH_ITF_Unlock(HAL_FLASH);

    if (HAL_FLASH_EraseByAddr(pFLASH, FLASH_USER_ADDR, sizeof(reset_count), 1000U) != HAL_OK)
    {
      return_status = EXEC_STATUS_ERROR;
    }
    else if (HAL_FLASH_ProgramByAddr(pFLASH, FLASH_USER_ADDR, &reset_count, sizeof(reset_count), 1000U) != HAL_OK)
    {
      return_status = EXEC_STATUS_ERROR;
    }

    HAL_FLASH_ITF_Lock(HAL_FLASH);
  }

  /* Register the periodic timebase callback.
   * The callback writes a counter value to FLASH_USER_ADDR.
   */
  HAL_TIM_RegisterUpdateCallback(pTIM, TimerExpirationCallback);

  PRINTF("[INFO] Step1: Initializing FileX...\r\n");
  /* Initialize the filex system component */
  if (app_filex_init() != 0)
  {
    return_status = EXEC_STATUS_ERROR;
  }

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\r\n");
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_OK;
  uint32_t random_number;

  if (HAL_RNG_GenerateRandomNumber(pRNG,
                                   (void *)&random_number,
                                   1U,
                                   RNG_GENERATION_TIMEOUT) != HAL_OK)
  {
    return EXEC_STATUS_ERROR;
  }

  HAL_TIM_SetPeriod(pTIM, random_number);

  /* Start the timer in interrupt mode (timeout armed). */
  HAL_TIM_Start_IT(pTIM);

  if (app_filex_process() != 0)
  {
    return_status = EXEC_STATUS_ERROR;
  }

  return return_status;
} /* end app_process */


/** ########## Step 5 ##########
  * De-initializes the XSPI instance before exiting the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_rng_deinit();
  mx_example_tim_deinit();
  mx_example_flash_deinit();
  PRINTF("[INFO] Step 5: Deinitializes the XSPI instance\r\n");
  return EXEC_STATUS_OK;
} /* end app_deinit */


static void TimerExpirationCallback(hal_tim_handle_t *htim)
{
  HAL_TIM_Stop_IT(pTIM);
  HAL_TIM_SetCounter(pTIM, 0);

  if (reset_count < NVIC_RESET_COUNT)
  {
    PRINTF("[INFO] reset_count %lu < %lu, triggering NVIC_SystemReset()\r\n",
           (unsigned long)reset_count,
           (unsigned long)NVIC_RESET_COUNT);
    PRINTF("[INFO] reset counter incremented, resetting the board...\r\n");
    NVIC_SystemReset();
  }
  else
  {
    PRINTF("[INFO] Maximum reset count reached, calling app_deinit()\r\n");
    reset_count = 0U;

    HAL_FLASH_ITF_Unlock(HAL_FLASH);
    if (HAL_FLASH_EraseByAddr(pFLASH, FLASH_USER_ADDR, sizeof(reset_count), 1000U) == HAL_OK)
    {
      (void)HAL_FLASH_ProgramByAddr(pFLASH,
                                    FLASH_USER_ADDR,
                                    &reset_count,
                                    sizeof(reset_count),
                                    1000U);
    }
    HAL_FLASH_ITF_Lock(HAL_FLASH);
    app_deinit();
  }
}
