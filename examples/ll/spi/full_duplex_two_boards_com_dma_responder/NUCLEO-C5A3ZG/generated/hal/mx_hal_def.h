/**
  ******************************************************************************
  * @file           : mx_hal_def.h
  * @brief          : Exporting peripherals initialization
  *                   Include entry for the target folder to the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_stm32c5xx_hal_drivers_license.md file
  * in the same directory as the generated code.
  * If no mx_stm32c5xx_hal_drivers_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_HAL_DEF_H
#define MX_HAL_DEF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mx_def.h"
#include "stm32_ll.h"
#include "mx_cortex_mpu.h"
#include "mx_cortex_nvic.h"
#include "mx_icache.h"
#include "mx_rcc.h"
#include "mx_spi1.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
  /* *************************************************************
    Cortex_MPU (LL): No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for CMSIS instance
    ************************************************************* */

  /* *************************************************************
    Cortex_NVIC (LL): No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for CMSIS instance
    ************************************************************* */

  /* *************************************************************
    ICACHE (LL): No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for CMSIS instance
    ************************************************************* */

  /* *************************************************************
    RCC (LL): No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for CMSIS instance
    ************************************************************* */

  /* ***********************************************************
                Aliases to SPI1 used in LL
    *********************************************************** */
#define MX_SPIx  SPI1

#define MX_LPDMAX_CH1Y_CH  LL_LPDMA1_CH0

#define MX_LPDMAX_CH2Y_CH  LL_LPDMA1_CH1

/* Exported macros -----------------------------------------------------------*/

/* ########### Aliases to initialization functions ########### */

  /* *************************************************************
    Cortex_MPU: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_cortex_mpu_init
    ************************************************************* */

  /* *************************************************************
    Cortex_NVIC: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_cortex_nvic_init
    ************************************************************* */

  /* *************************************************************
    ICACHE: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_icache_init
    ************************************************************* */

  /* *************************************************************
    RCC: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_rcc_init
    ************************************************************* */

  /* ***********************************************************
      SPI1: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_spi1 with LL layer
  *         Name of the User label:
  *                   MX_SPIx
  * @retval SPI1 Pointer on the SPI1
  */
#define mx_spix_init mx_spi1_init

/* ########################################################### */

/* ########### Aliases to De-Initialization functions ########### */

  /* *************************************************************
    Cortex_MPU: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_cortex_mpu_deinit
    ************************************************************* */

  /* *************************************************************
    Cortex_NVIC: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_cortex_nvic_deinit
    ************************************************************* */

  /* *************************************************************
    ICACHE: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_icache_deinit
    ************************************************************* */

  /* ***********************************************************
      SPI1: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_spi1 with LL layer
  *         Name of the User label:
  *                   MX_SPIx
  */
#define mx_spix_deinit mx_spi1_deinit

/* ########################################################### */

/* ########### Aliases to get HAL handle functions ########### */

/* ########################################################### */

/* ########### Aliases to get IRQ Handlers functions ########### */
  /* *************************************************************
    SPI1: SPI1 global interrupt
    Alias on SPI1_IRQHandler and SPI1_IRQn
    ************************************************************* */
#define mx_spix_irqhandler SPI1_IRQHandler
#define MX_SPIX_IRQN       SPI1_IRQn

  /* *************************************************************
    SPI1 / LPDMA1_CH0: LPDMA1 channel0 global interrupt
    Alias on LPDMA1_CH0_IRQHandler and LPDMA1_CH0_IRQn
    ************************************************************* */
#define mx_lpdmax_ch1y_irqhandler LPDMA1_CH0_IRQHandler
#define MX_LPDMAX_CH1Y_IRQN       LPDMA1_CH0_IRQn

  /* *************************************************************
    SPI1 / LPDMA1_CH1: LPDMA1 channel1 global interrupt
    Alias on LPDMA1_CH1_IRQHandler and LPDMA1_CH1_IRQn
    ************************************************************* */
#define mx_lpdmax_ch2y_irqhandler LPDMA1_CH1_IRQHandler
#define MX_LPDMAX_CH2Y_IRQN       LPDMA1_CH1_IRQn

  /* ########################################################### */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_HAL_DEF_H */
