/**
  ******************************************************************************
  * @file    pka/sign_verify_ecc_ecdsa/app_prime256/sig_gen.h
  * @brief   This file contains the headers of sig_gen.c .
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
#ifndef __SIGGEN_H
#define __SIGGEN_H

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t  SigGenMsg[];
extern const uint8_t  SigGenHashMsg[];
extern const uint32_t SigGenHashMsgLen;
extern const uint8_t  SigGen_D[];
extern const uint32_t SigGen_D_Len;
extern const uint8_t  SigGen_Qx[];
extern const uint32_t SigGen_Qx_Len;
extern const uint8_t  SigGen_Qy[];
extern const uint32_t SigGen_Qy_Len;
extern const uint8_t  SigGen_K[];
extern const uint32_t SigGen_K_Len;
extern const uint8_t  SigGen_R[];
extern const uint32_t SigGen_R_Len;
extern const uint8_t  SigGen_S[];
extern const uint32_t SigGen_S_Len;

#ifdef __cplusplus
}
#endif

#endif /* __SIGGEN_H */
