/**
  ******************************************************************************
  * @file    lps22hh_io.c
  * @brief   LPS22HH IO driver file for I2C.
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

#include "lps22hh_io.h"


/**
  * @brief  Initialize the LPS22HH IO I2C flavor.
  * @param  pio the IO instance object
  * @retval 0 in case of success, an error code otherwise
  */
int32_t __weak lps22hh_io_init(lps22hh_io_t *pio)
{
  (void) pio;
  return 0;
}

/**
  * @brief  The write register function for LPS22HH IO I2C flavor.
  * @param  pio the IO instance object
  * @param  reg the device register to write
  * @param  pdata the write data
  * @param  len the length of the write buffer
  * @retval 0 in case of success, an error code otherwise
  */

int32_t lps22hh_io_write_reg(lps22hh_io_t *pio, const uint8_t reg, const uint8_t *pdata, uint16_t len)
{
  int32_t ret = 0;
  if (HAL_I2C_MASTER_MemWrite(pio->hi2c, pio->addr, reg, HAL_I2C_MEM_ADDR_8BIT,
                              pdata, len, LPS22HH_I2C_POLL_TIMEOUT) != HAL_OK)
  {
    ret = -1;
  }
  return ret;
}

/**
  * @brief  The read register function for LPS22HH IO I2C flavor.
  * @param  pio the IO instance object
  * @param  reg the device register to read
  * @param  pdata the pointer where the read data are written
  * @param  len the length of the read buffer
  * @retval 0 in case of success, an error code otherwise
  */
int32_t lps22hh_io_read_reg(lps22hh_io_t *pio, const uint8_t reg, uint8_t *pdata, uint16_t len)
{
  int32_t ret = 0;
  if (HAL_I2C_MASTER_MemRead(pio->hi2c, pio->addr, reg, HAL_I2C_MEM_ADDR_8BIT,
                             pdata, len, LPS22HH_I2C_POLL_TIMEOUT) != HAL_OK)
  {
    ret = -1;
  }
  return ret;
}

/**
  * @brief  Enable the IRQ line and start the EXTI.
  * @param  pio the IO instance object
  * @retval 0 in case of success, an error code otherwise
  */
int32_t lps22hh_io_enable_irq(lps22hh_io_t *pio)
{
  hal_exti_handle_t *p_exti = pio->hexti;
  int32_t ret_val = -1;
  if (p_exti != NULL)
  {
    ret_val = 0;
    /* Clear interrupt pending bit */
    HAL_EXTI_ClearPending(p_exti, HAL_EXTI_TRIGGER_RISING);

    /* Start the selected EXTI line on Interrupt Mode. */
    if (HAL_EXTI_Enable(p_exti, HAL_EXTI_MODE_INTERRUPT) != HAL_OK)
    {
      ret_val = -1;
    }
  }
  return ret_val;
}

/**
  * @brief  Disable the IRQ line and clear the interrupt pending bit.
  * @param  pio the IO instance object
  * @retval 0 in case of success, an error code otherwise
  */
int32_t lps22hh_io_disable_irq(lps22hh_io_t *pio)
{
  hal_exti_handle_t *p_exti = pio->hexti;
  int32_t ret_val = -1;

  if (p_exti != NULL)
  {
    ret_val = 0;
    if (HAL_EXTI_Disable(p_exti) != HAL_OK)
    {
      ret_val = -1;
    }
    /* Clear interrupt pending bit */
    HAL_EXTI_ClearPending(p_exti, HAL_EXTI_TRIGGER_RISING);

  }
  return ret_val;
}

#if defined (LPS22HH_CALLBACKS) && (LPS22HH_CALLBACKS == 1)

static void lps22hh_exti_rising_callback(hal_exti_handle_t *p_exti, hal_exti_trigger_t trigger)
{
  lps22hh_io_t *pio = (lps22hh_io_t *)HAL_EXTI_GetUserData(p_exti);
  switch (trigger)
  {
    case HAL_EXTI_TRIGGER_RISING:
      pio->exti_cb_ctx.io_callback(pio, pio->exti_cb_ctx.arg);
      break;
    default:
      break;
  }
}

/**
  * @brief  Registers a callback on the IT pin of the LPS22HH.
  * @param  pio the IO instance object
  * @param  io_cb the callback function to be registered
  * @param  arg the optional argument given to the callback
  */
int32_t lps22hh_io_register_callback(lps22hh_io_t *pio, lps22hh_io_callback_t io_cb, void *arg)
{
  hal_exti_handle_t *p_exti = pio->hexti;
  int32_t ret_val = -1;

  if (p_exti != NULL)
  {
    ret_val = 0;
    pio->exti_cb_ctx.io_callback = io_cb;
    pio->exti_cb_ctx.arg = arg;
    HAL_EXTI_SetUserData(p_exti, pio);

    if (HAL_EXTI_RegisterTriggerCallback(p_exti, lps22hh_exti_rising_callback) != HAL_OK)
    {
      ret_val = -1;
    }
  }
  return ret_val;
}

#endif /* LPS22HH_CALLBACKS */
