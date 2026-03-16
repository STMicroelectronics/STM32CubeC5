/**
  ******************************************************************************
  * @file    pka/modular_exponentiation_crt/application/rsa_2048/ciphertext.h
  * @brief   This file contains the headers of the ciphertext buffers.
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
#ifndef __CIPHERTEXT_H
#define __CIPHERTEXT_H

#ifdef __cplusplus
extern "C" {
#endif

#define CIPHERTEXT_BIN_LEN 256
extern const uint8_t CiphertextBin[];

#ifdef __cplusplus
}
#endif

#endif /* __CIPHERTEXT_H */
