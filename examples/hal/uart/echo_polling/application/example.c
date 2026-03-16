/**
  ******************************************************************************
  * file           : example.c
  * brief          : UART communication with re-routed C library functions,
  *                  printf and getchar, in polling mode. The user interaction
  *                  via a terminal emulator is required.
  ******************************************************************************
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
#include "example.h"
#include <stdio.h> /* C library input/output functions */
#include <string.h> /* importing memcmp and memset functions */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* @user: Set the timeout period, in milliseconds, of the UART on reception of a character from the terminal input */
#define RX_TIMEOUT 10000U
#define TX_TIMEOUT 1000U /* Timeout period, in milliseconds, of the UART on transmission of a character */
/** The maximum size of the reception buffer in bytes. Also, the maximum length of a terminal input line.
  * @user: Set the maximum size of the buffer.
  */
#define MAX_BUFFER_SIZE 26U
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_uart_handle_t *pUART; /* pointer referencing the UART handle from the generated code */
uint8_t RxBuffer[MAX_BUFFER_SIZE] = ""; /* Reception buffer */
/* Private function prototypes -----------------------------------------------*/

/** Function-like macros redefining low-level library functions (hooks) to enable direct use of high-level library
  * functions in the C library, precisely printf and getchar, under all the supported toolchains: GNU, IAR EWARM, AC6.
  * @user: Copy-paste the prototypes of the functions with their definitions in your application.
  */
/* WRITE and READ PROTOTYPES */
#if defined(__GNUC__) && !defined(__clang__) /* GNU Compiler and not __clang__ */
int _write(int file, char *ptr, int len);
#define WRITE_PROTOTYPE(file, ptr, len) int _write(int file, char *ptr, int len)
#define READ_PROTOTYPE(void) int getchar(void)

#elif defined(__ICCARM__) && (__VER__ >= 9020001) /* IAR Compiler (v.>=9.20.1) */
#include <LowLevelIOInterface.h>
#define WRITE_PROTOTYPE(file, ptr, len) size_t __write(int32_t file, const uint8_t *ptr, size_t len)

#endif /* end WRITE and READ PROTOTYPE */


/** ########## Step 1 ##########
  * The init of UART is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status;

  pUART = mx_example_uart_init();

  if (pUART != NULL)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }
  else
  {
    return_status = EXEC_STATUS_ERROR;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  uint8_t num_char = 0; /* Number of received 1-byte characters per input line */
  int8_t data;
  app_status_t return_status = EXEC_STATUS_UNKNOWN;

  /** ########## Step 2 ##########
    * Receive characters on the Rx line of the UART, in polling mode, using the C library getchar function and store
    * them in RxBuffer.
    */
  printf("\r\n\nPlease use the input line below to send data to the device\n");
  printf("\r\n[input] << ");
  fflush(stdout); /* Flush the output buffer to see the text on the terminal */

  /* Reception loop */
  while ((num_char < MAX_BUFFER_SIZE) && (return_status == EXEC_STATUS_UNKNOWN))
  {
    /** Receive a character from the terminal input line.
      * Attention: Do not forget to enable the local echo feature in your terminal emulator to see the characters when
      * typing in the input line.
      */
    data = getchar();

    /* Check the received character */
    if ((data == '\n') || (data == '\r') || (data == '\b'))
    {
      /** The detection of new line ('\n'), carriage return ('\r'), or backspace ('\b') characters marks the end
        * of the reception loop.
        */
      return_status = EXEC_STATUS_OK;
    }
    else if (data >= 0)
    {
      /* Store the character in the buffer */
      RxBuffer[num_char] = data;
      /* Increment the counter of received characters */
      num_char++;
    }
    else
    {
      /* Report the reception error */
      printf("\r\n\n[error] >> Libc getchar function returned an ERROR.\r\n");
      printf("\r\n[help]  >> The error can be a UART HAL_TIMEOUT.\r\n");
      return_status = EXEC_STATUS_ERROR;
    } /* end of character check */
  } /* end while */

  if ((return_status == EXEC_STATUS_OK) || (num_char == MAX_BUFFER_SIZE))
  {
    /** ########## Step 3 ##########
      * Transmit, in polling mode, the RxBuffer using the C library printf function.
      */
    if (printf("\r\n[echo]  >> %.*s\r\n", num_char, RxBuffer) < 0)
    {
      return_status = EXEC_STATUS_ERROR;
    }
    else
    {
      /* Clear the RxBuffer before starting a new reception */
      memset(RxBuffer, 0, num_char);
    }
  }

  return return_status;
} /* end app_process */


/** Deinitialize UART before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_uart_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** -----------------------------------------------------------
  * Definitions of C library functions re-targeted to the UART.
  * -----------------------------------------------------------
  */

/* Re-target the C library getchar function to the UART */
#ifdef READ_PROTOTYPE

READ_PROTOTYPE(void)
{
  uint8_t ch = 0;

  if (HAL_UART_Receive(pUART, &ch, 1U, RX_TIMEOUT) != HAL_OK)
  {
    return EOF;
  }

  return (int8_t)ch;
}

#else

int fgetc(FILE *file)
{
  (void)(file); /* prevent "unused variable" warnings */

  uint8_t ch = 0;

  if (HAL_UART_Receive(pUART, &ch, 1U, RX_TIMEOUT) != HAL_OK)
  {
    return EOF;
  }

  return ch;
}

#endif /* end READ_PROTOTYPE */

/* Re-target the C library printf function to the UART */
#ifdef WRITE_PROTOTYPE

WRITE_PROTOTYPE(file, ptr, len)
{
  (void)(file); /* prevent "unused variable" warnings */

  if (HAL_UART_Transmit(pUART, (uint8_t *)ptr, len, TX_TIMEOUT) == HAL_OK)
  {
    return len;
  }
  else
  {
    return 0;
  }
}

#else

int fputc(int ch, FILE *file)
{
  (void)(file); /* prevent "unused variable" warnings */

  if (HAL_UART_Transmit(pUART, (uint8_t *)&ch, 1U, TX_TIMEOUT) == HAL_OK)
  {
    return ch;
  }
  else
  {
    return EOF;
  }
}

#endif /* end WRITE_PROTOTYPE */
