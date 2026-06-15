/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to create and validate a protected RSA private key
  *                  blob using the HAL CCB API.
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
/* Private variables ---------------------------------------------------------*/
hal_ccb_handle_t *pCCB;  /* pointer referencing the CCB handle from the generated code */

hal_ccb_wrapping_sw_key_context_t wrapped_key_conf;
hal_ccb_rsa_param_t RSAparam;
hal_ccb_rsa_clear_key_t RSAKey;
hal_ccb_rsa_key_blob_t RSAKeyBlob;

/* Input buffers */
uint32_t Symetric_AESKey256[8] = {0xdecafe00, 0xdecafe01, 0xdecafe02, 0xdecafe03,
                                  0xdecafe04, 0xdecafe05, 0xdecafe06, 0xdecafe07
                                 };
uint32_t AESIV[4] =   {0x00010203, 0x04050607, 0x08090A0B, 0x0C0D0E0F};
/* Input buffers */
uint8_t RSA256_exp_P[32]               = {0xB9, 0xA7, 0x6D, 0x7C, 0xCD, 0x75, 0x7C,
                                          0x71, 0x85, 0x35, 0x00, 0xBD, 0xA5, 0x6D,
                                          0x38, 0xF0, 0xA4, 0x93, 0xAF, 0x20, 0x7B,
                                          0xBD, 0xB3, 0x69, 0x00, 0x27, 0xC3, 0xB6,
                                          0x07, 0x76, 0x49, 0x29
                                         };
uint8_t RSA256_phi_P[32]               = {0xCC, 0x72, 0x3A, 0xA6, 0x14, 0x5B, 0x38,
                                          0x0A, 0x80, 0xC1, 0xD6, 0x6E, 0xAB, 0xFB,
                                          0x1F, 0xA3, 0x36, 0x7B, 0x1E, 0x5B, 0x62,
                                          0xF3, 0x37, 0x62, 0xC8, 0xB0, 0x73, 0x45,
                                          0xDB, 0xC1, 0x98, 0x68
                                         };

uint8_t RSA256_mod[32]                 = {0xCC, 0x72, 0x3A, 0xA6, 0x14, 0x5B, 0x38,
                                          0x0A, 0x80, 0xC1, 0xD6, 0x6E, 0xAB, 0xFB,
                                          0x1F, 0xA5, 0x00, 0xD5, 0xF7, 0xE8, 0x27,
                                          0x66, 0xDE, 0xCC, 0xED, 0x7F, 0xDB, 0x13,
                                          0xB7, 0xC4, 0xFB, 0xAF
                                         };
const uint32_t RSA256_exp_length       = 32;
const uint32_t RSA256_mod_length       = 32;

/* Output buffers */
uint32_t phi_encrypted_256[8]          = {0};
uint32_t exponent_encrypted_256[8]     = {0};

uint32_t Wrapped_key_Buffer[8] = {0}; /* to store symmetric wrapped key after key encryption */
uint32_t IV_Buffer[4] ;              /* to store the AES-GCM IV used for RSA private key wrapping */
uint32_t TAG_Buffer[4];
uint8_t ccb_pool_buffer[1536]   = {0};
/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the CCB instance.
  */
app_status_t app_init(void)
{
  app_status_t return_status;

  pCCB = mx_example_ccb_init();

  if (pCCB != NULL)
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
  *  Performs symmetric key wrapping and protected RSA private key blob
  *  creation.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /*
   * 2.1 Configure and perform symmetric key wrapping
   *    - Sets up AES context and wraps the symmetric key
   */
  wrapped_key_conf.p_init_vect                 = AESIV;
  wrapped_key_conf.aes_algorithm               = HAL_CCB_AES_CBC;
  wrapped_key_conf.key_size                    = KEY_SIZE;

  if (HAL_CCB_RSA_WrapSymmetricKey(pCCB, Symetric_AESKey256, &wrapped_key_conf, Wrapped_key_Buffer) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: Symmetric Key wrapping COMPLETED.\n");

  /*
   * 2.2 Create the protected RSA private key blob
   */
  RSAKey.p_exp                            = RSA256_exp_P;
  RSAKey.p_phi                            = RSA256_phi_P;

  RSAparam.exp_size_byte                   = RSA256_exp_length;
  RSAparam.modulus_size_byte               = RSA256_mod_length;
  RSAparam.p_mod                           = RSA256_mod;
  RSAparam.rsa_pool_buffer.p_buff         = ccb_pool_buffer;
  RSAparam.rsa_pool_buffer.buff_size_byte  = HAL_CCB_RSA_CALC_BUFFER_SIZE(RSA256_mod_length);

  RSAKeyBlob.p_iv                         = IV_Buffer;
  RSAKeyBlob.p_tag                        = TAG_Buffer;
  RSAKeyBlob.p_wrapped_exp                = exponent_encrypted_256;
  RSAKeyBlob.p_wrapped_phi                = phi_encrypted_256;

  if (HAL_CCB_RSA_SW_WrapPrivateKey(pCCB, &RSAparam, &RSAKey, &wrapped_key_conf, Wrapped_key_Buffer,
                                    &RSAKeyBlob) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: RSA private key wrapping COMPLETED.\n");

  /*
   * 2.3 Blob creation check for RSA private key blob
   */
  if ((RSAKeyBlob.p_iv == 0) || (RSAKeyBlob.p_tag == 0) || (RSAKeyBlob.p_wrapped_exp == 0)
      || (RSAKeyBlob.p_wrapped_phi == 0) || (pCCB->global_state != HAL_CCB_STATE_IDLE))
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.3: RSA private key blob creation check COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
}


/** ########## Step 3 ##########
  * Deinitializes the CCB instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{

  mx_example_ccb_deinit();

  PRINTF("[INFO] Step 3: de-init.\n");

  return EXEC_STATUS_OK;
}

