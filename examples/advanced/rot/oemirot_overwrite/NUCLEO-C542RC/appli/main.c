/**
  ******************************************************************************
  * @file    main.c
  * @brief   Main application file.
  *          This application demonstrates Secure Services
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
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "mx_hal_def.h"
#include "mx_system.h"
#include "main.h"
#include "com.h"
#include "common.h"
#include "data.h"
#include "Driver_Flash.h"
#include "mx_basic_stdio_app.h"
#include "fw_update_app.h"
#include "appli_flash_layout.h"

extern ARM_DRIVER_FLASH FLASH_PRIMARY_DEV_NAME;
extern ARM_DRIVER_FLASH FLASH_SECONDARY_DEV_NAME;

/* Private includes ----------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
#define IMAGE_VERSION_ADDRESS (FLASH_BASE + FLASH_PRIMARY_APP_VERSION_OFFSET)
#define DATA_VERSION_ADDRESS (FLASH_BASE +  FLASH_PRIMARY_DATA_VERSION_OFFSET)

/* Private variables ---------------------------------------------------------*/
uint8_t *pUserAppId;
const uint8_t UserAppId = 'A';
uint64_t time;
uint32_t end;
const version_t *imgver = (const version_t *)IMAGE_VERSION_ADDRESS;
const version_t *datver = (const version_t *)DATA_VERSION_ADDRESS;
/* External References -------------------------------------------------------*/
extern const VECTOR_TABLE_Type __VECTOR_TABLE[];
extern void SystemInit(void);
extern __NO_RETURN void __PROGRAM_START(void);

extern uint32_t __STACK_LIMIT;

/* Private function prototypes -----------------------------------------------*/
void FW_APP_PrintMainMenu(void);
void FW_APP_Run(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /*  Set example to const : this const changes in binary without rebuild */
  pUserAppId = (uint8_t *)&UserAppId;

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  if (mx_system_init() != SYSTEM_OK)
  {
    return (-1);
  }

  /* Initialize basic_stdio separately, but after system init. */
  mx_basic_stdio_init();

  /* Configure Communication module */
  COM_Init();

  printf("\r\n======================================================================");
  printf("\r\n=              (C) COPYRIGHT 2026 STMicroelectronics                 =");
  printf("\r\n=                                                                    =");
  printf("\r\n=                          User App #%c                               =", *pUserAppId);
  printf("\r\n======================================================================");
  printf("\r\n\r\n");
  printf("App image version: %u.%u.%u\r\n", imgver->iv_major, imgver->iv_minor, imgver->iv_revision);
#if (DATA_IMAGE_NUMBER == 1)
  printf("Data image version: %u.%u.%u\r\n", datver->iv_major, datver->iv_minor, datver->iv_revision);
#endif /* (DATA_IMAGE_NUMBER == 1) */
#if defined(__ARMCC_VERSION)
  printf("App SystemCoreClock: %u Hz\r\n", SystemCoreClock);
#else
  printf("App SystemCoreClock: %lu Hz\r\n", SystemCoreClock);
#endif /* __ARMCC_VERSION */

  if (FLASH_PRIMARY_DEV_NAME.Initialize(NULL) != ARM_DRIVER_OK)
  {
    printf("Driver Flash Init : Failed");
  }

#if (FLASH_PRIMARY_DEV_NAME != FLASH_SECONDARY_DEV_NAME)
  if (FLASH_SECONDARY_DEV_NAME.Initialize(NULL) != ARM_DRIVER_OK)
  {
    printf("Driver External Flash Init : Failed");
  }
#endif /* FLASH_PRIMARY_DEV_NAME != FLASH_SECONDARY_DEV_NAME */

  /* User App firmware runs*/
  FW_APP_Run();

  /* Infinite loop */

  while (1)
  {
  }
}

/**
  * @brief  Display the TEST Main Menu choices on HyperTerminal.
  * @param  None.
  * @retval None.
  */
void FW_APP_PrintMainMenu(void)
{
  printf("\r\n======================== Main menu ========================\r\n\n");
  printf("  New firmware image ------------------------------------ 1\r\n\n");
#if (DATA_IMAGE_NUMBER == 1)
  printf("  Read data image --------------------------------------- 2\r\n\n");
#endif /* (DATA_IMAGE_NUMBER == 1) */
  printf("  Selection :\r\n\n");
}

/**
  * @brief  Display the TEST Main Menu choices on HyperTerminal.
  * @param  None.
  * @retval None.
  */
void FW_APP_Run(void)
{
  uint8_t key = 0U;

  /*##1- Print Main Menu message*/
  FW_APP_PrintMainMenu();

  while (1U)
  {
    /* Clean the input path */
    COM_Flush();

    /* Receive key */
    if (COM_Receive(&key, 1U, RX_TIMEOUT) == HAL_OK)
    {
      switch (key)
      {
        case '1' :
          FW_UPDATE_Run();
          break;
#if (DATA_IMAGE_NUMBER == 1)
        case '2' :
          DATA_Run();
          break;
#endif /* (DATA_IMAGE_NUMBER == 1) */
        default:
          printf("Invalid number !\r");
          break;
      }

      /* Print Main Menu message */
      FW_APP_PrintMainMenu();
    }
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }

}
#endif /* USE_FULL_ASSERT */
