/**
  ******************************************************************************
  * file           : example.c
  * brief          : I3C direct polling controller example with LL API
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
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* @user Target device information for MX_I3Cx communication: update target information as needed for your hardware */
#define DEVICE_TARGET_NAME         "Device target"
#define DEVICE_TARGET_ID           (0U)
#define DEVICE_TARGET_STATIC_ADDR  (0U)
#define DEVICE_TARGET_DYNAMIC_ADDR (0x32U)

/* @user Maximum number of initial attempts for dynamic address assignment. */
#define MAX_DAA_ATTEMPTS           (10U)

/* Size of the control buffer in 32-bit words: 2 control words per CCC descriptor. */
#define CONTROL_BUFFER_SIZE        (2U * I3C_DESC_OPERATION_NUM)

/* @user Buffer size defines for MX_I3Cx CCC transfers (bytes/words) */
#define DIRECT_WRITE_CCC_SIZE      (4U)  /* SETMRL(2) + SETMWL(2) = 4 bytes */
#define DIRECT_READ_CCC_SIZE       (13U) /* GETMWL(2) + GETMRL(2) + GETPID(6) + GETBCR(1)
                                         + GETDCR(1) + GETSTATUS(1) = 13 bytes */

/**
  * MX_I3Cx CCC (Common Command Code) Operations code Defines.
  * For more details, see the I3C reference manual, CCC (Common Command Codes) section.
  */
#define I3C_DIRECT_SETMWL_CCC      (0x89)  /* Set Max Write Length                */
#define I3C_DIRECT_SETMRL_CCC      (0x8A)  /* Set Max Read Length                 */
#define I3C_DIRECT_GETMWL_CCC      (0x8B)  /* Get Max Write Length                */
#define I3C_DIRECT_GETMRL_CCC      (0x8C)  /* Get Max Read Length                 */
#define I3C_DIRECT_GETPID_CCC      (0x8D)  /* Get Provisional ID                  */
#define I3C_DIRECT_GETBCR_CCC      (0x8E)  /* Get Bus Characteristics Register    */
#define I3C_DIRECT_GETDCR_CCC      (0x8F)  /* Get Device Characteristics Register */
#define I3C_DIRECT_GETSTATUS_CCC   (0x90)  /* Get Status                          */

/* Data size for each CCC operation (in bytes) */
#define I3C_SETMWL_DATA_SIZE       (2U)
#define I3C_GETMWL_DATA_SIZE       (2U)
#define I3C_SETMRL_DATA_SIZE       (2U)
#define I3C_GETMRL_DATA_SIZE       (2U)
#define I3C_GETPID_DATA_SIZE       (6U)
#define I3C_GETBCR_DATA_SIZE       (1U)
#define I3C_GETDCR_DATA_SIZE       (1U)
#define I3C_GETSTATUS_DATA_SIZE    (1U)

/* MX_I3Cx transfer direction values for CCC operations */
#define I3C_DIRECTION_WRITE        (0U)
#define I3C_DIRECTION_READ         (0x10000UL)


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Buffer for transfer context */
uint32_t ControlBuffer[CONTROL_BUFFER_SIZE];

/* RX buffer used for MX_I3Cx reception */
uint8_t RxBuffer[DIRECT_READ_CCC_SIZE];

/* @user Structure holding associated data for SETMRL and SETMWL CCC write commands */
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

/**
  * Descriptor array for direct CCC (Common Command Code) write/read operations.
  * Each entry defines a single CCC transaction for the MX_I3Cx controller:
  *   {target_addr, ccc_opcode, data_size_bytes, direction}
  */
i3c_ccc_desc_t DirectWriteRead_CCC_Descriptor[I3C_DESC_OPERATION_NUM] =
{
  {DEVICE_TARGET_DYNAMIC_ADDR, I3C_DIRECT_SETMWL_CCC,    I3C_SETMWL_DATA_SIZE,    I3C_DIRECTION_WRITE},
  {DEVICE_TARGET_DYNAMIC_ADDR, I3C_DIRECT_GETMWL_CCC,    I3C_GETMWL_DATA_SIZE,    I3C_DIRECTION_READ },
  {DEVICE_TARGET_DYNAMIC_ADDR, I3C_DIRECT_SETMRL_CCC,    I3C_SETMRL_DATA_SIZE,    I3C_DIRECTION_WRITE},
  {DEVICE_TARGET_DYNAMIC_ADDR, I3C_DIRECT_GETMRL_CCC,    I3C_GETMRL_DATA_SIZE,    I3C_DIRECTION_READ },
  {DEVICE_TARGET_DYNAMIC_ADDR, I3C_DIRECT_GETPID_CCC,    I3C_GETPID_DATA_SIZE,    I3C_DIRECTION_READ },
  {DEVICE_TARGET_DYNAMIC_ADDR, I3C_DIRECT_GETBCR_CCC,    I3C_GETBCR_DATA_SIZE,    I3C_DIRECTION_READ },
  {DEVICE_TARGET_DYNAMIC_ADDR, I3C_DIRECT_GETDCR_CCC,    I3C_GETDCR_DATA_SIZE,    I3C_DIRECTION_READ },
  {DEVICE_TARGET_DYNAMIC_ADDR, I3C_DIRECT_GETSTATUS_CCC, I3C_GETSTATUS_DATA_SIZE, I3C_DIRECTION_READ }
};

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The MX_I3Cx peripheral is initialized by the application code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_i3cx_init() == NULL)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  uint32_t daa_attempts = 0U; /* Memorizes the number of attempts made to exchange a message between the boards */


  /** ########## Step 2 ##########
    * Initiates and manages the Dynamic Address Assignment (DAA) process for the MX_I3Cx controller
    * Warning: if the target MX_I3Cx peripheral is enabled before the controller starts the first ENTDAA,
    * the first DAA attempt can fail on some setups.
    */
  while (daa_attempts < MAX_DAA_ATTEMPTS)
  {

    daa_attempts ++;

    if (I3C_AssignDynamicAddress(DEVICE_TARGET_DYNAMIC_ADDR) != SYSTEM_OK)
    {
      continue;

    }

    break;

  } /* end while */


  /** ########## Step 3 ##########
    * Builds the control buffer for the upcoming sequence of direct MX_I3Cx CCC (Common Command Code) operations.
    * This step configures the transfer context for all required CCC commands and data phases.
    */

  I3C_BuildCCCTransferContext(ControlBuffer, DirectWriteRead_CCC_Descriptor);


  /** ########## Step 4 ##########
    * Performs the MX_I3Cx transfer in polling mode: transmit the DirectWriteCCC (with fixed-length data arrays)
    * and receives the response buffer, both using MX_I3Cx polling transfers for reliable communication.
    */

  /* Short delay before starting transfer */
  DelayMs(5U);

  I3C_Transfer(ControlBuffer,
               CONTROL_BUFFER_SIZE,
               (const uint8_t *)&DirectWriteCCC,
               DIRECT_WRITE_CCC_SIZE,
               RxBuffer,
               DIRECT_READ_CCC_SIZE);

  return_status = EXEC_STATUS_OK;

  return return_status;
} /* end app_process */


/** De-initializes the MX_I3Cx instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_i3cx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */

