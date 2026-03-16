/**
  ******************************************************************************
  * @file    pka/modular_exponentiation_crt/application/rsa_2048/rsa_priv_2048.h
  * @brief   This file contains the headers of rsa_priv_2048.c .
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RSA_PRIV_2048_H
#define __RSA_PRIV_2048_H

#ifdef __cplusplus
extern "C" {
#endif

#define RSA_PRIV_2048_PRIVATEEXPONENT_LEN  256
extern const uint8_t  RSA_Priv2048Modulus[];
extern const uint8_t  RSA_Priv2048PrivateExponent[];
extern const uint8_t  RSA_Priv2048Prime1[];
extern const uint8_t  RSA_Priv2048Prime2[];
extern const uint8_t  RSA_Priv2048Exponent1[];
extern const uint8_t  RSA_Priv2048Exponent2[];
extern const uint8_t  RSA_Priv2048Coefficient[];

#ifdef __cplusplus
}
#endif

#endif /* __RSA_PRIV_2048_H */
