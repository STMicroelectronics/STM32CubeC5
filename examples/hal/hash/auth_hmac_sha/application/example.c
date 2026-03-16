/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to use the STM32 HAL API to generate and to verify
  *                  a message authentication code (MAC) of a plaintext
  *                  using the HMAC SHA256 algorithm.
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
#define HASH_TIMEOUT_MS 1000
#define MESSAGE_SIZE 128
#define OUTPUT_BUFFER_SIZE 32
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_hash_handle_t *pHASH;

/** Extract from HMAC.rsp
  *
  * Klen = 40
  * Tlen = 32
  * Key = cfd4a44910c9e567507abb6cede4fe601a7a2765c9755aa2cf6ba4814223811a26a8a1ef499cebd9
  * Msg = 3fb301cb4092f9623aa5ffd690d22d65d56e5a1c330b9c4a0d910c34e391c90a76d5401a2d3caa44b8c5d5aef3e928b
  * 90d2ee233e9f9a2cec4a32cd019d06a0dc1fcb1125f5746a4fbd32169ed7bf0e4fd065fa7c8ac97c366380484495f5c5b6850
  * dd1c9d8cd6694cf8686e46308ed0ed1f5bdf98cd831339771db63de5a7de
  * Mac = 20153bf8ea2953c48251ebcc4161f8b6e28499e5c76c24014cff4a9e2f62d25c
  */

uint8_t Key[] =
{
  0xcf, 0xd4, 0xa4, 0x49, 0x10, 0xc9, 0xe5, 0x67, 0x50, 0x7a, 0xbb, 0x6c, 0xed, 0xe4, 0xfe, 0x60,
  0x1a, 0x7a, 0x27, 0x65, 0xc9, 0x75, 0x5a, 0xa2, 0xcf, 0x6b, 0xa4, 0x81, 0x42, 0x23, 0x81, 0x1a,
  0x26, 0xa8, 0xa1, 0xef, 0x49, 0x9c, 0xeb, 0xd9
};
/* Known plaintext to be signed and verified */
const uint8_t Message[] =
{
  0x3f, 0xb3, 0x01, 0xcb, 0x40, 0x92, 0xf9, 0x62, 0x3a, 0xa5, 0xff, 0xd6, 0x90, 0xd2, 0x2d, 0x65,
  0xd5, 0x6e, 0x5a, 0x1c, 0x33, 0x0b, 0x9c, 0x4a, 0x0d, 0x91, 0x0c, 0x34, 0xe3, 0x91, 0xc9, 0x0a,
  0x76, 0xd5, 0x40, 0x1a, 0x2d, 0x3c, 0xaa, 0x44, 0xb8, 0xc5, 0xd5, 0xae, 0xf3, 0xe9, 0x28, 0xb9,
  0x0d, 0x2e, 0xe2, 0x33, 0xe9, 0xf9, 0xa2, 0xce, 0xc4, 0xa3, 0x2c, 0xd0, 0x19, 0xd0, 0x6a, 0x0d,
  0xc1, 0xfc, 0xb1, 0x12, 0x5f, 0x57, 0x46, 0xa4, 0xfb, 0xd3, 0x21, 0x69, 0xed, 0x7b, 0xf0, 0xe4,
  0xfd, 0x06, 0x5f, 0xa7, 0xc8, 0xac, 0x97, 0xc3, 0x66, 0x38, 0x04, 0x84, 0x49, 0x5f, 0x5c, 0x5b,
  0x68, 0x50, 0xdd, 0x1c, 0x9d, 0x8c, 0xd6, 0x69, 0x4c, 0xf8, 0x68, 0x6e, 0x46, 0x30, 0x8e, 0xd0,
  0xed, 0x1f, 0x5b, 0xdf, 0x98, 0xcd, 0x83, 0x13, 0x39, 0x77, 0x1d, 0xb6, 0x3d, 0xe5, 0xa7, 0xde
};
const uint8_t Expected_Tag[] =
{
  0x20, 0x15, 0x3b, 0xf8, 0xea, 0x29, 0x53, 0xc4, 0x82, 0x51, 0xeb, 0xcc, 0x41, 0x61, 0xf8, 0xb6,
  0xe2, 0x84, 0x99, 0xe5, 0xc7, 0x6c, 0x24, 0x01, 0x4c, 0xff, 0x4a, 0x9e, 0x2f, 0x62, 0xd2, 0x5c
};

/* Computed HASH message */
uint8_t Sha256Hmacoutput[OUTPUT_BUFFER_SIZE] = {0};
uint32_t ComputedSize;

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * Initializes the HASH instance.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pHASH = mx_example_hash_init();
  if (pHASH != NULL)
  {
    PRINTF("[INFO] Step 1: HASH initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }
  else
  {
    PRINTF("[ERROR] Step 1: HASH initialization ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
  }

  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Generates and verifies a message authentication code (MAC) of a plaintext
  * using the HMAC-SHA256 algorithm.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Compute the message */
  if (HAL_HASH_HMAC_Compute(pHASH,
                            Message,
                            MESSAGE_SIZE,
                            Sha256Hmacoutput,
                            OUTPUT_BUFFER_SIZE,
                            &ComputedSize,
                            HASH_TIMEOUT_MS) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Verify the computed hash message */
  if (memcmp(Sha256Hmacoutput, Expected_Tag, 32) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2: MAC generation and verification using SHA256 algorithm COMPLETED.\n");


  /*Clears all data */
  memset((void *)Sha256Hmacoutput, 0U, sizeof(Sha256Hmacoutput));

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Deinitializes the HASH peripheral before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_hash_deinit();

  PRINTF("[INFO] Step 3: HASH peripheral deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */
