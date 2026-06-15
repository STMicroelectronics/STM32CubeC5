/**
  ******************************************************************************
  * @file    mx_openbl_user.c
  * @brief   Open Bootloader user application entry point
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_freertos_license.md file
  * in the same directory as the generated code.
  * If no mx_freertos_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "mx_openbl_user.h"
#include "mx_system.h"
#include "openbl_core.h"
#include "openbl_mem.h"

#include "openbl_usart_cmd.h"
#include "openbl_itf_usart.h"
#include "openbl_i2c_cmd.h"
#include "openbl_itf_i2c.h"
#include "openbl_spi_cmd.h"
#include "openbl_itf_spi.h"
#include "openbl_spi.h"
#include "mx_usbx_app.h"
#include "openbl_itf_usb.h"
#include "openbl_usb_dfu.h"

#include "openbl_itf_iwdg.h"

#include "openbl_itf_flash.h"
#include "openbl_itf_ram.h"
#include "openbl_itf_optionbytes.h"
#include "openbl_itf_read_only.h"

/* Private typedef -----------------------------------------------------------*/
/* List of supported interfaces */
typedef enum
{
  USART_ITF,
  I2C_ITF,
  SPI_ITF,
  USB_ITF,
} openbl_itf_type_t;

/* List of supported memories */
typedef enum
{
  FLASH_MEM,
  RAM_MEM,
  OB1_MEM,
  OB2_MEM,
  OB3_MEM,
  OB4_MEM,
  RO_MEM,
} openbl_mem_type_t;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Buffers used for interfaces communication */
static uint8_t USART_DataBuffer[OPENBL_USART_RAM_BUFFER_SIZE];
static uint8_t I2C_DataBuffer[OPENBL_I2C_RAM_BUFFER_SIZE];
static uint8_t SPI_DataBuffer[OPENBL_SPI_RAM_BUFFER_SIZE];
static uint8_t USB_DataBuffer[OPENBL_USB_RAM_BUFFER_SIZE];

/* Arrays used used for interfaces commands opcodes */
uint8_t USART_CmdOpcodesList[OPENBL_USART_COMMANDS_MAX_NBR];
uint8_t I2C_CmdOpcodesList[OPENBL_I2C_COMMANDS_MAX_NBR];
uint8_t SPI_CmdOpcodesList[OPENBL_SPI_COMMANDS_MAX_NBR];

static openbl_mem_list_t mem_list;

/* USART supported operations */
static openbl_itf_ops_t USART_Ops =
{
  OPENBL_ITF_USART_Init,
  OPENBL_ITF_USART_DeInit,
  OPENBL_ITF_USART_ProtocolDetection,
  OPENBL_ITF_USART_GetCommandOpcode,
  OPENBL_ITF_USART_SendAcknowledge,
  NULL
};

/* USART supported commands */
static openbl_commands_t USART_Cmd =
{
  OPENBL_USART_GetCommand,
  OPENBL_USART_GetVersion,
  OPENBL_USART_GetID,
  OPENBL_USART_ReadMemory,
  OPENBL_USART_WriteMemory,
  OPENBL_USART_Go,
  NULL,
  NULL,
  OPENBL_USART_EraseMemory,
  OPENBL_USART_WriteProtect,
  OPENBL_USART_WriteUnprotect,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  OPENBL_USART_SpecialCommand,
  NULL
};

/* I2C supported operations */
static openbl_itf_ops_t I2C_Ops =
{
  OPENBL_ITF_I2C_Init,
  OPENBL_ITF_I2C_DeInit,
  OPENBL_ITF_I2C_ProtocolDetection,
  OPENBL_ITF_I2C_GetCommandOpcode,
  OPENBL_ITF_I2C_SendAcknowledge,
  OPENBL_ITF_I2C_SendBusyByte
};

/* I2C supported commands */
openbl_commands_t I2C_Cmd =
{
  OPENBL_I2C_GetCommand,
  OPENBL_I2C_GetVersion,
  OPENBL_I2C_GetID,
  OPENBL_I2C_ReadMemory,
  OPENBL_I2C_WriteMemory,
  OPENBL_I2C_Go,
  NULL,
  NULL,
  OPENBL_I2C_EraseMemory,
  OPENBL_I2C_WriteProtect,
  OPENBL_I2C_WriteUnprotect,
  OPENBL_I2C_NonStretchWriteMemory,
  OPENBL_I2C_NonStretchEraseMemory,
  OPENBL_I2C_NonStretchWriteProtect,
  OPENBL_I2C_NonStretchWriteUnprotect,
  NULL,
  NULL,
  NULL,
  OPENBL_I2C_SpecialCommand,
  NULL
};

/* SPI supported operations */
static openbl_itf_ops_t SPI_Ops =
{
  OPENBL_ITF_SPI_Init,
  OPENBL_ITF_SPI_DeInit,
  OPENBL_ITF_SPI_ProtocolDetection,
  OPENBL_ITF_SPI_GetCommandOpcode,
  OPENBL_ITF_SPI_SendAcknowledge,
  NULL
};

/* SPI supported commands */
openbl_commands_t SPI_Cmd =
{
  OPENBL_SPI_GetCommand,
  OPENBL_SPI_GetVersion,
  OPENBL_SPI_GetID,
  OPENBL_SPI_ReadMemory,
  OPENBL_SPI_WriteMemory,
  OPENBL_SPI_Go,
  NULL,
  NULL,
  OPENBL_SPI_EraseMemory,
  OPENBL_SPI_WriteProtect,
  OPENBL_SPI_WriteUnprotect,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  OPENBL_SPI_SpecialCommand,
  NULL
};

/* USB supported operations */
static openbl_itf_ops_t USB_Ops =
{
  OPENBL_ITF_USB_Init,
  OPENBL_ITF_USB_DeInit,
  OPENBL_ITF_USB_ProtocolDetection,
  NULL,
  NULL
};

/* Exported variables --------------------------------------------------------*/
uint16_t SpecialCmdList[OPENBL_SPECIAL_CMD_MAX_NBR] =
{
  /* This opcode is used just as an example.
     This list can be modified by adding new opcodes and changing the "OPENBL_SPECIAL_CMD_MAX_NBR" value */
  OPENBL_SPECIAL_CMD_DEFAULT
};

uint16_t ExtSpecialCmdList[OPENBL_EXT_SPECIAL_CMD_MAX_NBR] =
{
  /* This opcode is used just as an example.
     This list can be modified by adding new opcodes and changing the "OPENBL_EXT_SPECIAL_CMD_MAX_NBR" value */
  OPENBL_SPECIAL_CMD_DEFAULT
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize open Bootloader.
  * @param  p_openbl_obj Pointer to the openbl_obj_t structure.
  * @retval Returns 0 in no errors otherwise returns -1.
  */
int32_t openbl_user_init(openbl_obj_t *p_openbl_obj)
{
  /* Reconfigure IWDG in case it was activated by user in option bytes.
     This is done here to avoid any reset in case IWDG is activated in option bytes */
  OPENBL_ITF_IWDG_Init((void *)OPENBL_SYS_IWDG);
  OPENBL_ITF_IWDG_Refresh((void *)OPENBL_SYS_IWDG);

  /* Register supported memories ---------------------------------------------*/
  /* -------------------------------------------------------------------------*/

  /* Initialize the number of supported memories */
  mem_list.memories_nbr = 0U;

  /* Fill memories list */
  /* ================== */

  /* Register user flash memory */
  mem_list.memories[FLASH_MEM].hw_context.p_itf_hw_context = (void *)OPENBL_FLASH_ITF;
  mem_list.memories[FLASH_MEM].hw_context.p_data           = (void *)OPENBL_SYS_IWDG;
  mem_list.memories[FLASH_MEM].mem_desc.start_address      = OPENBL_FLASH_START_ADDRESS;
  mem_list.memories[FLASH_MEM].mem_desc.end_address        = OPENBL_FLASH_END_ADDRESS;
  mem_list.memories[FLASH_MEM].mem_desc.size               = OPENBL_FLASH_MEM_SIZE;
  mem_list.memories[FLASH_MEM].mem_desc.type               = OPENBL_FLASH_AREA;
  mem_list.memories[FLASH_MEM].mem_ops.Read                = OPENBL_FLASH_Read;
  mem_list.memories[FLASH_MEM].mem_ops.Write               = OPENBL_FLASH_Write;
  mem_list.memories[FLASH_MEM].mem_ops.JumpToAddress       = OPENBL_FLASH_JumpToAddress;
  mem_list.memories[FLASH_MEM].mem_ops.MassErase           = OPENBL_FLASH_MassOrBankErase;
  mem_list.memories[FLASH_MEM].mem_ops.Erase               = OPENBL_FLASH_Erase;
  mem_list.memories[FLASH_MEM].mem_ops.SetReadoutProtect   = NULL;
  mem_list.memories[FLASH_MEM].mem_ops.SetWriteProtect     = OPENBL_FLASH_SetWriteProtection;

  /* Increment the number of supported memories */
  mem_list.memories_nbr += 1U;

  /* Register SRAM memory */
  /* OPENBL_STACK_SIZE is added to protect Open Bootloader RAM area */
  mem_list.memories[RAM_MEM].hw_context.p_itf_hw_context = NULL;
  mem_list.memories[RAM_MEM].hw_context.p_data           = (void *)OPENBL_SYS_IWDG;
  mem_list.memories[RAM_MEM].mem_desc.start_address      = OPENBL_RAM_START_ADDRESS + OPENBL_STACK_SIZE;
  mem_list.memories[RAM_MEM].mem_desc.end_address        = OPENBL_RAM_END_ADDRESS;
  mem_list.memories[RAM_MEM].mem_desc.size               = OPENBL_RAM_SIZE;
  mem_list.memories[RAM_MEM].mem_desc.type               = OPENBL_RAM_AREA;
  mem_list.memories[RAM_MEM].mem_ops.Read                = OPENBL_RAM_Read;
  mem_list.memories[RAM_MEM].mem_ops.Write               = OPENBL_RAM_Write;
  mem_list.memories[RAM_MEM].mem_ops.JumpToAddress       = OPENBL_RAM_JumpToAddress;
  mem_list.memories[RAM_MEM].mem_ops.MassErase           = NULL;
  mem_list.memories[RAM_MEM].mem_ops.Erase               = NULL;
  mem_list.memories[RAM_MEM].mem_ops.SetReadoutProtect   = NULL;
  mem_list.memories[RAM_MEM].mem_ops.SetWriteProtect     = NULL;

  /* Increment the number of supported memories */
  mem_list.memories_nbr += 1U;

  /* Register option bytes memory 1 */
  mem_list.memories[OB1_MEM].hw_context.p_itf_hw_context = (void *)OPENBL_FLASH_ITF;
  mem_list.memories[OB1_MEM].hw_context.p_data           = (void *)OPENBL_SYS_IWDG;
  mem_list.memories[OB1_MEM].mem_desc.start_address      = OPENBL_OB1_START_ADDRESS;
  mem_list.memories[OB1_MEM].mem_desc.end_address        = OPENBL_OB1_END_ADDRESS;
  mem_list.memories[OB1_MEM].mem_desc.size               = OPENBL_OB1_SIZE;
  mem_list.memories[OB1_MEM].mem_desc.type               = OPENBL_OB_AREA;
  mem_list.memories[OB1_MEM].mem_ops.Read                = OPENBL_OB_Read;
  mem_list.memories[OB1_MEM].mem_ops.Write               = OPENBL_OB_Write;
  mem_list.memories[OB1_MEM].mem_ops.JumpToAddress       = NULL;
  mem_list.memories[OB1_MEM].mem_ops.MassErase           = NULL;
  mem_list.memories[OB1_MEM].mem_ops.Erase               = NULL;
  mem_list.memories[OB1_MEM].mem_ops.SetReadoutProtect   = NULL;
  mem_list.memories[OB1_MEM].mem_ops.SetWriteProtect     = NULL;

  /* Increment the number of supported memories */
  mem_list.memories_nbr += 1U;

  /* Register option bytes memory 2 */
  mem_list.memories[OB2_MEM].hw_context.p_itf_hw_context = (void *)OPENBL_FLASH_ITF;
  mem_list.memories[OB2_MEM].hw_context.p_data           = (void *)OPENBL_SYS_IWDG;
  mem_list.memories[OB2_MEM].mem_desc.start_address      = OPENBL_OB2_START_ADDRESS;
  mem_list.memories[OB2_MEM].mem_desc.end_address        = OPENBL_OB2_END_ADDRESS;
  mem_list.memories[OB2_MEM].mem_desc.size               = OPENBL_OB2_SIZE;
  mem_list.memories[OB2_MEM].mem_desc.type               = OPENBL_OB_AREA;
  mem_list.memories[OB2_MEM].mem_ops.Read                = OPENBL_OB_Read;
  mem_list.memories[OB2_MEM].mem_ops.Write               = OPENBL_OB_Write;
  mem_list.memories[OB2_MEM].mem_ops.JumpToAddress       = NULL;
  mem_list.memories[OB2_MEM].mem_ops.MassErase           = NULL;
  mem_list.memories[OB2_MEM].mem_ops.Erase               = NULL;
  mem_list.memories[OB2_MEM].mem_ops.SetReadoutProtect   = NULL;
  mem_list.memories[OB2_MEM].mem_ops.SetWriteProtect     = NULL;

  /* Increment the number of supported memories */
  mem_list.memories_nbr += 1U;

  /* Register option bytes memory 3 */
  mem_list.memories[OB3_MEM].hw_context.p_itf_hw_context = (void *)OPENBL_FLASH_ITF;
  mem_list.memories[OB3_MEM].hw_context.p_data           = (void *)OPENBL_SYS_IWDG;
  mem_list.memories[OB3_MEM].mem_desc.start_address      = OPENBL_OB3_START_ADDRESS;
  mem_list.memories[OB3_MEM].mem_desc.end_address        = OPENBL_OB3_END_ADDRESS;
  mem_list.memories[OB3_MEM].mem_desc.size               = OPENBL_OB3_SIZE;
  mem_list.memories[OB3_MEM].mem_desc.type               = OPENBL_OB_AREA;
  mem_list.memories[OB3_MEM].mem_ops.Read                = OPENBL_OB_Read;
  mem_list.memories[OB3_MEM].mem_ops.Write               = OPENBL_OB_Write;
  mem_list.memories[OB3_MEM].mem_ops.JumpToAddress       = NULL;
  mem_list.memories[OB3_MEM].mem_ops.MassErase           = NULL;
  mem_list.memories[OB3_MEM].mem_ops.Erase               = NULL;
  mem_list.memories[OB3_MEM].mem_ops.SetReadoutProtect   = NULL;
  mem_list.memories[OB3_MEM].mem_ops.SetWriteProtect     = NULL;

  /* Increment the number of supported memories */
  mem_list.memories_nbr += 1U;

  /* Register option bytes memory 4 */
  mem_list.memories[OB4_MEM].hw_context.p_itf_hw_context = (void *)OPENBL_FLASH_ITF;
  mem_list.memories[OB4_MEM].hw_context.p_data           = (void *)OPENBL_SYS_IWDG;
  mem_list.memories[OB4_MEM].mem_desc.start_address      = OPENBL_OB4_START_ADDRESS;
  mem_list.memories[OB4_MEM].mem_desc.end_address        = OPENBL_OB4_END_ADDRESS;
  mem_list.memories[OB4_MEM].mem_desc.size               = OPENBL_OB4_SIZE;
  mem_list.memories[OB4_MEM].mem_desc.type               = OPENBL_OB_AREA;
  mem_list.memories[OB4_MEM].mem_ops.Read                = OPENBL_OB_Read;
  mem_list.memories[OB4_MEM].mem_ops.Write               = OPENBL_OB_Write;
  mem_list.memories[OB4_MEM].mem_ops.JumpToAddress       = NULL;
  mem_list.memories[OB4_MEM].mem_ops.MassErase           = NULL;
  mem_list.memories[OB4_MEM].mem_ops.Erase               = NULL;
  mem_list.memories[OB4_MEM].mem_ops.SetReadoutProtect   = NULL;
  mem_list.memories[OB4_MEM].mem_ops.SetWriteProtect     = NULL;

  /* Increment the number of supported memories */
  mem_list.memories_nbr += 1U;




  /* Register RO memory */
  mem_list.memories[RO_MEM].hw_context.p_itf_hw_context = NULL;
  mem_list.memories[RO_MEM].hw_context.p_data           = (void *)OPENBL_SYS_IWDG;
  mem_list.memories[RO_MEM].mem_desc.start_address      = OPENBL_RO_START_ADDRESS;
  mem_list.memories[RO_MEM].mem_desc.end_address        = OPENBL_RO_END_ADDRESS;
  mem_list.memories[RO_MEM].mem_desc.size               = OPENBL_RO_SIZE;
  mem_list.memories[RO_MEM].mem_desc.type               = OPENBL_RO_AREA;
  mem_list.memories[RO_MEM].mem_ops.Read                = OPENBL_RO_Read;
  mem_list.memories[RO_MEM].mem_ops.Write               = NULL;
  mem_list.memories[RO_MEM].mem_ops.JumpToAddress       = NULL;
  mem_list.memories[RO_MEM].mem_ops.MassErase           = NULL;
  mem_list.memories[RO_MEM].mem_ops.Erase               = NULL;
  mem_list.memories[RO_MEM].mem_ops.SetReadoutProtect   = NULL;
  mem_list.memories[RO_MEM].mem_ops.SetWriteProtect     = NULL;

  /* Increment the number of supported memories */
  mem_list.memories_nbr += 1U;


  /* Register supported interfaces -------------------------------------------*/
  /* -------------------------------------------------------------------------*/

  /* Set the number of used interfaces */
  p_openbl_obj->itf_list.interfaces_nbr = 0U;

  /* Fill interfaces list */
  /* ==================== */

  /* Register USART interface */
  p_openbl_obj->itf_list.interfaces[USART_ITF].hw_context.p_itf_hw_context            = (void *)OPENBL_USART_ITF;
  p_openbl_obj->itf_list.interfaces[USART_ITF].hw_context.p_data                      = (void *)OPENBL_SYS_IWDG;
  p_openbl_obj->itf_list.interfaces[USART_ITF].itf_type                               = OPENBL_ITF_TYPE_USART;
  p_openbl_obj->itf_list.interfaces[USART_ITF].p_ops                                  = &USART_Ops;
  p_openbl_obj->itf_list.interfaces[USART_ITF].p_cmd                                  = &USART_Cmd;
  p_openbl_obj->itf_list.interfaces[USART_ITF].cmd_opcodes_list.number                = OPENBL_USART_COMMANDS_MAX_NBR;
  p_openbl_obj->itf_list.interfaces[USART_ITF].cmd_opcodes_list.p_opcodes             = (uint8_t *)USART_CmdOpcodesList;
  p_openbl_obj->itf_list.interfaces[USART_ITF].special_cmd_opcodes_list.number        = OPENBL_SPECIAL_CMD_MAX_NBR;
  p_openbl_obj->itf_list.interfaces[USART_ITF].special_cmd_opcodes_list.p_opcodes     = (uint8_t *)SpecialCmdList;
  p_openbl_obj->itf_list.interfaces[USART_ITF].ext_special_cmd_opcodes_list.number    = OPENBL_EXT_SPECIAL_CMD_MAX_NBR;
  p_openbl_obj->itf_list.interfaces[USART_ITF].ext_special_cmd_opcodes_list.p_opcodes = (uint8_t *)ExtSpecialCmdList;
  p_openbl_obj->itf_list.interfaces[USART_ITF].buffer.max_length                      = OPENBL_USART_RAM_BUFFER_SIZE;
  p_openbl_obj->itf_list.interfaces[USART_ITF].buffer.p_data                          = (uint8_t *)USART_DataBuffer;
  p_openbl_obj->itf_list.interfaces[USART_ITF].p_mem_list                             = &mem_list;

  OPENBL_USART_FillCommandsOpcodesList(&USART_Cmd, &p_openbl_obj->itf_list.interfaces[USART_ITF].cmd_opcodes_list);

  /* Increment the number of used interfaces */
  p_openbl_obj->itf_list.interfaces_nbr += 1U;

  /* Register I2C interface */
  p_openbl_obj->itf_list.interfaces[I2C_ITF].hw_context.p_itf_hw_context            = (void *)OPENBL_I2C_ITF;
  p_openbl_obj->itf_list.interfaces[I2C_ITF].hw_context.p_data                      = (void *)OPENBL_SYS_IWDG;
  p_openbl_obj->itf_list.interfaces[I2C_ITF].itf_type                               = OPENBL_ITF_TYPE_I2C;
  p_openbl_obj->itf_list.interfaces[I2C_ITF].p_ops                                  = &I2C_Ops;
  p_openbl_obj->itf_list.interfaces[I2C_ITF].p_cmd                                  = &I2C_Cmd;
  p_openbl_obj->itf_list.interfaces[I2C_ITF].cmd_opcodes_list.number                = OPENBL_I2C_COMMANDS_MAX_NBR;
  p_openbl_obj->itf_list.interfaces[I2C_ITF].cmd_opcodes_list.p_opcodes             = (uint8_t *)I2C_CmdOpcodesList;
  p_openbl_obj->itf_list.interfaces[I2C_ITF].special_cmd_opcodes_list.number        = OPENBL_SPECIAL_CMD_MAX_NBR;
  p_openbl_obj->itf_list.interfaces[I2C_ITF].special_cmd_opcodes_list.p_opcodes     = (uint8_t *)SpecialCmdList;
  p_openbl_obj->itf_list.interfaces[I2C_ITF].ext_special_cmd_opcodes_list.number    = OPENBL_EXT_SPECIAL_CMD_MAX_NBR;
  p_openbl_obj->itf_list.interfaces[I2C_ITF].ext_special_cmd_opcodes_list.p_opcodes = (uint8_t *)ExtSpecialCmdList;
  p_openbl_obj->itf_list.interfaces[I2C_ITF].buffer.max_length                      = OPENBL_I2C_RAM_BUFFER_SIZE;
  p_openbl_obj->itf_list.interfaces[I2C_ITF].buffer.p_data                          = (uint8_t *)I2C_DataBuffer;
  p_openbl_obj->itf_list.interfaces[I2C_ITF].p_mem_list                             = &mem_list;

  OPENBL_I2C_FillCommandsOpcodesList(&I2C_Cmd, &p_openbl_obj->itf_list.interfaces[I2C_ITF].cmd_opcodes_list);

  /* Increment the number of used interfaces */
  p_openbl_obj->itf_list.interfaces_nbr += 1U;

  /* Register SPI interface */
  p_openbl_obj->itf_list.interfaces[SPI_ITF].hw_context.p_itf_hw_context            = (void *)OPENBL_SPI_ITF;
  p_openbl_obj->itf_list.interfaces[SPI_ITF].hw_context.p_data                      = (void *)OPENBL_SYS_IWDG;
  p_openbl_obj->itf_list.interfaces[SPI_ITF].itf_type                               = OPENBL_ITF_TYPE_SPI;
  p_openbl_obj->itf_list.interfaces[SPI_ITF].p_ops                                  = &SPI_Ops;
  p_openbl_obj->itf_list.interfaces[SPI_ITF].p_cmd                                  = &SPI_Cmd;
  p_openbl_obj->itf_list.interfaces[SPI_ITF].cmd_opcodes_list.number                = OPENBL_SPI_COMMANDS_MAX_NBR;
  p_openbl_obj->itf_list.interfaces[SPI_ITF].cmd_opcodes_list.p_opcodes             = (uint8_t *)SPI_CmdOpcodesList;
  p_openbl_obj->itf_list.interfaces[SPI_ITF].special_cmd_opcodes_list.number        = OPENBL_SPECIAL_CMD_MAX_NBR;
  p_openbl_obj->itf_list.interfaces[SPI_ITF].special_cmd_opcodes_list.p_opcodes     = (uint8_t *)SpecialCmdList;
  p_openbl_obj->itf_list.interfaces[SPI_ITF].ext_special_cmd_opcodes_list.number    = OPENBL_EXT_SPECIAL_CMD_MAX_NBR;
  p_openbl_obj->itf_list.interfaces[SPI_ITF].ext_special_cmd_opcodes_list.p_opcodes = (uint8_t *)ExtSpecialCmdList;
  p_openbl_obj->itf_list.interfaces[SPI_ITF].buffer.max_length                      = OPENBL_SPI_RAM_BUFFER_SIZE;
  p_openbl_obj->itf_list.interfaces[SPI_ITF].buffer.p_data                          = (uint8_t *)SPI_DataBuffer;
  p_openbl_obj->itf_list.interfaces[SPI_ITF].p_mem_list                             = &mem_list;

  OPENBL_SPI_FillCommandsOpcodesList(&SPI_Cmd, &p_openbl_obj->itf_list.interfaces[SPI_ITF].cmd_opcodes_list);

  /* Increment the number of used interfaces */
  p_openbl_obj->itf_list.interfaces_nbr += 1U;

  /* Register USB interface */
  p_openbl_obj->itf_list.interfaces[USB_ITF].hw_context.p_itf_hw_context            = NULL;
  p_openbl_obj->itf_list.interfaces[USB_ITF].hw_context.p_data                      = (void *)OPENBL_SYS_IWDG;
  p_openbl_obj->itf_list.interfaces[USB_ITF].itf_type                               = OPENBL_ITF_TYPE_USB;
  p_openbl_obj->itf_list.interfaces[USB_ITF].p_ops                                  = &USB_Ops;
  p_openbl_obj->itf_list.interfaces[USB_ITF].p_cmd                                  = NULL;
  p_openbl_obj->itf_list.interfaces[USB_ITF].cmd_opcodes_list.number                = 0U;
  p_openbl_obj->itf_list.interfaces[USB_ITF].cmd_opcodes_list.p_opcodes             = NULL;
  p_openbl_obj->itf_list.interfaces[USB_ITF].special_cmd_opcodes_list.number        = OPENBL_SPECIAL_CMD_MAX_NBR;
  p_openbl_obj->itf_list.interfaces[USB_ITF].special_cmd_opcodes_list.p_opcodes     = (uint8_t *)SpecialCmdList;
  p_openbl_obj->itf_list.interfaces[USB_ITF].ext_special_cmd_opcodes_list.number    = OPENBL_EXT_SPECIAL_CMD_MAX_NBR;
  p_openbl_obj->itf_list.interfaces[USB_ITF].ext_special_cmd_opcodes_list.p_opcodes = (uint8_t *)ExtSpecialCmdList;
  p_openbl_obj->itf_list.interfaces[USB_ITF].buffer.max_length                      = OPENBL_USB_RAM_BUFFER_SIZE;
  p_openbl_obj->itf_list.interfaces[USB_ITF].buffer.p_data                          = (uint8_t *)USB_DataBuffer;
  p_openbl_obj->itf_list.interfaces[USB_ITF].p_mem_list                             = &mem_list;

  OPENBL_USBD_DFU_SetInterfaceInstance(&p_openbl_obj->itf_list.interfaces[USB_ITF]);

  /* Increment the number of used interfaces */
  p_openbl_obj->itf_list.interfaces_nbr += 1U;

  return 0;
}

/**
  * @brief  DeInitialize the initialized resources.
  * @retval None.
  */
void openbl_user_deinit(void)
{
  /* De-initialize the used resources */
  /* ================================ */

  mx_rcc_deinit();
  openbl_user_usart_deinit();
  openbl_user_i2c_deinit();
  openbl_user_spi_deinit();
  openbl_user_usb_deinit();
}

/**
  * @brief  This function is used to select which protocol will be used when communicating with the host.
  * @param  p_openbl_obj Pointer to openbl_obj_t structure that contains the list of supported interfaces and memories.
  * @retval Always returns 0.
  */
int32_t openbl_user_process_communication(openbl_obj_t *p_openbl_obj)
{
  static int32_t com_idx = -1;

  if (com_idx == -1)
  {
    com_idx = OPENBL_InterfaceDetection(p_openbl_obj);

    /* De-initialize the interfaces that are not detected to avoid any conflicts or interferences */
    if (com_idx != -1)
    {
      OPENBL_DeInitNotDetectedInterfaces(p_openbl_obj);
    }
  }

  if (com_idx != -1)
  {
    if (p_openbl_obj->itf_list.interfaces[com_idx].itf_type == OPENBL_ITF_TYPE_USB)
    {
      OPENBL_ITF_USB_Process(&p_openbl_obj->itf_list.interfaces[com_idx]);

      app_usbx_process();
    }
    else
    {
      OPENBL_CommandProcess(p_openbl_obj, com_idx);
    }
  }

  return 0;
}

/**
  * @brief  DeInitialize USART interface.
  * @retval None.
  */
void openbl_user_usart_deinit(void)
{
  mx_usart2_uart_deinit();
  
}

/**
  * @brief  DeInitialize I2C interface.
  * @retval None.
  */
void openbl_user_i2c_deinit(void)
{
  mx_i2c1_i2c_deinit();
  
}

/**
  * @brief  DeInitialize SPI interface.
  * @retval None.
  */
void openbl_user_spi_deinit(void)
{
  mx_spi2_deinit();
  
}

/**
  * @brief  SPI Interface Handler.
  * @retval None.
  */
void SPI2_IRQHandler(void)
{
  OPENBL_SPI_IRQHandler(SPI2);
}


/**
  * @brief  DeInitialize USB interface.
  * @retval None.
  */
void openbl_user_usb_deinit(void)
{}
