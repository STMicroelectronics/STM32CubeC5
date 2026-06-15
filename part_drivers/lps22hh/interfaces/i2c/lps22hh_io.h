/**
  ******************************************************************************
  * @file    lps22hh_io.h
  * @brief   LPS22HH IO header driver file for I2C.
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
extern "C"
{
#endif

#include "stm32_hal.h"

#if defined (USE_HAL_EXTI_REGISTER_CALLBACKS) && (USE_HAL_EXTI_REGISTER_CALLBACKS == 1)
#if defined (USE_HAL_EXTI_USER_DATA) && (USE_HAL_EXTI_USER_DATA == 1)
#define LPS22HH_CALLBACKS 1
#endif /* USE_HAL_EXTI_USER_DATA */
#endif /* USE_HAL_EXTI_REGISTER_CALLBACKS */

#define LPS22HH_USE_I2C_POLL                /*!< Use I2C in polling mode             */
#define LPS22HH_I2C_POLL_TIMEOUT 0x1000     /*!< Default I2C timeout in polling mode */

typedef struct lps22hh_io_s lps22hh_io_t;

#if defined (LPS22HH_CALLBACKS) && (LPS22HH_CALLBACKS == 1)
typedef void (*lps22hh_io_callback_t)(lps22hh_io_t *pio, void *arg); /*!< IO callback function pointer definition */

typedef struct
{
  lps22hh_io_callback_t io_callback;
  void *arg;
} lps22hh_io_cb_ctx_t;

#endif /* LPS22HH_CALLBACKS */

struct lps22hh_io_s
{
  hal_i2c_handle_t *hi2c;      /*!< I2C handle                */
  uint8_t addr;                /*!< LPS22HH I2C address       */
  uint32_t id;                 /*!< Device id                 */
  hal_exti_handle_t *hexti;    /*!< EXTI handle               */
#if defined (LPS22HH_CALLBACKS) && (LPS22HH_CALLBACKS == 1)
  lps22hh_io_cb_ctx_t exti_cb_ctx;
#endif /* LPS22HH_CALLBACKS */
};


int32_t lps22hh_io_init(lps22hh_io_t *pio);
int32_t lps22hh_io_write_reg(lps22hh_io_t *pio, const uint8_t reg, const uint8_t *pdata, uint16_t len);
int32_t lps22hh_io_read_reg(lps22hh_io_t *pio, const uint8_t reg, uint8_t *pdata, uint16_t len);

int32_t lps22hh_io_enable_irq(lps22hh_io_t *pio);
int32_t lps22hh_io_disable_irq(lps22hh_io_t *pio);

#if defined (LPS22HH_CALLBACKS) && (LPS22HH_CALLBACKS == 1)
int32_t lps22hh_io_register_callback(lps22hh_io_t *pio, lps22hh_io_callback_t io_cb, void *arg);
#endif /* LPS22HH_CALLBACKS */

#ifdef __cplusplus
}
#endif

#endif /* LPS22HH_IO_H */
