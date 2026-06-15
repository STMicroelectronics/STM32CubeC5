/**
  ******************************************************************************
  * @file    lps22hh_io.h
  * @brief   This file contains all the functions prototypes for the
  *          lps22hh_io.c .
  ******************************************************************************
  * @attention
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LPS22HH_IO_H
#define LPS22HH_IO_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
/*
 * Callback features are optional; the feature flag LPS22HH_CALLBACKS signals their availability.
 * You can conditionally define this flag based on any preprocessor condition.
 * Examples using feature flags in mx_hal_def.h: USE_HAL_EXTI_REGISTER_CALLBACKS, USE_HAL_EXTI_USER_DATA...
 */
#define LPS22HH_CALLBACKS 1
typedef struct lps22hh_io_s lps22hh_io_t;
#if defined (LPS22HH_CALLBACKS) && (LPS22HH_CALLBACKS == 1)
typedef void (*lps22hh_io_callback_t)(lps22hh_io_t *pio, uint8_t polarity, void *arg); /*!< IO callback function pointer definition */
typedef struct
{
  lps22hh_io_callback_t io_callback;
  void *arg;
} lps22hh_io_cb_ctx_t;
#endif /* LPS22HH_CALLBACKS */
struct lps22hh_io_s
{
  uint32_t                   id;
  /* Add fields as needed to store HAL handles and other resources */
#if defined (LPS22HH_CALLBACKS) && (LPS22HH_CALLBACKS == 1)
  lps22hh_io_cb_ctx_t exti_cb_ctx;
#endif /* LPS22HH_CALLBACKS */
};
int32_t lps22hh_io_init(lps22hh_io_t *pio);
int32_t lps22hh_io_write_reg(lps22hh_io_t *pio, const uint8_t reg, uint8_t *pdata, uint16_t len_bytes);
int32_t lps22hh_io_read_reg(lps22hh_io_t *pio, const uint8_t reg, uint8_t *pdata, uint16_t len_bytes);
int32_t lps22hh_io_enable_irq(lps22hh_io_t *pio);
int32_t lps22hh_io_disable_irq(lps22hh_io_t *pio);
#if defined (LPS22HH_CALLBACKS) && (LPS22HH_CALLBACKS == 1)
int32_t lps22hh_io_register_callback(lps22hh_io_t *pio, lps22hh_io_callback_t io_cb, void *arg);
#endif /* LPS22HH_CALLBACKS */
#ifdef __cplusplus
}
#endif
#endif /* LPS22HH_IO_H */
