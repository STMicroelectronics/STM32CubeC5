/**
  ******************************************************************************
  * @file    low_level_security_config.h
  * @brief   Config header for low_level_security.c module
  *
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LOW_LEVEL_SECURITY_CONFIG_H
#define LOW_LEVEL_SECURITY_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief MPU configuration structure definition
  */
typedef struct
{
  hal_cortex_mpu_region_idx_t region_idx;       /*!< MPU Region Index */
  hal_cortex_mpu_region_config_t region_config; /*!< MPU Region Configuration */
#if defined(OEMIROT_FLOW_CONTROL)
  uint32_t flow_step_enable;                    /*!< Flow control step (configuration stage) */
  uint32_t flow_ctrl_enable;                    /*!< Flow control (configuration stage) */
  uint32_t flow_step_check;                     /*!< Flow control step (verification stage)  */
  uint32_t flow_ctrl_check;                     /*!< Flow control (verification stage) */
#endif /* OEMIROT_FLOW_CONTROL */
} oemirot_mpu_init_t;

#define LAST_PAGE_IN_BANK                         ((BANK_SIZE - 1) / PAGE_SIZE)
#define SRAM_ECC                                  HAL_RAMCFG_SRAM2

/* MPU configuration
  ================== */
/**
  * Set 4 pre-defined MAIR_ATTR for memory. The attributes come
  * from default memory map.
  *
  * MAIR0_0: Peripheral, Device-nGnRE.
  * MAIR0_1: Code, WT RA. Same attr for Outer and Inner.
  * MAIR0_2: SRAM, WBWA RA. Same attr for Outer and Inner.
  * MAIR0_3: Non cacheable RA. Same attr for Outer and Inner.
  */
#define MPU_MEM_ATTR_DEVICE_VAL      HAL_CORTEX_MPU_DEVICE_MEM_NGNRE
#define MPU_MEM_ATTR_DEVICE_IDX      HAL_CORTEX_MPU_MEM_ATTR_0
#define MPU_MEM_ATTR_CODE_VAL        HAL_CORTEX_MPU_NORMAL_MEM_WT_RA
#define MPU_MEM_ATTR_CODE_IDX        HAL_CORTEX_MPU_MEM_ATTR_1
#define MPU_MEM_ATTR_DATA_VAL        HAL_CORTEX_MPU_NORMAL_MEM_WB_RWA
#define MPU_MEM_ATTR_DATA_IDX        HAL_CORTEX_MPU_MEM_ATTR_2
#define MPU_MEM_ATTR_DATANOCACHE_VAL HAL_CORTEX_MPU_NORMAL_MEM_NCACHEABLE
#define MPU_MEM_ATTR_DATANOCACHE_IDX HAL_CORTEX_MPU_MEM_ATTR_3

/* Initial MPU configuration for OEMiRoT execution */
static const oemirot_mpu_init_t mpu_regions_cfg_init[] =
{
  /* Region 0: Allows execution to ROT code */
  {
    HAL_CORTEX_MPU_REGION_0,
    {
      ROT_CODE_START,
      ROT_CODE_END,
      HAL_CORTEX_MPU_REGION_ONLY_PRIV_RO,
      HAL_CORTEX_MPU_EXECUTION_ATTR_ENABLE,
      MPU_MEM_ATTR_CODE_IDX
    },
#if defined(OEMIROT_FLOW_CONTROL)
    FLOW_STEP_MPU_I_EN_R0,
    FLOW_CTRL_MPU_I_EN_R0,
    FLOW_STEP_MPU_I_CH_R0,
    FLOW_CTRL_MPU_I_CH_R0
#endif /* OEMIROT_FLOW_CONTROL */
  },
  /* Region 1: Allows RW access to all primary and secondary slots areas (Not cachable) */
  {
    HAL_CORTEX_MPU_REGION_1,
    {
      ROT_AREAS_START,
      ROT_AREAS_END,
      HAL_CORTEX_MPU_REGION_ONLY_PRIV_RW,
      HAL_CORTEX_MPU_EXECUTION_ATTR_DISABLE,
      MPU_MEM_ATTR_DATANOCACHE_IDX
    },
#if defined(OEMIROT_FLOW_CONTROL)
    FLOW_STEP_MPU_I_EN_R1,
    FLOW_CTRL_MPU_I_EN_R1,
    FLOW_STEP_MPU_I_CH_R1,
    FLOW_CTRL_MPU_I_CH_R1
#endif /* OEMIROT_FLOW_CONTROL */
  },
  /* Region 2: Allows RW access to all SRAM */
  {
    HAL_CORTEX_MPU_REGION_2,
    {
      SRAM_START,
      SRAM_END,
      HAL_CORTEX_MPU_REGION_ONLY_PRIV_RW,
      HAL_CORTEX_MPU_EXECUTION_ATTR_DISABLE,
      MPU_MEM_ATTR_DATA_IDX
    },
#if defined(OEMIROT_FLOW_CONTROL)
    FLOW_STEP_MPU_I_EN_R2,
    FLOW_CTRL_MPU_I_EN_R2,
    FLOW_STEP_MPU_I_CH_R2,
    FLOW_CTRL_MPU_I_CH_R2
#endif /* OEMIROT_FLOW_CONTROL */
  },
  /* Region 3: Allows RW access to peripherals */
  {
    HAL_CORTEX_MPU_REGION_3,
    {
      PERIPH_START,
      PERIPH_END,
      HAL_CORTEX_MPU_REGION_ONLY_PRIV_RW,
      HAL_CORTEX_MPU_EXECUTION_ATTR_DISABLE,
      MPU_MEM_ATTR_DEVICE_IDX
    },
#if defined(OEMIROT_FLOW_CONTROL)
    FLOW_STEP_MPU_I_EN_R3,
    FLOW_CTRL_MPU_I_EN_R3,
    FLOW_STEP_MPU_I_CH_R3,
    FLOW_CTRL_MPU_I_CH_R3
#endif /* OEMIROT_FLOW_CONTROL */
  },
  /* Region 4: Allows execution of ExitHDPLib */
  {
    HAL_CORTEX_MPU_REGION_4,
    {
      EXITHDPLIB_START,
      EXITHDPLIB_END,
      HAL_CORTEX_MPU_REGION_ONLY_PRIV_RO,
      HAL_CORTEX_MPU_EXECUTION_ATTR_ENABLE,
      MPU_MEM_ATTR_CODE_IDX
    },
#if defined(OEMIROT_FLOW_CONTROL)
    FLOW_STEP_MPU_I_EN_R4,
    FLOW_CTRL_MPU_I_EN_R4,
    FLOW_STEP_MPU_I_CH_R4,
    FLOW_CTRL_MPU_I_CH_R4
#endif /* OEMIROT_FLOW_CONTROL */
  },
  /* Region 5: Allows RW access to boot data flash area */
  {
    HAL_CORTEX_MPU_REGION_5,
    {
      ROT_BOOT_DATA_START,
      ROT_BOOT_DATA_END,
      HAL_CORTEX_MPU_REGION_ONLY_PRIV_RW,
      HAL_CORTEX_MPU_EXECUTION_ATTR_DISABLE,
      MPU_MEM_ATTR_DATANOCACHE_IDX
    },
#if defined(OEMIROT_FLOW_CONTROL)
    FLOW_STEP_MPU_I_EN_R5,
    FLOW_CTRL_MPU_I_EN_R5,
    FLOW_STEP_MPU_I_CH_R5,
    FLOW_CTRL_MPU_I_CH_R5
#endif /* OEMIROT_FLOW_CONTROL */
  },
  /* Region 6: Allows RO access without cache to read FLASHSIZE_BASE */
  {
    HAL_CORTEX_MPU_REGION_6,
    {
      RO_AND_SYSTEM_MEM_START,
      RO_AND_SYSTEM_MEM_END,
      HAL_CORTEX_MPU_REGION_ONLY_PRIV_RO,
      HAL_CORTEX_MPU_EXECUTION_ATTR_DISABLE,
      MPU_MEM_ATTR_DATANOCACHE_IDX
    },
#if defined(OEMIROT_FLOW_CONTROL)
    FLOW_STEP_MPU_I_EN_R6,
    FLOW_CTRL_MPU_I_EN_R6,
    FLOW_STEP_MPU_I_CH_R6,
    FLOW_CTRL_MPU_I_CH_R6
#endif /* OEMIROT_FLOW_CONTROL */
  }
};

/* Updated MPU configuration for jump to application.
   First MPU region in this list will be configured only at this stage,
   this region will be enabled later by RSSlib service, after having closed
   HDP area (containing RoT code/data area).
   Following regions in this list are configured and directly enabled. */
#define SECURITY_APP_MPU_REGION_COUNTER (1U)
static const oemirot_mpu_init_t mpu_regions_cfg_appli[SECURITY_APP_MPU_REGION_COUNTER] =
{
  /* Region 1: Allows execution of the next stage application */
  {
    HAL_CORTEX_MPU_REGION_1,
    {
      CODE_START,
      CODE_END,
      HAL_CORTEX_MPU_REGION_ONLY_PRIV_RO,
      HAL_CORTEX_MPU_EXECUTION_ATTR_ENABLE,
      MPU_MEM_ATTR_CODE_IDX
    },
#if defined(OEMIROT_FLOW_CONTROL)
    FLOW_STEP_MPU_A_EN_R1,
    FLOW_CTRL_MPU_A_EN_R1,
    FLOW_STEP_MPU_A_CH_R1,
    FLOW_CTRL_MPU_A_CH_R1
#endif /* OEMIROT_FLOW_CONTROL */
  }
};

/* RDP_Level control
   ===================== */
static const hal_flash_itf_ob_rdp_level_t RDPLevelPrioList[] =
{
  HAL_FLASH_ITF_OB_RDP_LEVEL_0,
  HAL_FLASH_ITF_OB_RDP_LEVEL_2_WBS,
  HAL_FLASH_ITF_OB_RDP_LEVEL_2
};

#define NB_RDP_LEVEL (3U)

/* Tamper
   ====== */
/* OEMiRoT tampers configuration begin */
/* Format the definitions for configuring supported tampers required by the HAL/LL driver */
#define TAMP_INTERNAL_TAMPER        (HAL_TAMP_INTERNAL_TAMPER_9)
#define TAMP_INTERNAL_IT_TAMPER     (HAL_TAMP_INTERNAL_IT_TAMPER_9)
#define TAMP_ITAMP_NOERASE_TAMPER   (LL_TAMP_ITAMP_NOERASE_TAMPER9)

/* OEMiRoT tampers configuration end */

#if defined(OEMIROT_TAMPER_ENABLE)
/* Configure tamper in confirmed mode: secrets are erased on a tamper fault */
static const hal_tamp_internal_individual_config_t IndivInternalTamperConf =
{
  .erase_secrets = HAL_TAMP_INTERNAL_SECRETS_ERASE
};
#endif /* OEMIROT_TAMPER_ENABLE */

#ifdef __cplusplus
}
#endif

#endif /* LOW_LEVEL_SECURITY_CONFIG_H */
