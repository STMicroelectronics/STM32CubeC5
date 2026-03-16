/**
  ******************************************************************************
  * file           : example.c
  * brief          : TIM Encoder interface with HAL API
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Pointer referencing the TIM handle from the generated code used as encoder interface. */
hal_tim_handle_t *pTIM_EncoderInterface;
/* Pointer referencing the TIM handle from the generated code used to emulate a quadrature encoder. */
hal_tim_handle_t *pTIM_EncoderEmulator;
/* Variable to store the counting direction of the encoder interface timer. */
uint8_t EncoderDirection = 0;

/* Private functions prototype -----------------------------------------------*/
static int8_t EmulateForwardDirection(hal_tim_handle_t *htim);
static int8_t EmulateBackwardDirection(hal_tim_handle_t *htim);

/** ########## Step 1 ##########
  * Initializes the GPIO, TIM for emulation and TIM for encoder interface.
  * The init of the TIMs and the GPIO is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pTIM_EncoderEmulator = mx_example_tim_emulator_encoder_init();
  if (pTIM_EncoderEmulator == NULL)
  {
    goto _app_init_exit;
  }

  pTIM_EncoderInterface = mx_example_tim_encoder_interface_init();
  if (pTIM_EncoderInterface == NULL)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;
  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * Starts both timer, the timer to emulate quadrature encoder signals and the timer encoder interface.
    */
  if (HAL_TIM_OC_StartChannel(pTIM_EncoderEmulator, MX_TIM_CHANNEL_ENCODER_EMULATOR_1) != HAL_OK)
  {
    goto _app_process_exit;
  }
  if (HAL_TIM_OC_StartChannel(pTIM_EncoderEmulator, MX_TIM_CHANNEL_ENCODER_EMULATOR_2) != HAL_OK)
  {
    goto _app_process_exit;
  }
  if (HAL_TIM_Start(pTIM_EncoderEmulator) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_TIM_IC_StartChannel(pTIM_EncoderInterface, MX_TIM_CHANNEL_ENCODER_INTERFACE_1) != HAL_OK)
  {
    goto _app_process_exit;
  }
  if (HAL_TIM_IC_StartChannel(pTIM_EncoderInterface, MX_TIM_CHANNEL_ENCODER_INTERFACE_2) != HAL_OK)
  {
    goto _app_process_exit;
  }
  if (HAL_TIM_Start(pTIM_EncoderInterface) != HAL_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2: Emulation signals and encoder interface started.\n");


  /** ########## Step 3 ##########
    * Update the emulated signals to toggle every 1s, between two phases to emulate a Forward/Backward rotation.
    */
  /* Emulates backward direction and checks the counting direction of the timer. */
  if (EmulateForwardDirection(pTIM_EncoderEmulator) != 0)
  {
    goto _app_process_exit;
  }
  HAL_Delay(1000);

  /** The following HAL return the counter mode of the timer, so if the timer is counting up or down.
    * Here, we expect the timer to count up, meaning the encoder emulator is configured as a Forward direction.
    */
  if (HAL_TIM_GetCounterMode(pTIM_EncoderInterface) == HAL_TIM_COUNTER_UP)
  {
    EncoderDirection = 0;
    PRINTF("[INFO] Step 3.1: Encoder interface is in Forward direction.\n");
  }
  else
  {
    goto _app_process_exit;
  }

  /* Emulates backward direction and checks the counting direction of the timer. */
  if (EmulateBackwardDirection(pTIM_EncoderEmulator) != 0)
  {
    goto _app_process_exit;
  };
  HAL_Delay(1000);

  /** The following HAL return the counter mode of the timer, so if the timer is counting up or down.
    * Here, we expect the timer to count down, meaning the encoder emulator is configured as a Backward direction.
    */
  if (HAL_TIM_GetCounterMode(pTIM_EncoderInterface) == HAL_TIM_COUNTER_DOWN)
  {
    EncoderDirection = 1;
    PRINTF("[INFO] Step 3.2: Encoder interface is in Backward direction.\n");
  }
  else
  {
    goto _app_process_exit;
  }

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** Deinitializes the GPIO and TIM peripherals before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  app_status_t return_status = EXEC_STATUS_OK;

  mx_example_tim_encoder_interface_deinit();
  mx_example_tim_emulator_encoder_deinit();

  return return_status;
} /* end app_deinit */


/** Emulates a forward direction. Pulses are inverted compared to the backward direction.
  */
static int8_t EmulateForwardDirection(hal_tim_handle_t *htim)
{
  if (HAL_TIM_OC_SetCompareUnitPulse(
        htim, hal_tim_oc_channel_to_compare_unit(MX_TIM_CHANNEL_ENCODER_INTERFACE_1), MX_PULSE_25_PERCENT) != HAL_OK)
  {
    return -1;
  }

  if (HAL_TIM_OC_SetCompareUnitPulse(
        htim, hal_tim_oc_channel_to_compare_unit(MX_TIM_CHANNEL_ENCODER_INTERFACE_2), MX_PULSE_75_PERCENT) != HAL_OK)
  {
    return -1;
  }

  return 0;
}

/** Emulates a backward direction. Pulses are inverted compared to the forward direction.
  */
static int8_t EmulateBackwardDirection(hal_tim_handle_t *htim)
{
  if (HAL_TIM_OC_SetCompareUnitPulse(
        htim, hal_tim_oc_channel_to_compare_unit(MX_TIM_CHANNEL_ENCODER_INTERFACE_1), MX_PULSE_75_PERCENT) != HAL_OK)
  {
    return -1;
  }

  if (HAL_TIM_OC_SetCompareUnitPulse(
        htim, hal_tim_oc_channel_to_compare_unit(MX_TIM_CHANNEL_ENCODER_INTERFACE_2), MX_PULSE_25_PERCENT) != HAL_OK)
  {
    return -1;
  }

  return 0;
}
