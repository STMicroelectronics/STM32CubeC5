/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to compute ECDSA signature using a protected ECDSA
  *                  private key blob.
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
#define ECDSA_HASH_SIZE_BYTE 24U
/* Private variables ---------------------------------------------------------*/
hal_ccb_handle_t *pCCB;  /* CCB handle */
hal_pka_handle_t *pPKA;  /* PKA handle */

hal_ccb_ecdsa_curve_param_t ecdsa_param;
hal_ccb_ecdsa_key_blob_t ecdsa_key_blob;
hal_ccb_ecdsa_sign_t signature;
hal_ccb_ecc_point_t public_key_out;
hal_pka_ecdsa_verif_config_t InVerif;

/* Input buffers */
uint8_t ecdsa192_prvt_keyP[24] = {0x58, 0x9B, 0x8F, 0x39, 0x62, 0xD2, 0xF7,
                                  0x40, 0x37, 0x8F, 0x68, 0x95, 0xDD, 0x42,
                                  0xD5, 0x41, 0xB6, 0x3E, 0x01, 0x49, 0xD0,
                                  0x81, 0x11, 0x8C
                                 };

const uint32_t ecdsa192_n_length = 24;

const uint32_t ecdsa192_p_length = 24;

const uint32_t ecdsa192_a_sign   = 0x00000001;

const uint8_t ecdsa192_a_abs[24] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x03
                                   };


const uint8_t ecdsa192_b[24]     = {0x64, 0x21, 0x05, 0x19, 0xE5, 0x9C, 0x80,
                                    0xE7, 0x0F, 0xA7, 0xE9, 0xAB, 0x72, 0x24,
                                    0x30, 0x49, 0xFE, 0xB8, 0xDE, 0xEC, 0xC1,
                                    0x46, 0xB9, 0xB1
                                   };

const uint8_t ecdsa192_p[24]     = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                    0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                    0xFF, 0xFF, 0xFF
                                   };

const uint8_t ecdsa192_n[24]     = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x99, 0xDE,
                                    0xF8, 0x36, 0x14, 0x6B, 0xC9, 0xB1, 0xB4,
                                    0xD2, 0x28, 0x31
                                   };

const uint8_t ecdsa192_Gx[24]    = {0x18, 0x8D, 0xA8, 0x0E, 0xB0, 0x30, 0x90,
                                    0xF6, 0x7C, 0xBF, 0x20, 0xEB, 0x43, 0xA1,
                                    0x88, 0x00, 0xF4, 0xFF, 0x0A, 0xFD, 0x82,
                                    0xFF, 0x10, 0x12
                                   };

const uint8_t ecdsa192_Gy[24]    = {0x07, 0x19, 0x2B, 0x95, 0xFF, 0xC8, 0xDA,
                                    0x78, 0x63, 0x10, 0x11, 0xED, 0x6B, 0x24,
                                    0xCD, 0xD5, 0x73, 0xF9, 0x77, 0xA1, 0x1E,
                                    0x79, 0x48, 0x11
                                   };


uint8_t ecdsa192_hash[24]        = {0xA9, 0x80, 0xB6, 0xBF, 0x67, 0x7A, 0xE3,
                                    0x65, 0x27, 0x32, 0xCC, 0x78, 0x58, 0x54,
                                    0x88, 0x9D, 0xCE, 0xE4, 0x69, 0x8F, 0xDF,
                                    0x73, 0x8D, 0x08
                                   };

/* Output buffers */
uint32_t IV_Buffer[4]                 = {0};
uint32_t TAG_Buffer[4]                = {0};
uint32_t private_key_encrypted_192[8] = {0};

uint8_t ECDSASout_192[24]             = {0};
uint8_t ECDSARout_192[24]             = {0};

uint8_t PublicKeyXout_192[24]         = {0};
uint8_t PublicKeyYout_192[24]         = {0};
uint8_t ccb_pool_buffer[1536]         = {0};

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
  *  Performs protected ECDSA blob creation and signature generation.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /*
  * 2.1 Wrap ECDSA private key using hardware key XOR software key.
   */

  /* Fill ECDSA parameters */
  ecdsa_param.prime_order_size_byte             = ecdsa192_n_length;
  ecdsa_param.modulus_size_byte                 = ecdsa192_p_length;
  ecdsa_param.coef_sign_a                       = ecdsa192_a_sign;
  ecdsa_param.p_abs_coef_a                      = ecdsa192_a_abs;
  ecdsa_param.p_coef_b                          = ecdsa192_b;
  ecdsa_param.p_modulus                         = ecdsa192_p;
  ecdsa_param.p_prime_order                     = ecdsa192_n;
  ecdsa_param.p_point_x                         = ecdsa192_Gx;
  ecdsa_param.p_point_y                         = ecdsa192_Gy;
  ecdsa_param.ecdsa_pool_buffer.p_buff          = ccb_pool_buffer;
  ecdsa_param.ecdsa_pool_buffer.buff_size_byte  = HAL_CCB_ECDSA_CALC_BUFFER_SIZE(ecdsa192_p_length);

  /* Prepare ECDSA Out parameters  */
  ecdsa_key_blob.p_wrapped_key                   = private_key_encrypted_192;
  ecdsa_key_blob.p_iv                            = IV_Buffer;
  ecdsa_key_blob.p_tag                           = TAG_Buffer;

  if (HAL_CCB_ECDSA_HW_WrapPrivateKey(pCCB, &ecdsa_param, ecdsa192_prvt_keyP,
                                      HAL_CCB_KEY_HSW, &ecdsa_key_blob) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: ECDSA private key wrapping COMPLETED.\n");

  /*
   * 2.2 Sign hash using wrapped private key blob
   */
  signature.p_s_sign = ECDSASout_192;
  signature.p_r_sign = ECDSARout_192;

  if (HAL_CCB_ECDSA_HW_Sign(pCCB, &ecdsa_param, HAL_CCB_KEY_HSW, &ecdsa_key_blob,
                            ecdsa192_hash, ECDSA_HASH_SIZE_BYTE, &signature) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: ECDSA signature generation COMPLETED.\n");

  /*
   * 2.3 Use wrapped key blob for public key computation
   */
  public_key_out.p_point_x = PublicKeyXout_192;
  public_key_out.p_point_y = PublicKeyYout_192;

  if (HAL_CCB_ECDSA_HW_ComputePublicKey(pCCB, &ecdsa_param, HAL_CCB_KEY_HSW,
                                        &ecdsa_key_blob, &public_key_out) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.3: ECDSA public key computation COMPLETED.\n");

  /** ########## Step 3 ##########
    * Performs unprotected PKA signature verification.
    */

  /* Initialize PKA instance */
  pPKA = mx_example_pka_init();

  /**
    * 3.1 Configure input parameters for PKA signature verification
    */
  InVerif.prime_order_size_byte   = ecdsa_param.prime_order_size_byte;
  InVerif.modulus_size_byte       = ecdsa_param.modulus_size_byte;
  InVerif.coeff_sign              = ecdsa_param.coef_sign_a;
  InVerif.p_coeff                 = ecdsa_param.p_abs_coef_a;
  InVerif.p_modulus               = ecdsa_param.p_modulus;
  InVerif.p_base_pt_x             = ecdsa_param.p_point_x;
  InVerif.p_base_pt_y             = ecdsa_param.p_point_y;
  InVerif.p_prime_order           = ecdsa_param.p_prime_order;
  InVerif.p_pub_key_curve_pt_x    = public_key_out.p_point_x;
  InVerif.p_pub_key_curve_pt_y    = public_key_out.p_point_y;
  InVerif.p_r_sign                = signature.p_r_sign;
  InVerif.p_s_sign                = signature.p_s_sign;
  InVerif.p_hash                  = ecdsa192_hash;

  if (HAL_PKA_ECDSA_SetConfigVerifSignature(pPKA, &InVerif) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3.1: PKA configuration to process message verification COMPLETED.\n");

  /**
    * 3.2 Run PKA signature verification computation
    */
  if ((HAL_PKA_Compute(pPKA, 5000) != HAL_OK))
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3.2: PKA Signature verification computation COMPLETED.\n");

  /**
    * 3.3 Validate the signature verification result
    */
  if (HAL_PKA_ECDSA_IsValidVerifSignature(pPKA) != PKA_ECDSA_SIGNATURE_VALID)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3.3: Signature verification COMPLETED.\n");


  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
}


/** ########## Step 4 ##########
  * Deinitializes the CCB instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{

  mx_example_ccb_deinit();

  PRINTF("[INFO] Step 4: de-init.\n");

  return EXEC_STATUS_OK;
}
