/**
  ******************************************************************************
  * @file    pka/sign_verify_ecc_ecdsa/app_prime256/sig_ver.c
  * @brief   This file contains reference buffers from
  *          NIST Cryptographic Algorithm Validation Program (CAVP).
  *          (http://csrc.nist.gov/groups/STM/cavp/)
  *          1 test vector is extracted to demonstrate PKA capability to
  *          verify a signature using ECDSA (Elliptic Curve Digital Signature Algorithm)
  *          signature verification function principle.
  *          It is adapted from SigVer.rsp section [P-256,SHA-256] available under
  *          http://csrc.nist.gov/groups/STM/cavp/documents/dss/186-3ecdsatestvectors.zip
  *          and provided in the same directory for reference.
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
#ifndef __SIGVER_H
#define __SIGVER_H

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t  SigVerMsg[];
extern const uint8_t  SigVerHashMsg[];
extern const uint8_t  SigVerHashMsgFalse[];
extern const uint32_t SigVerHashMsgLen;
extern const uint8_t  SigVer_Qx[];
extern const uint32_t SigVer_Qx_Len;
extern const uint8_t  SigVer_Qy[];
extern const uint32_t SigVer_Qy_Len;
extern const uint8_t  SigVer_R[];
extern const uint32_t SigVer_R_Len;
extern const uint8_t  SigVer_S[];
extern const uint32_t SigVer_S_Len;
extern const uint32_t SigVer_Result;

#ifdef __cplusplus
}
#endif

#endif /* __SIGVER_H */

