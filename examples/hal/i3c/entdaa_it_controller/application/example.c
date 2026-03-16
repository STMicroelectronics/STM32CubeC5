/**
  ******************************************************************************
  * file           : example.c
  * brief          : I3C Controller example for multi-target ENTDAA using interrupt-driven events.
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

/* Target descriptor structure for ENTDAA multi-target management */
typedef struct
{
  char         *target_name;        /* Human-readable name or reference for the target device */
  uint32_t      target_id;          /* Unique identifier for the target on the I3C bus
                                       (can link to index in device table) */
  uint64_t      target_bcr_dcr_pid; /* Concatenated value of PID, BCR, and DCR for device identification
                                       and ENTDAA verification */
  uint8_t       static_addr;        /* Static address from device datasheet (used before dynamic assignment) */
  uint8_t       dynamic_addr;       /* Dynamic address assigned by controller during ENTDAA */
} target_desc_t;

/* Private define ------------------------------------------------------------*/

/** Target device addresses for I3C communication.
  * @user: it possible to modify these addresses as needed.
  */
#define DEVICE_TARGET1_ADDR      0x32U
#define DEVICE_TARGET2_ADDR      0x34U

/* @user Maximum number of initial attempts for dynamic address assignment. */
#define DAA_MAX_ATTEMPTS    2U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_i3c_handle_t *pI3C;   /* Pointer referencing the I3C handle from the generated code */

/* Set to 1 when the Dynamic Address Assignment (DAA) process reaches completion */
volatile uint8_t DAAProcessComplete = 0U;
/* Set to 1 when a transmission or reception error is detected */
volatile uint8_t DAAError = 0U;

uint32_t TargetCount = 0U; /* Number of targets detected and assigned during ENTDAA */

/** Descriptor table for known targets.
  * @user: it possible to modify STATIC_ADDR and DYNAMIC_ADDR as needed for your hardware.
  */
target_desc_t Targets_Descriptor[2] =
{
  {"", 0U, 0, 0, DEVICE_TARGET1_ADDR},
  {"", 1U, 0, 0, DEVICE_TARGET2_ADDR},
};

/* Private functions prototype -----------------------------------------------*/
static app_status_t HandleDAAError(hal_status_t hal_status, uint32_t i3c_error_code);

/* Functions allowing the user to configure dynamically the I3C callbacks instead of weak functions */
static void CtrlTgtReqDynAddrCallback(hal_i3c_handle_t *hi3c, uint64_t target_payload);
static void CtrlDAACpltCallback(hal_i3c_handle_t *pI3C);
static void DAAErrorCallback(hal_i3c_handle_t *pI3C);


/** ########## Step 1 ##########
  * The I3C peripheral is initialized by the application code.
  * User callbacks for I3C controller events are registered.
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
  if (HAL_I3C_CTRL_RegisterTgtReqDynAddrCallback(pI3C, CtrlTgtReqDynAddrCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_RegisterErrorCallback(pI3C, DAAErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_I3C_CTRL_RegisterDAACpltCallback(pI3C, CtrlDAACpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status;       /* Memorizes the HAL status of the I3C operations */
  uint32_t daa_attempts = 0U;    /* Memorizes the number of DAA retry attempts */
  uint32_t i3c_error_code;       /* Memorizes the I3C error code limited to the last process */


  /** ########## Step 2 ##########
    * Multi-target ENTDAA: Controller initiates dynamic address assignment by sending ENTDAA CCC command.
    * Each target responds with its payload; controller assigns dynamic addresses in sequence.
    * The process repeats until all targets are assigned or no further responses are received.
    */

  while ((daa_attempts < DAA_MAX_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Reset DAA status flags before starting a new DAA attempt */
    DAAProcessComplete = 0U;
    DAAError = 0U;

    daa_attempts++;

    /* Initiate Dynamic Address Assignment (DAA) process for the controller */
    hal_status = HAL_I3C_CTRL_DynAddrAssign_IT(pI3C, HAL_I3C_DYN_ADDR_ONLY_ENTDAA);
    if (hal_status != HAL_OK)
    {
      /* Error occurred during DAA process initiation. */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status = HandleDAAError(hal_status, i3c_error_code);
      goto _app_process_exit;

    }

    PRINTF("[INFO] Step 2: DAA initiation COMPLETED.");

    /** ########## Step 3 ##########
      * Wait for completion of the DAA process or a DAA error interrupt.
      */

    while ((DAAProcessComplete == 0) && (DAAError == 0))
    {
      /** Put the CPU in Wait For Interrupt state. An I3C interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the IT mode. It can be replaced by your own code.
        */
      __WFI();
    }

    if (DAAError == 1)
    {
      /* DAA process did not complete as expected. Report the error and restart the communication attempt. */
      i3c_error_code = HAL_I3C_GetLastErrorCodes(pI3C);
      return_status = HandleDAAError(hal_status, i3c_error_code);
      continue;
    }
    else
    {
      return_status = EXEC_STATUS_OK;
      PRINTF("[INFO] Step 3: DAA process COMPLETED. Number of targets detected: %" PRIu32 "\n", (uint32_t)TargetCount);
    }
  }

_app_process_exit:
  return return_status;
} /* end app_process */


/**
  * Target requests dynamic address callback
  * @user This implementation of the I3C target request dynamic address callback can be customized.
  * This function is executed when a target requests a dynamic address assignment.
  */
static void CtrlTgtReqDynAddrCallback(hal_i3c_handle_t *hi3c, uint64_t target_payload)
{
  /* Assign received payload and dynamic address to the target */
  Targets_Descriptor[TargetCount].target_bcr_dcr_pid = target_payload;
  HAL_I3C_CTRL_SetDynAddr(hi3c, Targets_Descriptor[TargetCount].dynamic_addr);
  TargetCount++;
}


/**
  * Dynamic Address Assignment (DAA) complete callback
  * @user This implementation of the I3C DAA complete callback can be customized.
  * This function is executed when the DAA process is completed.
  */
static void CtrlDAACpltCallback(hal_i3c_handle_t *hi3c)
{
  /* Asynchronous processing related to DAA completion */
  DAAProcessComplete = 1U;
}


/**
  * I3C ENTDAA error callback (interrupt mode).
  * @user This implementation of the I3C ENTDAA error callback can be customized.
  * This function is executed when an error interrupt occurs during the Dynamic Address Assignment (ENTDAA) procedure.
  */
static void DAAErrorCallback(hal_i3c_handle_t *hi3c)
{
  /* Signal ENTDAA error during Dynamic Address Assignment procedure */
  DAAError = 1U;
}


/** De-initializes the I3C instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_i3c_deinit();
  return EXEC_STATUS_OK;
} /* end app_deinit */


/** brief:  This function is executed in case of an ENTDAA (Dynamic Address Assignment) error.
  * param hal_status:  HAL status of the I3C operation during ENTDAA.
  * param i3c_error_code:  I3C error code during ENTDAA.
  * retval: example status
  */
static app_status_t HandleDAAError(hal_status_t hal_status, uint32_t i3c_error_code)
{
  PRINTF("[ERROR] Controller - ENTDAA ERROR: hal_status = %" PRIu8 ", HAL_I3C_GetLastErrorCodes = %" PRIu32
         "TRYING AGAIN.\n", hal_status, i3c_error_code);

  return EXEC_STATUS_ERROR;
} /* end HandleDAAError */
