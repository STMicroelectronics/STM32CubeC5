/**
  ******************************************************************************
  * @file    boot_nv_counters.c
  * @brief   This file provides all the Non volatile firmware functions.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <inttypes.h> /* PRIx32 */
#include "boot_hal_cfg.h"
#include "region_defs.h"
#include "low_level_flash.h"
#include "boot_hal_utils.h"

#include "plat_nv_counters.h"
#include "plat_defs.h"
#include "mx_hal_def.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum plat_err_t plat_err_Typedef;
typedef enum nv_counter_t nv_counter_Typedef;

/* Private configuration  ----------------------------------------------------*/
#define HEADER_SIZE                (4U)
#define NVCNT_ID_HEADER            ((NVCNT_ID_TYPE)0x4855U)
#define NVCNT_HEADER_VALUE         ((uint32_t)(0xaaddeeccU))

/* CRC polynomial configuration */
#define CRC_POLYNOMIAL_LENGTH      HAL_CRC_POLY_SIZE_16B       /* CRC polynomial length 16 bits */
#define CRC_POLYNOMIAL_VALUE       0x8005U                     /* Polynomial to use for CRC computation */

/* Configuration of crc computation for eeprom emulation in flash */
#define NVCNT_ID_TYPE              uint16_t                    /*!< Type of Virtual addr */
#define NVCNT_ID_SHIFT             0U                          /*!< Bits Shifting to get Virtual addr in element */
#define NVCNT_CRC_TYPE             uint16_t                    /*!< Type of Crc */
#define NVCNT_CRC_SHIFT            16U                         /*!< Bits Shifting to get Crc in element */
#define NVCNT_DATA_TYPE            uint32_t                    /*!< Type of data */
#define NVCNT_DATA_SHIFT           32U                         /*!< Bits Shifting to get data value in element */
#define NVCNT_MASK_ID              ((uint64_t)UINT16_MAX << NVCNT_ID_SHIFT)
#define NVCNT_MASK_CRC             ((uint64_t)UINT16_MAX << NVCNT_CRC_SHIFT)
#define NVCNT_MASK_DATA            ((uint64_t)UINT32_MAX << NVCNT_DATA_SHIFT)
#define NVCNT_MASK_FULL            ((uint64_t)(UINT64_MAX))

/*!< Get Crc value from element value */
#define NVCNT_CRC_VALUE(__ELEMENT__)                    (NVCNT_CRC_TYPE)(((__ELEMENT__) &\
                                                                          NVCNT_MASK_CRC) >> NVCNT_CRC_SHIFT)
/*!< Get element value from id, data and crc values */
#define NVCNT_ELEMENT_VALUE(__ID__,__DATA__,__CRC__)    (((NVCNT_ELEMENT_TYPE)(__DATA__) << NVCNT_DATA_SHIFT)\
                                                         | ((__CRC__) << NVCNT_CRC_SHIFT) | (__ID__))
#define NVCNT_ELEMENT_TYPE                              uint64_t
/* only 64bits are used to store data , information is stored only in lsb other 64 bits area is for ECC padding */
/* NVCNT_ELEMENTS_PER_WRITE computed with (sizeof(NVCNT_ELEMENT_TYPE) / FLASH_PROGRAM_UNIT) */
#define NVCNT_ELEMENTS_PER_WRITE                        2U
#define NVCNT_WRITE_SIZE                                (NVCNT_ELEMENTS_PER_WRITE * sizeof(NVCNT_ELEMENT_TYPE))
#define PAGE_HEADER_SIZE                                ((HEADER_SIZE / sizeof(NVCNT_DATA_TYPE)) * NVCNT_WRITE_SIZE)
/*!< Flash value after erase */
#define NVCNT_PAGESTAT_ERASED                           ((uint64_t) UINT64_MAX) /*!< Flash value after erase */
#define NVCNT_MAX_WRITTEN_ELEMENTS                      ((FLASH_NVCNT_SIZE - PAGE_HEADER_SIZE) / NVCNT_WRITE_SIZE)
/*!< Get id value from element value */
#define NVCNT_ID_VALUE(__ELEMENT__)                     ((NVCNT_ID_TYPE)((__ELEMENT__) & NVCNT_MASK_ID))
/*!< Get data value from element value*/
#define NVCNT_DATA_VALUE(__ELEMENT__)                   ((NVCNT_DATA_TYPE)(((NVCNT_ELEMENT_TYPE)(__ELEMENT__) &\
                                                                            NVCNT_MASK_DATA) >> NVCNT_DATA_SHIFT))

#define NVCNT_TRUE  ((uint32_t) HAL_OK)
#define NVCNT_FALSE ((uint32_t) HAL_ERROR)

#define NV_COUNTER_MAX ((uint32_t) BOOT_NV_COUNTER_MAX)

/* Private variables ---------------------------------------------------------*/
/* Global variables used to store  status */
static uint32_t NbWrittenElements = 0U;                 /* Nb of elements written in valid and active pages */
static uint32_t AddrNextWrite = FLASH_NVCNT_SIZE;       /* Initialize write position just after page header */
static uint32_t p_CurrentCounterPos[NV_COUNTER_MAX] = {0U};   /* Store the position of each current counter */

/* Private function prototypes -----------------------------------------------*/
static uint16_t Compute_CRC(NVCNT_DATA_TYPE data, uint16_t virt_addr);
static plat_err_Typedef Check_Header(void);
static plat_err_Typedef Check_Counter(NVCNT_ELEMENT_TYPE counter);
static plat_err_Typedef Set_CRC(void);

/* Exported function ---------------------------------------------------------*/
/* Function prototypes are defined in MCUBoot mdw (plat_nv_counters.h), naming convention can not be respected */

/**
  * @brief  Initialize nv counter storage block if not formatted.
  *         Check consistency of nv counter storage
  *         Initialize global variable for write access.
  *
  * @retval plat_err_Typedef
  *           - PLAT_ERR_SUCCESS in case of success
  *           - PLAT_ERR_SYSTEM_ERR in case of error
  */
plat_err_Typedef plat_init_nv_counter(void)
{
  NVCNT_ELEMENT_TYPE p_addressvalue[NVCNT_ELEMENTS_PER_WRITE] = {0U};
  uint32_t counter_found = 0U;
  uint32_t found_next_write = NVCNT_FALSE;
  uint32_t counter_pos = PAGE_HEADER_SIZE;
  const uint32_t last_counter_pos = FLASH_NVCNT_SIZE;
  uint32_t current_counter_id = UINT32_MAX;
  uint32_t counter_iterator = NV_COUNTER_MAX;

  /* crc config */
  if (mx_rot_crc_init() == NULL)
  {
    return PLAT_ERR_SYSTEM_ERR;
  }

  if (Set_CRC() != PLAT_ERR_SUCCESS)
  {
    return PLAT_ERR_SYSTEM_ERR;
  }

  if (Check_Header() != PLAT_ERR_SUCCESS)
  {
    ROT_LOG_ERR("Wrong NV security counter area header");
    return PLAT_ERR_SYSTEM_ERR;
  }

  /* step 1- Find the first empty value (from the beginning to the end) */
  while ((counter_pos < last_counter_pos) && (found_next_write != NVCNT_TRUE))
  {
    /* Get the current location*/
    if (FLASH_DEV_NAME.ReadData(FLASH_NVCNT_OFFSET + counter_pos, p_addressvalue, sizeof(p_addressvalue))
        != ARM_DRIVER_OK)
    {
      ROT_LOG_ERR("NV security counter read failure");
      return PLAT_ERR_SYSTEM_ERR;
    }

    /* Check if this location is written*/
    if (p_addressvalue[0U] == NVCNT_PAGESTAT_ERASED)
    {
      AddrNextWrite = counter_pos;
      found_next_write = NVCNT_TRUE;
    }
    else
    {
      NbWrittenElements++;
      counter_pos += NVCNT_WRITE_SIZE;
    }
  }

  /* step 2 - Find the position of the different counters,
   * loop from the free area position (AddrNextWrite) to the start
   */
  counter_pos = AddrNextWrite - NVCNT_WRITE_SIZE;

  ROT_LOG_INF("Checking NV security counters");

  while (counter_pos >= PAGE_HEADER_SIZE)
  {
    /* Get the current location content to be compared with virtual addr */
    if (FLASH_DEV_NAME.ReadData(FLASH_NVCNT_OFFSET + counter_pos, p_addressvalue, sizeof(p_addressvalue))
        != ARM_DRIVER_OK)
    {
      ROT_LOG_ERR("NV security counter read error");
      return PLAT_ERR_SYSTEM_ERR;
    }

    current_counter_id = NVCNT_ID_VALUE(p_addressvalue[0]);

    if (current_counter_id < NV_COUNTER_MAX)
    {
      if (Check_Counter(p_addressvalue[0]) == PLAT_ERR_SUCCESS)
      {
        /* update the position and value of the current counter if not already found */
        if ((counter_found & ((uint32_t)1U << current_counter_id)) == 0U)
        {
          p_CurrentCounterPos[current_counter_id] = counter_pos;
          counter_found |= ((uint32_t)1U << current_counter_id);
        }
      }
      else
      {
        /* This counter instance is not consistent, look for a valid one
        * No update, continue searching
        */
      }

      /* Early exit if all counters found */
      if (counter_found == ((1U << NV_COUNTER_MAX) - 1U))
      {
        break;
      }
    }
    else
    {
      return PLAT_ERR_SYSTEM_ERR;
    }
    counter_pos -= NVCNT_WRITE_SIZE;
  }

  /* Check that all counters are found */
  if (counter_found != ((1U << NV_COUNTER_MAX) - 1U))
  {
    ROT_LOG_ERR("Some counters are not found");
    return PLAT_ERR_SYSTEM_ERR;
  }

  /* Step 3 - Iterate from the saved positions of all current counters to the free area
   * Check there is no duplicated counter in this area and no other free area
   */

  for (counter_iterator = (uint32_t)BOOT_NV_COUNTER_0; counter_iterator < NV_COUNTER_MAX; counter_iterator++)
  {
    /* check the next counters until addrAddrNextWrite */
    counter_pos = p_CurrentCounterPos[counter_iterator] + NVCNT_WRITE_SIZE;
    while (counter_pos < AddrNextWrite)
    {
      /* Get the current location content */
      if (FLASH_DEV_NAME.ReadData(FLASH_NVCNT_OFFSET + counter_pos, p_addressvalue, sizeof(p_addressvalue))
          != ARM_DRIVER_OK)
      {
        return PLAT_ERR_SYSTEM_ERR;
      }

      /* check there is no other id valid counter of the current one */
      if (NVCNT_ID_VALUE(p_addressvalue[0]) == counter_iterator)
      {
        if (Check_Counter(p_addressvalue[0]) == PLAT_ERR_SUCCESS)
        {
          return PLAT_ERR_SYSTEM_ERR;
        }
      }

      /* check there is no empty counter*/
      if (p_addressvalue[0] == NVCNT_PAGESTAT_ERASED)
      {
        return PLAT_ERR_SYSTEM_ERR;
      }
      counter_pos += NVCNT_WRITE_SIZE;
    }
  }

  return PLAT_ERR_SUCCESS;
}

/**
  * @brief  Writes/updates counter data in NV security counter area.
  * @param  counter_id to be written
  * @param  value 32bits data to be written
  * @param  updated Pointer to cnt updated status flag (1: yes, 0: no)
  * @warning This function is not reentrant
  * @retval plat_err_Typedef
  *           PLAT_ERR_SUCCESS on write successfully done.
              PLAT_ERR_SYSTEM_ERR when max updateable value written
  */
plat_err_Typedef plat_set_nv_counter(nv_counter_Typedef counter_id,
                                     uint32_t value, uint32_t *updated)
{
  int32_t err = ARM_DRIVER_ERROR;
  uint64_t crc = 0U;
  NVCNT_ELEMENT_TYPE p_new_counter[NVCNT_ELEMENTS_PER_WRITE] = {0U};
  NVCNT_DATA_TYPE current_value = UINT32_MAX;

  /* reset updated flag */
  *updated = 0U;

  /* check current value is not already in flash, and is consistent */
  /* Get the current location content to be compared with virtual addr */
  if (plat_read_nv_counter(counter_id, sizeof(current_value), &current_value) != PLAT_ERR_SUCCESS)
  {
    ROT_LOG_ERR("NV security counter read error");
    return PLAT_ERR_SYSTEM_ERR;
  }

  /* check counter to be added */
  if (value < current_value)
  {
    return PLAT_ERR_SYSTEM_ERR;
  }

  /* check if same value is requested */
  if (current_value == value)
  {
    /* nothing to do */
    return PLAT_ERR_SUCCESS;
  }

  /* Check if pages are full, i.e. max number of written elements achieved */
  if (NbWrittenElements >= NVCNT_MAX_WRITTEN_ELEMENTS)
  {
    ROT_LOG_INF("NV security counter not updated (area is full)");
    return PLAT_ERR_SUCCESS;
  }

  /* Compute crc of variable data and virtual addr */
  crc = Compute_CRC(value, (uint16_t)counter_id);
  /*  build element  */
  p_new_counter[0] = NVCNT_ELEMENT_VALUE((uint32_t)counter_id, value, crc);
  p_new_counter[1] = UINT64_MAX; /* used for alignment */

  /* Program variable data + virtual addr + crc */
  err = FLASH_DEV_NAME.ProgramData(FLASH_NVCNT_OFFSET + AddrNextWrite, p_new_counter, NVCNT_WRITE_SIZE);

  /* If program operation was failed, a Flash error code is returned */
  if (err != ARM_DRIVER_OK)
  {
    return PLAT_ERR_SYSTEM_ERR;
  }

  /* set updated flag */
  *updated = 1U;

  /* Increment global variables relative to write operation done*/
  p_CurrentCounterPos[counter_id] = AddrNextWrite;
  AddrNextWrite += NVCNT_WRITE_SIZE;
  NbWrittenElements++;
  ROT_LOG_INF("Counter %d set to 0x%" PRIx32, counter_id, value);
  return PLAT_ERR_SUCCESS;
}

/**
  * @brief  Read counter data in NV security counter area.
  * @param  counter_id to be read
  * @param  size counter size to be read
  * @param  val reference data to retrieve read counter value
  * @retval plat_err_Typedef
  *           PLAT_ERR_SUCCESS on write successfully done.
              PLAT_ERR_SYSTEM_ERR when max updateable value written
  */
plat_err_Typedef plat_read_nv_counter(nv_counter_Typedef counter_id,
                                      uint32_t size, uint32_t *val)
{
  NVCNT_ELEMENT_TYPE p_counter[NVCNT_ELEMENTS_PER_WRITE] = {0U};

  /* check correct counter position initialization */
  if ((size < sizeof(NVCNT_DATA_TYPE)) || (p_CurrentCounterPos[counter_id] == 0U))
  {
    return PLAT_ERR_SYSTEM_ERR;
  }

  /* CRC must be reconfigured here due to possible conflicts with stcryptolib CRC settings */
  if (Set_CRC() != PLAT_ERR_SUCCESS)
  {
    return PLAT_ERR_SYSTEM_ERR;
  }

  (void)FLASH_DEV_NAME.ReadData(FLASH_NVCNT_OFFSET + p_CurrentCounterPos[counter_id], p_counter, sizeof(p_counter));

  if (Check_Counter(*p_counter) != PLAT_ERR_SUCCESS)
  {
    return PLAT_ERR_SYSTEM_ERR;
  }

  *val = NVCNT_DATA_VALUE(p_counter[0]);

  return PLAT_ERR_SUCCESS;
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function performs CRC computation on data and Virtual addr.
  * @param  data value of  the eeprom variable.
  * @param  virt_addr addr of the eeprom variable.
  * @retval 16-bit CRC value computed on data and Virtual addr.
  */
static uint16_t Compute_CRC(NVCNT_DATA_TYPE data, uint16_t virt_addr)
{
  /* Reset CRC computation unit */
  LL_CRC_ResetCRCCalculationUnit(CRC);

  LL_CRC_FeedData32(CRC, data);
  LL_CRC_FeedData16(CRC, virt_addr);

  /* Return computed CRC value */
  return (LL_CRC_ReadData16(CRC));
}

/**
  * @brief  This function checks the header from NVM count area.
  *
  * @retval plat_err_Typedef return PLAT_ERR_SUCCESS on success
  */
static plat_err_Typedef Check_Header(void)
{
  NVCNT_ELEMENT_TYPE addrvalue = 0U;
  uint32_t  crc = 0U;

  if (FLASH_DEV_NAME.ReadData(FLASH_NVCNT_OFFSET, (void *)&addrvalue, sizeof(addrvalue)) != ARM_DRIVER_OK)
  {
    return PLAT_ERR_SYSTEM_ERR;
  }

  if (addrvalue == NVCNT_PAGESTAT_ERASED)
  {
    return PLAT_ERR_SYSTEM_ERR;
  }

  crc = Compute_CRC(NVCNT_DATA_VALUE(addrvalue), NVCNT_ID_VALUE(addrvalue));

  /* if crc or header is invalid, the page is considered as corrupted */
  return ((crc != NVCNT_CRC_VALUE(addrvalue)) || (NVCNT_DATA_VALUE(addrvalue) != NVCNT_HEADER_VALUE))
         ? PLAT_ERR_SYSTEM_ERR : PLAT_ERR_SUCCESS;
}

/**
  * @brief  Check counter consistency.
  * @param  counter data to be checked
  * @retval plat_err_Typedef
  *           PLAT_ERR_SUCCESS when counter is consistent
              PLAT_ERR_SYSTEM_ERR when error
  */
static plat_err_Typedef Check_Counter(NVCNT_ELEMENT_TYPE counter)
{
  uint32_t crc = 0U;

  /* Compute crc of variable data and virtual addr */
  crc = Compute_CRC(NVCNT_DATA_VALUE(counter), NVCNT_ID_VALUE(counter));

  /* if crc verification pass, data is correct and is returned.
     if crc verification fails, data is corrupted and has to be skip */
  return (crc == NVCNT_CRC_VALUE(counter)) ? PLAT_ERR_SUCCESS : PLAT_ERR_SYSTEM_ERR;
}

/**
  * @brief  Set CRC configuration for NV counters.
  * @param  None
  * @retval plat_err_Typedef
  *           PLAT_ERR_SUCCESS when CRC is set correctly
              PLAT_ERR_SYSTEM_ERR when error
  */
static plat_err_Typedef Set_CRC(void)
{
  hal_crc_config_t crc_config;

  crc_config.polynomial_coefficient   = CRC_POLYNOMIAL_VALUE;
  crc_config.polynomial_size          = CRC_POLYNOMIAL_LENGTH;
  crc_config.crc_init_value           = 0xFFFFFFFFU;
  crc_config.output_data_reverse_mode = HAL_CRC_OUTDATA_REVERSE_NONE;
  crc_config.input_data_reverse_mode  = HAL_CRC_INDATA_REVERSE_NONE;

  return (HAL_CRC_SetConfig(mx_rot_crc_gethandle(), &crc_config) != HAL_OK) ? PLAT_ERR_SYSTEM_ERR : PLAT_ERR_SUCCESS;
}
