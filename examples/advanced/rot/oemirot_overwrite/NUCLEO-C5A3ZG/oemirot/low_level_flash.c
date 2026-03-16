/**
  * Copyright (c) 2013-2018 Arm Limited
  *
  * Copyright (c) 2019-2026 STMicroelectronics.
  *
  * Licensed under the Apache License, Version 2.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  *     http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  */

#include <string.h>
#include "low_level_flash.h"
#include "flash_layout.h"
#include "boot_hal_utils.h"

/* Uncomment to enable flash debugging features */

/* Allows printing logs with flash information */
/* #define DEBUG_FLASH_ACCESS */

/* Allows reading back after a flash erase */
/* #define CHECK_ERASE */

/* Allows reading back after a flash program */
/* #define CHECK_PROGRAM */

#if defined(DEBUG_FLASH_ACCESS)
#define ROT_LOG_FLASH(...)  ROT_LOG_INF(__VA_ARGS__)
#else
#define ROT_LOG_FLASH(...)
#endif /* DEBUG_FLASH_ACCESS */

/* Driver version */
#define ARM_FLASH_DRV_VERSION   ARM_DRIVER_VERSION_MAJOR_MINOR(1U, 0U)

/**
  * @brief Flash driver capability macro definitions \ref ARM_FLASH_CAPABILITIES
  */
/* Flash Ready event generation capability values */
#define EVENT_READY_NOT_AVAILABLE   (0U)
#define EVENT_READY_AVAILABLE       (1U)
/* Data access size values */
#define DATA_WIDTH_8BIT             (0U)
#define DATA_WIDTH_16BIT            (1U)
#define DATA_WIDTH_32BIT            (2U)
/* Chip erase capability values */
#define CHIP_ERASE_NOT_SUPPORTED    (0U)
#define CHIP_ERASE_SUPPORTED        (1U)

#define FLASH_TIMEOUT_VALUE         (1000U)

/* Private typedefs  ------------------------------------------------------------------*/

/**
  * @brief Arm Flash device structure.
  */
typedef struct
{
  low_level_device_t *dev;
  ARM_FLASH_INFO *data;       /*!< FLASH memory device data */
} arm_flash_dev_t;

/* Private variables ------------------------------------------------------------------*/

static const ARM_DRIVER_VERSION DriverVersion =
{
  ARM_FLASH_API_VERSION,  /* Defined in the CMSIS Flash Driver header file */
  ARM_FLASH_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_FLASH_CAPABILITIES DriverCapabilities =
{
  EVENT_READY_NOT_AVAILABLE,
  DATA_WIDTH_32BIT,
  CHIP_ERASE_SUPPORTED,
  0U /* reserved field */
};

/* Private functions ------------------------------------------------------------------*/
/**
  * @brief  Check if the Flash memory boundaries are not violated.
  * @param  flash_dev: Flash device structure \ref arm_flash_dev_t
  * @param  offset: Highest Flash memory address which would be accessed.
  * @retval Returns true if Flash memory boundaries are not violated, false otherwise.
  */

static bool is_range_valid(arm_flash_dev_t *flash_dev, uint32_t offset)
{
  uint32_t flash_limit = 0U;

  ARG_UNUSED(flash_dev);

  /* Calculating the highest address of the Flash memory address range */
  flash_limit = FLASH_TOTAL_SIZE - 1U;

  return (offset > flash_limit) ? (false) : (true) ;
}

/**
  * @brief  Check if the parameter is an erasable page.
  * @param  flash_dev: Flash device structure \ref arm_flash_dev_t
  * @param  param: Any number that can be checked against the
  *                program_unit, e.g. Flash memory address or
  *                data length in bytes.
  * @retval Returns true if param is a page erasable, false otherwise.
  */
static bool is_erase_allow(arm_flash_dev_t *flash_dev, uint32_t param)
{
  /*  allow erase in range provided by device info */
  flash_vect_t *vect = &flash_dev->dev->erase;
  uint32_t nb;
  for (nb = 0U; nb < vect->nb; nb++)
  {
    if ((param >= vect->range[nb].base) && (param <= vect->range[nb].limit))
    {
      return true;
    }
  }

  return false;
}
/**
  * @brief  Check if the parameter is writeable area.
  * @param  flash_dev: Flash device structure \ref arm_flash_dev_t
  * @param  start: Start address of the area to be checked
  * @param  len: Length of the area to be checked
  * @retval Returns true if the area is writeable, false otherwise.
  */
static bool is_write_allow(arm_flash_dev_t *flash_dev, uint32_t start, uint32_t len)
{
  /*  allow write access in area provided by device info */
  flash_vect_t *vect = &flash_dev->dev->write;
  uint32_t nb;
  for (nb = 0U; nb < vect->nb; nb++)
  {
    if ((start >= vect->range[nb].base) && ((start + len - 1U) <= vect->range[nb].limit))
    {
      return true;
    }
  }

  return false;
}

/**
  * @brief  Check if the parameter is aligned to program_unit.
  * @param  flash_dev: Flash device structure \ref arm_flash_dev_t
  * @param  param: Any number that can be checked against the
  *                program_unit, e.g. Flash memory address or
  *                data length in bytes.
  * @retval Returns true if param is aligned to program_unit, false otherwise.
  */

static bool is_write_aligned(arm_flash_dev_t *flash_dev, uint32_t param)
{
  return ((param % flash_dev->data->program_unit) != 0U) ? (false) : (true);
}

/**
  * @brief  Check if the parameter is aligned to page_unit.
  * @param  flash_dev:  Flash device structure \ref arm_flash_dev_t
  * @param  param: Any number that can be checked against the
  *                program_unit, e.g. Flash memory address or
  *                data length in bytes.
  * @retval Returns true if param is aligned to sector_unit, false otherwise.
  */
static bool is_erase_aligned(arm_flash_dev_t *flash_dev, uint32_t param)
{
  /*  2 pages */
  return ((param % (flash_dev->data->sector_size)) != 0U) ? (false) : (true);
}

static ARM_FLASH_INFO ARM_FLASH0_DEV_DATA =
{
  .sector_info    = NULL,                                   /* Uniform sector layout */
  .sector_count   = FLASH_TOTAL_SIZE / FLASH_PAGE_SIZE,     /* Number of sectors */
  .sector_size    = FLASH_PAGE_SIZE,                        /* Uniform sector size in bytes */
  .page_size      = NULL,                                   /* No page size defined */
  .program_unit   = FLASH_PROGRAM_UNIT,                     /* Quad FLASH word size in bytes */
  .erased_value   = 0xFFU,                                  /* Contents of erased memory */
};

static arm_flash_dev_t ARM_FLASH0_DEV =
{
  .dev    = &(FLASH0_DEV),
  .data   = &(ARM_FLASH0_DEV_DATA)
};

/* Flash Status */
static ARM_FLASH_STATUS ARM_FLASH0_STATUS = {0U, 0U, 0U};

static ARM_DRIVER_VERSION Flash_GetVersion(void)
{
  return DriverVersion;
}

static ARM_FLASH_CAPABILITIES Flash_GetCapabilities(void)
{
  return DriverCapabilities;
}

static int32_t Flash_Initialize(ARM_Flash_SignalEvent_t cb_event)
{
  ARG_UNUSED(cb_event);

  if (mx_rot_flash_init() == NULL)
  {
    return ARM_DRIVER_ERROR;
  }
  return ARM_DRIVER_OK;
}

static int32_t Flash_Uninitialize(void)
{
  mx_rot_flash_deinit();
  return ARM_DRIVER_OK;
}

static int32_t Flash_PowerControl(ARM_POWER_STATE state)
{
  int32_t result;

  switch (state)
  {
    case ARM_POWER_FULL:
    {
      /* Nothing to be done */
      result = ARM_DRIVER_OK;
      break;
    }
    case ARM_POWER_OFF:
    case ARM_POWER_LOW:
    {
      result = ARM_DRIVER_ERROR_UNSUPPORTED;
      break;
    }
    default:
    {
      result = ARM_DRIVER_ERROR_PARAMETER;
      break;
    }
  }

  return result;
}

static int32_t Flash_ReadData(uint32_t off, void *data, uint32_t size_byte)
{
  uint8_t *p_flash_addr = (uint8_t *)FLASH_BASE + off;

  /* Check Flash memory boundaries */
  if (is_range_valid(&ARM_FLASH0_DEV, off + size_byte - 1U) != true)
  {
    ROT_LOG_FLASH(" Data read not allowed %x \r\n", (uint32_t)(p_flash_addr + size_byte - 1U));
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  ROT_LOG_FLASH("read %lx n=%x \r\n", (uintptr_t)p_flash_addr, size_byte);

  (void)memcpy(data, (void *)p_flash_addr, size_byte);

  return ARM_DRIVER_OK;
}

static int32_t Flash_ProgramData(uint32_t off, const void *data, uint32_t size_byte)
{
  /* buffer aligned on the maximum write granularity (quad word mode) */
  uint32_t buf[4U] = {0U};
  __IO uint32_t copied_size = 0U;
  uint8_t *p_flash_addr = (uint8_t *)FLASH_BASE + off;
  hal_flash_program_mode_t programming_mode = HAL_FLASH_PROGRAM_QUADWORD;
  hal_flash_handle_t *p_hflash = mx_rot_flash_gethandle();
  const uint32_t erased_pattern = UINT32_MAX;
  bool valid_range = false;
  bool aligned_off = false;
  bool aligned_size = false;
  bool allow_write = false;

#if defined(CHECK_PROGRAM) || defined(DEBUG_FLASH_ACCESS)
  void *dest;
  dest = (void *)p_flash_addr;
#endif /* (CHECK_PROGRAM) || (DEBUG_FLASH_ACCESS) */

  ROT_LOG_FLASH("write %lx n=%x \r\n", (uintptr_t) dest, size_byte);

  /* Check Flash memory boundaries and alignment with minimum write size
   * (program_unit), data size also needs to be a multiple of program_unit.
   */
  valid_range = is_range_valid(&ARM_FLASH0_DEV, off + size_byte - 1U);
  aligned_off = is_write_aligned(&ARM_FLASH0_DEV, off);
  aligned_size = is_write_aligned(&ARM_FLASH0_DEV, size_byte);
  allow_write = is_write_allow(&ARM_FLASH0_DEV, off, size_byte);

  if ((!valid_range) || (!aligned_off) || (!aligned_size) || (!allow_write) || (size_byte == 0U))
  {
    ROT_LOG_FLASH("failed flash program (parameters) %x\r\n", p_flash_addr);
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if (HAL_FLASH_ITF_Unlock(p_hflash->instance) != HAL_OK)
  {
    return ARM_DRIVER_ERROR_SPECIFIC;
  }

  if (HAL_FLASH_SetProgrammingMode(p_hflash, programming_mode) != HAL_OK)
  {
    return ARM_DRIVER_ERROR_SPECIFIC;
  }

  do
  {
    (void)memcpy(buf, (void *)((uint32_t)data + copied_size), FLASH_PROGRAM_UNIT);
    /* Avoid to write a quadword of 0xFF in flash*/
    if ((buf[0U] != erased_pattern)
        || (buf[1U] != erased_pattern)
        || (buf[2U] != erased_pattern)
        || (buf[3U] != erased_pattern))
    {
      if (HAL_FLASH_ProgramByAddr(p_hflash,
                                  (uint32_t)p_flash_addr,
                                  (uintptr_t *)buf,
                                  sizeof(buf),
                                  FLASH_TIMEOUT_VALUE) != HAL_OK)
      {
        ROT_LOG_FLASH("failed flash program %x\r\n", p_flash_addr);
        return ARM_DRIVER_ERROR_SPECIFIC;
      }
    }

    copied_size += FLASH_PROGRAM_UNIT;
    p_flash_addr += FLASH_PROGRAM_UNIT;
  } while (copied_size < size_byte);

  if (copied_size != size_byte)
  {
    return ARM_DRIVER_ERROR;
  }

  if (HAL_FLASH_ITF_Lock(p_hflash->instance) != HAL_OK)
  {
    return ARM_DRIVER_ERROR_SPECIFIC;
  }

  /* Check Programming operation has been done in the expected mode */
  if (HAL_FLASH_GetProgrammingMode(p_hflash) != programming_mode)
  {
    return ARM_DRIVER_ERROR;
  }

#if defined(CHECK_PROGRAM)
  /* compare data written */
  if (memcmp(dest, data, size_byte))
  {
    ROT_LOG_FLASH("write %x n=%x (cmp failed)\r\n", (uint32_t)(dest), size_byte);
    return ARM_DRIVER_ERROR_SPECIFIC;
  }
#endif /* CHECK_PROGRAM */

  return ARM_DRIVER_OK;
}

static int32_t Flash_ErasePage(uint32_t off)
{
  hal_flash_handle_t *p_hflash = mx_rot_flash_gethandle();
  uint32_t flash_addr = (uint32_t)(FLASH_BASE + off);

  ROT_LOG_FLASH("erase %x\r\n", off);

  if (!(is_range_valid(&ARM_FLASH0_DEV, off))
      || !(is_erase_aligned(&ARM_FLASH0_DEV, off))
      || !(is_erase_allow(&ARM_FLASH0_DEV, off)))
  {
    ROT_LOG_FLASH("failed erase %x\r\n", off);
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if (HAL_FLASH_ITF_Unlock(p_hflash->instance) != HAL_OK)
  {
    return ARM_DRIVER_ERROR_SPECIFIC;
  }

  if (HAL_FLASH_EraseByAddr(p_hflash, flash_addr, FLASH_PAGE_SIZE, FLASH_TIMEOUT_VALUE) != HAL_OK)
  {
    ROT_LOG_FLASH("erase failed \r\n");
    return ARM_DRIVER_ERROR;
  }

  if (HAL_FLASH_ITF_Lock(p_hflash->instance) != HAL_OK)
  {
    return ARM_DRIVER_ERROR_SPECIFIC;
  }

#if defined(CHECK_ERASE)
  uint32_t i;
  uint32_t *pt;

  pt = (uint32_t *)((uint32_t)FLASH_BASE + off);

  for (i = 0; i < (FLASH_PAGE_SIZE / 4); i++)
  {
    if (pt[i] != 0XFFFFFFFFU)
    {
      ROT_LOG_FLASH("erase failed off %x %x %x\r\n", off, &pt[i], pt[i]);
      return ARM_DRIVER_ERROR;
    }
  }
#endif /* CHECK_ERASE */

  return ARM_DRIVER_OK;
}

static int32_t Flash_EraseChip(void)
{
  return ARM_DRIVER_ERROR_UNSUPPORTED;
}

static ARM_FLASH_STATUS Flash_GetStatus(void)
{
  return ARM_FLASH0_STATUS;
}

static ARM_FLASH_INFO *Flash_GetInfo(void)
{
  return ARM_FLASH0_DEV.data;
}

/* Exported variables -----------------------------------------------------------------*/

ARM_DRIVER_FLASH Driver_FLASH0 =
{
  /* Get Version */
  Flash_GetVersion,
  /* Get Capability */
  Flash_GetCapabilities,
  /* Initialize */
  Flash_Initialize,
  /* UnInitialize */
  Flash_Uninitialize,
  /* power control */
  Flash_PowerControl,
  /* Read data */
  Flash_ReadData,
  /* Program data */
  Flash_ProgramData,
  /* Erase Sector */
  Flash_ErasePage,
  /* Erase chip */
  Flash_EraseChip,
  /* Get Status */
  Flash_GetStatus,
  /* Get Info */
  Flash_GetInfo
};
