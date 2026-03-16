/**
  ******************************************************************************
  * @file    data.c
  * @brief   data module.
  *          This file provides examples of Data access.
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
#include <string.h>
#include "data.h"
#include "com.h"
#include "appli_flash_layout.h"

#if (DATA_IMAGE_NUMBER == 1)

static void DATA_Print_menu(void);
static void DATA_Display(void);

/**
  * @brief  Display the Data Main Menu choices on HyperTerminal.
  * @param  None.
  * @retval None.
  */
void DATA_Run(void)
{
  uint8_t key = 0;
  uint8_t exit = 0;

  /* Print Data welcome message */
  DATA_Print_menu();

  while (exit == 0U)
  {
    key = 0U;

    /* Clean the user input path */
    COM_Flush();

    /* Receive key */
    if (COM_Receive(&key, 1U, COM_UART_TIMEOUT_MAX) == HAL_OK)
    {
      switch (key)
      {
        case '1' :
          DATA_Display();
          break;
        case 'x':
          exit = 1;
          break;
        default:
          printf("Invalid number !\r");
          break;
      }
      DATA_Print_menu();
    }
  }
}

/**
  * @brief  Display the Data Menu choices on HyperTerminal.
  * @param  None.
  * @retval None.
  */
static void DATA_Print_menu(void)
{
  printf("\r\n================== Data image menu ==================\r\n\n");
  printf("  Display data from data image -------------------------- 1\r\n\n");
  printf("  Previous menu ----------------------------------------- x\r\n\n");
}

/**
  * @brief  Display the Data on HyperTerminal.
  * @param  None.
  * @retval None.
  */
static void DATA_Display(void)
{
  uint8_t *data1;
  data1 = (uint8_t *)(FLASH_BASE + FLASH_PRIMARY_DATA_SLOT_OFFSET + DATA_IMAGE_HEADER_SIZE);

  unsigned long data_words[4];
  uint32_t i;

  for (i = 0; i < 4; i++)
  {
    uint32_t base = 4 * i;
    data_words[i] = ((unsigned long)data1[base + 0] << 24) |
                    ((unsigned long)data1[base + 1] << 16) |
                    ((unsigned long)data1[base + 2] << 8)  |
                    ((unsigned long)data1[base + 3] << 0);
  }

  printf("  -- Data: %08lX%08lX%08lX%08lX..\r\n\n", data_words[0], data_words[1], data_words[2], data_words[3]);
}

#endif /* (DATA_IMAGE_NUMBER == 1) */
