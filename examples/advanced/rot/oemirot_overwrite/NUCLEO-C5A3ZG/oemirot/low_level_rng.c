/**
  ******************************************************************************
  * @file    low_level_rng.c
  * @brief   Low Level Interface module to use STM32 RNG Ip
  *          This file provides mbed-crypto random generataor
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "low_level_rng.h"
#include "boot_hal_utils.h"

extern void Error_Handler(void);

/* Timeout for generation in millisecond */
#define RNG_GENERATION_TIMEOUT   10U

void RNG_Init(void)
{
  uint32_t dummy;
  hal_rng_handle_t *p_hrng;

  p_hrng = mx_rot_rng_init();

  if (p_hrng == NULL)
  {
    Error_Handler();
  }

  /* first random number generated after setting the RNGEN bit must not be used */
  (void)HAL_RNG_GenerateRandomNumber(p_hrng, &dummy, 1U, RNG_GENERATION_TIMEOUT);
}

void RNG_GetBytes(uint8_t *output, size_t length, size_t *output_length)
{
  int32_t ret = 0;
  uint32_t try = 0U;
  uint32_t i = 0U;
  __IO uint8_t random[4U] = {0U};
  uint32_t *p_rnd = (uint32_t *)random;
  *output_length = 0;
  hal_rng_handle_t *p_hrng = mx_rot_rng_gethandle();

  /* Get Random byte */
  while ((*output_length < length) && (ret == 0))
  {
    if (HAL_RNG_GenerateRandomNumber(p_hrng, p_rnd, 1U, RNG_GENERATION_TIMEOUT) != HAL_OK)
    {
      /* retry when random number generated are not immediately available */
      if (try < 3U)
      {
        (void)HAL_RNG_RecoverSeedError(p_hrng);
        try++;
      }
      else
      {
        ret = -1;
      }
    }
    else
    {
      for (i = 0U; (i < ARRAY_SIZE(random)) && (*output_length < length) ; i++)
      {
        *output++ = random[i];
        *output_length += 1;
        random[i] = 0;
      }
    }
  }

  /* Ensure no errors occurred during RNG operation */
  if ((LL_RNG_IsActiveFlag_CECS(RNG) != 0UL)
      || (LL_RNG_IsActiveFlag_SECS(RNG) != 0UL))
  {
    *output_length = 0;
  }
}

void RNG_DeInit(void)
{
  /* Disable the RNG peripheral */
  mx_rot_rng_deinit();
}
