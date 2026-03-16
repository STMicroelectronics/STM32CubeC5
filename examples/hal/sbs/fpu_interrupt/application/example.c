/**
  ******************************************************************************
  * file           : example.c
  * brief          : example program body (SBS FPU interrupt configuration with HAL API)
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

/**  This example makes divisions by zero on purpose.
  *  So, we disable the compilers warnings for the toolchains the Cube ecosystem supports.
  */
#if defined(__GNUC__) && !defined(__clang__) /* GNU Compiler and not __clang__ */
#pragma GCC diagnostic ignored "-Wdiv-by-zero"
#elif defined(__ICCARM__) /* IAR Compiler */
#pragma diag_suppress=Pe039
#elif defined(__clang__) /* Clang compiler */
#pragma clang diagnostic ignored "-Wdivision-by-zero"
#else
#warning The selected compiler might issue division by zero warnings
#endif /* Compiler */


/* Includes ------------------------------------------------------------------*/
#include "example.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Arbitrary value (millisecond) to wait for the FPU ISR to be processed */
#define FPU_ISR_TIMEOUT_MS 5U

/** The FPSCR stores the status (condition bit and exception flags) and the configuration
  * (rounding modes and alternative modes) of the FPU.
  */
#define ARM_FPU_EXCEPTION_DIVIDE_BY_ZERO 0x00000002 /* bit 1 of the FPSCR: Division by zero (DZC) */

/** Mask to clear the FPU exception flags in the FPU_IRQHandler.
  * We clear the DZC.
  */
#define ARM_FPU_FPSCR_CLEAR_FLAGS ~(ARM_FPU_EXCEPTION_DIVIDE_BY_ZERO)

/** In ARM Cortex-M processors that use the automatic context saving mechanism (FPCCR.ASPEN=1 and FPCCR.LSPEN=1),
  * the FPSCR register is located at an offset of 0x40 bytes from the start of the saved context.
  *
  * LSPEN=1: space is reserved on the stack for the floating point registers (s0-s15 & FPSCR)
  *          but the data is pushed by default only if the FPU_IRQHandler executes a FPU instruction.
  */
#define FPSCR_OFFSET_BYTES 0x40U /* s0-s15 are stacked */


/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Arbitrary value: pi */
float Dividend = 3.14f;
/* The volatile keyword is used to prevent the computation from being optimized out */
volatile float Quotient = 0.0f;
/* Flag to indicate if the division by zero has been caught by the FPU interrupt */
volatile uint8_t DivByZeroCaught = 0U;

/* Private functions prototype -----------------------------------------------*/
void FPU_IRQHandler(void); /* redefinition of the IRQ handler */

/** ########## Step 1 ##########
  * Enables the FPU interrupt on division by zero detection
  * and enables the FPU interrupt in the NVIC.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Check the current status of the FPU configuration in SBS for the divide by zero interrupt */
  if (HAL_SBS_IsEnabledFPUIT(HAL_SBS_IT_FPU_DZC) == HAL_SBS_IT_FPU_DISABLED)
  {
    /** In IEEE 754 arithmetic:
      *   dividend divided by +0 is positive infinity when the dividend is positive,
      *   negative infinity when the dividend  is negative,
      *   and NaN when dividend = (+/-)0.
      *
      * We enable the divide by zero interrupt to intercept the division by zero.
      */
    HAL_SBS_EnableFPUIT(HAL_SBS_IT_FPU_DZC);
  }
  /* else: nothing to do, the divide by zero interrupt is already enabled for the FPU */

  /* cross-check */
  if (HAL_SBS_IsEnabledFPUIT(HAL_SBS_IT_FPU_DZC) == HAL_SBS_IT_FPU_ENABLED)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  /* Enable the FPU interrupt in the NVIC */
  HAL_CORTEX_NVIC_EnableIRQ(FPU_IRQn);

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  /** After the division by 0 we wait for a few milliseconds
    * for the interrupt to be caught and processed in FPU_IRQHandler.
    */
  uint32_t tick_count = 0U;

  /** ########## Step 2 ##########
    * Runs a valid operation to make sure the FPU works fine.
    */
  Quotient = Dividend / 1.0f;
  if (Quotient != Dividend)
  {
    goto _exit_app_process;
  }

  /** ########## Step 3 ##########
    * Triggers the floating point division by zero:
    * the interrupt is caught by the FPU_IRQHandler ISR defined in this file.
    */
  Quotient = Dividend / 0.0f; /* a FPU interrupt must occur here */

  /* Wait for an arbitrary duration for the FPU IRQ to be processed */
  while ((DivByZeroCaught == 0U) && (tick_count < FPU_ISR_TIMEOUT_MS))
  {
    /* The HAL tick is running (systick fired every 1 ms) */
    __WFI();
    /* we increment the counter when the systick fires */
    tick_count ++;
  }

_exit_app_process:
  /* Check if the flag has been toggled in FPU_IRQHandler() */
  if (DivByZeroCaught == 1U)
  {
    return EXEC_STATUS_OK;
  }
  else
  {
    return EXEC_STATUS_ERROR;
  }
} /* end app_process */


/** ########## Step 5 ##########
  * Disables the FPU IT
  */
app_status_t app_deinit(void)
{
  HAL_SBS_DisableFPUIT(HAL_SBS_IT_FPU_DZC);
  HAL_CORTEX_NVIC_DisableIRQ(FPU_IRQn);
  return EXEC_STATUS_OK;
} /* end app_deinit */


/** ----------------------------------------------------------
  * Specific ISR definition for this example.
  * ----------------------------------------------------------
  */

/** brief  This function is the FPU IRQ handler.
  *        In this example, it is specialized for the divide by zero case.
  * param  None
  * retval None
  */
void FPU_IRQHandler(void)
{
  /** The exception status bits in the FPSCR register must be cleaned explicitly
    * in the floating-point exception handler.
    * Otherwise, after the exception returns, the same exception is raised again.
    *
    * The FPCCR.ASPEN and FPCCR.LSPEN settings determine how the FPSCR register is saved in the stack
    * and how to get and clear FPSCR status bits in the float-point exception handler.
    * The example expects the default settings: FPCCR.ASPEN = 1 and FPCCR.LSPEN = 1
    * These settings enable the lazy floating-point context saving on exception entry.
    *
    * The FPSCR is saved in the stack when the FPU_IRQHandler runs a FPU instruction.
    * In the FPU_IRQHandler, we use the FPCAR register to get the stack address for the FPSCR register.
    * We need to clear the flag(s) in the stack as the FPSCR is restored from it when leaving the ISR.
    */
  uint32_t *p_fpscr;

  /* Execute a dummy FPU instruction to activate the lazy stacking */
  (void)__get_FPSCR();

  /* Get the FPSCR address in the stack */
  p_fpscr = (uint32_t *)(FPU->FPCAR + FPSCR_OFFSET_BYTES);

  /* Check the exception flags */
  if ((*p_fpscr & ARM_FPU_EXCEPTION_DIVIDE_BY_ZERO) == ARM_FPU_EXCEPTION_DIVIDE_BY_ZERO)
  {
    /** ########## Step 4 ##########
      * Handles the FPU exception via the FPU ISR.
      * Raise a flag to indicate the interrupt has been caught.
      */
    DivByZeroCaught = 1U;

    /* Clear the DZC flag in the stacked FPSCR register. */
    *p_fpscr = *p_fpscr & ARM_FPU_FPSCR_CLEAR_FLAGS;
  }
}
