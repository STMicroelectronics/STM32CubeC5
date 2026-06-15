/**
  ******************************************************************************
  * file    mx_stfcf_config.h
  * brief   ST Flexible Crypto Framework configuration options.
  *         This allows to include only required configuration files depending on
  *         selected features, and reduce the global memory footprint.
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
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef MX_STFCF_CONFIG_H
#define MX_STFCF_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "mx_mbedtls_config.h"
#include "mx_mbedtls_alt_config.h"

#if defined(STFCF_USER_CONFIG_FILE)
#include STFCF_USER_CONFIG_FILE
#endif /* STFCF_DEFAULT_CONFIG_FILE */

/* Exported constants --------------------------------------------------------*/
#define STFCF_KEY_LOCATION PSA_KEY_LOCATION_LOCAL_STORAGE

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_STFCF_CONFIG_H */
