/**
  ******************************************************************************
  * @file    pka/modular_exponentiation_crt/application/rsa_2048/rsa_pub_2048.h
  * @brief   This file contains the headers of rsa_pub_2048.c .
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
#ifndef __RSA_PUB_2048_H
#define __RSA_PUB_2048_H

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t  RSA_Pub2048Modulus[];
extern const uint8_t  RSA_Pub2048PublicExponent[];
extern const uint32_t RSA_Pub2048PublicExponentLen;

#ifdef __cplusplus
}
#endif

#endif /* __RSA_PUB_2048_H */
