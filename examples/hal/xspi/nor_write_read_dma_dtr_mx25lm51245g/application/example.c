/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to use the MX25LM51245G Octo SPI NOR Flash memory
  *                  in DMA mode with HAL API.
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
#include <string.h> /* importing memcmp function */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Timeout period in milliseconds for handling XSPI operations */
#define TIMEOUT                          2000U

/** Power-up guard time in milliseconds to cover:
  * LDO ramp-up and stabilization.
  * MX25LM51245G internal power-up/reset time before accepting XSPI commands.
  */
#define MX25_LDO_POWER_ON_DELAY_MS        300U

/* Specific parameters of the MX25LM51245G external memory from the datasheet */
/* Flash commands */
#define MX25_OCTAL_WRITE_ENABLE_CMD      0x06F9U
#define MX25_OCTAL_READ_STATUS_REG_CMD   0x05FAU
#define MX25_OCTAL_IO_DTR_READ_CMD       0xEE11U
#define MX25_OCTAL_SECTOR_ERASE_CMD      0x21DEU
#define MX25_OCTAL_PAGE_PROG_CMD         0x12EDU

#define MX25_WRITE_ENABLE_CMD            0x06U
#define MX25_READ_STATUS_REG_CMD         0x05U
#define MX25_WRITE_CONFIG_REG2_CMD       0x72U

/* Dummy clock cycles */
#define MX25_0_DUMMY_CYCLES              0U
#define MX25_OCTAL_READ_20_DUMMY_CYCLES  20U

/* Auto-polling mode values */
#define MX25_WRITE_ENABLE_MATCH_VALUE    0x02U
#define MX25_WRITE_ENABLE_MASK_VALUE     0x02U
#define MX25_MEMORY_READY_MATCH_VALUE    0x00U
#define MX25_MEMORY_READY_MASK_VALUE     0x01U
#define MX25_AUTO_POLLING_INTERVAL       0x10U

/* Memory registers */
#define MX25_STATUS_REG_ADDR             0x0U
#define MX25_CONFIG_REG2_ADDR1           0x0U
#define MX25_CR2_DTR_OPI_ENABLE          0x2U

/* Memory organisation */
#define MX25_FIRST_SECTOR_ADDR           0x0U
#define MX25_FIRST_PAGE_ADDR             0x0U

/* Timing configuration */
/** MX25_WR_REG_MAX_DELAY_MS: Write Status/Configuration Register maximum cycle time in milliseconds.
  * @user: this time can be adjusted in order to minimize the waiting time,
  * after performing one of the mentioned commands.
  */
#define MX25_WR_REG_MAX_DELAY_MS         40U

/** BUFFER_SIZE: size of the TX and RX buffers in bytes.
  * @user: modify this parameter to set the size of the data to be written in the memory.
  * The data size must not exceed 256 bytes. Otherwise the last 256 bytes are
  * programmed and the previous ones will be disregarded.
  */
#define BUFFER_SIZE                      256U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_xspi_handle_t *pXSPI;  /* pointer referencing the XSPI handle from the generated code */

/* Buffer used for sending data to the memory, initialized at the start of the application */
uint8_t TxBuffer[BUFFER_SIZE];
/* Buffer used for receiving data from the memory */
uint8_t RxBuffer[BUFFER_SIZE] = {0U};

volatile uint8_t RxCplt = 0U; /* Set to 1 if the receive transfer is correctly completed */
volatile uint8_t TxCplt = 0U; /* Set to 1 if the transmit transfer is correctly completed */
volatile uint8_t CmdCplt = 0U; /* Set to 1 if the command transfer is correctly completed */
volatile uint8_t TransferError = 0U; /* Set to 1 if a transmission or a reception error is detected */

/* Private functions prototype -----------------------------------------------*/
static app_status_t WriteEnable(void);
static app_status_t OctalWriteEnable(void);
static app_status_t OctalAutoPollingMemReady(void);
static app_status_t OctalDtrMemCfg(void);
static void TxTransferCpltCallback(hal_xspi_handle_t *pXSPI);
static void RxTransferCpltCallback(hal_xspi_handle_t *pXSPI);
static void CmdTransferCpltCallback(hal_xspi_handle_t *pXSPI);
static void TransferErrorCallback(hal_xspi_handle_t *pXSPI);

/** ########## Step 1 ##########
  * The init of the XSPI instance is triggered by the applicative code.
  * Registers the user callbacks for XSPI events.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Set the LDO enable pin to power on the external memory */
  HAL_GPIO_WritePin(MX_LDO_EN_PORT, MX_LDO_EN_PIN, MX_LDO_EN_ACTIVE_STATE);

  /* Wait for the power-up guard time for memory supply stabilization before accepting XSPI commands */
  HAL_Delay(MX25_LDO_POWER_ON_DELAY_MS);

  pXSPI = mx_example_xspi_init();

  if (pXSPI != NULL)
  {

    return_status = EXEC_STATUS_INIT_OK;
  }

  /* Register the XSPI callbacks */
  if (HAL_XSPI_RegisterTxCpltCallback(pXSPI, TxTransferCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_XSPI_RegisterRxCpltCallback(pXSPI, RxTransferCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_XSPI_RegisterCmdCpltCallback(pXSPI, CmdTransferCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_XSPI_RegisterErrorCallback(pXSPI, TransferErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  hal_xspi_regular_cmd_t command;
  uint32_t index;

  /* Initialize the transmission buffer with consecutive values to be written in the external memory */
  for (index = 0U; index < BUFFER_SIZE; index++)
  {
    TxBuffer[index] = index;
  }

  /** ########## Step 2 ##########
    * Configures the NOR Flash memory in Octal DTR (double-transfer rate) mode
    */

  /* Enable write operation in Single-SPI mode */
  if (WriteEnable() != EXEC_STATUS_OK)
  {
    goto _app_process_exit;
  }
  /* Configure the memory to enter Octal DTR mode */
  if (OctalDtrMemCfg() != EXEC_STATUS_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2: Memory configuration COMPLETED.\n");

  /** ########## Step 3 ##########
    * Erases the first sector of the memory in interrupt mode.
    */
  if (OctalWriteEnable() != EXEC_STATUS_OK)
  {
    goto _app_process_exit;
  }

  /* Common parameters for all commands handled in this example */
  command.operation_type                  = HAL_XSPI_OPERATION_COMMON_CFG;
  command.io_select                       = HAL_XSPI_IO_7_0;
  command.alternate_bytes                 = 0U;
  command.alternate_bytes_width           = HAL_XSPI_ALTERNATE_BYTES_8BIT;
  command.alternate_bytes_mode            = HAL_XSPI_ALTERNATE_BYTES_NONE;
  command.alternate_bytes_dtr_mode_status = HAL_XSPI_ALTERNATE_BYTES_DTR_DISABLED;

  /* Specific parameters for the Sector Erase command */
  command.instruction          = MX25_OCTAL_SECTOR_ERASE_CMD;
  command.instruction_mode     = HAL_XSPI_INSTRUCTION_8LINES;
  command.instruction_width    = HAL_XSPI_INSTRUCTION_16BIT;
  command.instruction_dtr_mode_status = HAL_XSPI_INSTRUCTION_DTR_ENABLED;
  command.addr                 = MX25_FIRST_SECTOR_ADDR;
  command.addr_mode            = HAL_XSPI_ADDR_8LINES;
  command.addr_width           = HAL_XSPI_ADDR_32BIT;
  command.addr_dtr_mode_status = HAL_XSPI_ADDR_DTR_ENABLED;
  command.data_mode            = HAL_XSPI_REGULAR_DATA_NONE;
  command.data_dtr_mode_status = HAL_XSPI_DATA_DTR_ENABLED;
  command.dummy_cycle          = MX25_0_DUMMY_CYCLES;
  command.dqs_mode_status      = HAL_XSPI_DQS_DISABLED;
  command.size_byte            = 1U;
  if (HAL_XSPI_SendRegularCmd_IT(pXSPI, &command) != HAL_OK)
  {
    goto _app_process_exit;
  }

  while (CmdCplt == 0)
  {
    /** Put the CPU in Wait For Interrupt state. A XSPI interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
      */
    __WFI();
  }

  /* Wait for the end of the Sector Erase operation */
  if (OctalAutoPollingMemReady() != EXEC_STATUS_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 3: Memory erasing COMPLETED.\n");

  /** ########## Step 4 ##########
    * Writes the TX buffer to the first page of the memory in DMA mode.
    */
  if (OctalWriteEnable() != EXEC_STATUS_OK)
  {
    goto _app_process_exit;
  }

  command.instruction = MX25_OCTAL_PAGE_PROG_CMD;
  command.addr        = MX25_FIRST_PAGE_ADDR;
  command.data_mode   = HAL_XSPI_REGULAR_DATA_8LINES;
  command.size_byte   = BUFFER_SIZE;
  if (HAL_XSPI_SendRegularCmd(pXSPI, &command, TIMEOUT) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_XSPI_Transmit_DMA(pXSPI, TxBuffer) != HAL_OK)
  {
    goto _app_process_exit;
  }


  while ((TxCplt == 0) && (TransferError == 0))
  {
    /** Put the CPU in Wait For Interrupt state. A XSPI interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
      */
    __WFI();
  }

  /* Wait for the end of the Page Program operation */
  if (OctalAutoPollingMemReady() != EXEC_STATUS_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 4: Memory programming COMPLETED.\n");

  /** ########## Step 5 ##########
    * Reads back the written TxBuffer from the memory in DMA mode.
    */
  command.instruction = MX25_OCTAL_IO_DTR_READ_CMD;
  command.addr        = MX25_FIRST_PAGE_ADDR;
  command.size_byte   = BUFFER_SIZE;
  command.dummy_cycle = MX25_OCTAL_READ_20_DUMMY_CYCLES;
  if (HAL_XSPI_SendRegularCmd(pXSPI, &command, TIMEOUT) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_XSPI_Receive_DMA(pXSPI, (uint8_t *)RxBuffer) != HAL_OK)
  {
    goto _app_process_exit;
  }

  while ((RxCplt == 0) && (TransferError == 0))
  {
    /** Put the CPU in Wait For Interrupt state. A XSPI interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
      */
    __WFI();
  }

  PRINTF("[INFO] Step 5: Memory read COMPLETED.\n");

  /** ########## Step 6 ##########
    * Checks that the read data matches the written one.
    */
  if (memcmp(TxBuffer, RxBuffer, BUFFER_SIZE) != 0)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 6: Data written and read match. \n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 7 ##########
  * Deinitializes the XSPI instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  PRINTF("[INFO] Step 7: de-init \n");
  mx_example_xspi_deinit() ;

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** brief:  Configures the MX25LM51245G memory in Octal DTR mode
  * param:  None
  * retval: Application status
  */
static app_status_t OctalDtrMemCfg(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  hal_xspi_regular_cmd_t  command;
  uint8_t config_cr2;

  /* Common parameters for all commands handled in this example */
  command.operation_type                  = HAL_XSPI_OPERATION_COMMON_CFG;
  command.io_select                       = HAL_XSPI_IO_7_0;
  command.alternate_bytes                 = 0U;
  command.alternate_bytes_width           = HAL_XSPI_ALTERNATE_BYTES_8BIT;
  command.alternate_bytes_mode            = HAL_XSPI_ALTERNATE_BYTES_NONE;
  command.alternate_bytes_dtr_mode_status = HAL_XSPI_ALTERNATE_BYTES_DTR_DISABLED;

  /* Specific parameters for the Write Configuration Register command */
  command.instruction                 = MX25_WRITE_CONFIG_REG2_CMD;
  command.instruction_mode            = HAL_XSPI_INSTRUCTION_1LINE;
  command.instruction_width           = HAL_XSPI_INSTRUCTION_8BIT;
  command.instruction_dtr_mode_status = HAL_XSPI_INSTRUCTION_DTR_DISABLED;
  command.addr                        = MX25_CONFIG_REG2_ADDR1;
  command.addr_mode                   = HAL_XSPI_ADDR_1LINE;
  command.addr_width                  = HAL_XSPI_ADDR_32BIT;
  command.addr_dtr_mode_status        = HAL_XSPI_ADDR_DTR_DISABLED;
  command.data_mode                   = HAL_XSPI_REGULAR_DATA_1LINE;
  command.data_dtr_mode_status        = HAL_XSPI_DATA_DTR_DISABLED;
  command.size_byte                   = 1U;
  command.dummy_cycle                 = MX25_0_DUMMY_CYCLES;
  command.dqs_mode_status             = HAL_XSPI_DQS_DISABLED;
  if (HAL_XSPI_SendRegularCmd(pXSPI, &command, TIMEOUT) != HAL_OK)
  {
    goto _OctalDtrMemCfg_exit;
  }

  config_cr2 = MX25_CR2_DTR_OPI_ENABLE;
  if (HAL_XSPI_Transmit(pXSPI, &config_cr2, TIMEOUT) != HAL_OK)
  {
    goto _OctalDtrMemCfg_exit;
  }

  /* Wait that the memory configuration is effective */
  HAL_Delay(MX25_WR_REG_MAX_DELAY_MS);
  return_status = EXEC_STATUS_OK;

_OctalDtrMemCfg_exit:
  return return_status;
}


/** brief:  Enables writing to the external memory in Single SPI mode
  * The function sends a write enable WE command and waits its effective
  * param:  None
  * retval: Application status
  */
static app_status_t WriteEnable(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  hal_xspi_regular_cmd_t  command;
  hal_xspi_auto_polling_config_t polling_mode_cfg;

  /* Common parameters for all commands handled in this example */
  command.operation_type                  = HAL_XSPI_OPERATION_COMMON_CFG;
  command.io_select                       = HAL_XSPI_IO_7_0;
  command.alternate_bytes                 = 0U;
  command.alternate_bytes_width           = HAL_XSPI_ALTERNATE_BYTES_8BIT;
  command.alternate_bytes_mode            = HAL_XSPI_ALTERNATE_BYTES_NONE;
  command.alternate_bytes_dtr_mode_status = HAL_XSPI_ALTERNATE_BYTES_DTR_DISABLED;

  /* Specific parameters for the Write Enable command */
  command.instruction                 = MX25_WRITE_ENABLE_CMD;
  command.instruction_mode            = HAL_XSPI_INSTRUCTION_1LINE;
  command.instruction_width           = HAL_XSPI_INSTRUCTION_8BIT;
  command.instruction_dtr_mode_status = HAL_XSPI_INSTRUCTION_DTR_DISABLED;
  command.addr                        = 0U;
  command.addr_mode                   = HAL_XSPI_ADDR_NONE;
  command.addr_width                  = HAL_XSPI_ADDR_8BIT;
  command.addr_dtr_mode_status        = HAL_XSPI_ADDR_DTR_DISABLED;
  command.data_mode                   = HAL_XSPI_REGULAR_DATA_NONE;
  command.data_dtr_mode_status        = HAL_XSPI_DATA_DTR_DISABLED;
  command.size_byte                   = 1U;
  command.dummy_cycle                 = MX25_0_DUMMY_CYCLES;
  command.dqs_mode_status             = HAL_XSPI_DQS_DISABLED;
  if (HAL_XSPI_SendRegularCmd(pXSPI, &command, TIMEOUT) != HAL_OK)
  {
    goto  _WriteEnable_exit;
  }

  /* Configure the automatic polling mode to wait for write enabling in Single-SPI mode */
  command.instruction          = MX25_READ_STATUS_REG_CMD;
  command.instruction_mode     = HAL_XSPI_INSTRUCTION_1LINE;
  command.instruction_width    = HAL_XSPI_INSTRUCTION_8BIT;
  command.addr_mode            = HAL_XSPI_ADDR_NONE;
  command.data_mode            = HAL_XSPI_REGULAR_DATA_1LINE;
  command.data_dtr_mode_status = HAL_XSPI_DATA_DTR_DISABLED;
  command.size_byte            = 1U;
  if (HAL_XSPI_SendRegularCmd(pXSPI, &command, TIMEOUT) != HAL_OK)
  {
    goto  _WriteEnable_exit;
  }

  polling_mode_cfg.match_mask     = MX25_WRITE_ENABLE_MASK_VALUE;
  polling_mode_cfg.match_value    = MX25_WRITE_ENABLE_MATCH_VALUE;
  polling_mode_cfg.match_mode     = HAL_XSPI_MATCH_MODE_AND;
  polling_mode_cfg.interval_cycle = MX25_AUTO_POLLING_INTERVAL;
  polling_mode_cfg.automatic_stop_status = HAL_XSPI_AUTOMATIC_STOP_ENABLED;
  if (HAL_XSPI_ExecRegularAutoPoll(pXSPI, &polling_mode_cfg, TIMEOUT) != HAL_OK)
  {
    goto  _WriteEnable_exit;
  }

  return_status = EXEC_STATUS_OK;

_WriteEnable_exit:
  return return_status;
}

/** brief:  Enables writing to the external memory in Octo-SPI mode
  * The function sends a write enable command and waits its effective
  * param:  None
  * retval: Application status
  */
static app_status_t OctalWriteEnable(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  hal_xspi_regular_cmd_t  command;
  hal_xspi_auto_polling_config_t polling_mode_cfg;

  /* Common parameters for all commands handled in this example */
  command.operation_type                  = HAL_XSPI_OPERATION_COMMON_CFG;
  command.io_select                       = HAL_XSPI_IO_7_0;
  command.alternate_bytes                 = 0U;
  command.alternate_bytes_width           = HAL_XSPI_ALTERNATE_BYTES_8BIT;
  command.alternate_bytes_mode            = HAL_XSPI_ALTERNATE_BYTES_NONE;
  command.alternate_bytes_dtr_mode_status = HAL_XSPI_ALTERNATE_BYTES_DTR_DISABLED;

  /* Specific parameters for the Write Enable command */
  command.instruction                  = MX25_OCTAL_WRITE_ENABLE_CMD;
  command.instruction_mode             = HAL_XSPI_INSTRUCTION_8LINES;
  command.instruction_width            = HAL_XSPI_INSTRUCTION_16BIT;
  command.instruction_dtr_mode_status  = HAL_XSPI_INSTRUCTION_DTR_ENABLED;
  command.addr                         = 0U;
  command.addr_mode                    = HAL_XSPI_ADDR_NONE;
  command.addr_width                   = HAL_XSPI_ADDR_8BIT;
  command.addr_dtr_mode_status         = HAL_XSPI_ADDR_DTR_DISABLED;
  command.data_mode                    = HAL_XSPI_REGULAR_DATA_NONE;
  command.data_dtr_mode_status         = HAL_XSPI_DATA_DTR_DISABLED;
  command.size_byte                    = 1U;
  command.dummy_cycle                  = MX25_0_DUMMY_CYCLES;
  command.dqs_mode_status              = HAL_XSPI_DQS_DISABLED;
  if (HAL_XSPI_SendRegularCmd(pXSPI, &command, TIMEOUT) != HAL_OK)
  {
    goto  _OctalWriteEnable_exit;
  }

  /* Configure the automatic polling mode to wait for write enabling in Octo-SPI mode */
  command.instruction          = MX25_OCTAL_READ_STATUS_REG_CMD;
  command.addr                 = MX25_STATUS_REG_ADDR;
  command.addr_mode            = HAL_XSPI_ADDR_8LINES;
  command.addr_width           = HAL_XSPI_ADDR_32BIT;
  command.addr_dtr_mode_status = HAL_XSPI_ADDR_DTR_ENABLED;
  command.data_mode            = HAL_XSPI_REGULAR_DATA_1LINE;
  command.data_dtr_mode_status = HAL_XSPI_DATA_DTR_ENABLED;
  command.size_byte            = 2U;
  command.dummy_cycle          = MX25_OCTAL_READ_20_DUMMY_CYCLES;
  command.dqs_mode_status      = HAL_XSPI_DQS_ENABLED;
  if (HAL_XSPI_SendRegularCmd(pXSPI, &command, TIMEOUT) != HAL_OK)
  {
    goto _OctalWriteEnable_exit;
  }

  polling_mode_cfg.match_mask     = MX25_WRITE_ENABLE_MASK_VALUE;
  polling_mode_cfg.match_value    = MX25_WRITE_ENABLE_MATCH_VALUE;
  polling_mode_cfg.match_mode     = HAL_XSPI_MATCH_MODE_AND;
  polling_mode_cfg.interval_cycle = MX25_AUTO_POLLING_INTERVAL;
  polling_mode_cfg.automatic_stop_status = HAL_XSPI_AUTOMATIC_STOP_ENABLED;
  if (HAL_XSPI_ExecRegularAutoPoll(pXSPI, &polling_mode_cfg, TIMEOUT) != HAL_OK)
  {
    goto _OctalWriteEnable_exit;
  }

  return_status = EXEC_STATUS_OK;

_OctalWriteEnable_exit:
  return return_status;
}

/** brief:  Reads the status register in polling mode and waits until the memory is ready
  * The function checks if there is a program/erase/write operation ongoing
  * param:  None
  * retval: Application status
  */
static app_status_t OctalAutoPollingMemReady(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  hal_xspi_regular_cmd_t  command;
  hal_xspi_auto_polling_config_t polling_mode_cfg;

  /* Common parameters for all commands handled in this example */
  command.operation_type                  = HAL_XSPI_OPERATION_COMMON_CFG;
  command.io_select                       = HAL_XSPI_IO_7_0;
  command.alternate_bytes                 = 0U;
  command.alternate_bytes_width           = HAL_XSPI_ALTERNATE_BYTES_8BIT;
  command.alternate_bytes_mode            = HAL_XSPI_ALTERNATE_BYTES_NONE;
  command.alternate_bytes_dtr_mode_status = HAL_XSPI_ALTERNATE_BYTES_DTR_DISABLED;

  /* Specific parameters for the Read Status Register command */
  command.instruction                 = MX25_OCTAL_READ_STATUS_REG_CMD;
  command.instruction_mode            = HAL_XSPI_INSTRUCTION_8LINES;
  command.instruction_width           = HAL_XSPI_INSTRUCTION_16BIT;
  command.instruction_dtr_mode_status = HAL_XSPI_INSTRUCTION_DTR_ENABLED;
  command.addr                        = MX25_STATUS_REG_ADDR;
  command.addr_mode                   = HAL_XSPI_ADDR_8LINES;
  command.addr_width                  = HAL_XSPI_ADDR_32BIT;
  command.addr_dtr_mode_status        = HAL_XSPI_ADDR_DTR_ENABLED;
  command.data_mode                   = HAL_XSPI_REGULAR_DATA_8LINES;
  command.data_dtr_mode_status        = HAL_XSPI_DATA_DTR_ENABLED;
  command.size_byte                   = 2U;
  command.dummy_cycle                 = MX25_OCTAL_READ_20_DUMMY_CYCLES;
  command.dqs_mode_status             = HAL_XSPI_DQS_ENABLED;
  if (HAL_XSPI_SendRegularCmd(pXSPI, &command, TIMEOUT) != HAL_OK)
  {
    goto _OctalAutoPollingMemReady_exit;
  }

  polling_mode_cfg.match_mask     = MX25_MEMORY_READY_MASK_VALUE;
  polling_mode_cfg.match_value    = MX25_MEMORY_READY_MATCH_VALUE;
  polling_mode_cfg.match_mode     = HAL_XSPI_MATCH_MODE_AND;
  polling_mode_cfg.interval_cycle = MX25_AUTO_POLLING_INTERVAL;
  polling_mode_cfg.automatic_stop_status = HAL_XSPI_AUTOMATIC_STOP_ENABLED;
  if (HAL_XSPI_ExecRegularAutoPoll(pXSPI, &polling_mode_cfg, TIMEOUT) != HAL_OK)
  {
    goto _OctalAutoPollingMemReady_exit;
  }

  return_status = EXEC_STATUS_OK;

_OctalAutoPollingMemReady_exit:
  return return_status;
}

/** XSPI TX Transfer complete callback
  * @user: This implementation of the XSPI TX transfer complete callback can be customized.
  */
static void TxTransferCpltCallback(hal_xspi_handle_t *pXSPI)
{
  TxCplt = 1U;
}

/** XSPI RX Transfer complete callback
  * @user: This implementation of the XSPI RX transfer complete callback can be customized.
  */
static void RxTransferCpltCallback(hal_xspi_handle_t *pXSPI)
{
  RxCplt = 1U;
}

/** XSPI Command Transfer complete callback
  * @user: This implementation of the XSPI command transfer complete callback can be customized.
  */
static void CmdTransferCpltCallback(hal_xspi_handle_t *pXSPI)
{
  CmdCplt = 1U;
}

/** XSPI TX/RX Transfer Error callback
  * @user: This implementation of the XSPI TX/RX transfer complete callback can be customized.
  */
static void TransferErrorCallback(hal_xspi_handle_t *pXSPI)
{
  /* Asynchronous processing related to step 3 */
  TransferError = 1U;
}

