/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to compute public key from a protected ECDSA private key blob.
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
/* Private macro -------------------------------------------------------------*/
#define KEY_SIZE 256U
/* Private variables ---------------------------------------------------------*/
hal_ccb_handle_t *pCCB; /* pointer referencing the CCB handle from the generated code */

hal_ccb_wrapping_sw_key_context_t wrapped_key_conf;
hal_ccb_ecdsa_curve_param_t ecdsa_param;
hal_ccb_ecdsa_key_blob_t ecdsa_key_blob;
hal_ccb_ecdsa_sign_t signature;
hal_ccb_ecc_point_t public_key_out;


/* Input buffers */
uint32_t Symetric_AESKey256[8] = {0xdecafe00, 0xdecafe01, 0xdecafe02,
                                  0xdecafe03, 0xdecafe04, 0xdecafe05,
                                  0xdecafe06, 0xdecafe07
                                 };

/* Output buffers */
uint32_t Wrapped_key_Buffer[8] = {0};
uint32_t IV_Buffer[4];
uint32_t TAG_Buffer[4];
uint8_t ccb_pool_buffer[1536]   = {0};


/* Input buffers for ECDSA 256-bit */

uint8_t ecdsa256_prvt_keyP[32] = {0x51, 0x9b, 0x42, 0x3d, 0x71, 0x5f, 0x8b,
                                  0x58, 0x1f, 0x4f, 0xa8, 0xee, 0x59, 0xf4,
                                  0x77, 0x1a, 0x5b, 0x44, 0xc8, 0x13, 0x0b,
                                  0x4e, 0x3e, 0xac, 0xca, 0x54, 0xa5, 0x6d,
                                  0xda, 0x72, 0xb4, 0x64
                                 };

const uint32_t ecdsa256_n_length = 32;

const uint32_t ecdsa256_p_length = 32;

const uint32_t ecdsa256_a_sign   = 0x00000001;

const uint8_t ecdsa256_a_abs[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x03
                                   };

const uint8_t ecdsa256_b[32]     = {0x5a, 0xc6, 0x35, 0xd8, 0xaa, 0x3a, 0x93,
                                    0xe7, 0xb3, 0xeb, 0xbd, 0x55, 0x76, 0x98,
                                    0x86, 0xbc, 0x65, 0x1d, 0x06, 0xb0, 0xcc,
                                    0x53, 0xb0, 0xf6, 0x3b, 0xce, 0x3c, 0x3e,
                                    0x27, 0xd2, 0x60, 0x4b
                                   };

const uint8_t ecdsa256_p[32]     = {0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
                                    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
                                    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                    0xff, 0xff, 0xff, 0xff
                                   };

const uint8_t ecdsa256_n[32]     = {0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
                                    0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                    0xff, 0xff, 0xbc, 0xe6, 0xfa, 0xad, 0xa7,
                                    0x17, 0x9e, 0x84, 0xf3, 0xb9, 0xca, 0xc2,
                                    0xfc, 0x63, 0x25, 0x51
                                   };

const uint8_t ecdsa256_Gx[32]    = {0x6b, 0x17, 0xd1, 0xf2, 0xe1, 0x2c, 0x42,
                                    0x47, 0xf8, 0xbc, 0xe6, 0xe5, 0x63, 0xa4,
                                    0x40, 0xf2, 0x77, 0x03, 0x7d, 0x81, 0x2d,
                                    0xeb, 0x33, 0xa0, 0xf4, 0xa1, 0x39, 0x45,
                                    0xd8, 0x98, 0xc2, 0x96
                                   };

const uint8_t ecdsa256_Gy[32]    = {0x4f, 0xe3, 0x42, 0xe2, 0xfe, 0x1a, 0x7f,
                                    0x9b, 0x8e, 0xe7, 0xeb, 0x4a, 0x7c, 0x0f,
                                    0x9e, 0x16, 0x2b, 0xce, 0x33, 0x57, 0x6b,
                                    0x31, 0x5e, 0xce, 0xcb, 0xb6, 0x40, 0x68,
                                    0x37, 0xbf, 0x51, 0xf5
                                   };

uint8_t expected_Qx[32]          = {0x1c, 0xcb, 0xe9, 0x1c, 0x07, 0x5f, 0xc7, 0xf4,
                                    0xf0, 0x33, 0xbf, 0xa2, 0x48, 0xdb, 0x8f, 0xcc,
                                    0xd3, 0x56, 0x5d, 0xe9, 0x4b, 0xbf, 0xb1, 0x2f,
                                    0x3c, 0x59, 0xff, 0x46, 0xc2, 0x71, 0xbf, 0x83
                                   };

uint8_t expected_Qy[32]          = {0xce, 0x40, 0x14, 0xc6, 0x88, 0x11, 0xf9, 0xa2,
                                    0x1a, 0x1f, 0xdb, 0x2c, 0x0e, 0x61, 0x13, 0xe0,
                                    0x6d, 0xb7, 0xca, 0x93, 0xb7, 0x40, 0x4e, 0x78,
                                    0xdc, 0x7c, 0xcd, 0x5c, 0xa8, 0x9a, 0x4c, 0xa9
                                   };

/* Output buffers */
uint32_t IvBuffer[4]                  = {0};
uint32_t TagBuffer[4]                 = {0};
uint32_t private_key_encrypted_256[8] = {0};

uint8_t PublicKeyXout_256[32]         = {0};
uint8_t PublicKeyYout_256[32]         = {0};
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
  *  Performs protected ECDSA blob creation and public key computation.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /*
   * 2.1 Configure and perform private key wrapping using an AES symmetric Hardware Key.
   *    - Sets up AES context and wraps the private key
   */

  ecdsa_param.prime_order_size_byte            = ecdsa256_n_length;
  ecdsa_param.modulus_size_byte                = ecdsa256_p_length;
  ecdsa_param.coef_sign_a                      = ecdsa256_a_sign;
  ecdsa_param.p_abs_coef_a                     = ecdsa256_a_abs;
  ecdsa_param.p_coef_b                         = ecdsa256_b;
  ecdsa_param.p_modulus                        = ecdsa256_p;
  ecdsa_param.p_prime_order                    = ecdsa256_n;
  ecdsa_param.p_point_x                        = ecdsa256_Gx;
  ecdsa_param.p_point_y                        = ecdsa256_Gy;
  ecdsa_param.ecdsa_pool_buffer.p_buff         = ccb_pool_buffer;
  ecdsa_param.ecdsa_pool_buffer.buff_size_byte = HAL_CCB_ECDSA_CALC_BUFFER_SIZE(ecdsa256_p_length);

  ecdsa_key_blob.p_iv                          = IV_Buffer;
  ecdsa_key_blob.p_tag                         = TAG_Buffer;
  ecdsa_key_blob.p_wrapped_key                 = private_key_encrypted_256;


  if (HAL_CCB_ECDSA_HW_WrapPrivateKey(pCCB, &ecdsa_param, ecdsa256_prvt_keyP,
                                      HAL_CCB_KEY_HW, &ecdsa_key_blob) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: ECDSA private key wrapping COMPLETED.\n");

  /*
   * 2.2 Configure and perform public key computation using the wrapped private key.
   *    - Sets up the public key output structure and computes the public key
   */
  public_key_out.p_point_x = PublicKeyXout_256;
  public_key_out.p_point_y = PublicKeyYout_256;

  if (HAL_CCB_ECDSA_HW_ComputePublicKey(pCCB, &ecdsa_param, HAL_CCB_KEY_HW,
                                        &ecdsa_key_blob, &public_key_out) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: ECDSA public key computation COMPLETED.\n");

  /*
   * 2.3 Perform public key verification using the expected values.
   *    - Compares the computed public key with the expected values
   */
  if ((memcmp(public_key_out.p_point_x, expected_Qx, sizeof(expected_Qx)) != 0)
      || (memcmp(public_key_out.p_point_y, expected_Qy, sizeof(expected_Qy)) != 0))
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.3: ECDSA public key check COMPLETED.\n");


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

