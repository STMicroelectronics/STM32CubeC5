/**
  ******************************************************************************
  * @file    lps22hh.h
  * @brief   LPS22HH header driver file.
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
#ifndef LPS22HH_H
#define LPS22HH_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "lps22hh_reg.h"
#include "lps22hh_io.h"

typedef struct lps22hh_object_s lps22hh_object_t;   /*!< LPS22HH object structure type */

#if defined (LPS22HH_CALLBACKS) && (LPS22HH_CALLBACKS == 1)
/*!< callback function pointer definition */
typedef void (*lps22hh_callback_t)(lps22hh_object_t *pobj, void *arg);

/**
  * @brief Callback context structure.
  */
typedef struct
{
  lps22hh_callback_t callback;   /*!< Callback function to register           */
  void *arg;                     /*!< Optional argument given to the callback */
} lps22hh_cb_ctx_t;

#endif /* LPS22HH_CALLBACKS */

/**
  * @brief LPS22HH object structure.
  */
struct lps22hh_object_s
{
  lps22hh_io_t        io_ctx;               /*!< IO                                */
  stmdev_ctx_t        ctx;                  /*!< LPS22HH context                   */
  uint8_t             is_initialized;       /*!< Is LPS22HH initialized            */
  uint8_t             is_enabled;           /*!< Is enabled                        */
  lps22hh_odr_t       last_odr;             /*!< Last output data rate             */
#if defined (LPS22HH_CALLBACKS) && (LPS22HH_CALLBACKS == 1)
  lps22hh_cb_ctx_t    cb_ctx_irq;           /*!< callback context for IRQ events   */
#endif /* LPS22HH_CALLBACKS */
};

/**
  * @brief LPS22HH 3 axis 16 bit union.
  */
typedef union
{
  int16_t i16bit[3];      /*!< axis value 16 bit         */
  uint8_t u8bit[6];       /*!< axis value 8 bit          */
} lps22hh_axis3bit16_t;

/**
  * @brief LPS22HH 1 axis 16 bit union.
  */
typedef union
{
  int16_t i16bit;       /*!< axis value 16 bit         */
  uint8_t u8bit[2];     /*!< axis value 8 bit         */
} lps22hh_axis1bit16_t;

/**
  * @brief LPS22HH 3 axis 32 bit union.
  */
typedef union
{
  int32_t i32bit[3];    /*!< axis value 32 bit         */
  uint8_t u8bit[12];    /*!< axis value 8  bit         */
} lps22hh_axis3bit32_t;

/**
  * @brief LPS22HH 1 axis 32 bit union.
  */
typedef union
{
  int32_t i32bit;     /*!< axis value 32 bit         */
  uint8_t u8bit[4];   /*!< axis value 8  bit         */
} lps22hh_axis1bit32_t;

/**
  * @brief LPS22HH 1 axis 32 bit union.
  */
typedef union
{
  uint32_t u32bit;     /*!< axis value 32 bit         */
  uint8_t u8bit[4];   /*!< axis value 8  bit         */
} lps22hh_axis1bitu32_t;

/**
  * @brief Fifo mode.
  */

typedef enum
{
  LPS22HH_FIFO_BYPASS_MODE                    = (uint8_t)0x00,    /*!< The FIFO is disabled and empty. The pressure is read directly*/
  LPS22HH_FIFO_FIFO_MODE                      = (uint8_t)0x20,    /*!< Stops collecting data when full */
  LPS22HH_FIFO_STREAM_MODE                    = (uint8_t)0x40,    /*!< Keep the newest measurements in the FIFO*/
  LPS22HH_FIFO_TRIGGER_STREAMTOFIFO_MODE      = (uint8_t)0x60,    /*!< STREAM MODE until trigger deasserted, then change to FIFO MODE*/
  LPS22HH_FIFO_TRIGGER_BYPASSTOSTREAM_MODE    = (uint8_t)0x80,    /*!< BYPASS MODE until trigger deasserted, then STREAM MODE*/
  LPS22HH_FIFO_TRIGGER_BYPASSTOFIFO_MODE      = (uint8_t)0xE0     /*!< BYPASS mode until trigger deasserted, then FIFO MODE*/
} lps22hh_fifomode;


typedef enum
{
  LPS22HH_OK = 0,
  LPS22HH_ERROR = -1,
} lps22hh_drv_status_t;

/* LPS22HH BUS type */
#define LPS22HH_I2C_BUS          0U         /*!< LPS22HH I2C BUS            */
#define LPS22HH_SPI_4WIRES_BUS   1U         /*!< LPS22HH SPI 4 WIRES BUS    */
#define LPS22HH_SPI_3WIRES_BUS   2U         /*!< LPS22HH SPI 4 WIRES BUS    */

#define LPS22HH_FIFO_FULL        (uint8_t)0x20  /*!< LPS22HH FIFO FULL */

/** LPS22HH low noise mode  **/
#define LPS22HH_LOW_NOISE_DIS      0      /*!< LPS22HH LOW NOISE MODE DISABLED  */
#define LPS22HH_LOW_NOISE_EN       1      /*!< LPS22HH LOW NOISE MODE ENABLED   */

/* LPS22HH output data rate values */
#define  LPS22HH_ODR_1_HZ_FP  1.0f        /*!< LPS22HH ODR 1 Hz            */
#define  LPS22HH_ODR_10_HZ_FP 10.0f       /*!< LPS22HH ODR 10 Hz           */
#define  LPS22HH_ODR_25_HZ_FP 25.0f       /*!< LPS22HH ODR 25 Hz           */
#define  LPS22HH_ODR_50_HZ_FP 50.0f       /*!< LPS22HH ODR 50 Hz           */
#define  LPS22HH_ODR_75_HZ_FP 75.0f       /*!< LPS22HH ODR 75 Hz           */
#define  LPS22HH_ODR_100_HZ_FP  100.0f    /*!< LPS22HH ODR 100 Hz          */
#define  LPS22HH_ODR_200_HZ_FP  200.0f    /*!< LPS22HH ODR 200 Hz          */
#define LPS22HH_MAX_ODR 200.0f       /*!< LPS22HH TEMP MAX ODR        */


lps22hh_drv_status_t lps22hh_drv_init(lps22hh_object_t *p_obj, uint32_t id);
lps22hh_drv_status_t lps22hh_drv_deinit(lps22hh_object_t *p_obj);
lps22hh_drv_status_t lps22hh_drv_read_id(lps22hh_object_t *p_obj, uint8_t *id);
lps22hh_drv_status_t lps22hh_drv_get_init_status(lps22hh_object_t *p_obj, uint8_t *p_status);

lps22hh_drv_status_t lps22hh_drv_enable(lps22hh_object_t *p_obj);
lps22hh_drv_status_t lps22hh_drv_disable(lps22hh_object_t *p_obj);
lps22hh_drv_status_t lps22hh_drv_get_outputdatarate(lps22hh_object_t *p_obj, float_t *odr);
lps22hh_drv_status_t lps22hh_drv_set_outputdatarate(lps22hh_object_t *p_obj, float_t odr);
lps22hh_drv_status_t lps22hh_drv_get_pressure(lps22hh_object_t *p_obj, float_t *value);
lps22hh_drv_status_t lps22hh_drv_get_temperature(lps22hh_object_t *p_obj, float_t *value);
lps22hh_drv_status_t lps22hh_drv_temp_get_drdy_status(lps22hh_object_t *p_obj, uint8_t *p_status);
lps22hh_drv_status_t lps22hh_drv_press_get_drdy_status(lps22hh_object_t *p_obj, uint8_t *p_status);


lps22hh_drv_status_t lps22hh_drv_read_reg(lps22hh_object_t *p_obj, uint8_t reg, uint8_t *p_data);
lps22hh_drv_status_t lps22hh_drv_write_reg(lps22hh_object_t *p_obj, uint8_t reg, uint8_t data);

lps22hh_drv_status_t lps22hh_drv_fifo_get_data(lps22hh_object_t *p_obj, float_t *press, float_t *temp);
lps22hh_drv_status_t lps22hh_drv_fifo_get_fth_status(lps22hh_object_t *p_obj, uint8_t *p_status);
lps22hh_drv_status_t lps22hh_drv_fifo_get_full_status(lps22hh_object_t *p_obj, uint8_t *p_status);
lps22hh_drv_status_t lps22hh_drv_fifo_get_ovr_status(lps22hh_object_t *p_obj, uint8_t *p_status);
lps22hh_drv_status_t lps22hh_drv_fifo_get_level(lps22hh_object_t *p_obj, uint8_t *p_status);
lps22hh_drv_status_t lps22hh_drv_fifo_reset_interrupt(lps22hh_object_t *p_obj, uint8_t interrupt);
lps22hh_drv_status_t lps22hh_drv_fifo_set_interrupt(lps22hh_object_t *p_obj, uint8_t interrupt);
lps22hh_drv_status_t lps22hh_drv_fifo_set_mode(lps22hh_object_t *p_obj, uint8_t mode);
lps22hh_drv_status_t lps22hh_drv_fifo_set_watermark_level(lps22hh_object_t *p_obj, uint8_t watermark);
lps22hh_drv_status_t lps22hh_drv_fifo_stop_on_watermark(lps22hh_object_t *p_obj, uint8_t stop);

lps22hh_drv_status_t lps22hh_drv_set_one_shot(lps22hh_object_t *p_obj);
lps22hh_drv_status_t lps22hh_drv_get_one_shot_status(lps22hh_object_t *p_obj, uint8_t *p_status);

lps22hh_drv_status_t lps22hh_drv_enable_drdy_interrupt(lps22hh_object_t *p_obj);
lps22hh_drv_status_t lps22hh_drv_set_power_mode(lps22hh_object_t *p_obj, uint8_t powermode);
lps22hh_drv_status_t lps22hh_drv_set_filter_mode(lps22hh_object_t *p_obj, uint8_t filtermode);

lps22hh_drv_status_t lps22hh_drv_enable_irq(lps22hh_object_t *p_obj);
lps22hh_drv_status_t lps22hh_drv_disable_irq(lps22hh_object_t *p_obj);

#if defined (LPS22HH_CALLBACKS) && (LPS22HH_CALLBACKS == 1)
lps22hh_drv_status_t lps22hh_drv_register_callback(lps22hh_object_t *pobj, lps22hh_callback_t cb, void *arg);
#endif /* LPS22HH_CALLBACKS */

#ifdef __cplusplus
}
#endif

#endif /* LPS22HH */



