/**
  ******************************************************************************
  * @file    boot_hal_utils.h
  * @brief   Header for providing helper utility definitions
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BOOT_HAL_UTILS_H
#define BOOT_HAL_UTILS_H

#include "bootutil_log.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported defines ------------------------------------------------------------------*/
/* Helper macros to concatenate tokens */
#define CONCAT(a, b) a##b
#define CONCAT2(a, b) CONCAT(a, b)

/* get an array size */
#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

/* Count the number of arguments (up to eight here) */
#define NUM_ARGS(...)  NUM_ARGS_IMPL(__VA_ARGS__,8,7,6,5,4,3,2,1)
#define NUM_ARGS_IMPL(_1,_2,_3,_4,_5,_6,_7,_8, N,...) N

/* separator defines */
#define SEP_COMMA ,
#define SEP_OR    |
#define SEP_AND   &
#define SEP_XOR   ^

/* Macro to initialize an array with a specific value 'x' */
#define INIT_STRUCT_1(x)    x
#define INIT_STRUCT_2(x)    x, INIT_STRUCT_1(x)
#define INIT_STRUCT_3(x)    x, INIT_STRUCT_2(x)
#define INIT_STRUCT_4(x)    x, INIT_STRUCT_3(x)
#define INIT_STRUCT_5(x)    x, INIT_STRUCT_4(x)
#define INIT_STRUCT_6(x)    x, INIT_STRUCT_5(x)
#define INIT_STRUCT_7(x)    x, INIT_STRUCT_6(x)
#define INIT_STRUCT_8(x)    x, INIT_STRUCT_7(x)
/**
  * example:
  * input: n=3 x=0xA
  * output: 0xA 0xA 0xA
  */
#define INIT_STRUCT(n, x)   CONCAT2(INIT_STRUCT_, n)(x)

/* Recursive macros to call a 'macro' with a argument list 'x' separated with 'sep'*/
#define REPEAT_1(macro, sep, x)            macro(x)
#define REPEAT_2(macro, sep, x, ...)       macro(x) sep REPEAT_1(macro, sep, __VA_ARGS__)
#define REPEAT_3(macro, sep, x, ...)       macro(x) sep REPEAT_2(macro, sep, __VA_ARGS__)
#define REPEAT_4(macro, sep, x, ...)       macro(x) sep REPEAT_3(macro, sep, __VA_ARGS__)
#define REPEAT_5(macro, sep, x, ...)       macro(x) sep REPEAT_4(macro, sep, __VA_ARGS__)
#define REPEAT_6(macro, sep, x, ...)       macro(x) sep REPEAT_5(macro, sep, __VA_ARGS__)
#define REPEAT_7(macro, sep, x, ...)       macro(x) sep REPEAT_6(macro, sep, __VA_ARGS__)
#define REPEAT_8(macro, sep, x, ...)       macro(x) sep REPEAT_7(macro, sep, __VA_ARGS__)
#define REPEAT_N_IMPL(N, macro, sep, ...)  CONCAT2(REPEAT_, N)(macro, sep, __VA_ARGS__)
/**
  * example:
  * input: macro=sizeof; sep=SEP_COMMA; __VA_ARGS__=uint8_t, uint16_t, uint32_t
  * output: 1, 2, 4
  */
#define REPEAT_N(macro, sep, ...)          REPEAT_N_IMPL(NUM_ARGS(__VA_ARGS__), macro, sep, __VA_ARGS__)

#ifndef ARG_UNUSED
#define ARG_UNUSED(arg)  ((void)arg)
#endif /* ARG_UNUSED */

#ifdef OEMIROT_DEV_MODE
#define ROT_LOG_ERR(...) ((void)BOOT_LOG_ERR(__VA_ARGS__))
#define ROT_LOG_WRN(...) ((void)BOOT_LOG_WRN(__VA_ARGS__))
#define ROT_LOG_INF(...) ((void)BOOT_LOG_INF(__VA_ARGS__))
#define ROT_LOG_DBG(...) ((void)BOOT_LOG_DBG(__VA_ARGS__))
#define ROT_LOG_SIM(...) ((void)BOOT_LOG_SIM(__VA_ARGS__))
#else
#define ROT_LOG_ERR(...) IGNORE(__VA_ARGS__)
#define ROT_LOG_WRN(...) IGNORE(__VA_ARGS__)
#define ROT_LOG_INF(...) IGNORE(__VA_ARGS__)
#define ROT_LOG_DBG(...) IGNORE(__VA_ARGS__)
#define ROT_LOG_SIM(...) IGNORE(__VA_ARGS__)
#endif /* OEMIROT_DEV_MODE */

#ifdef __cplusplus
}
#endif

#endif /* BOOT_HAL_UTILS_H */
