/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to use the STM32 HAL API to suspend and to resume
  *                  a computation process in the interrupt mode.
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
#define MESSAGE_BUFFER_SIZE 32
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_hash_handle_t *pHASH;
hal_tim_handle_t *pTim;

/** Extract from NIST Publication: SHA Test Vectors for Hashing Byte-Oriented Messages SHA256LongMsg.rsp
  *
  * Len = 1304
  * Msg = 451101250ec6f26652249d59dc974b7361d571a8101cdfd36aba3b5854d3ae0
  * 86b5fdd4597721b66e3c0dc5d8c606d9657d0e323283a5217d1f53f2f284f57b85c8a
  * 61ac8924711f895c5ed90ef17745ed2d728abd22a5f7a13479a462d71b56c19a74a40
  * b655c58edfe0a188ad2cf46cbf30524f65d423c837dd1ff2bf462ac4198007345bb44
  * dbb7b1c861298cdf61982a833afc728fae1eda2f87aa2c9480858bec
  * MD = 3c593aa539fdcdae516cdf2f15000f6634185c88f505b39775fb9ab137a10aa2
  *
  * Extract from NIST Publication: SHA Test Vectors for Hashing Byte-Oriented Messages SHA256ShortMsg.rsp
  * Len = 40
  * Msg = c299209682
  * MD = f0887fe961c9cd3beab957e8222494abb969b1ce4c6557976df8b0f6d20e9166
  */

/* Message to be computed */
uint8_t Message_proc1[163] =
{
  0x45, 0x11, 0x01, 0x25, 0x0e, 0xc6, 0xf2, 0x66, 0x52, 0x24, 0x9d, 0x59, 0xdc, 0x97, 0x4b, 0x73,
  0x61, 0xd5, 0x71, 0xa8, 0x10, 0x1c, 0xdf, 0xd3, 0x6a, 0xba, 0x3b, 0x58, 0x54, 0xd3, 0xae, 0x08,
  0x6b, 0x5f, 0xdd, 0x45, 0x97, 0x72, 0x1b, 0x66, 0xe3, 0xc0, 0xdc, 0x5d, 0x8c, 0x60, 0x6d, 0x96,
  0x57, 0xd0, 0xe3, 0x23, 0x28, 0x3a, 0x52, 0x17, 0xd1, 0xf5, 0x3f, 0x2f, 0x28, 0x4f, 0x57, 0xb8,
  0x5c, 0x8a, 0x61, 0xac, 0x89, 0x24, 0x71, 0x1f, 0x89, 0x5c, 0x5e, 0xd9, 0x0e, 0xf1, 0x77, 0x45,
  0xed, 0x2d, 0x72, 0x8a, 0xbd, 0x22, 0xa5, 0xf7, 0xa1, 0x34, 0x79, 0xa4, 0x62, 0xd7, 0x1b, 0x56,
  0xc1, 0x9a, 0x74, 0xa4, 0x0b, 0x65, 0x5c, 0x58, 0xed, 0xfe, 0x0a, 0x18, 0x8a, 0xd2, 0xcf, 0x46,
  0xcb, 0xf3, 0x05, 0x24, 0xf6, 0x5d, 0x42, 0x3c, 0x83, 0x7d, 0xd1, 0xff, 0x2b, 0xf4, 0x62, 0xac,
  0x41, 0x98, 0x00, 0x73, 0x45, 0xbb, 0x44, 0xdb, 0xb7, 0xb1, 0xc8, 0x61, 0x29, 0x8c, 0xdf, 0x61,
  0x98, 0x2a, 0x83, 0x3a, 0xfc, 0x72, 0x8f, 0xae, 0x1e, 0xda, 0x2f, 0x87, 0xaa, 0x2c, 0x94, 0x80,
  0x85, 0x8b, 0xec
};
/* The expected message of the compute process */
uint8_t ExpectedHash_proc1[MESSAGE_BUFFER_SIZE] =
{
  0x3c, 0x59, 0x3a, 0xa5, 0x39, 0xfd, 0xcd, 0xae, 0x51, 0x6c, 0xdf, 0x2f, 0x15, 0x00, 0x0f, 0x66,
  0x34, 0x18, 0x5c, 0x88, 0xf5, 0x05, 0xb3, 0x97, 0x75, 0xfb, 0x9a, 0xb1, 0x37, 0xa1, 0x0a, 0xa2
};
/* Message to be computed */
uint8_t MsgSHA256ShortMsg_proc2[5] =
{
  0xc2, 0x99, 0x20, 0x96, 0x82
};
/* Expected computed message */
uint8_t expectedSHA256_proc2  [MESSAGE_BUFFER_SIZE] =
{
  0xf0, 0x88, 0x7f, 0xe9, 0x61, 0xc9, 0xcd, 0x3b, 0xea, 0xb9, 0x57, 0xe8, 0x22, 0x24, 0x94, 0xab,
  0xb9, 0x69, 0xb1, 0xce, 0x4c, 0x65, 0x57, 0x97, 0x6d, 0xf8, 0xb0, 0xf6, 0xd2, 0x0e, 0x91, 0x66
};
/* Computed HASH message */
uint8_t computed_message_proc1[MESSAGE_BUFFER_SIZE] = {0};
uint32_t ComputedSize_proc1;
/* Computed HASH message */
uint8_t Sha256output_proc2[MESSAGE_BUFFER_SIZE] = {0};
uint32_t shaoutput_size_proc2;
/* Set to 1 if the computation is correctly completed*/
uint32_t ComputeCpltCb;
/* Set to 1 if the computation error is detected */
uint32_t ComputeErrorCb;
/* Set to 1 if the suspension is occurred */
uint32_t SuspendCb;
uint32_t Suspendflag = 0;
/* Private functions prototype -----------------------------------------------*/
static void DigestCpltCallback(hal_hash_handle_t *pHASH);
static void ErrorCallback(hal_hash_handle_t *pHASH);
void HAL_TIM_UpdateCallback(hal_tim_handle_t *pTim);
/** ########## Step 1 ##########
  * Initializes the HASH and the timer peripherals.
  * This step is scheduled as follows:
  * Step 1.1: Initializes the HASH peripheral.
  * Step 1.2: Initializes and starts the Timer peripheral.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1.1 ##########
    * Initializes the HASH peripheral.
    */
  pHASH = mx_example_hash_init();

  if (pHASH == NULL)
  {
    goto _app_init_exit;
  }

  /* Register the user hash digest Callback*/
  if (HAL_HASH_RegisterDigestComputationCpltCallback(pHASH, DigestCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  /* Register the user hash error Callback*/
  if (HAL_HASH_RegisterErrorCpltCallback(pHASH, ErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1.1: HASH initialization COMPLETED.\n");

  /** ########## Step 1.2 ##########
    * Initializes and starts the Timer peripheral.
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

  if (HAL_TIM_Start_IT(pTim) != HAL_OK)
  {
    goto _app_init_exit;
  }
  PRINTF("[INFO] Step 1.2: Timer initialization and start COMPLETED.\n");

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Demonstrates how to suspend a HASH computation process in the interrupt mode,
  * using the timer to request suspend computation called in HAL_TIM_UpdateCallback,
  * by setting the Suspendflag.
  * This step is scheduled as follows:
  * Step 2.1: Suspends IT computation process.
  * Step 2.2: Computes and verifies the second message in polling mode.
  * Step 2.3: Resumes the computation process in interrupt mode.
  * Step 2.4: Verifies a message in interrupt mode.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_hash_suspended_context_t p_hash_context;

  /** ########## Step 2.1 ##########
    * Suspends IT computation process.
    */

  Suspendflag = 1;
  if (HAL_HASH_Compute_IT(pHASH,
                          Message_proc1, 163,
                          computed_message_proc1, MESSAGE_BUFFER_SIZE, &ComputedSize_proc1) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Save the context */
  HAL_HASH_SaveContext(pHASH, &p_hash_context);

  PRINTF("[INFO] Step 2.1: Suspension of computation IT process COMPLETED.\n");

  /** ########## Step 2.2 ##########
    * Computes and verifies the second message in polling mode.
    */

  /* Compute the short message */
  if (HAL_HASH_Compute(pHASH,
                       MsgSHA256ShortMsg_proc2, 5,
                       Sha256output_proc2, MESSAGE_BUFFER_SIZE, &shaoutput_size_proc2,
                       1) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /* Verify the computed hash message */
  if (memcmp(Sha256output_proc2, expectedSHA256_proc2, 32) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: Computation and verification of the second message in polling mode COMPLETED.\n");

  /* Clear all data */
  memset((void *)Sha256output_proc2, 0U, sizeof(Sha256output_proc2));

  /** ########## Step 2.3 ##########
    * Resumes and verifies the computation process in interrupt mode.
    */
  /* Restore the context */
  HAL_HASH_RestoreContext(pHASH, &p_hash_context);

  /* Resume the computation process */
  if (HAL_HASH_ResumeComputation(pHASH) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Wait for one of these HASH interrupts: compute complete or compute error.*/
  while ((ComputeCpltCb == 0) && (ComputeErrorCb == 0))
  {
    /* Put the CPU in Wait For Interrupt state.*/
    __WFI();
  }
  if (ComputeErrorCb == 1)
  {
    goto _app_process_exit;
  }

  /* Verify the computed hash message */
  if (memcmp(computed_message_proc1, ExpectedHash_proc1, 32) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.3: Resumption and verification of HASH computation process COMPLETED.\n");

  /* Clear all data */
  memset((void *)computed_message_proc1, 0U, sizeof(computed_message_proc1));

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Deinitializes  HASH and timer peripherals before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_hash_deinit();
  mx_example_tim_deinit();

  PRINTF("[INFO] Step 3: HASH and timer peripherals deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */


/* Digest computation complete call back.*/
static void DigestCpltCallback(hal_hash_handle_t *pHASH)
{
  ComputeCpltCb = 1U;
}
/* HASH error callback.*/
static void ErrorCallback(hal_hash_handle_t *pHASH)
{
  ComputeErrorCb = 1U;
}

/** Update callback.
  * brief Function called when the timer update interrupt is generated.
  */

void HAL_TIM_UpdateCallback(hal_tim_handle_t *pTIM)
{
  if (Suspendflag == 1)
  {
    HAL_HASH_RequestSuspendComputation(pHASH);
    Suspendflag = 0;
  }
}