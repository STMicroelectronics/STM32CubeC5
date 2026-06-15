/**
  ******************************************************************************
  * @file    lps22hh.c
  * @brief   LPS22HH driver file.
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

/* Includes ------------------------------------------------------------------*/
#include "lps22hh.h"
#include <string.h>

static lps22hh_drv_status_t lps22hh_drv_set_outputdatarate_when_enabled(lps22hh_object_t *p_obj, float_t odr);
static lps22hh_drv_status_t lps22hh_drv_set_outputdatarate_when_disabled(lps22hh_object_t *p_obj, float_t odr);

/**
  * @brief  Override the LPS22HH read register weak function.
  * @param  p_ctx the context of the reg file
  * @param  reg the device register to write
  * @param  p_data the write data
  * @param  len the length of the write buffer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t lps22hh_read_reg(const stmdev_ctx_t *p_ctx, uint8_t reg, uint8_t *p_data, uint16_t len)
{
  int32_t ret;
  lps22hh_object_t *p_handle = (lps22hh_object_t *)(p_ctx->handle);
  ret = lps22hh_io_read_reg(&(p_handle->io_ctx), reg, p_data, len);
  return ret;
}

/**
  * @brief  Override the LPS22HH write register weak function.
  * @param  p_ctx the context of the reg file
  * @param  reg the device register to write
  * @param  p_data the write data
  * @param  len the length of the write buffer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t lps22hh_write_reg(const stmdev_ctx_t *p_ctx, uint8_t reg, const uint8_t *p_data, uint16_t len)
{
  int32_t ret;
  lps22hh_object_t *p_handle = (lps22hh_object_t *)(p_ctx->handle);
  ret = lps22hh_io_write_reg(&(p_handle->io_ctx), reg, p_data, len);
  return ret;
}

/**
  * @brief  Initialize the LPS22HH sensor.
  * @param  p_obj the device instance object
  * @param  id the device instance id
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_init(lps22hh_object_t *p_obj, uint32_t id)
{
  p_obj->ctx.handle = p_obj;
  p_obj->io_ctx.id = id;
  if (lps22hh_io_init(&(p_obj->io_ctx)) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  /* Disable MIPI I3C(SM) interface */
  if (lps22hh_i3c_interface_set(&(p_obj->ctx), LPS22HH_I3C_DISABLE) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  /* Power down the device, set Low Noise Enable (bit 5), clear One Shot (bit 4) */
  if (lps22hh_data_rate_set(&(p_obj->ctx), LPS22HH_POWER_DOWN) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  /* Disable low-pass filter on LPS22HH pressure data */
  if (lps22hh_lp_bandwidth_set(&(p_obj->ctx), LPS22HH_LPF_ODR_DIV_2) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  /* Set block data update mode */
  if (lps22hh_block_data_update_set(&(p_obj->ctx), PROPERTY_ENABLE) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  /* Set autoincrement for multi-byte read/write */
  if (lps22hh_auto_increment_set(&(p_obj->ctx), PROPERTY_ENABLE) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  p_obj->last_odr = LPS22HH_25_Hz;


  p_obj->is_initialized = 1;

  return LPS22HH_OK;
}

/**
  * @brief  Deinitialize the LPS22HH sensor.
  * @param  p_obj the device instance object
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_deinit(lps22hh_object_t *p_obj)
{

  if (lps22hh_drv_disable(p_obj) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }
  p_obj->is_initialized = 0;
  return LPS22HH_OK;
}

/**
  * @brief  Read the LPS22HH id.
  * @param  p_obj the device instance object
  * @param  id the pointer where the id is written
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_read_id(lps22hh_object_t *p_obj, uint8_t *id)
{
  if (lps22hh_device_id_get(&(p_obj->ctx), id) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  return LPS22HH_OK;
}

/**
  * @brief  Get the LPS22HH initialization status.
  * @param  p_obj the device instance object
  * @param  p_status 1 if initialized, 0 otherwise
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_get_init_status(lps22hh_object_t *p_obj, uint8_t *p_status)
{
  if (p_obj == NULL)
  {
    return LPS22HH_ERROR;
  }

  *p_status = p_obj->is_initialized;

  return LPS22HH_OK;
}

/**
  * @brief  Enable the LPS22HH pressure sensor.
  * @param  p_obj the device instance object
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_enable(lps22hh_object_t *p_obj)
{
  /* check if the component is already enabled */
  if (p_obj->is_enabled == 1u)
  {
    return LPS22HH_OK;
  }

  /* Output data rate selection. */
  if (lps22hh_data_rate_set(&(p_obj->ctx), p_obj->last_odr) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  p_obj->is_enabled = 1;

  return LPS22HH_OK;
}

/**
  * @brief  Disable the LPS22HH pressure sensor.
  * @param  p_obj the device instance object
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_disable(lps22hh_object_t *p_obj)
{
  /* check if the component is already disabled */
  if (p_obj->is_enabled == 0U)
  {
    return LPS22HH_OK;
  }

  /* Check if the LPS22HH sensor is still enable. */
  /* If yes, skip the disable function, if not call disable function */

  /* get current output data rate. */
  if (lps22hh_data_rate_get(&(p_obj->ctx), &p_obj->last_odr) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  /* Output data rate selection - power down. */
  if (lps22hh_data_rate_set(&(p_obj->ctx), LPS22HH_POWER_DOWN) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }


  p_obj->is_enabled = 0;

  return LPS22HH_OK;
}


/**
  * @brief  Set the LPS22HH pressure sensor output data rate.
  * @param  p_obj the device instance object
  * @param  odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_set_outputdatarate(lps22hh_object_t *p_obj, float_t odr)
{
  /* Check if the component is enabled */
  if (p_obj->is_enabled == 1U)
  {
    return lps22hh_drv_set_outputdatarate_when_enabled(p_obj, odr);
  }
  else
  {
    return lps22hh_drv_set_outputdatarate_when_disabled(p_obj, odr);
  }
}

/**
  * @brief  Get the LPS22HH pressure value.
  * @param  p_obj the device instance object
  * @param  value pointer where the pressure value is written
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_get_pressure(lps22hh_object_t *p_obj, float_t *value)
{
  lps22hh_axis1bitu32_t data_raw_pressure;

  (void)memset(data_raw_pressure.u8bit, 0x00, sizeof(int32_t));
  if (lps22hh_pressure_raw_get(&(p_obj->ctx), &data_raw_pressure.u32bit) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  *value = lps22hh_from_lsb_to_hpa(data_raw_pressure.u32bit);

  return LPS22HH_OK;
}

/**
  * @brief  Get the LPS22HH pressure data ready bit value.
  * @param  p_obj the device instance object
  * @param  p_status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_press_get_drdy_status(lps22hh_object_t *p_obj, uint8_t *p_status)
{
  if (lps22hh_press_flag_data_ready_get(&(p_obj->ctx), p_status) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  return LPS22HH_OK;
}

/**
  * @brief  Get the LPS22HH temperature value.
  * @param  p_obj the device instance object
  * @param  value pointer where the temperature value is written
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_get_temperature(lps22hh_object_t *p_obj, float_t *value)
{
  lps22hh_axis1bit16_t data_raw_temperature;

  (void)memset(data_raw_temperature.u8bit, 0x00, sizeof(int16_t));
  if (lps22hh_temperature_raw_get(&(p_obj->ctx), &data_raw_temperature.i16bit) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  *value = lps22hh_from_lsb_to_celsius(data_raw_temperature.i16bit);

  return LPS22HH_OK;
}

/**
  * @brief  Get the LPS22HH temperature data ready bit value.
  * @param  p_obj the device instance object
  * @param  p_status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_temp_get_drdy_status(lps22hh_object_t *p_obj, uint8_t *p_status)
{
  if (lps22hh_temp_flag_data_ready_get(&(p_obj->ctx), p_status) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  return LPS22HH_OK;
}

/**
  * @brief  Get the LPS22HH FIFO data level.
  * @param  p_obj the device instance object
  * @param  press pointer where the temperature value is written
  * @param  temp pointer where the pressure value is written
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_fifo_get_data(lps22hh_object_t *p_obj, float_t *press, float_t *temp)
{
  lps22hh_axis1bitu32_t data_raw_pressure;
  lps22hh_axis1bit16_t data_raw_temperature;

  (void)memset(data_raw_pressure.u8bit, 0x00, sizeof(int32_t));
  if (lps22hh_fifo_pressure_raw_get(&(p_obj->ctx), (uint32_t *)&data_raw_pressure.u32bit) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  *press = lps22hh_from_lsb_to_hpa((uint32_t)data_raw_pressure.u32bit);

  (void)memset(data_raw_temperature.u8bit, 0x00, sizeof(int16_t));
  if (lps22hh_fifo_temperature_raw_get(&(p_obj->ctx), &data_raw_temperature.i16bit) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  *temp = lps22hh_from_lsb_to_celsius(data_raw_temperature.i16bit);

  return LPS22HH_OK;
}

/**
  * @brief  Get the LPS22HH FIFO threshold.
  * @param  p_obj the device instance object
  * @param  p_status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_fifo_get_fth_status(lps22hh_object_t *p_obj, uint8_t *p_status)
{
  if (lps22hh_fifo_wtm_flag_get(&(p_obj->ctx), p_status) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  return LPS22HH_OK;
}

/**
  * @brief  Get the LPS22HH FIFO full status.
  * @param  p_obj the device instance object
  * @param  p_status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_fifo_get_full_status(lps22hh_object_t *p_obj, uint8_t *p_status)
{
  if (lps22hh_fifo_full_flag_get(&(p_obj->ctx), p_status) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  return LPS22HH_OK;
}

/**
  * @brief  Get the LPS22HH FIFO OVR status.
  * @param  p_obj the device instance object
  * @param  p_status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_fifo_get_ovr_status(lps22hh_object_t *p_obj, uint8_t *p_status)
{
  if (lps22hh_fifo_ovr_flag_get(&(p_obj->ctx), p_status) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  return LPS22HH_OK;
}

/**
  * @brief  Get the LPS22HH FIFO data level.
  * @param  p_obj the device instance object
  * @param  p_status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_fifo_get_level(lps22hh_object_t *p_obj, uint8_t *p_status)
{
  if (lps22hh_fifo_data_level_get(&(p_obj->ctx), p_status) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  return LPS22HH_OK;
}

/**
  * @brief  Reset the FIFO interrupt.
  * @param  p_obj the device instance object
  * @param  interrupt The FIFO interrupt to be reset; values: 0 = FTH; 1 = FULL; 2 = OVR
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_fifo_reset_interrupt(lps22hh_object_t *p_obj, uint8_t interrupt)
{
  lps22hh_pin_int_route_t val;

  if (lps22hh_pin_int_route_get(&(p_obj->ctx), &val) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }
  switch (interrupt)
  {
    case 0U:
      val.fifo_th = 0;
      break;
    case 1U:
      val.fifo_full = 0;
      break;
    case 2U:
      val.fifo_ovr = 0;
      break;
    default:
      return LPS22HH_ERROR;
      break;
  }
  if (lps22hh_pin_int_route_set(&(p_obj->ctx), val) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  return LPS22HH_OK;
}

/**
  * @brief  Set the FIFO interrupt.
  * @param  p_obj the device instance object
  * @param  interrupt The FIFO interrupt to be reset; values: 0 = FTH; 1 = FULL; 2 = OVR
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_fifo_set_interrupt(lps22hh_object_t *p_obj, uint8_t interrupt)
{
  lps22hh_pin_int_route_t val;

  if (lps22hh_pin_int_route_get(&(p_obj->ctx), &val) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }
  switch (interrupt)
  {
    case 0U:
      val.fifo_th = 1;
      break;
    case 1U:
      val.fifo_full = 1;
      break;
    case 2U:
      val.fifo_ovr = 1;
      break;
    default:
      return LPS22HH_ERROR;
      break;
  }
  if (lps22hh_pin_int_route_set(&(p_obj->ctx), val) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  return LPS22HH_OK;
}

/**
  * @brief  Set the FIFO mode.
  * @param  p_obj the device instance object
  * @param  mode the FIFO mode to be set
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_fifo_set_mode(lps22hh_object_t *p_obj, uint8_t mode)
{
  lps22hh_f_mode_t fifo_mode;
  fifo_mode = LPS22HH_BYPASS_MODE;
  /* Verify that the passed parameter contains one of the valid values */
  switch (mode)
  {
    case LPS22HH_BYPASS_MODE:
      fifo_mode = LPS22HH_BYPASS_MODE;
      break;
    case LPS22HH_FIFO_MODE:
      fifo_mode = LPS22HH_FIFO_MODE;
      break;
    case LPS22HH_STREAM_MODE:
      fifo_mode = LPS22HH_STREAM_MODE;
      break;
    case LPS22HH_BYPASS_TO_FIFO_MODE:
      fifo_mode = LPS22HH_BYPASS_TO_FIFO_MODE;
      break;
    case LPS22HH_BYPASS_TO_STREAM_MODE:
      fifo_mode = LPS22HH_BYPASS_TO_STREAM_MODE;
      break;
    case LPS22HH_STREAM_TO_FIFO_MODE:
      fifo_mode = LPS22HH_STREAM_TO_FIFO_MODE;
      break;
    default:
      return LPS22HH_ERROR;
      break;
  }
  if (lps22hh_fifo_mode_set(&(p_obj->ctx), fifo_mode) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  return LPS22HH_OK;
}

/**
  * @brief  Set the LPS22HH FIFO data level.
  * @param  p_obj the device instance object
  * @param  watermark the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_fifo_set_watermark_level(lps22hh_object_t *p_obj, uint8_t watermark)
{
  if (lps22hh_fifo_watermark_set(&(p_obj->ctx), watermark) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  return LPS22HH_OK;
}

/**
  * @brief  Set the LPS22HH stop on watermark function.
  * @param  p_obj the device instance object
  * @param  stop the state of stop on watermark function
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_fifo_stop_on_watermark(lps22hh_object_t *p_obj, uint8_t stop)
{
  if (lps22hh_fifo_stop_on_wtm_set(&(p_obj->ctx), stop) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  return LPS22HH_OK;
}

/**
  * @brief  Set the LPS22HH One Shot mode.
  * @param  p_obj the device instance object
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_set_one_shot(lps22hh_object_t *p_obj)
{
  /* Start One Shot Measurement */
  if (lps22hh_data_rate_set(&(p_obj->ctx), LPS22HH_ONE_SHOOT) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  return LPS22HH_OK;
}

/**
  * @brief  Get the LPS22HH One Shot status.
  * @param  p_obj the device instance object
  * @param  p_status pointer to the one shot status
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_get_one_shot_status(lps22hh_object_t *p_obj, uint8_t *p_status)
{
  uint8_t p_da;
  uint8_t t_da;

  /* Get DataReady for pressure */
  if (lps22hh_press_flag_data_ready_get(&(p_obj->ctx), &p_da) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  /* Get DataReady for temperature */
  if (lps22hh_temp_flag_data_ready_get(&(p_obj->ctx), &t_da) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  if ((p_da != 0U) && (t_da != 0U))
  {
    *p_status = 1;
  }
  else
  {
    *p_status = 0;
  }

  return LPS22HH_OK;
}

/**
  * @brief  Enable LPS22HH DRDY interrupt mode.
  * @param  p_obj the device instance object
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_enable_drdy_interrupt(lps22hh_object_t *p_obj)
{
  lps22hh_pin_int_route_t val;

  /* Enable Interrupt DRDY on INT1 */
  if (lps22hh_pin_int_route_get(&(p_obj->ctx), &val) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }
  val.drdy_pres = 1;
  if (lps22hh_pin_int_route_set(&(p_obj->ctx), val) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  return LPS22HH_OK;
}

/**
  * @brief  Set the LPS22HH power mode.
  * @param  p_obj the device instance object
  * @param  powermode mode to be set
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_set_power_mode(lps22hh_object_t *p_obj, uint8_t powermode)
{
  lps22hh_ctrl_reg2_t ctrl_reg2;

  if (lps22hh_read_reg(&(p_obj->ctx), LPS22HH_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  if (powermode == (uint8_t)1)
  {
    ctrl_reg2.low_noise_en = LPS22HH_LOW_NOISE_EN;
  }
  else
  {
    ctrl_reg2.low_noise_en = LPS22HH_LOW_NOISE_DIS;
  }

  if (lps22hh_write_reg(&(p_obj->ctx), LPS22HH_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  return LPS22HH_OK;
}

/**
  * @brief  Set the LPS22HH filter mode.
  * @param  p_obj the device instance object
  * @param  filtermode to be set
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_set_filter_mode(lps22hh_object_t *p_obj, uint8_t mode)
{
  lps22hh_lpfp_cfg_t filter_mode  = LPS22HH_LPF_ODR_DIV_2;
  /* Verify that the passed parameter contains one of the valid values */
  switch (mode)
  {
    case LPS22HH_LPF_ODR_DIV_2:
      filter_mode = LPS22HH_LPF_ODR_DIV_2;
      break;
    case LPS22HH_LPF_ODR_DIV_9:
      filter_mode = LPS22HH_LPF_ODR_DIV_9;
      break;
    case LPS22HH_LPF_ODR_DIV_20:
      filter_mode = LPS22HH_LPF_ODR_DIV_20;
      break;
    default:
      return LPS22HH_ERROR;
      break;
  }
  if (lps22hh_lp_bandwidth_set(&(p_obj->ctx), filter_mode) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }
  return LPS22HH_OK;
}

/**
  * @brief  Wrap Read register component function to Bus IO function.
  * @param  p_obj the device instance object
  * @param  reg the register address
  * @param  p_data pointer where the register value is written
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_read_reg(lps22hh_object_t *p_obj, uint8_t reg, uint8_t *p_data)
{
  if (lps22hh_read_reg(&(p_obj->ctx), reg, p_data, 1) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  return LPS22HH_OK;
}

/**
  * @brief  Override the LPS22HH write register weak function.
  * @param  p_obj the device instance object
  * @param  reg the device register to write
  * @param  p_data data to be written
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_write_reg(lps22hh_object_t *p_obj, uint8_t reg, const uint8_t p_data)
{
  if (lps22hh_write_reg(&(p_obj->ctx), reg, &p_data, 1) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  return LPS22HH_OK;
}

/**
  * @brief  Get the output data rate.
  * @param  p_obj the device instance object
  * @param  odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_get_outputdatarate(lps22hh_object_t *p_obj, float_t *odr)
{
  lps22hh_odr_t odr_low_level;

  if (lps22hh_data_rate_get(&(p_obj->ctx), &odr_low_level) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  switch (odr_low_level)
  {
    case LPS22HH_POWER_DOWN:
      *odr = 0.0f;
      break;

    case LPS22HH_1_Hz:
      *odr = LPS22HH_ODR_1_HZ_FP;
      break;

    case LPS22HH_10_Hz:
      *odr = LPS22HH_ODR_10_HZ_FP;
      break;

    case LPS22HH_25_Hz:
      *odr = LPS22HH_ODR_25_HZ_FP;
      break;

    case LPS22HH_50_Hz:
      *odr = LPS22HH_ODR_50_HZ_FP;
      break;

    case LPS22HH_75_Hz:
      *odr = LPS22HH_ODR_75_HZ_FP;
      break;

    case LPS22HH_100_Hz:
      *odr = LPS22HH_ODR_100_HZ_FP;
      break;

    case LPS22HH_200_Hz:
      *odr = LPS22HH_ODR_200_HZ_FP;
      break;

    default:
      return LPS22HH_ERROR;
      break;
  }

  return LPS22HH_OK;
}

/**
  * @brief  Set output data rate.
  * @param  p_obj the device instance object
  * @param  odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
static lps22hh_drv_status_t lps22hh_drv_set_outputdatarate_when_enabled(lps22hh_object_t *p_obj, float_t odr)
{
  lps22hh_odr_t new_odr;

  new_odr = (odr <=   1.0f) ? LPS22HH_1_Hz
            : (odr <=  10.0f) ? LPS22HH_10_Hz
            : (odr <=  25.0f) ? LPS22HH_25_Hz
            : (odr <=  50.0f) ? LPS22HH_50_Hz
            : (odr <=  75.0f) ? LPS22HH_75_Hz
            : (odr <= 100.0f) ? LPS22HH_100_Hz
            :                   LPS22HH_200_Hz;

  if (lps22hh_data_rate_set(&(p_obj->ctx), new_odr) != LPS22HH_OK)
  {
    return LPS22HH_ERROR;
  }

  return LPS22HH_OK;
}

/**
  * @brief  Set output data rate when disabled.
  * @param  p_obj the device instance object
  * @param  odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
static lps22hh_drv_status_t lps22hh_drv_set_outputdatarate_when_disabled(lps22hh_object_t *p_obj, float_t odr)
{
  p_obj->last_odr = (odr <=   1.0f) ? LPS22HH_1_Hz
                    : (odr <=  10.0f) ? LPS22HH_10_Hz
                    : (odr <=  25.0f) ? LPS22HH_25_Hz
                    : (odr <=  50.0f) ? LPS22HH_50_Hz
                    : (odr <=  75.0f) ? LPS22HH_75_Hz
                    : (odr <= 100.0f) ? LPS22HH_100_Hz
                    :                   LPS22HH_200_Hz;

  return LPS22HH_OK;
}

/**
  * @brief  Enable the interrupt.
  * @param  p_obj the device instance object
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_enable_irq(lps22hh_object_t *p_obj)
{
  if (lps22hh_io_enable_irq(&(p_obj->io_ctx)) != 0)
  {
    return LPS22HH_ERROR;
  }
  return LPS22HH_OK;
}

/**
  * @brief  Disable the interrupt.
  * @param  p_obj the device instance object
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_disable_irq(lps22hh_object_t *p_obj)
{
  if (lps22hh_io_disable_irq(&(p_obj->io_ctx)) != 0)
  {
    return LPS22HH_ERROR;
  }
  return LPS22HH_OK;
}

#if defined (LPS22HH_CALLBACKS) && (LPS22HH_CALLBACKS == 1)

static void lps22hh_io_callback(lps22hh_io_t *pio, void *arg)
{
  (void)pio;
  lps22hh_object_t *pobj = (lps22hh_object_t *)arg;

  pobj->cb_ctx_irq.callback(pobj, pobj->cb_ctx_irq.arg);
}

/**
  * @brief  Register the callback function for interrupt events.
  * @param  p_obj the device instance object
  * @param  cb the callback function
  * @param  arg the optional argument given to the callback
  * @retval 0 in case of success, an error code otherwise
  */
lps22hh_drv_status_t lps22hh_drv_register_callback(lps22hh_object_t *p_obj, lps22hh_callback_t cb, void *arg)
{
  p_obj->cb_ctx_irq.callback = cb;
  p_obj->cb_ctx_irq.arg = arg;

  if (lps22hh_io_register_callback(&(p_obj->io_ctx), lps22hh_io_callback, (void *)p_obj) == 0)
  {
    return LPS22HH_OK;
  }
  else
  {
    return LPS22HH_ERROR;
  }
}
#endif /* LPS22HH_CALLBACKS */

