/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to calculate sines and cosines array in DMA mode using
                     the CORDIC HAL APIs
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
#include <stdlib.h> /* Required for function abs() */
#include <arm_math.h> /* Required for conversion function arm_float_to_q31() and PI definition */
#include "example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Size of data array */
#define ARRAY_SIZE        64U
#define DMA_ALIGNMENT     4U  /* User constant to match the DMA transfer size: 4 bytes alignment for word transfer */


/** Reference values in Q1.31 format
  * Max residual error for sines and cosines, with 6 cycles precision:
  * 2^-19 max residual error, ie 31-19=12 LSB, ie 0x1000
  */
#define MAX_RESIDUAL_ERROR (int32_t)0x1000

/* Private variables ---------------------------------------------------------*/
hal_cordic_handle_t *pCORDIC;  /* pointer referencing the CORDIC handle from the generated code */

volatile uint8_t CordicCalculationCompleted = 0U; /* Set to 1 if a CORDIC calculation completes */
volatile uint8_t CordicError = 0U; /* Set to 1 if a CORDIC error occurs */

/* Input angles in radians in range [-pi, pi[ regularly incremented */
static const float32_t Angles[ARRAY_SIZE] =
{
  -3.141592653589793,      -3.043417883165112,     -2.945243112740431,     -2.84706834231575,
  -2.748893571891069,      -2.650718801466388,     -2.552544031041707,     -2.454369260617026,
  -2.356194490192345,      -2.2580197197676637,    -2.1598449493429825,    -2.061670178918302,
  -1.9634954084936207,     -1.8653206380689396,    -1.7671458676442586,    -1.6689710972195777,
  -1.5707963267948966,     -1.4726215563702154,    -1.3744467859455345,    -1.2762720155208536,
  -1.1780972450961724,     -1.0799224746714913,    -0.9817477042468106,    -0.8835729338221294,
  -0.7853981633974483,     -0.6872233929727671,    -0.589048622548086,     -0.4908738521234053,
  -0.39269908169872414,    -0.294524311274043,     -0.1963495408493623,    -0.09817477042468115,
  0.0,                     0.09817477042468115,    0.1963495408493623,     0.294524311274043,
  0.39269908169872414,     0.4908738521234053,     0.589048622548086,      0.6872233929727671,
  0.7853981633974483,      0.883572933822129,      0.9817477042468106,     1.0799224746714913,
  1.178097245096172,       1.2762720155208536,     1.3744467859455343,     1.4726215563702159,
  1.5707963267948966,      1.6689710972195773,     1.7671458676442588,     1.8653206380689396,
  1.9634954084936211,      2.061670178918302,      2.1598449493429825,     2.258019719767664,
  2.356194490192345,       2.4543692606170255,     2.552544031041707,      2.650718801466388,
  2.7488935718910685,      2.84706834231575,       2.945243112740431,      3.0434178831651124
};

/* Input angles in radians divided by pi */
static float32_t AnglesDivPi[ARRAY_SIZE];

/** Q1.31 format representation of the angles divided by pi, buffer used by CPU and DMA.
  * - Non-cacheable memory for data cache consistency.
  * - Aligned for DMA constraints.
  * - Mandatory with data cache enabled, harmless otherwise: portable across STM32 series.
  *
  * Refer to the README and the STM32 reference manual for data cache details.
  * Non-cacheable section not mapped if unnecessary.
  */
__attribute__((section(".non_cacheable_variables"), aligned(DMA_ALIGNMENT)))
static q31_t Q1_31[ARRAY_SIZE];

/** Output array of the CORDIC calculated sines (even indexes) and cosines (odd indexes) in Q1.31 format, used by CPU
  * and DMA.
  * - Non-cacheable memory for data cache consistency.
  * - Aligned for DMA constraints.
  * - Mandatory with data cache enabled, harmless otherwise: portable across STM32 series.
  *
  * Refer to the README and the STM32 reference manual for data cache details.
  * Non-cacheable section not mapped if unnecessary.
  */
__attribute__((section(".non_cacheable_variables"), aligned(DMA_ALIGNMENT)))
static int32_t CalculatedSinCos[2 * ARRAY_SIZE];

/** HAL CORDIC data buffer descriptors
  * Size of destination buffer is doubled to gather sines and cosines
  */
hal_cordic_buffer_desc_t SrcBuffer =
{
  Q1_31,
  ARRAY_SIZE
};

hal_cordic_buffer_desc_t DstBuffer =
{
  CalculatedSinCos,
  2 * ARRAY_SIZE
};

/* Array of reference sines in Q1.31 format. Used for check purpose only. */
static const int32_t RefSin[ARRAY_SIZE] =
{
  0x00000000,    0xF3742CA2,    0xE70747C4,    0xDAD7F3A2,
  0xCF043AB3,    0xC3A94590,    0xB8E31319,    0xAECC336C,
  0xA57D8666,    0x9D0DFE54,    0x9592675C,    0x8F1D343A,
  0x89BE50C3,    0x8582FAA5,    0x8275A0C0,    0x809DC971,
  0x80000000,    0x809DC971,    0x8275A0C0,    0x8582FAA5,
  0x89BE50C3,    0x8F1D343A,    0x9592675C,    0x9D0DFE54,
  0xA57D8666,    0xAECC336C,    0xB8E31319,    0xC3A94590,
  0xCF043AB3,    0xDAD7F3A2,    0xE70747C4,    0xF3742CA2,
  0x00000000,    0x0C8BD35E,    0x18F8B83C,    0x25280C5E,
  0x30FBC54D,    0x3C56BA70,    0x471CECE7,    0x5133CC94,
  0x5A82799A,    0x62F201AC,    0x6A6D98A4,    0x70E2CBC6,
  0x7641AF3D,    0x7A7D055B,    0x7D8A5F40,    0x7F62368F,
  0x80000000,    0x7F62368F,    0x7D8A5F40,    0x7A7D055B,
  0x7641AF3D,    0x70E2CBC6,    0x6A6D98A4,    0x62F201AC,
  0x5A82799A,    0x5133CC94,    0x471CECE7,    0x3C56BA70,
  0x30FBC54D,    0x25280C5E,    0x18F8B83C,    0x0C8BD35E
};

/* Array of reference cosines in Q1.31 format. Used for check purpose only. */
static const int32_t RefCos[ARRAY_SIZE] =
{
  0x80000000,    0x809DC971,    0x8275A0C0,    0x8582FAA5,
  0x89BE50C3,    0x8F1D343A,    0x9592675C,    0x9D0DFE54,
  0xA57D8666,    0xAECC336C,    0xB8E31319,    0xC3A94590,
  0xCF043AB3,    0xDAD7F3A2,    0xE70747C4,    0xF3742CA2,
  0x00000000,    0x0C8BD35E,    0x18F8B83C,    0x25280C5E,
  0x30FBC54D,    0x3C56BA70,    0x471CECE7,    0x5133CC94,
  0x5A82799A,    0x62F201AC,    0x6A6D98A4,    0x70E2CBC6,
  0x7641AF3D,    0x7A7D055B,    0x7D8A5F40,    0x7F62368F,
  0x80000000,    0x7F62368F,    0x7D8A5F40,    0x7A7D055B,
  0x7641AF3D,    0x70E2CBC6,    0x6A6D98A4,    0x62F201AC,
  0x5A82799A,    0x5133CC94,    0x471CECE7,    0x3C56BA70,
  0x30FBC54D,    0x25280C5E,    0x18F8B83C,    0x0C8BD35E,
  0x00000000,    0xF3742CA2,    0xE70747C4,    0xDAD7F3A2,
  0xCF043AB3,    0xC3A94590,    0xB8E31319,    0xAECC336C,
  0xA57D8666,    0x9D0DFE54,    0x9592675C,    0x8F1D343A,
  0x89BE50C3,    0x8582FAA5,    0x8275A0C0,    0x809DC971
};

/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * The applicative code initializes the CORDIC instance.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pCORDIC = mx_example_cordic_init();
  if (pCORDIC != NULL)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * Prepares the input values for CORDIC: angles must be in radians divided by pi and presented in Q1.31 format.
    */
  /* CORDIC input must be angles in radians divided by pi (range [-1, 1]) */
  for (uint32_t i = 0 ; i < ARRAY_SIZE; i++)
  {
    AnglesDivPi[i] = Angles[i] / PI;
  }

  /* Cordic input must be written in Q1.31 format */
  arm_float_to_q31(AnglesDivPi, Q1_31, ARRAY_SIZE);


  /** ########## Step 3 ##########
    * Starts CORDIC calculation in DMA mode.
    */
  if (HAL_CORDIC_Calculate_DMA(pCORDIC, &SrcBuffer, &DstBuffer) != HAL_OK)
  {
    goto _app_process_exit;
  }


  /** ########## Step 4 ##########
    * Waits for the CORDIC read DMA transfer completion.
    * Then compares the CORDIC results to the reference values.
    */
  while (!CordicCalculationCompleted && !CordicError)
  {
    /** Put the CPU in Wait For Interrupt state. A CORDIC, DMA or SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the DMA. It can be replaced by your own code.
      */
    __WFI();
  }

  if ((CordicCalculationCompleted != 1) || (CordicError == 1))
  {
    goto _app_process_exit;
  }

  /** The buffer CalculatedSinCos, filled by CORDIC, contains the sines (even indexes) and the cosines (odd indexes).
    * Check the CORDIC results against the reference values to verify that they comply with the expected precision.
    */
  for (uint32_t i = 0; i < ARRAY_SIZE; i++)
  {
    if ((abs(CalculatedSinCos[2 * i] - RefSin[i]) > MAX_RESIDUAL_ERROR)
        || (abs(CalculatedSinCos[(2 * i) + 1] - RefCos[i]) > MAX_RESIDUAL_ERROR))
    {
      goto _app_process_exit;
    }
  }

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 5 ##########
  * Deinitializes the CORDIC instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_cordic_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** CORDIC calculation completed callback.
  * @user This implementation of the CORDIC event callback can be customized.
  * This function is executed if a CORDIC Read DMA transfer is completed.
  */
void HAL_CORDIC_CalculateCpltCallback(hal_cordic_handle_t *hcordic)
{
  /* asynchronous processing related to step 4 */
  CordicCalculationCompleted = 1U;
}

/** CORDIC error callback.
  * This function is executed if a CORDIC Error occurs
  */
void HAL_CORDIC_ErrorCallback(hal_cordic_handle_t *hcordic)
{
  CordicError = 1;
}
