/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to create a protected ECDSA private key blob using the HAL CCB API.
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
#define ECDSA_CTX_SIZE_BYTE 194
/* Private variables ---------------------------------------------------------*/
hal_ccb_handle_t *pCCB;  /* pointer referencing the CCB handle from the generated code */

hal_ccb_wrapping_sw_key_context_t wrapped_key_conf;
hal_ccb_ecdsa_curve_param_t ecdsa_param;
hal_ccb_ecdsa_key_blob_t ecdsa_key_blob;
hal_ccb_ecdsa_sign_t signature;


uint8_t ecdsa_ctx_raw[194] = {0};


/* Input buffers */
uint32_t SymetricAESKey256[8]         = {0xdecafe00, 0xdecafe01, 0xdecafe02, 0xdecafe03, 0xdecafe04, 0xdecafe05,
                                         0xdecafe06, 0xdecafe07
                                        };

uint32_t AESIV[4]                      = {0x00010203, 0x04050607, 0x08090A0B, 0x0C0D0E0F};

uint32_t WrappedKeyBuffer[8]         = {0x1ff4d7fb, 0xbb515f14, 0x676df6b4, 0xcb58c0c8, 0x2b48e65e, 0xf9bfcf32,
                                        0xd9c0a66c, 0xeec362d7
                                       };

uint8_t ecdsa224_prvt_keyP[28]         = {0xf3, 0x32, 0xb6, 0x8c, 0xa9, 0x5a, 0x3c, 0xfa, 0x4e, 0x99, 0x8e, 0x43,
                                          0x4d, 0x31, 0xbc, 0x10, 0x58, 0x9c, 0x17, 0x47, 0xb7, 0xff, 0x9d, 0xd4,
                                          0x08, 0x9f, 0x79, 0x2b
                                         };

const uint32_t ecdsa224_n_length       = 28;

const uint32_t ecdsa224_p_length       = 28;

const uint32_t ecdsa224_a_sign         = 0x00000001;

const uint8_t ecdsa224_a_abs[28]       = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                          0x00, 0x00, 0x00, 0x03
                                         };

const uint8_t ecdsa224_b[28]           = {0xB4, 0x05, 0x0A, 0x85, 0x0C, 0x04, 0xB3, 0xAB, 0xF5, 0x41, 0x32, 0x56,
                                          0x50, 0x44, 0xB0, 0xB7, 0xD7, 0xBF, 0xD8, 0xBA, 0x27, 0x0B, 0x39, 0x43,
                                          0x23, 0x55, 0xFF, 0xB4
                                         };

const uint8_t ecdsa224_p[28]           = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                          0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                          0x00, 0x00, 0x00, 0x01
                                         };

const uint8_t ecdsa224_n[28]           = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                          0xFF, 0xFF, 0x16, 0xA2, 0xE0, 0xB8, 0xF0, 0x3E, 0x13, 0xDD, 0x29, 0x45,
                                          0x5C, 0x5C, 0x2A, 0x3D
                                         };

const uint8_t ecdsa224_Gx[28]          = {0xB7, 0x0E, 0x0C, 0xBD, 0x6B, 0xB4, 0xBF, 0x7F, 0x32, 0x13, 0x90, 0xB9,
                                          0x4A, 0x03, 0xC1, 0xD3, 0x56, 0xC2, 0x11, 0x22, 0x34, 0x32, 0x80, 0xD6,
                                          0x11, 0x5C, 0x1D, 0x21
                                         };

const uint8_t ecdsa224_Gy[28]          = {0xBD, 0x37, 0x63, 0x88, 0xB5, 0xF7, 0x23, 0xFB, 0x4C, 0x22, 0xDF, 0xE6,
                                          0xCD, 0x43, 0x75, 0xA0, 0x5A, 0x07, 0x47, 0x64, 0x44, 0xD5, 0x81, 0x99,
                                          0x85, 0x00, 0x7E, 0x34
                                         };

/* Output buffers */
uint32_t IvBuffer[4]                  = {0};
uint32_t TagBuffer[4]                 = {0};
uint32_t private_key_encrypted_224[8]   = {0};
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
  *  Performs protected blob creation and signature generation using EDSCA.
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

  if (HAL_CCB_ECDSA_WrapSymmetricKey(pCCB, SymetricAESKey256, &wrapped_key_conf, WrappedKeyBuffer) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: Symmetric Key wrapping COMPLETED.\n");

  /* Prepare ECDSA Out parameters  */
  ecdsa_key_blob.p_iv                        = IvBuffer;
  ecdsa_key_blob.p_tag                       = TagBuffer;
  ecdsa_key_blob.p_wrapped_key               = private_key_encrypted_224;

  /*
   * 2.2  Wrap EDSCA private key.
   */

  ecdsa_param.prime_order_size_byte            = ecdsa224_n_length;
  ecdsa_param.modulus_size_byte                = ecdsa224_p_length;
  ecdsa_param.coef_sign_a                      = ecdsa224_a_sign;
  ecdsa_param.p_abs_coef_a                     = ecdsa224_a_abs;
  ecdsa_param.p_coef_b                         = ecdsa224_b;
  ecdsa_param.p_modulus                        = ecdsa224_p;
  ecdsa_param.p_prime_order                    = ecdsa224_n;
  ecdsa_param.p_point_x                        = ecdsa224_Gx;
  ecdsa_param.p_point_y                        = ecdsa224_Gy;
  ecdsa_param.ecdsa_pool_buffer.p_buff         = ecdsa_ctx_raw;
  ecdsa_param.ecdsa_pool_buffer.buff_size_byte = ECDSA_CTX_SIZE_BYTE;

  if (HAL_CCB_ECDSA_SW_WrapPrivateKey(pCCB, &ecdsa_param, ecdsa224_prvt_keyP, &wrapped_key_conf, WrappedKeyBuffer,
                                      &ecdsa_key_blob) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: EDSCA private key wrapping COMPLETED.\n");

  /*
   * 2.3 Blob creation check for EDSCA
   */
  if ((ecdsa_key_blob.p_iv == 0) || (ecdsa_key_blob.p_tag == 0) || (ecdsa_key_blob.p_wrapped_key == 0)
      || (pCCB->global_state != HAL_CCB_STATE_IDLE))
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.3: Blob creation for EDSCA verification COMPLETED.\n");

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

