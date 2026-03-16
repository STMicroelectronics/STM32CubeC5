/**
  ******************************************************************************
  * @file    pka/check_point_ecc/application/app_prime256/app_prime256.h
  * @brief   This file contains the headers of prime256v1.c .
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
#ifndef __PRIME256V1_H
#define __PRIME256V1_H

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t  Prime256v1Prime[];
extern const uint32_t Prime256v1PrimeLen;
extern const uint8_t  Prime256v1_A[];
extern const uint8_t  Prime256v1AbsA[];
extern const uint32_t Prime256v1_A_Len;
extern const uint32_t Prime256v1_A_Sign;
extern const uint8_t  Prime256v1_B[];
extern const uint32_t Prime256v1_B_Len;
extern const uint8_t  Prime256v1Generator[];
extern const uint32_t Prime256v1GeneratorLen;
extern const uint8_t  Prime256v1GeneratorX[];
extern const uint32_t Prime256v1GeneratorXLen;
extern const uint8_t  Prime256v1GeneratorY[];
extern const uint32_t Prime256v1GeneratorYLen;
extern const uint8_t  Prime256v1Order[];
extern const uint32_t Prime256v1OrderLen;
extern const uint32_t Prime256v1Cofactor;
extern const uint8_t  Prime256v1Seed[];
extern const uint32_t prime256v1SeedLen;

#ifdef __cplusplus
}
#endif

#endif /* __PRIME256V1_H */
