/**
  ******************************************************************************
  * file           : example.c
  * brief          : This example describes how to use the STM32 Cryptographic Library
  *                  to sign and to verify a message using the SM2 algorithm.
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
#include <string.h>
#include "example.h"
#include "cmox_crypto.h"

/* Global variables ----------------------------------------------------------*/
/* SM3 context handle */
cmox_sm3_handle_t sm3_ctx;

/* ECC context */
cmox_ecc_handle_t p_ecc_ctx;
/* ECC working buffer */
uint8_t Working_Buffer[2048] __ALIGNED(8);

/* Random data buffer */
uint32_t Computed_Random[8];
/* RNG peripheral handle */
hal_rng_handle_t *hrng;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define GENERATION_TIMEOUT_MS      1000U  /* The more random data to generate, the longer the timeout */

/** Extract from IETF draft-shen-sm2-ecdsa-02
  * A.1.  General Introduction

   This appendix uses the hash algorithm described in
   draft-shen-sm3-hash-00, which applies on a bit string of length less
   than 2^54 and output a hash value of size 256, denotes as H256( ).

   In this appendix, all the hexadecimal number has high digits on the
   left and low digits on the right.

   In this appendix, all the messages are in ASCII code.

   Let the user A's identity be: ALICE123@YAHOO.COM.  Denoted in ASCII
   code IDA:

   414C 49434531 32334059 41484F4F 2E434F4D

   ENTLA=0090.

   A.2.  Digital Signature of over E(Fp)

   The elliptic curve equationi is:

   RFC1341
   y^2 = x^3 + ax + b


 Example 1: Fp-256
 A Prime p:
 8542D69E 4C044F18 E8B92435 BF6FF7DE 45728391 5C45517D 722EDB8B 08F1DFC3

 The coefficient a:
 787968B4 FA32C3FD 2417842E 73BBFEFF 2F3C848B 6831D7E0 EC65228B 3937E498

 The coefficient b:
 63E4C6D3 B23B0C84 9CF84241 484BFE48 F61D59A5 B16BA06E 6E12D1DA 27C5249A

 The base point G=(xG,yG)AGBPA[not]whose degree is n:
 x-coordinate xG:
 421DEBD6 1B62EAB6 746434EB C3CC315E 32220B3B ADD50BDC 4C4E6C14 7FEDD43D
 y-coordinate yG:
 0680512B CBB42C07 D47349D2 153B70C4 E5D7FDFC BFA36EA1 A85841B9 E46E09A2
 degree n:
 8542D69E 4C044F18 E8B92435 BF6FF7DD 29772063 0485628D 5AE74EE7 C32E79B7

 The message M to be signed:message digest

 The private key dA:
 128B2FA8 BD433C6C 068C8D80 3DFF7979 2A519A55 171B1B65 0C23661D 15897263

 The public key PA=(xA,yA):
 x-coordinate xA:
 0AE4C779 8AA0F119 471BEE11 825BE462 02BB79E2 A5844495 E97C04FF 4DF2548A
 y-coordinate yA:
 7C0240F8 8F1CD4E1 6352A73C 17B7F16F 07353E53 A176D684 A9FE0C6B B798E857

 Hash value ZA=H256(ENTLA || IDA || a || b || xG || yG || xA || yA)

 ZA:
 F4A38489 E32B45B6 F876E3AC 2168CA39 2362DC8F 23459C1D 1146FC3D BFB7BC9A

 The intermediate value during signing processing:
 M~=ZA || M:
 F4A38489 E32B45B6 F876E3AC 2168CA39 2362DC8F 23459C1D 1146FC3D BFB7BC9A
 6D657373 61676520 64696765 7374
 hash value e=H256(M):
 B524F552 CD82B8B0 28476E00 5C377FB1 9A87E6FC 682D48BB 5D42E3D9 B9EFFE76

  ...

 random number k:
 6CB28D99 385C175C 94F94E93 4817663F C176D925 DD72B727 260DBAAE 1FB2F96F
 point (x1,y1)=[k]G:
 x-coordinate x1:
 110FCDA5 7615705D 5E7B9324 AC4B856D 23E6D918 8B2AE477 59514657 CE25D112
 y-coordinate y1:
 1C65D68A 4A08601D F24B431E 0CAB4EBE 084772B3 817E8581 1A8510B2 DF7ECA1A
 r=(e+x1) modn:
 40F1EC59 F793D9F4 9E09DCEF 49130D41 94F79FB1 EED2CAA5 5BACDB49 C4E755D1
 (1 + dA)^(-1)
 79BFCF30 52C80DA7 B939E0C6 914A18CB B2D96D85 55256E83 122743A7 D4F5F956
 s = ((1 + dA)^(-1)1 * (k - r * dA)) modn:
 6FC6DAC3 2C5D5CF1 0C77DFB2 0F7C2EB6 67A45787 2FB09EC5 6327A67E C7DEEBE7

 Digital Signature of the message M: (r,s)
 r:
 40F1EC59 F793D9F4 9E09DCEF 49130D41 94F79FB1 EED2CAA5 5BACDB49 C4E755D1
 s:
 6FC6DAC3 2C5D5CF1 0C77DFB2 0F7C2EB6 67A45787 2FB09EC5 6327A67E C7DEEBE7

 The intermediate value during verification processing:
 hash value e' = H256(M'~):
 B524F552 CD82B8B0 28476E00 5C377FB1 9A87E6FC 682D48BB 5D42E3D9 B9EFFE76
 t=(rA!aa^3A!aS. modn:
 2B75F07E D7ECE7CC C1C8986B 991F441A D324D6D6 19FE06DD 63ED32E0 C997C801
 point (x0A!aa y0')=[s']G:
 x-coordinate x0':
 7DEACE5F D121BC38 5A3C6317 249F413D 28C17291 A60DFD83 B835A453 92D22B0A
 y-coordinate y0':
 2E49D5E5 279E5FA9 1E71FD8F 693A64A3 C4A94611 15A4FC9D 79F34EDC 8BDDEBD0
 point (x00', y00')=[t]PA:
 x-coordinate x00':
 1657FA75 BF2ADCDC 3C1F6CF0 5AB7B45E 04D3ACBE 8E4085CF A669CB25 64F17A9F
 y-coordinate y00':
 19F0115F 21E16D2F 5C3A485F 8575A128 BBCDDF80 296A62F6 AC2EB842 DD058E50
 point (x1', y1')=[s']G + [t]PA:
 x-coordinate x1':
 110FCDA5 7615705D 5E7B9324 AC4B856D 23E6D918 8B2AE477 59514657 CE25D112
 y-coordinate y1':
 1C65D68A 4A08601D F24B431E 0CAB4EBE 084772B3 817E8581 1A8510B2 DF7ECA1A
 R = (e' + x1') modn:
 40F1EC59 F793D9F4 9E09DCEF 49130D41 94F79FB1 EED2CAA5 5BACDB49 C4E755D1

  */
const uint8_t User_ID[] =
{
  'A', 'L', 'I', 'C', 'E', '1', '2', '3', '@', 'Y', 'A', 'H', 'O', 'O', '.', 'C', 'O', 'M'
};

const uint8_t Message[] =
{
  'm', 'e', 's', 's', 'a', 'g', 'e', ' ', 'd', 'i', 'g', 'e', 's', 't'
};

const uint8_t Private_Key[] =
{
  0x12, 0x8B, 0x2F, 0xA8, 0xBD, 0x43, 0x3C, 0x6C, 0x06, 0x8C, 0x8D, 0x80, 0x3D, 0xFF, 0x79, 0x79,
  0x2A, 0x51, 0x9A, 0x55, 0x17, 0x1B, 0x1B, 0x65, 0x0C, 0x23, 0x66, 0x1D, 0x15, 0x89, 0x72, 0x63
};

const uint8_t Public_Key[] =
{
  0x0A, 0xE4, 0xC7, 0x79, 0x8A, 0xA0, 0xF1, 0x19, 0x47, 0x1B, 0xEE, 0x11, 0x82, 0x5B, 0xE4, 0x62,
  0x02, 0xBB, 0x79, 0xE2, 0xA5, 0x84, 0x44, 0x95, 0xE9, 0x7C, 0x04, 0xFF, 0x4D, 0xF2, 0x54, 0x8A,
  0x7C, 0x02, 0x40, 0xF8, 0x8F, 0x1C, 0xD4, 0xE1, 0x63, 0x52, 0xA7, 0x3C, 0x17, 0xB7, 0xF1, 0x6F,
  0x07, 0x35, 0x3E, 0x53, 0xA1, 0x76, 0xD6, 0x84, 0xA9, 0xFE, 0x0C, 0x6B, 0xB7, 0x98, 0xE8, 0x57
};

const uint8_t Known_ZA[] =
{
  0xF4, 0xA3, 0x84, 0x89, 0xE3, 0x2B, 0x45, 0xB6, 0xF8, 0x76, 0xE3, 0xAC, 0x21, 0x68, 0xCA, 0x39,
  0x23, 0x62, 0xDC, 0x8F, 0x23, 0x45, 0x9C, 0x1D, 0x11, 0x46, 0xFC, 0x3D, 0xBF, 0xB7, 0xBC, 0x9A
};

const uint8_t Known_Hash[] =
{
  0xB5, 0x24, 0xF5, 0x52, 0xCD, 0x82, 0xB8, 0xB0, 0x28, 0x47, 0x6E, 0x00, 0x5C, 0x37, 0x7F, 0xB1,
  0x9A, 0x87, 0xE6, 0xFC, 0x68, 0x2D, 0x48, 0xBB, 0x5D, 0x42, 0xE3, 0xD9, 0xB9, 0xEF, 0xFE, 0x76
};

const uint8_t Known_Random[] = /* = k - 1 */
{
  0x6C, 0xB2, 0x8D, 0x99, 0x38, 0x5C, 0x17, 0x5C, 0x94, 0xF9, 0x4E, 0x93, 0x48, 0x17, 0x66, 0x3F,
  0xC1, 0x76, 0xD9, 0x25, 0xDD, 0x72, 0xB7, 0x27, 0x26, 0x0D, 0xBA, 0xAE, 0x1F, 0xB2, 0xF9, 0x6E
};

const uint8_t Known_Signature[] =
{
  0x40, 0xF1, 0xEC, 0x59, 0xF7, 0x93, 0xD9, 0xF4, 0x9E, 0x09, 0xDC, 0xEF, 0x49, 0x13, 0x0D, 0x41,
  0x94, 0xF7, 0x9F, 0xB1, 0xEE, 0xD2, 0xCA, 0xA5, 0x5B, 0xAC, 0xDB, 0x49, 0xC4, 0xE7, 0x55, 0xD1,
  0x6F, 0xC6, 0xDA, 0xC3, 0x2C, 0x5D, 0x5C, 0xF1, 0x0C, 0x77, 0xDF, 0xB2, 0x0F, 0x7C, 0x2E, 0xB6,
  0x67, 0xA4, 0x57, 0x87, 0x2F, 0xB0, 0x9E, 0xC5, 0x63, 0x27, 0xA6, 0x7E, 0xC7, 0xDE, 0xEB, 0xE7
};

/* Computed data buffer */
uint8_t Computed_Hash[CMOX_SM3_SIZE];
uint8_t Computed_ZA[CMOX_SM3_SIZE];
uint8_t Computed_Signature[CMOX_ECC_SM2TEST_SIG_LEN];

hal_rng_handle_t *pRNG;  /* pointer referencing the PPP handle from the generated code */

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes STM32 Cryptographic library and the RNG instance.
  * As it is a description:
  * Step 1.1: Initializes the STM32 Cryptographic library.
  * Step 1.2: Initializes the RNG instance.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Init the RNG */
  hrng = mx_example_rng_init();

  /** ########## Step 1.1 ##########
    * Initializes the STM32 Cryptographic library.
    */
  if (cmox_initialize(NULL) != CMOX_INIT_SUCCESS)
  {
    PRINTF("[ERROR] Step 1.1: STM32 Cryptographic library initialization ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
  }
  else
  {
    PRINTF("[INFO] Step 1.1: STM32 Cryptographic library initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }

  /** ########## Step 1.2 ##########
    * Initializes the RNG instance.
    */
  if (hrng != NULL)
  {
    PRINTF("[INFO] Step 1.2: RNG initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }
  else
  {
    PRINTF("[ERROR] Step 1.2: RNG initialization ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
  }

  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * This example describes how to use the STM32 Cryptographic Library
  * to compute ZA and generate the digest data, then use them to sign and to verify a message with a known random
  * then a true random using the SM2 algorithm.
  * As it is a description:
  * - Step 2.1: Computes and verifies the user identity hash (ZA).
  * - Step 2.2: Computes and verifies the digest of a known message.
  * - Step 2.3: Computes and verifies the signature of the computed digest of a known message using a known random.
  * - Step 2.4: Computes and verifies the signature of the computed digest of a known message using a true random.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  cmox_hash_retval_t hretval;
  cmox_ecc_retval_t retval;
  size_t computed_size;
  /* General hash context */
  cmox_hash_handle_t *hash_ctx;
  /* Fault check verification variable */
  uint32_t fault_check = CMOX_ECC_AUTH_FAIL;

  /** ########## Step 2.1 ##########
    * Computes and verifies the user identity hash (ZA).
    */

  /* Construct a ECC context, specifying mathematics implementation and working buffer for later processing */
  cmox_ecc_construct(&p_ecc_ctx, CMOX_ECC256_MATH_FUNCS, Working_Buffer, sizeof(Working_Buffer));

  /* Compute ZA */
  hretval = cmox_sm2_computeZA(&p_ecc_ctx,                     /* ECC context */
                               CMOX_ECC_CURVE_SM2TEST,         /* SM2 Test ECC curve selected */
                               User_ID,                        /* User ID buffer */
                               sizeof(User_ID) * 8,            /* User ID length in bit */
                               Public_Key,                     /* Public key */
                               sizeof(Public_Key),             /* Public key length */
                               Computed_ZA, &computed_size);   /* Computed ZA buffer */

  /* Verify generated data size is the expected one */
  if (computed_size != CMOX_SM3_SIZE)
  {
    goto _app_process_exit;
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Known_ZA, Computed_ZA, computed_size) != 0)
  {
    goto _app_process_exit;
  }

  /* Cleanup context */
  cmox_ecc_cleanup(&p_ecc_ctx);

  PRINTF("[INFO] Step 2.1: Computation and verification of the user identity hash (ZA) COMPLETED.\n");

  /** ########## Step 2.2 ##########
    * Computes and verifies the digest of a known message.
    */

  /* Construct a hash context that is configured to perform SM3 digest operations */
  hash_ctx = cmox_sm3_construct(&sm3_ctx);
  if (hash_ctx == NULL)
  {
    goto _app_process_exit;
  }

  /* Initialize the hash context */
  hretval = cmox_hash_init(hash_ctx);

  /* Verify API returned value */
  if (hretval != CMOX_HASH_SUCCESS)
  {
    goto _app_process_exit;
  }

  hretval = cmox_hash_append(hash_ctx, Computed_ZA, sizeof(Computed_ZA));

  /* Verify API returned value */
  if (hretval != CMOX_HASH_SUCCESS)
  {
    goto _app_process_exit;
  }

  hretval = cmox_hash_append(hash_ctx, Message, sizeof(Message));

  /* Verify API returned value */
  if (hretval != CMOX_HASH_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Generate the digest data */
  hretval = cmox_hash_generateTag(hash_ctx, Computed_Hash, &computed_size);

  /* Verify API returned value */
  if (hretval != CMOX_HASH_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != CMOX_SM3_SIZE)
  {
    goto _app_process_exit;
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Known_Hash, Computed_Hash, computed_size) != 0)
  {
    goto _app_process_exit;
  }

  /* Cleanup the context */
  hretval = cmox_hash_cleanup(hash_ctx);
  if (hretval != CMOX_HASH_SUCCESS)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.2: Computation and verification of the digest of a known message COMPLETED.\n");

  /** ########## Step 2.3 ##########
    * Computes and verifies the signature of the computed digest of a known message using a known random.
    */

  /* Construct a ECC context, specifying mathematics implementation and working buffer for later processing */
  cmox_ecc_construct(&p_ecc_ctx, CMOX_ECC256_MATH_FUNCS, Working_Buffer, sizeof(Working_Buffer));

  /* Compute directly the signature passing all the needed parameters */
  retval = cmox_sm2_sign(&p_ecc_ctx,                               /* ECC context */
                         CMOX_ECC_CURVE_SM2TEST,                   /* SM2 Test ECC curve selected */
                         Known_Random, sizeof(Known_Random),       /* Random data buffer */
                         Private_Key, sizeof(Private_Key),         /* Private key for signature */
                         Computed_Hash, CMOX_SM3_SIZE,             /* Digest to sign */
                         Computed_Signature, &computed_size);      /* Data buffer to receive signature */

  /* Verify API returned value */
  if (retval != CMOX_ECC_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Known_Signature))
  {
    goto _app_process_exit;
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Computed_Signature, Known_Signature, computed_size) != 0)
  {
    goto _app_process_exit;
  }

  /* Cleanup context */
  cmox_ecc_cleanup(&p_ecc_ctx);

  /* Construct a ECC context, specifying mathematics implementation and working buffer for later processing */
  cmox_ecc_construct(&p_ecc_ctx, CMOX_ECC256_MATH_FUNCS, Working_Buffer, sizeof(Working_Buffer));

  /* Verify directly the signature passing all the needed parameters */
  retval = cmox_sm2_verify(&p_ecc_ctx,                                /* ECC context */
                           CMOX_ECC_CURVE_SM2TEST,                    /* SM2 Test ECC curve selected */
                           Public_Key, sizeof(Public_Key),            /* Public key for verification */
                           Computed_Hash, CMOX_SM3_SIZE,              /* Digest to verify */
                           Known_Signature, sizeof(Known_Signature),  /* Data buffer to receive signature */
                           &fault_check);                             /* Fault check variable:
                                                            to ensure no fault injection occurs during this API call */

  /* Verify API returned value */
  if (retval != CMOX_ECC_AUTH_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify Fault check variable value */
  if (fault_check != CMOX_ECC_AUTH_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Cleanup context */
  cmox_ecc_cleanup(&p_ecc_ctx);

  PRINTF("[INFO] Step 2.3: Computation and verification of the signature of the computed digest "
         "of a known message using a known random COMPLETED.\n");

  /** ########## Step 2.4 ##########
    * Computes and verifies the signature of the computed digest of a known message using a true random.
    */

  /* Construct a ECC context, specifying mathematics implementation and working buffer for later processing */
  cmox_ecc_construct(&p_ecc_ctx, CMOX_ECC256_MATH_FUNCS, Working_Buffer, sizeof(Working_Buffer));

  /** Note: The random value must satisfy some arithmetic constraints versus the selected curve and points
    * to minimize the statictical vulnerability.
    * In case this is not satisfied, cmox_sm2_sign returns CMOX_ECC_ERR_WRONG_RANDOM: new random has to be
    * generated and API call again.
    */

  do
  {
    if (HAL_RNG_GenerateRandomNumber(hrng, Computed_Random, sizeof(Computed_Random) / sizeof(uint32_t),
                                     GENERATION_TIMEOUT_MS) != HAL_OK)
    {
      /* Random number generation error */
      goto _app_process_exit;
    }

    /* Compute directly the signature passing all the needed parameters */
    retval = cmox_sm2_sign(&p_ecc_ctx,                                            /* ECC context */
                           CMOX_ECC_CURVE_SM2TEST,                                /* SM2 Test ECC curve selected */
                           (uint8_t *)Computed_Random, sizeof(Computed_Random),   /* Random data buffer */
                           Private_Key, sizeof(Private_Key),                      /* Private key for signature */
                           Computed_Hash, CMOX_SM3_SIZE,                          /* Digest to sign */
                           Computed_Signature, &computed_size);

  } while (retval ==  CMOX_ECC_ERR_WRONG_RANDOM);

  /* Verify API returned value */
  if (retval != CMOX_ECC_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Known_Signature))
  {
    goto _app_process_exit;
  }

  /* Cleanup context */
  cmox_ecc_cleanup(&p_ecc_ctx);

  /* Construct a ECC context, specifying mathematics implementation and working buffer for later processing */
  cmox_ecc_construct(&p_ecc_ctx, CMOX_ECC256_MATH_FUNCS, Working_Buffer, sizeof(Working_Buffer));

  /* Verify directly the signature passing all the needed parameters */
  retval = cmox_sm2_verify(&p_ecc_ctx,                                      /* ECC context */
                           CMOX_ECC_CURVE_SM2TEST,                          /* SM2 Test ECC curve selected */
                           Public_Key, sizeof(Public_Key),                  /* Public key for verification */
                           Computed_Hash, CMOX_SM3_SIZE,                    /* Digest to verify */
                           Computed_Signature, sizeof(Computed_Signature),  /* Data buffer to receive signature */
                           &fault_check);                                   /* Fault check variable:
                                                            to ensure no fault injection occurs during this API call */

  /* Verify API returned value */
  if (retval != CMOX_ECC_AUTH_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Verify Fault check variable value */
  if (fault_check != CMOX_ECC_AUTH_SUCCESS)
  {
    goto _app_process_exit;
  }

  /* Cleanup context */
  cmox_ecc_cleanup(&p_ecc_ctx);

  PRINTF("[INFO] Step 2.4: Computation and verification of the signature of the computed digest "
         "of a known message using a true random COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Deinitializes STM32 Cryptographic library and the RNG instance before leaving the scenario.
  * As it is a description:
  * Step 3.1: Deinitializes the STM32 Cryptographic library.
  * Step 3.2: Deinitializes the RNG instance.
  */
app_status_t app_deinit(void)
{
  /** ########## Step 3.1 ##########
    * Deinitializes the STM32 Cryptographic library.
    */
  if (cmox_finalize(NULL) != CMOX_INIT_SUCCESS)
  {
    return EXEC_STATUS_ERROR;
  }
  PRINTF("[INFO] Step 3.1: STM32 Cryptographic library deinitialization COMPLETED.\n");

  /** ########## Step 3.2 ##########
    * Deinitializes the RNG instance.
    */
  mx_example_rng_deinit();
  PRINTF("[INFO] Step 3.2: RNG deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */
