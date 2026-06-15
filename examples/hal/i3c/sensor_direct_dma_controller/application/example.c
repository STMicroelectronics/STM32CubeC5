/**
  ******************************************************************************
  * file           : example.c
  * brief          : I3C Controller direct transfer example with X-NUCLEO-IKS4A1
  *                  using DMA mode and HAL API.
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
#define DEVICE_TARGET_ADDR 0x32U

/* Broadcast Command code */
#define BROADCAST_DISEC 0x01
#define BROADCAST_RSTDAA 0x06

/* Direct Command code */
#define DIRECT_SETDASA 0x87
#define DIRECT_SETMWL 0x89
#define DIRECT_SETMRL 0x8A
#define DIRECT_GETMWL 0x8B
#define DIRECT_GETMRL 0x8C
#define DIRECT_GETPID 0x8D
#define DIRECT_GETBCR 0x8E
#define DIRECT_GETDCR 0x8F
#define DIRECT_GETSTATUS 0x90

/* Define LSM6DS0 Static Address */
#define LSM6DSO_STA 0x6B

/* Number of bytes used for direct CCC write associated data: 2 bytes for SETMRL and 2 bytes for SETMWL */
#define DIRECT_WRITE_CCC_SIZE 4U

/* Size of Reception buffer */
#define BUFFER_SIZE 14U

/* Size of control buffer */
#define CONTROL_BUFFER_SIZE 20U

/* Size of direct CCC array */
#define CCCLIST_SIZE 8U

/* Time between two display refresh in ms */
#define DISPLAY_REFRESH_DELAY       1000U

/* Broadcast CCC associated data */
uint8_t DISECData[1] = {0x08U};

/* Direct CCC associated data for SETDASA */
uint8_t SETDASAData[1] = {(DEVICE_TARGET_ADDR << 1)};

/* Structure holding associated data for SETMWL and SETMRL CCC write commands */
struct
{
  uint8_t SETMWL_associated_data[2];
  uint8_t SETMRL_associated_data[2];
} DirectWriteCCC __attribute__((aligned(4))) =
{
  .SETMWL_associated_data = {0x00U, 0x04U},
  .SETMRL_associated_data = {0x00U, 0x04U}
};

/* Context buffer related to Frame context, contain different buffer value for a
   communication */
hal_i3c_transfer_ctx_t ContextBuffers;

/* Buffer for transfer context */
uint32_t ControlBuffer[CONTROL_BUFFER_SIZE];

/* Buffer used for reception */
__attribute__((section(".non_cacheable_variables"), aligned(1)))
uint8_t RxBuffer[0x1FU];

/* Descriptor for broadcast CCC */
hal_i3c_ccc_desc_t BroadcastCCC[] =
{
  /* Target Addr   CCC Value         CCC size       Direction        */
  {0U, BROADCAST_DISEC,  1U, HAL_I3C_DIRECTION_WRITE},
  {0U, BROADCAST_RSTDAA, 0U, HAL_I3C_DIRECTION_WRITE},
};

/* Descriptor for direct CCC used only for SETDASA */
hal_i3c_ccc_desc_t DirectCCC[] =
{
  {LSM6DSO_STA, DIRECT_SETDASA, 1U, HAL_I3C_DIRECTION_WRITE},
};

/* Descriptor for direct CCC */
hal_i3c_ccc_desc_t CCCList[] =
{
  /*   Target Addr    CCC Value   CCC size    Direction        */
  {DEVICE_TARGET_ADDR, DIRECT_GETPID, 6, HAL_I3C_DIRECTION_READ},
  {DEVICE_TARGET_ADDR, DIRECT_GETBCR, 1, HAL_I3C_DIRECTION_READ},
  {DEVICE_TARGET_ADDR, DIRECT_GETDCR, 1, HAL_I3C_DIRECTION_READ},
  {DEVICE_TARGET_ADDR, DIRECT_SETMWL, 2, HAL_I3C_DIRECTION_WRITE},
  {DEVICE_TARGET_ADDR, DIRECT_GETMWL, 2, HAL_I3C_DIRECTION_READ},
  {DEVICE_TARGET_ADDR, DIRECT_SETMRL, 2, HAL_I3C_DIRECTION_WRITE},
  {DEVICE_TARGET_ADDR, DIRECT_GETMRL, 2, HAL_I3C_DIRECTION_READ},
  {DEVICE_TARGET_ADDR, DIRECT_GETSTATUS, 2, HAL_I3C_DIRECTION_READ},
};

/* Array contain Get CCC name in char, this array is use to print the value received during Get CCC procedure */
char *CommandCode[] =
{
  "GETPID",
  "GETBCR",
  "GETDCR",
  "GETMWL",
  "GETMRL",
  "GETSTATUS"
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_i3c_handle_t    *pI3C; /* pointer referencing the I3C handle from the generated code */
volatile uint8_t TransferComplete =    0U; /* Set to 1 when a data transfer operation is complete */
volatile uint8_t TransferError =    0U; /* Set to 1 when a transmission or reception error is detected */
uint8_t SensorConfigured = 0U;

/* Private functions prototype -----------------------------------------------*/
static void CtrlTransferCpltCallback(hal_i3c_handle_t *hi3c);
static void TransferErrorCallback(hal_i3c_handle_t *pI3C);
static app_status_t HandleTransferError(hal_status_t hal_status, uint32_t i3c_error_code);
static void DisplayCCCValue(void);


/** ########## Step 1 ##########
  * Initialization of the I3C peripheral and registers user callbacks for controller events.
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

  PRINTF("[INFO] Step 1: I3C initialized. \n");

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
      * parameters.
      */
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
    if (HAL_I3C_CTRL_InitTransferCtxTx(&ContextBuffers, DISECData, 1U) != HAL_OK)
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

    /* Reset transfer flags for the next asynchronous transfer. */
    TransferComplete = 0U;
    TransferError = 0U;

    /* Start transfer Direct CCC Command in interrupt mode*/
    hal_status = HAL_I3C_CTRL_Transfer_IT(pI3C, &ContextBuffers);
    if (hal_status != HAL_OK)
    {
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status = HandleTransferError(hal_status, i3c_error_code);
      goto _app_process_exit;
    }

    /* Wait for completion or error indicated by callbacks */
    while ((TransferComplete == 0U) && (TransferError == 0U))
    {
      __WFI();
    }

    if (TransferError == 1U)
    {
      /* The write transfer is not correctly completed. Report the error and exit the process. */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status = HandleTransferError(hal_status, i3c_error_code);
      goto _app_process_exit;
    }

    PRINTF("[INFO] Step 2: CCC Broadcast command sent.\n");

    /** ########## Step 3 ##########
      * The controller sends the configuration buffer to the sensor.
      */
    /***** Direct ******/
    /* Configure the transfer context */
    if (HAL_I3C_CTRL_InitTransferCtxTc(&ContextBuffers, ControlBuffer, CONTROL_BUFFER_SIZE) != HAL_OK)
    {
      goto _app_process_exit;
    }

    /* Initialize the transfer context for transmission with SETDASAData .*/
    if (HAL_I3C_CTRL_InitTransferCtxTx(&ContextBuffers, SETDASAData, 1U) != HAL_OK)
    {
      goto _app_process_exit;
    }

    /* Build a transfer context from CCC transfer descriptor */
    if (HAL_I3C_CTRL_BuildTransferCtxCCC(&ContextBuffers,
                                         DirectCCC,
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
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status = HandleTransferError(hal_status, i3c_error_code);
      goto _app_process_exit;
    }

    /* Wait for completion or error indicated by callbacks */
    while ((TransferComplete == 0U) && (TransferError == 0U))
    {
      __WFI();
    }

    if (TransferError == 1U)
    {
      /* The write transfer is not correctly completed. Report the error and exit the process. */
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

    SensorConfigured = 1U;
    PRINTF("[INFO] Step 3: Sensor is configured.\n");
  }

  /** ########## Step 4 ##########
    * The controller repeatedly sends a list of commands to the sensor, which
    * process the commands by sending or receiving the associated data. The
    * received values are displayed in real time.
  */
  /* Initialize the transfer context for transmission with DirectWriteCCC. */
  if (HAL_I3C_CTRL_InitTransferCtxTx(&ContextBuffers, (uint8_t *)&DirectWriteCCC, DIRECT_WRITE_CCC_SIZE) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Initialize the transfer context for reception with RxBuffer. */
  if (HAL_I3C_CTRL_InitTransferCtxRx(&ContextBuffers, RxBuffer, BUFFER_SIZE) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Build a transfer context from CCC transfer descriptor */
  if (HAL_I3C_CTRL_BuildTransferCtxCCC(&ContextBuffers,
                                       CCCList,
                                       CCCLIST_SIZE,
                                       HAL_I3C_CCC_DIRECT_WITHOUT_DEFBYTE_RESTART) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Reset transfer flags for the next asynchronous transfer. */
  TransferComplete = 0U;
  TransferError = 0U;

  /* Start transfer Direct CCC Command in DMA mode*/
  hal_status = HAL_I3C_CTRL_Transfer_DMA(pI3C, &ContextBuffers);
  if (hal_status != HAL_OK)
  {
    /* An error occurs during the write/read transfer. Report the error and exit the process. */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
    goto _app_process_exit;
  }

  /* Waits for one of these I3C interrupts: write transfer complete or transfer error.*/
  while ((TransferComplete == 0U) && (TransferError == 0U))
  {
    /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
      */
    __WFI();
  }

  if (TransferError == 1U)
  {
    /* The write transfer is not correctly completed. Report the error and exit the process. */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
    goto _app_process_exit;
  }

  DisplayCCCValue();

  PRINTF("[INFO] ----------------- SETMRL and SETMWL updated ---------------\n");

  /* Update SETMRL and SETMWL */
  DirectWriteCCC.SETMRL_associated_data[1]++;
  DirectWriteCCC.SETMWL_associated_data[1]++;

  return_status = EXEC_STATUS_OK;
_app_process_exit:
  return return_status;
} /* end app_process */


/** De-initializes the I3C  instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference
  * only.
  */
app_status_t app_deinit(void)
{
  mx_example_i3c_deinit();
  return EXEC_STATUS_OK;
} /* end app_deinit */


/**
  * Display the different value retrieve during GET CCC.
  */
static void DisplayCCCValue(void)
{
  uint8_t index_ccc_char_list = 0;
  uint8_t offset = 0;

  PRINTF("[INFO] ------------------ GETCCC retrieve value ------------------\n");
  for (uint8_t j = 0U; j < CCCLIST_SIZE; j++)
  {
    if (CCCList[j].direction == HAL_I3C_DIRECTION_READ)
    {
      PRINTF("[INFO] %s: 0x", CommandCode[index_ccc_char_list]);
      for (uint8_t i = 1U; i <= CCCList[j].data_size_byte; i++)
      {
        if (i == CCCList[j].data_size_byte)
        {
          PRINTF("%" PRIx16 "\n", RxBuffer[offset + i - 1U]);
        }
        else
        {
          PRINTF("%" PRIx16, RxBuffer[offset + i - 1U]);
        }
      }
      offset += CCCList[j].data_size_byte;
      index_ccc_char_list++;

    }
  }
  HAL_Delay(DISPLAY_REFRESH_DELAY);
}


/** Controller transmission end of transfer callback
  * @user This implementation of the I3C controller transfer complete callback
  * can be customized. This function is executed when the controller transfer
  * complete interrupt is generated.
  */
static void CtrlTransferCpltCallback(hal_i3c_handle_t *hi3c)
{
  /* Asynchronous processing for transfer completion (Step 3) */
  TransferComplete = 1U;
}

/**
  * I3C transfer error callback
  * @user This implementation of the I3C controller transfer error callback can
  * be customized. This function is executed when the controller transfer error
  * interrupt is generated.
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
  HAL_I3C_GetLastErrorCodes = %" PRIu32 ". TRYING AGAIN.\n",
         hal_status, i3c_error_code);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */

