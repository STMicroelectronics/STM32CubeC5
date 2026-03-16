/**  ******************************************************************************
  * file           : example.c
  * brief          : I3C Controller direct transfer example using DMA mode and HAL API.
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

/* @user Target device address for I3C communication. */
#define DEVICE_TARGET_ADDR      0x32U

/* @user Maximum number of initial attempts for dynamic address assignment. */
#define DAA_MAX_ATTEMPTS    2U

/* Direct Command code */
#define I3C_DIRECT_SETMWL_CCC   0x89
#define I3C_DIRECT_SETMRL_CCC   0x8A

#define I3C_DIRECT_GETMWL_CCC     0x8B
#define I3C_DIRECT_GETMRL_CCC     0x8C
#define I3C_DIRECT_GETPID_CCC     0x8D
#define I3C_DIRECT_GETBCR_CCC     0x8E
#define I3C_DIRECT_GETDCR_CCC     0x8F
#define I3C_DIRECT_GETSTATUS_CCC  0x90

/* DirectWrite CCC payload size: 2 bytes (SETMRL data) + 2 bytes (SETMWL data) = 4 bytes. */
#define DIRECT_WRITE_CCC_SIZE     (2+2)
/* Size of the data to be received from the target device */
#define DATA_SIZE               13U

/* Size of the RX Buffer in bytes. */
#define BUFFER_SIZE             31U

/**
  * Returns the number of elements in an array.
  */
#define COUNTOF(arr)            (sizeof(arr) / sizeof((arr)[0]))

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_i3c_handle_t *pI3C;   /* pointer referencing the I3C handle from the generated code */

/* Context buffer related to Frame context, contain different buffer value for a communication */
hal_i3c_transfer_ctx_t ContextBuffers;

/* Set to 1 when a data transfer operation is complete */
volatile uint8_t TransferComplete   = 0U;
/* Set to 1 when a transmission or reception error is detected */
volatile uint8_t TransferError = 0U;
/* Set to 1 when the Dynamic Address Assignment process reaches completion */
volatile uint8_t DAAProcessComplete = 0U;

/** Reception buffer for CPU and DMA.
  * - Buffer placed in non-cacheable memory for data cache consistency.
  * Mandatory with data cache enabled, harmless otherwise: portable across STM32 series.
  * - Buffer aligned for DMA constraints. As DMA is configured for bytes transfer, byte-alignment is required.
  * This is always ensured but the aligned(1) directive is harmless.
  */
__attribute__((section(".non_cacheable_variables"), aligned(1)))
uint8_t RxBuffer[BUFFER_SIZE] = {0U};

/* Buffer for transfer context */
uint32_t aControlBuffer[20] __attribute__((section("noncacheable_buffer")));

/* CCC names used by PrintCommandResults() (trace only). */
char *CommandCode[] =
{
  "GETMWL",
  "GETMRL",
  "GETPID",
  "GETBCR",
  "GETDCR",
  "GETSTATUS"
};

/* Bytes received for each GET* CCC above (trace only). Sum must match DATA_SIZE. */
uint8_t CommandCodeSize[] = {2U, 2U, 6U, 1U, 1U, 1U};

/* Structure holding associated data for SETMRL and SETMWL CCC write command */
struct
{
  uint8_t SETMRL_Direct_associated_data[2];
  uint8_t SETMWL_associated_data[2];
}
DirectWriteCCC =
{
  .SETMRL_Direct_associated_data = {0x0, 0x4},
  .SETMWL_associated_data = {0x0, 0x4}
};

/* Descriptor array for direct I3C CCC transactions write and read to the target device */
hal_i3c_ccc_desc_t DirectWriteRead_CCC_Descriptor[8] =
{
  {DEVICE_TARGET_ADDR, I3C_DIRECT_SETMWL_CCC,   2U, HAL_I3C_DIRECTION_WRITE},
  {DEVICE_TARGET_ADDR, I3C_DIRECT_GETMWL_CCC,   2U, HAL_I3C_DIRECTION_READ},
  {DEVICE_TARGET_ADDR, I3C_DIRECT_SETMRL_CCC,   2U, HAL_I3C_DIRECTION_WRITE},
  {DEVICE_TARGET_ADDR, I3C_DIRECT_GETMRL_CCC,   2U, HAL_I3C_DIRECTION_READ},
  {DEVICE_TARGET_ADDR, I3C_DIRECT_GETPID_CCC,   6U, HAL_I3C_DIRECTION_READ},
  {DEVICE_TARGET_ADDR, I3C_DIRECT_GETBCR_CCC,   1U, HAL_I3C_DIRECTION_READ},
  {DEVICE_TARGET_ADDR, I3C_DIRECT_GETDCR_CCC,   1U, HAL_I3C_DIRECTION_READ},
  {DEVICE_TARGET_ADDR, I3C_DIRECT_GETSTATUS_CCC,   1U, HAL_I3C_DIRECTION_READ}
};

/* Private functions prototype -----------------------------------------------*/
/* Handles actions after a successful data transfer */
static app_status_t HandleTransferCplt(void);
/* Handles actions in case of a data transfer error */
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t i3c_error_code);
/* Run the I3C Dynamic Address Assignment (ENTDAA) sequence and waits for completion (with retries). */
static app_status_t PerformDAAProcess(void);
/* Functions allowing the user to configure dynamically the I3C callbacks instead of weak functions */
static void CtrlDAACpltCallback(hal_i3c_handle_t *pI3C);
static void CtrlTgtReqDynAddrCallback(hal_i3c_handle_t *hi3c, uint64_t targetPayload);
static void CtrlXferCpltCallback(hal_i3c_handle_t *pI3C);
static void TransferErrorCallback(hal_i3c_handle_t *pI3C);
static void PrintCommandResults(uint8_t *RxBuffer, char *CommandCode[], uint8_t CommandCodeSize[], uint8_t numCommands);


/** ########## Step 1 ##########
  * Initialization of the I3C peripheral is triggered by the application code.
  * This example demonstrates I3C communication in DMA mode.
  * The user callbacks for I3C controller events are registered:
  * Transfer complete
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status;       /* Memorizes the HAL status of the I3C TX/RX operations */
  uint32_t i3c_error_code;       /* Memorizes the I3C error code limited to the last process */

  pI3C = mx_example_i3c_init();
  if (pI3C == NULL)
  {
    goto _app_init_exit;
  }

  /* Register the I3C Controller callbacks */
  if (HAL_I3C_CTRL_RegisterTransferCpltCallback(pI3C, CtrlXferCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_CTRL_RegisterDAACpltCallback(pI3C, CtrlDAACpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_CTRL_RegisterTgtReqDynAddrCallback(pI3C, CtrlTgtReqDynAddrCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_RegisterErrorCallback(pI3C, TransferErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /* Reset the transfer context to ensure a clean state before configuring new transfer parameters. */
  hal_status = HAL_I3C_CTRL_ResetTransferCtx(&ContextBuffers);
  if (hal_status != HAL_OK)
  {
    /* An error occurs when resetting a transfer. */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status  = HandleTransferError(hal_status, i3c_error_code);
  }

  /* Initialize the transfer context control structure with the control buffer and descriptor size. */
  hal_status = HAL_I3C_CTRL_InitTransferCtxTc(
                 &ContextBuffers,
                 aControlBuffer,
                 HAL_I3C_GET_CTRL_BUFFER_SIZE_WORD(
                   COUNTOF(DirectWriteRead_CCC_Descriptor),
                   HAL_I3C_CCC_DIRECT_WITHOUT_DEFBYTE_RESTART));
  if (hal_status != HAL_OK)
  {
    /* An error occurs at the transfer context */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
  }

  /* Initialize the transfer context for transmission with BufferB. */
  hal_status = HAL_I3C_CTRL_InitTransferCtxTx(&ContextBuffers,
                                              (uint8_t *) &DirectWriteCCC,
                                              DIRECT_WRITE_CCC_SIZE);
  if (hal_status != HAL_OK)
  {
    /* An error occurs at the initialise the transfer context with BufferB */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
  }

  /* Initialize the transfer context for reception with RxBuffer. */
  hal_status = HAL_I3C_CTRL_InitTransferCtxRx(&ContextBuffers, RxBuffer, DATA_SIZE);
  if (hal_status != HAL_OK)
  {
    /* An error occurs while initializing the transfer context for reception with RxBuffer. */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
  }

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status;    /* Memorizes the HAL status of the I3C TX/RX operations */
  uint32_t i3c_error_code;    /* Memorizes the I3C error code limited to the last process */


  /** ########## Step 2 ##########
    * Controller initiates dynamic address assignment by sending ENTDAA CCC command.
    * Each target responds with its payload; controller assigns dynamic addresses in sequence.
    * All DAA logic is encapsulated in the PerformDAAProcess() function.
    * I3C target must be initialized after I3C controller to avoid error during the first DAA.
    */

  return_status = PerformDAAProcess();
  if (return_status != EXEC_STATUS_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2: DAA process and transfer context configuration COMPLETED.\n");

  /** ########## Step 3 ##########
    * Transmits in DMA mode the DirectWriteCCC, which contains fixed-length data arrays,
    * and receives another fixed-length data array buffer, both using I3C in DMA mode.
    */

  hal_status = HAL_I3C_CTRL_BuildTransferCtxCCC(&ContextBuffers,
                                                DirectWriteRead_CCC_Descriptor,
                                                COUNTOF(DirectWriteRead_CCC_Descriptor),
                                                HAL_I3C_CCC_DIRECT_WITHOUT_DEFBYTE_RESTART);
  if (hal_status != HAL_OK)
  {
    /* An error occurs while building the private transfer context. */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
    goto _app_process_exit;
  }

  hal_status = HAL_I3C_CTRL_Transfer_DMA(pI3C, &ContextBuffers);
  if (hal_status != HAL_OK)
  {
    /* An error occurs during the write/read transfer. Report the error and restart the communication attempt. */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
    goto _app_process_exit;
  }

  /** ########## Step 4 ##########
    * Waits for one of these I3C interrupts: write transfer complete or transfer error.
    */
  while ((TransferComplete == 0) && (TransferError == 0))
  {
    /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
      */
    __WFI();
  }

  if (TransferError == 1)
  {
    /* The read and write transfer did not complete successfully. Report the error and restart the communication. */
    i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
    return_status = HandleTransferError(hal_status, i3c_error_code);
    goto _app_process_exit;
  }

  /** ########## Step 5 ##########
    * Checks the correctness of the received buffer after the data transfer is completed.
    */
  return_status = HandleTransferCplt();

  /* Print received CCC responses (trace only). */
  PrintCommandResults(RxBuffer, CommandCode, CommandCodeSize, (uint8_t)COUNTOF(CommandCodeSize));

_app_process_exit:
  return return_status;
} /* end app_process */


/** brief:  Initiates and completes Dynamic Address Assignment (DAA) for all targets using ENTDAA.
  * param:  none
  * retval: example status (EXEC_STATUS_OK if successful, EXEC_STATUS_ERROR otherwise)
  */
static app_status_t PerformDAAProcess(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status;    /* Memorizes the HAL status of the last I3C operation */
  uint32_t i3c_error_code;    /* Memorizes the I3C error code limited to the last process */
  uint32_t daa_attempts = 0U; /* Memorizes the number of attempts for Dynamic Address Assignment */

  while ((daa_attempts < DAA_MAX_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Reset DAA status flags before starting a new DAA attempt */
    TransferError = 0U;
    DAAProcessComplete = 0U;

    daa_attempts++;

    /* Initiate Dynamic Address Assignment (DAA) process for the controller */
    hal_status = HAL_I3C_CTRL_DynAddrAssign_IT(pI3C, HAL_I3C_DYN_ADDR_ONLY_ENTDAA);
    if (hal_status != HAL_OK)
    {
      /* Error occurred during DAA process initiation. Handle and retry. */
      goto _app_process_exit;

    }

    while ((DAAProcessComplete == 0) && (TransferError == 0))
    {
      /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
        */
      __WFI();
    }

    if (TransferError == 1)
    {
      /* Transfer process did not complete as expected. Report the error and restart the communication attempt. */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status = HandleTransferError(hal_status, i3c_error_code);
      continue;
    }
    else
    {
      return_status = EXEC_STATUS_OK;

    }
  }

_app_process_exit:
  return return_status;
}


/**
  * Dynamic Address Assignment `DAA` complete callback
  * @user This implementation of the I3C DAA complete callback can be customized.
  * This function is executed when the DAA process is completed.
  */
static void CtrlDAACpltCallback(hal_i3c_handle_t *hi3c)
{
  /* Asynchronous processing related to DAA completion */
  DAAProcessComplete = 1U;
}


/**
  * Target requests dynamic address callback
  * @user This implementation of the I3C target request dynamic address callback can be customized.
  * This function is executed when a target requests a dynamic address assignment.
  */
static void CtrlTgtReqDynAddrCallback(hal_i3c_handle_t *hi3c, uint64_t targetPayload)
{
  /* Assign the dynamic address to the target device */
  HAL_I3C_CTRL_SetDynAddr(hi3c, DEVICE_TARGET_ADDR);
}


/** Controller transmission end of transfer callback
  * @user This implementation of the I3C controller transfer complete callback can be customized.
  * This function is executed when the controller transfer complete interrupt is generated.
  */
static void CtrlXferCpltCallback(hal_i3c_handle_t *hi3c)
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

/**
  * brief                 Prints the results of received CCC command responses.
  * param RxBuffer        Pointer to the buffer containing received data.
  * param CommandCode     Array of command code names as strings.
  * param CommandCodeSize Array of data sizes for each command.
  * retval                None
  */
static void PrintCommandResults(uint8_t *RxBuffer, char *CommandCode[], uint8_t CommandCodeSize[], uint8_t numCommands)
{
  uint8_t offset = 0;

  PRINTF("[INFO] Controller - ");
  for (uint8_t i = 0; i < numCommands; i++)
  {
    PRINTF("%s = 0x", CommandCode[i]);

    for (uint8_t j = 0; j < CommandCodeSize[i]; j++)
    {
      PRINTF("%01X", RxBuffer[offset + j]);
    }

    offset += CommandCodeSize[i];

    if (i < (uint8_t)(numCommands - 1U))
    {
      PRINTF(" , ");
    }
  }
  PRINTF(".\n");
}

/** De-initializes the I3C and DMA instances before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_i3c_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** brief:  Checks the correctness of the data when the transfer is completed.
  * param:  none
  * retval: example status
  */
static app_status_t HandleTransferCplt(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  PRINTF("[INFO] Step 3: Data transfer and reception completed.\n");

  return_status = EXEC_STATUS_OK;
  return return_status;
} /* end HandleTransferCplt */


/** brief:  This function is executed in case of a data transfer error.
  * param hal_status:  HAL status of the I3C TX/RX operations.
  * param i3c_error_code:  I3C Error Code.
  * retval: example status
  */
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t i3c_error_code)
{
  PRINTF("[ERROR] Controller - Communication ERROR: hal_status = %" PRIu32 ", \
  HAL_I3C_GetLastErrorCodes = %" PRIu32 ". TRYING AGAIN.\n", hal_status, i3c_error_code);

  /* Take the time to check the setup before restarting the communication */
  HAL_Delay(1000U);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
