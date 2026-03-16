/**
  ******************************************************************************
  * @file           : mx_hal_def.h
  * @brief          : Exporting peripherals initialization
  *                   Include entry for the target folder to the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32_hal.h"
#include "mx_gpio_default.h"
#include "mx_usb_drd_fs.h"
#include "mx_usart2.h"
#include "mx_tim6.h"
#include "mx_cortex_mpu.h"
#include "mx_icache.h"
#include "mx_crs.h"
#include "mx_rcc.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/


/* ########### Aliases to initialization functions ########### */

/* *********************************************************** */
/*        ICACHE_MyICACHE_1: aliases for initialization functions */
/* *********************************************************** */

/**
  * @brief  Initialize the mx_icache_hal with HAL layer
  *         Name of the User label:
  *                   MyICACHE_1
  * @retval hal_icache_handle_t Pointer on the handle on the ICACHE instance
  */
#define myicache_1_init mx_icache_hal_init


/* *********************************************************** */
/*        USB_DRD_FS: aliases for initialization functions     */
/* *********************************************************** */

/**
  * @brief  Initialize the mx_pcd_usb_drd_fs_cfg1_init with HAL layer
  *         Name of the User label:
  *                   mx_example_pcd
  * @retval hal_pcd_handle_t Pointer on the handle on the USB_DRD_FS instance
  */
#define mx_example_pcd_init           mx_pcd_usb_drd_fs_cfg1_init

/* *********************************************************** */
/*        gpio_default_MyGPIO_1: aliases for initialization functions */
/* *********************************************************** */

/**
  * @brief  Initialize the mx_gpio_default_cfg1_hal with HAL layer
  *         Name of the User label:
  *                   MyGPIO_1
  *                   mx_status_led
  * @retval hal_gpio_handle_t Pointer on the handle on the gpio_default instance
  */
#define mygpio_1_init mx_gpio_default_cfg1_hal_init

/* ########################################################### */

/* ########### Aliases to De-Initialization functions ########### */

/* *********************************************************** */
/*        USB_DRD_FS: aliases for De-Initialization functions */
/* *********************************************************** */

/**
  * @brief  De-Initialize the mx_pcd_USB_DRD_FS_cfg1_init with HAL layer
  *         Name of the User label:
  *                   mx_example_pcd
  */
#define mx_example_pcd_deinit mx_pcd_usb_drd_fs_cfg1_deinit

/* *********************************************************** */
/*        gpio_default_MyGPIO_1: aliases for De-Initialization functions */
/* *********************************************************** */

/**
  * @brief  De-Initialize the mx_gpio_default_cfg1_hal with HAL layer
  *         Name of the User label:
  *                   MyGPIO_1
  */
#define mygpio_1_deinit mx_gpio_default_cfg1_hal_deinit

/* ########################################################### */

/* ########### Aliases to get HAL handle functions ########### */


/* *********************************************************** */
/*        USB_DRD_FS: aliases for get handle functions */
/* *********************************************************** */

/**
  * @brief  Get the HAL handle for PCD
  *         Name of the User label:
  *                   mx_example_pcd
  * @retval hal_pcd_handle_t Pointer on the handle on the CRC instance
  */
#define mx_example_pcd_gethandle                mx_usb_pcd_gethandle

/* *********************************************************** */
/*        usart2_instance2: aliases for initialization functions */
/* *********************************************************** */

/**
  * @brief  Initialize the mx_usart2_cfg1_hal_uart with HAL layer
  *         Name of the User label:
  *                   mx_usart2
  *         Name of the Config label:
  *
  * @retval hal_uart_handle_t Pointer on the handle on the usart2 instance
  */
#define mx_example_uart_init          mx_usart2_hal_uart_init

/* *********************************************************** */
/*        usart2_instance2: aliases for De-Initialization functions */
/* *********************************************************** */

/**
  * @brief  De-Initialize the mx_usart2_cfg1_hal_uart with HAL layer
  *         Name of the User label:
  *                   mx_usart2
  *         Name of the Config label:
  *
  */
#define mx_example_uart_deinit        mx_usart2_hal_uart_deinit

/* *********************************************************** */
/*        usart2_instance2: aliases for get handle functions */
/* *********************************************************** */

/**
  * @brief  Get the HAL handle for usart2
  *         Name of the User label:
  *                   mx_usart2
  * @retval hal_uart_handle_t Pointer on the handle on the usart2 instance
  */
#define mx_example_uart_gethandle mx_usart2_hal_uart_gethandle

/* ########################################################### */

/* ########################################################### */
/* *********************************************************** */
/*        tim6_instance1: aliases for initialization functions */
/* *********************************************************** */

/**
  * @brief  Initialize the mx_tim6_cfg1_hal with HAL layer
  *         Name of the User label:
  *                   tim_timebase
  *         Name of the Config label:
  *                   1ms
  * @retval hal_tim_handle_t Pointer on the handle on the TIM6 instance
  */
#define tim_timebase_1ms_init mx_tim6_cfg1_hal_init


/**
  * @brief  Initialize the mx_tim6_cfg2_hal with HAL layer
  *         Name of the User label:
  *                   tim_timebase
  *         Name of the Config label:
  *                   10ms
  * @retval hal_tim_handle_t Pointer on the handle on the TIM6 instance
  */
#define tim_timebase_10ms_init mx_tim6_cfg2_hal_init


/**
  * @brief  Initialize the mx_tim6_cfg3_hal with HAL layer
  *         Name of the User label:
  *                   tim_timebase
  *         Name of the Config label:
  *                   100ms
  * @retval hal_tim_handle_t Pointer on the handle on the TIM6 instance
  */
#define tim_timebase_100ms_init mx_tim6_cfg3_hal_init


/* ########################################################### */

/* ########### Aliases to De-Initialization functions ########### */

/* *********************************************************** */
/*        tim6_instance1: aliases for De-Initialization functions */
/* *********************************************************** */
/* *********************************************************** */
/*        ICACHE_MyICACHE_1: aliases for De-Initialization functions */
/* *********************************************************** */

/**
  * @brief  De-Initialize the mx_icache_hal with HAL layer
  *         Name of the User label:
  *                   MyICACHE_1
  */
#define myicache_1_deinit mx_icache_hal_deinit
/**
  * @brief  De-Initialize the mx_tim6_cfg1_hal with HAL layer
  *         Name of the User label:
  *                   tim_timebase
  *         Name of the Config label:
  *                   1ms
  */
#define tim_timebase_1ms_deinit mx_tim6_cfg1_hal_deinit


/**
  * @brief  De-Initialize the mx_tim6_cfg2_hal with HAL layer
  *         Name of the User label:
  *                   tim_timebase
  *         Name of the Config label:
  *                   10ms
  */
#define tim_timebase_10ms_deinit mx_tim6_cfg2_hal_deinit


/**
  * @brief  De-Initialize the mx_tim6_cfg3_hal with HAL layer
  *         Name of the User label:
  *                   tim_timebase
  *         Name of the Config label:
  *                   100ms
  */
#define tim_timebase_100ms_deinit mx_tim6_cfg3_hal_deinit


/* *********************************************************** */
/*        tim6_instance1: aliases for get handle functions */
/* *********************************************************** */

/**
  * @brief  Get the HAL handle for TIM6
  *         Name of the User label:
  *                   tim_timebase
  * @retval hal_tim_handle_t Pointer on the handle on the TIM6 instance
  */
#define tim_timebase_gethandle mx_tim6_hal_gethandle

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_HAL_DEF_H */
