/**
  * Licensed to the Apache Software Foundation (ASF) under one
  * or more contributor license agreements.  See the NOTICE file
  * distributed with this work for additional information
  * regarding copyright ownership.  The ASF licenses this file
  * to you under the Apache License, Version 2.0 (the
  * "License"); you may not use this file except in compliance
  * with the License.  You may obtain a copy of the License at
  *
  *  http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing,
  * software distributed under the License is distributed on an
  * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
  * KIND, either express or implied.  See the License for the
  * specific language governing permissions and limitations
  * under the License.
  */
/**
  * Original code taken from mcuboot project at:
  * https://github.com/JuulLabs-OSS/mcuboot
  * Git SHA of the original version: ac55554059147fff718015be9f4bd3108123f50a
  * Modifications are Copyright (c) 2019 Arm Limited.
  *
  * Copyright (c) 2025-2026 STMicroelectronics.
  */

#include "region_defs.h"
#include "boot_hal_hash_ref.h"

#include "string.h"
#include "sign_key.h"
#include "sysflash.h"
#include "crypto_types.h"
#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls_config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif /* !MBEDTLS_CONFIG_FILE */

#define AUTH_PUB_KEY_HASH_LENGTH  SHA256_LENGTH
#define ENC_PRIV_KEY_LENGTH       32U

typedef struct bootutil_key bootutil_key_t;

static unsigned int ecdsa_pub_key_hash_len = AUTH_PUB_KEY_HASH_LENGTH;

uint32_t pub_key_len;
const int bootutil_key_cnt = MCUBOOT_IMAGE_NUMBER;
#if defined(MCUBOOT_ENC_IMAGES)
unsigned int enc_priv_key_len = ENC_PRIV_KEY_LENGTH;
#endif/* MCUBOOT_ENC_IMAGES */

#define AUTHENTICATION_PUBKEY_ADDRESS       (uint8_t *)((uint32_t)(KEYS_START))
#define ENCRYPTION_PRIVKEY_ADDRESS          (uint8_t *)((uint32_t)(KEYS_START + AUTH_PUB_KEY_HASH_LENGTH))

static const bootutil_key_t bootutil_key_hash[] =
{
  {
    .key = AUTHENTICATION_PUBKEY_ADDRESS,
    .len = &ecdsa_pub_key_hash_len
  },

#if (MCUBOOT_DATA_IMAGE_NUMBER == 1)
  {
    .key = AUTHENTICATION_PUBKEY_ADDRESS,
    .len = &ecdsa_pub_key_hash_len
  },
#endif /* (MCUBOOT_DATA_IMAGE_NUMBER == 1) */
};

/* Create a variable that carry public key */
bootutil_key_t bootutil_keys[1] =
{
  {
    .key = 0,
    .len = &pub_key_len,
  },
};

/**
  * @brief Retrieve the hash of the corresponding public key.
  * @param image_index Index of the image.
  * @param public_key_hash Buffer to store the key-hash in.
  * @param key_hash_size Size of the buffer
  * @return 0 on success; nonzero on failure.
  */
int boot_retrieve_public_key_hash(uint8_t image_index,
                                  uint8_t *public_key_hash,
                                  size_t *key_hash_size)
{
  /* Key is selected according to image index */
  (void) memcpy(public_key_hash,  bootutil_key_hash[image_index].key, *key_hash_size);
  return 0;
}

#if defined(MCUBOOT_ENC_IMAGES)
const bootutil_key_t bootutil_enc_key =
{
  .key = ENCRYPTION_PRIVKEY_ADDRESS,
  .len = &enc_priv_key_len,
};
#else
const bootutil_key_t bootutil_enc_key =
{
  .key = (const unsigned char *)0,
  .len = (const unsigned int *)0,
};
#endif/* MCUBOOT_ENC_IMAGES */
