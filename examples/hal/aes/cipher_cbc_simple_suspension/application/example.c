/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to use the STM32 HAL API to suspend and resumes encryption,
  *                  in IT mode using the AES CBC algorithm, with modifying the context.
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
#define AES_TIMEOUT_MS 1000
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_aes_handle_t *pAES; /* pointer referencing the AES handle from the generated code */
hal_tim_handle_t *pTim; /* pointer referencing the TIM handle from the generated code */
/** Extract from NIST Special Publication 800-38A
  * F.2.1 CBC-AES128.Encrypt
  * Key 2b7e151628aed2a6abf7158809cf4f3c
  * IV 000102030405060708090a0b0c0d0e0f
  * Block #1
  * Plaintext 6bc1bee22e409f96e93d7e117393172a
  * Input Block 6bc0bce12a459991e134741a7f9e1925
  * Output Block 7649abac8119b246cee98e9b12e9197d
  * Ciphertext 7649abac 8119b246 cee98e9b 12e9197d
  * Block #2
  * Plaintext ae2d8a571e03ac9c9eb76fac45af8e51
  * Input Block d86421fb9f1a1eda505ee1375746972c
  * Output Block 5086cb9b507219ee95db113a917678b2
  * Ciphertext 5086cb9b 507219ee 95db113a 917678b2
  * Block #3
  * Plaintext 30c81c46a35ce411e5fbc1191a0a52ef
  * Input Block 604ed7ddf32efdff7020d0238b7c2a5d
  * Output Block 73bed6b8e3c1743b7116e69e22229516
  * Ciphertext 73bed6b8 e3c1743b 7116e69e 22229516
  * Block #4
  * Plaintext f69f2445df4f9b17ad2b417be66c3710
  * Input Block 8521f2fd3c8eef2cdc3da7e5c44ea206
  * Output Block 3ff1caa1681fac09120eca307586e1a7
  * Ciphertext 3ff1caa1 681fac09 120eca30 7586e1a7
  */
const uint32_t Key[4] =
{
  0x2b7e1516, 0x28aed2a6, 0xabf71588, 0x09cf4f3c
};

/* Initialization vector */
const uint32_t IV[4] =
{
  0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f
};

const uint32_t plainText[16] =
{
  0x6bc1bee2, 0x2e409f96, 0xe93d7e11, 0x7393172a,
  0xae2d8a57, 0x1e03ac9c, 0x9eb76fac, 0x45af8e51,
  0x30c81c46, 0xa35ce411, 0xe5fbc119, 0x1a0a52ef,
  0xf69f2445, 0xdf4f9b17, 0xad2b417b, 0xe66c3710
};

const uint32_t expectedCiphertext[16] =
{
  0x7649abac, 0x8119b246, 0xcee98e9b, 0x12e9197d,
  0x5086cb9b, 0x507219ee, 0x95db113a, 0x917678b2,
  0x73bed6b8, 0xe3c1743b, 0x7116e69e, 0x22229516,
  0x3ff1caa1, 0x681fac09, 0x120eca30, 0x7586e1a7
};
/* Computed data buffers */
uint32_t computedCiphertext[16] = {0};
/* Set to 1 if the output transfer is correctly completed */
uint32_t OutTransferCpltCb;
/* Set to 1 if a transfer error is detected */
uint32_t ErrorCb;
/* Set to 1 if a suspension is detected */
uint32_t SuspendCb;
uint32_t Suspendflag = 0;
/* Private functions prototype -----------------------------------------------*/
/* Functions allowing the user to configure dynamically the AES callbacks instead of weak functions */
static void OutTransfertCpltCallback(hal_aes_handle_t *pAES);
static void ErrorCallback(hal_aes_handle_t *pAES);
static void SuspendCallback(hal_aes_handle_t *pAES);
void HAL_TIM_UpdateCallback(hal_tim_handle_t *pTim);


/** ########## Step 1 ##########
  * Initializes the AES and the timer peripherals.
  * This step is scheduled as follows:
  * Step 1.1: Initializes the AES peripheral.
  * Step 1.2: Initializes the timer peripheral.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1.1 ##########
    * Initializes and configures the AES peripheral.
    */
  pAES = mx_example_aes_init();
  if (pAES == NULL)
  {
    goto _app_init_exit;
  }
  /* Register the user AES suspend Callback*/
  if (HAL_AES_RegisterSuspendCallback(pAES, SuspendCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  /* Register the user AES Output transfer completed callback */
  if (HAL_AES_RegisterOutTransferCpltCallback(pAES, OutTransfertCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  /* Register the user AES error Callback*/
  if (HAL_AES_RegisterErrorCallback(pAES, ErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  PRINTF("[INFO] Step 1.1: Initialization, configuration of the AES peripheral with CBC algorithm COMPLETED.\n");

  /** ########## Step 1.2 ##########
    * Initializes the timer peripheral.
    */
  pTim = mx_example_tim_init();
  if (pTim == NULL)
  {
    goto _app_init_exit;
  }
  /* Register the user timer update Callback*/
  if (HAL_TIM_RegisterUpdateCallback(pTim, HAL_TIM_UpdateCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1.2: Timer initialization COMPLETED.\n");

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Demonstrates how to suspend the AES CBC encryption IT process, using the timer
  * to request suspend encryption, called in HAL_TIM_UpdateCallback, by setting the Suspendflag.
  * Then Resumes the IT encryption process.
  * This step is scheduled as follows:
  * Step 2.1: Starts and Suspends the IT encryption process.
  * Step 2.2: Resumes the IT encryption process.
  * Step 2.3: Verifies if the IT encryption process is completed.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_aes_save_context_t p_aes_context;
  HAL_TIM_Start_IT(pTim);
  /** ########## Step 2.1 ##########
    * Starts and Suspends the IT encryption process.
    */

  Suspendflag = 1;
  /* Start the encryption of plaintext in IT mode */
  if (HAL_AES_Encrypt_IT(pAES, plainText, 64, computedCiphertext) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* Wait for an IT computation process suspension. */
  while (SuspendCb == 0)
  {
    /* Put the CPU in Wait For Interrupt state.*/
    __WFI();
  }

  PRINTF("[INFO] Step 2.1: Suspension of IT encryption process COMPLETED.\n");

  /* Save parameters of the suspended AES encryption */
  HAL_AES_SaveContext(pAES, &p_aes_context);

  /** ########## Step 2.2 ##########
    *  Resumes the IT encryption process.
    */
  /* Restore context*/
  if (HAL_AES_RestoreContext(pAES, &p_aes_context) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* Resume the IT encryption process */
  if (HAL_AES_Resume(pAES) != HAL_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.2: Resumption of IT encryption process COMPLETED.\n");

  /** ########## Step 2.3 ##########
    *  Verifies if the IT encryption process is completed.
    */

  if ((ErrorCb == 1) && (OutTransferCpltCb == 0))
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (memcmp(computedCiphertext, expectedCiphertext, 64) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.3: Verification of computed cipher text as expected COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Deinitializes the AES and the timer peripherals before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_aes_deinit();
  mx_example_tim_deinit();

  PRINTF("[INFO] Step 3: AES and timer deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** Output FIFO transfer completed callback.
  * @user this implementation of the AES Output FIFO transfer completed callback can be customized.
  */
static void OutTransfertCpltCallback(hal_aes_handle_t *pAES)
{
  OutTransferCpltCb = 1U;
}

/** AES error callback
  * @user This implementation of the AES error callback can be customized.
  */
static void ErrorCallback(hal_aes_handle_t *pAES)
{
  ErrorCb = 1U;
}

/** AES suspend callback
  * @user This implementation of the AES suspend callback can be customized.
  */
static void SuspendCallback(hal_aes_handle_t *pAES)
{
  SuspendCb = 1U;
}

/** TIM update callback
  * @user This implementation of the TIM update callback can be customized.
  */
void HAL_TIM_UpdateCallback(hal_tim_handle_t *pTIM)
{
  if (Suspendflag == 1)
  {
    if (HAL_AES_GetState(pAES) == HAL_AES_STATE_ACTIVE)
    {
      HAL_AES_RequestSuspend(pAES);
      Suspendflag = 0;
    }
  }
}
