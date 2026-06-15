/**
  ******************************************************************************
  * file           : example.c
  * brief          : I3C Controller private transfer example with X-NUCLEO-IKS4A1
  *                  using interrupt (IT) mode and HAL API.
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
#include <string.h> /* importing memcmp and memset functions */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Target device address for I3C communication. */
#define DEVICE_TARGET_ADDR          0x32U

/* Broadcast Command code */
#define BROADCAST_DISEC             0x01U
#define BROADCAST_RSTDAA            0x06U

/* Direct Command code */
#define DIRECT_SETDASA              0x87U

/* Define LSM6DS0 Static Address */
#define LSM6DSO_STA                 0x6BU

/* Time between two display refresh in ms */
#define DISPLAY_REFRESH_DELAY       1000U

#define LSM6DSO_CONFIG_SIZE         3U
#define LSM6DSO_DATA_SIZE           1U

/* Size of Reception buffer */
#define BUFFER_SIZE                 14U

/* Size of control buffer */
#define CONTROL_BUFFER_SIZE         2U

/* Buffer used for LSM6DSO configuration, and reading data raw */
uint8_t LSM6DSOConfig[] = {0x10U, 0x36U, 0x62U}; /* Enable Accelerometer then Gyroscope */
uint8_t LSM6DSOData[]   = {0x20U}; /* First Temperature registers + 12 Read continuous register access */

/* DISEC value and SETDASA target address */
uint8_t DISECData = 0x08;
uint8_t SETDASALSM6DSOData = (DEVICE_TARGET_ADDR << 1U);

/* Context buffer related to Frame context, contain different buffer value for a communication */
hal_i3c_transfer_ctx_t ContextBuffers;

/* Buffer for transfer context */
uint32_t ControlBuffer[0xF] = {0U};

/* Buffer used for transmission */
uint8_t TxBuffer[BUFFER_SIZE];

/* Buffer used for reception */
uint8_t RxBuffer[BUFFER_SIZE];

/* Temperature offset is 25 degree celsius, mean Temperature value 0 correspond to 25 Celsius degree */
#define TEMP_OFFSET        25U

/* Variable to display reading data */
int16_t Temperature      = 0U;
int16_t Gyroscope[3]     = {0U};
int16_t Accelerometer[3] = {0U};
uint8_t SensorConfigured = 0U;

/* Descriptor for broadcast CCC */
hal_i3c_ccc_desc_t BroadcastCCC[] =
{
  /* Target Addr   CCC Value           CCC size    Direction        */
  {0,              BROADCAST_DISEC,    1U,          HAL_I3C_DIRECTION_WRITE},
  {0,              BROADCAST_RSTDAA,   0U,          HAL_I3C_DIRECTION_WRITE},
};

/* Descriptor for direct CCC */
hal_i3c_ccc_desc_t DirectCCC =
{
  /* Target Addr      CCC Value          CCC size    Direction        */
  LSM6DSO_STA,        DIRECT_SETDASA,    1U,          HAL_I3C_DIRECTION_WRITE,
};

/* Descriptor for private data transmit */
hal_i3c_private_desc_t PrivateDescriptorConfig =
{
  DEVICE_TARGET_ADDR,  LSM6DSO_CONFIG_SIZE, HAL_I3C_DIRECTION_WRITE,
};

/* Descriptor for private data transmit */
hal_i3c_private_desc_t PrivateDescriptor[2] =
{
  {DEVICE_TARGET_ADDR, LSM6DSO_DATA_SIZE,  HAL_I3C_DIRECTION_WRITE},
  {DEVICE_TARGET_ADDR, BUFFER_SIZE,        HAL_I3C_DIRECTION_READ}
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_i3c_handle_t *pI3C;   /* pointer referencing the I3C handle from the generated code */

volatile uint8_t TransferComplete = 0U; /* Set to 1 when a data transfer operation is complete */
volatile uint8_t TransferError = 0U;    /* Set to 1 when a transmission or reception error is detected */

/* Private functions prototype -----------------------------------------------*/
static void CtrlTransferCpltCallback(hal_i3c_handle_t *hi3c);
static void TransferErrorCallback(hal_i3c_handle_t *pI3C);
static app_status_t HandleTransferError(hal_status_t hal_status, uint32_t i3c_error_code);
static void DisplayValue(void);


/** ########## Step 1 ##########
  * Initialization of the I3C peripheral and registers user callbacks for controller events.
  * This example demonstrates I3C communication in interrupt (IT) mode.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pI3C = mx_example_i3c_init();
  if (pI3C == NULL)
  {
    goto _app_init_exit;
  }

  /* Register the I3C Controller callbacks */
  if (HAL_I3C_CTRL_RegisterTransferCpltCallback(pI3C, CtrlTransferCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_RegisterErrorCallback(pI3C, TransferErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step1: I3C initialized.\n");

  return_status = EXEC_STATUS_INIT_OK;
_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  uint32_t i3c_error_code;       /* Memorizes the I3C error code limited to the last process */
  hal_status_t hal_status;       /* Memorizes the HAL status of the I3C TX/RX operations */
  hal_i3c_ctrl_config_t i3c_ctrl_config;

  /** ########## Step 2 ##########
    * Configure the I3C target address and the sensor only once.
    */
  if (SensorConfigured == 0U)
  {
    /************* For CCC Broadcast Command *************/
    /* Reset context to ensure a clean state before configuring new transfer
      * parameters  */
    if (HAL_I3C_CTRL_ResetTransferCtx(&ContextBuffers) != HAL_OK)
    {
      goto _app_process_exit;
    }

    /* Configure the transfer context */
    if (HAL_I3C_CTRL_InitTransferCtxTc(&ContextBuffers, ControlBuffer,
                                       CONTROL_BUFFER_SIZE) != HAL_OK)
    {
      goto _app_process_exit;
    }

    /* Initialize the transfer context for transmission with DISECData. */
    if (HAL_I3C_CTRL_InitTransferCtxTx(&ContextBuffers, &DISECData, 1U) != HAL_OK)
    {
      goto _app_process_exit;
    }

    /* Build a transfer context from CCC transfer descriptor */
    if (HAL_I3C_CTRL_BuildTransferCtxCCC(&ContextBuffers,
                                         BroadcastCCC,
                                         2U,
                                         HAL_I3C_CCC_BROADCAST_WITHOUT_DEFBYTE_RESTART) != HAL_OK)
    {
      goto _app_process_exit;
    }

    TransferComplete = 0U;
    TransferError = 0U;

    /* Start transfer Direct CCC Command in interrupt mode*/
    hal_status = HAL_I3C_CTRL_Transfer_IT(pI3C, &ContextBuffers);
    if (hal_status != HAL_OK)
    {
      goto _app_process_exit;
    }

    while ((TransferComplete == 0U) && (TransferError == 0U))
    {
      /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
        */
      __WFI();
    }

    if (TransferError == 1U)
    {
      /* The write transfer is not correctly completed. Report the error, then
        * restart the communication */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status = HandleTransferError(hal_status, i3c_error_code);
      goto _app_process_exit;
    }

    PRINTF("[INFO] Step2: CCC Broadcast command sent.\n");

    /** ########## Step 3 ##########
      * The controller sends the configuration buffer to the sensor.
      * Then the bus speed is reconfigured to 12.5 MHz, which is the standard
      * maximum speed for I3C.
      */
    /***** Direct ******/
    /* Configure the transfer context */
    if (HAL_I3C_CTRL_InitTransferCtxTc(&ContextBuffers, ControlBuffer, CONTROL_BUFFER_SIZE) != HAL_OK)
    {
      goto _app_process_exit;
    }

    /* Initialize the transfer context for transmission with SETDASALSM6DSOData.*/
    if (HAL_I3C_CTRL_InitTransferCtxTx(&ContextBuffers, &SETDASALSM6DSOData, 1U) != HAL_OK)
    {
      goto _app_process_exit;
    }

    /* Build a transfer context from CCC transfer descriptor */
    if (HAL_I3C_CTRL_BuildTransferCtxCCC(&ContextBuffers,
                                         &DirectCCC,
                                         1U,
                                         HAL_I3C_CCC_DIRECT_WITHOUT_DEFBYTE_STOP) != HAL_OK)
    {
      goto _app_process_exit;
    }

    /* Reset transfer flags for the next asynchronous transfer. */
    TransferComplete = 0U;
    TransferError = 0U;

    /* Start transfer Direct CCC Command in interrupt mode*/
    hal_status = HAL_I3C_CTRL_Transfer_IT(pI3C, &ContextBuffers);
    if (hal_status != HAL_OK)
    {
      goto _app_process_exit;
    }

    while ((TransferComplete == 0U) && (TransferError == 0U))
    {
      /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
        */
      __WFI();
    }

    if (TransferError == 1U)
    {
      /* The write transfer is not correctly completed. Report the error, then
        * restart the communication */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status = HandleTransferError(hal_status, i3c_error_code);
      goto _app_process_exit;
    }

    /************* Bus speed re-configuration  *************/
    /**
      *  I3C Timing registers calculated by cubemx with I3C bus frequency = 12.5 MHz
      */
    i3c_ctrl_config.timing_reg0 = MX_I3C_TIMING_REGISTER_0;
    i3c_ctrl_config.timing_reg1 = MX_I3C_TIMING_REGISTER_1;

    if (HAL_I3C_CTRL_SetConfig(pI3C, &i3c_ctrl_config) != HAL_OK)
    {
      goto _app_process_exit;
    }

    /*  The I3C controller proceeds to send a configuration buffer to the sensor. */
    /************* For Private Command *************/
    /* Configure the transfer context */
    if (HAL_I3C_CTRL_InitTransferCtxTc(&ContextBuffers, ControlBuffer, 1U) != HAL_OK)
    {
      goto _app_process_exit;
    }
    /* Initialize the transfer context for transmission with LSM6DSOConfig. */
    if (HAL_I3C_CTRL_InitTransferCtxTx(&ContextBuffers, LSM6DSOConfig, LSM6DSO_CONFIG_SIZE) != HAL_OK)
    {
      goto _app_process_exit;
    }
    /* Build a transfer context from Private transfer descriptor */
    if (HAL_I3C_CTRL_BuildTransferCtxPrivate(&ContextBuffers,
                                             &PrivateDescriptorConfig,
                                             1U,
                                             HAL_I3C_PRIVATE_WITH_ARB_STOP) != HAL_OK)
    {
      goto _app_process_exit;
    }

    /* Reset transfer flags for the next asynchronous transfer. */
    TransferComplete = 0U;
    TransferError = 0U;

    /* Start transfer Private Command in interrupt mode*/
    hal_status = HAL_I3C_CTRL_Transfer_IT(pI3C, &ContextBuffers);
    if (hal_status != HAL_OK)
    {
      goto _app_process_exit;
    }

    while ((TransferComplete == 0U) && (TransferError == 0U))
    {
      /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
        */
      __WFI();
    }

    if (TransferError == 1U)
    {
      /* The write transfer is not correctly completed. Report the error, then
        * restart the communication */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status = HandleTransferError(hal_status, i3c_error_code);
      goto _app_process_exit;
    }

    SensorConfigured = 1U;

    PRINTF("[INFO] Step3: Sensor is configured.\n");
  }

  /** ########## Step 4 ##########
    * The controller repeatedly exchanges data with the sensor, retrieving temperature, gyroscope, and magnetometer
    * readings, which are displayed in real time.
    */
  /************* For Private Command *************/
  /* Configure the transfer context */
  if (HAL_I3C_CTRL_InitTransferCtxTc(&ContextBuffers, ControlBuffer, CONTROL_BUFFER_SIZE) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Initialize the transfer context for transmission with LSM6DSOData. */
  if (HAL_I3C_CTRL_InitTransferCtxTx(&ContextBuffers, LSM6DSOData, LSM6DSO_DATA_SIZE) != HAL_OK)
  {
    goto _app_process_exit;
  }
  /*Initialise the transfer context with RxBuffer. */
  if (HAL_I3C_CTRL_InitTransferCtxRx(&ContextBuffers, RxBuffer, BUFFER_SIZE) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Build a transfer context from Private transfer descriptor */
  if (HAL_I3C_CTRL_BuildTransferCtxPrivate(&ContextBuffers,
                                           PrivateDescriptor,
                                           2U,
                                           HAL_I3C_PRIVATE_WITHOUT_ARB_RESTART) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Reset transfer flags for the next asynchronous transfer. */
  TransferComplete = 0U;
  TransferError = 0U;

  /* Start transfer Private Command in interrupt mode*/
  hal_status = HAL_I3C_CTRL_Transfer_IT(pI3C, &ContextBuffers);
  if (hal_status != HAL_OK)
  {
    /* An error occurs during the write/read transfer. Report the error and restart the communication attempt. */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
  }

  /* Waits for one of these I3C interrupts: write transfer complete or transfer error.*/
  while ((TransferComplete == 0U) && (TransferError == 0U))
  {
    /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
      */
    __WFI();
  }

  if (TransferError == 1U)
  {
    /* The write transfer is not correctly completed. Report the error, then
      * restart the communication */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
  }

  /* Display through external Terminal IO the value received */
  DisplayValue();

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
  /* end app_process */
}


/** De-initializes the I3C  instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_i3c_deinit();
  return EXEC_STATUS_OK;
} /* end app_deinit */


/**
  * brief: Display the different value retrieve during Communication.
  * param: pPrivateBuffer   Pointer to a receive Private buffer.
  * retval: None.
  */
static void DisplayValue(void)
{
  Temperature = TEMP_OFFSET + (((int16_t)(((int16_t)RxBuffer[1] * 256) + (int16_t)RxBuffer[0])) / 100);

  PRINTF("[INFO] -------------------LSM6DSO Measurement------------------\n");
  PRINTF("[INFO]  Temperature (Celsius degree): %"PRId16" \n", Temperature);

  for (uint8_t i = 0; i < 3; i++)
  {
    Gyroscope[i] = ((int16_t)RxBuffer[3 + 2 * i] * 256) + (int16_t)RxBuffer[2 + 2 * i];
  }

  PRINTF("[INFO]  GYR_X: %" PRId16 ", GYR_Y: %" PRId16 ", GYR_Z: %" PRId16 " \n", \
         (int)Gyroscope[0], (int)Gyroscope[1], (int)Gyroscope[2]);

  for (uint8_t i = 0; i < 3; i++)
  {
    Accelerometer[i] = ((int16_t)RxBuffer[9 + 2 * i] * 256) + (int16_t)RxBuffer[8 + 2 * i];
  }

  PRINTF("[INFO]  ACC_X: %" PRId16 " , ACC_Y: %" PRId16 ", ACC_Z: %" PRId16 " \n", \
         (int16_t)Accelerometer[0], (int16_t)Accelerometer[1], (int16_t)Accelerometer[2]);

  HAL_Delay(DISPLAY_REFRESH_DELAY);
}


/** Controller transmission end of transfer callback
  * @user This implementation of the I3C controller transfer complete callback can be customized.
  * This function is executed when the controller transfer complete interrupt is generated.
  */
static void CtrlTransferCpltCallback(hal_i3c_handle_t *hi3c)
{
  /* Asynchronous processing for transfer completion (Step 3) */
  TransferComplete = 1U;
}

/**
  * I3C transfer error callback
  * @user This implementation of the I3C controller transfer error callback can be customized.
  * This function is executed when the controller transfer error interrupt is generated.
  */
static void TransferErrorCallback(hal_i3c_handle_t *hi3c)
{
  /* Asynchronous processing for transfer error (Step 3 and Step 5) */
  TransferError = 1U;
}

/** brief:  This function is executed in case of a data transfer error.
  * param: hal_status:  HAL status of the I3C TX/RX operations.
  * param: i3c_error_code:  I3C Error Code.
  * retval: example status
  */
static app_status_t HandleTransferError(hal_status_t hal_status, uint32_t i3c_error_code)
{
  PRINTF("[ERROR] Controller - Communication ERROR: hal_status = %" PRIu16 ", \
  HAL_I3C_GetLastErrorCodes = %" PRIu32 ". TRYING AGAIN.\n", hal_status, i3c_error_code);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
