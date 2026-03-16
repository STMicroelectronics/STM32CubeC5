/**
  ******************************************************************************
  * file           : example.c
  * brief          : example program body
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
/* Private macro -------------------------------------------------------------*/
#define KEY_SIZE 256U
#define ECC_CTX_SIZE_BYTE 96U
/* Private variables ---------------------------------------------------------*/
hal_ccb_handle_t *pCBB;  /* pointer referencing the CBB handle from the generated code */

hal_ccb_wrapping_sw_key_context_t wrapped_key_conf;
hal_ccb_ecc_mul_curve_param_t ecc_param;
hal_ccb_ecc_mul_key_blob_t ecc_key_blob;
hal_ccb_ecc_point_t point_in;
hal_ccb_ecc_point_t point_out;

/* Input buffers */
uint32_t Symetric_AESKey256[8] = {0xdecafe00, 0xdecafe01, 0xdecafe02,
                                  0xdecafe03, 0xdecafe04, 0xdecafe05,
                                  0xdecafe06, 0xdecafe07
                                 };
uint32_t AESIV[4] = {0x00010203, 0x04050607, 0x08090A0B, 0x0C0D0E0F};

uint8_t eccMul192_Sclr_k_P[24] = {0xD5, 0x6E, 0x0A, 0x82, 0xAF, 0x91, 0xEE,
                                  0xDD, 0x62, 0x82, 0x00, 0x3B, 0x02, 0x55,
                                  0x38, 0x35, 0x1F, 0x9B, 0x90, 0xB4, 0x00,
                                  0x81, 0x2A, 0x43
                                 };

const uint32_t eccMul192_n_length = 24;

const uint32_t eccMul192_p_length = 24;

const uint32_t eccMul192_a_sign = 0x00000001;

const uint8_t eccMul192_a_abs[24] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                     0x00, 0x00, 0x03
                                    };

const uint8_t eccMul192_b[24] = {0x64, 0x21, 0x05, 0x19, 0xE5, 0x9C, 0x80, 0xE7,
                                 0x0F, 0xA7, 0xE9, 0xAB, 0x72, 0x24, 0x30, 0x49,
                                 0xFE, 0xB8, 0xDE, 0xEC, 0xC1, 0x46, 0xB9, 0xB1
                                };

const uint8_t eccMul192_p[24] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
                                };

const uint8_t eccMul192_n[24] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                 0xFF, 0xFF, 0xFF, 0xFF, 0x99, 0xDE, 0xF8, 0x36,
                                 0x14, 0x6B, 0xC9, 0xB1, 0xB4, 0xD2, 0x28, 0x31
                                };

uint8_t eccMul192_Custumized_x[24] = {0xEA, 0x31, 0x34, 0xD4, 0xCC, 0x80, 0xF0,
                                      0xEA, 0xAB, 0xEB, 0x1B, 0x08, 0x70, 0x12,
                                      0x52, 0x26, 0x36, 0x0F, 0x08, 0xD7, 0xA9,
                                      0x4B, 0xF6, 0x19
                                     };

uint8_t eccMul192_Custumized_y[24] = {0x49, 0x7B, 0x27, 0x0A, 0x26, 0xEE, 0xC8,
                                      0xBB, 0x6E, 0x58, 0x40, 0x9D, 0x8E, 0x8A,
                                      0xDD, 0x09, 0x03, 0xD3, 0x30, 0x1D, 0x04,
                                      0x0D, 0x3A, 0x03
                                     };
uint8_t ecc_ctx_raw[96] = {0};

/* Output buffers */
uint32_t Wrapped_key_Buffer[8] = {0};
uint32_t IV_Buffer[4]  = {0};
uint32_t TAG_Buffer[4] = {0};

uint32_t SclrK_encrypted_192[8]    = {0};


/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the CBB instance.
  */
app_status_t app_init(void)
{
  app_status_t return_status;

  pCBB = mx_example_ccb_init();

  if (pCBB != NULL)
  {
    PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }
  else
  {
    PRINTF("[ERROR] Step 1: Device initialization ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
  }

  return return_status;
}


/** ########## Step 2 ##########
  *  Performs protected blob creation for ECC scalar multiplication.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /*
   * 2.1 Configure and perform symmetric key wrapping
   *    - Sets up AES context and wraps the symmetric key
   */
  wrapped_key_conf.p_init_vect   = AESIV;
  wrapped_key_conf.aes_algorithm = HAL_CCB_AES_CBC;
  wrapped_key_conf.key_size      = KEY_SIZE;

  if (HAL_CCB_ECC_WrapSymmetricKey(pCBB, Symetric_AESKey256, &wrapped_key_conf, Wrapped_key_Buffer) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: Symmetric key wrapping COMPLETED.\n");

  /*
   * 2.2 Perform ECC private key wrapping for scalar multiplication
   */
  ecc_param.prime_order_size_byte          = eccMul192_n_length;
  ecc_param.modulus_size_byte              = eccMul192_p_length;
  ecc_param.coef_sign_a                    = eccMul192_a_sign;
  ecc_param.p_abs_coef_a                   = eccMul192_a_abs;
  ecc_param.p_coef_b                       = eccMul192_b;
  ecc_param.p_modulus                      = eccMul192_p;
  ecc_param.p_prime_order                  = eccMul192_n;
  ecc_param.p_point_x                      = eccMul192_Custumized_x;
  ecc_param.p_point_y                      = eccMul192_Custumized_y;
  ecc_param.ecc_pool_buffer.p_buff         = ecc_ctx_raw;
  ecc_param.ecc_pool_buffer.buff_size_byte = ECC_CTX_SIZE_BYTE;

  /* Prepare ECC output parameters */
  ecc_key_blob.p_iv          = IV_Buffer;
  ecc_key_blob.p_tag         = TAG_Buffer;
  ecc_key_blob.p_wrapped_key = SclrK_encrypted_192;

  if (HAL_CCB_ECC_SW_WrapPrivateKey(pCBB, &ecc_param, eccMul192_Sclr_k_P, &wrapped_key_conf, Wrapped_key_Buffer,
                                    &ecc_key_blob) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: ECC private key wrapping COMPLETED.\n");


  /*
   * 2.3 Blob creation check for ECC scalar multiplication
   */
  if ((ecc_key_blob.p_iv == 0) || (ecc_key_blob.p_tag == 0) || (ecc_key_blob.p_wrapped_key == 0)
      || (pCBB->global_state != HAL_CCB_STATE_IDLE))
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.3: Blob creation for ECC scalar multiplication verification COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
}


/** ########## Step 3 ##########
  * Deinitializes the CBB instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{

  mx_example_ccb_deinit();

  PRINTF("[INFO] Step 3: de-init. \n");

  return EXEC_STATUS_OK;
}

