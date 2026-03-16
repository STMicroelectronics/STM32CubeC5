/**
  ******************************************************************************
  * @file    boot_hal_flowcontrol.h
  * @brief   Header for flow control in boot_hal.c module
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
#ifndef BOOT_HAL_FLOWCONTROL_H
#define BOOT_HAL_FLOWCONTROL_H

/* Includes ------------------------------------------------------------------*/
#include "boot_hal_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/**
  * @brief  Flow Control : Initial value.
  */
#define FLOW_STAGE_CFG              0xfffffe00U
#define FLOW_STAGE_CHK              0xffffe01eU

/**
  * @brief  Flow Control : Initial value.
  */
#define FLOW_CTRL_INIT_VALUE        0x00005776U        /*!< Init value definition */

/**
  * @brief  Flow Control : Step definition.
  */
/* -- CACHE_LOCK --------------------------------------------------------------------------------------------------- */
/* ---- Enable ----------------------------------------------------------------------------------------------------- */
/* ------ Init ----------------------------------------------------------------------------------------------------- */
#define FLOW_STEP_CACHE_LOCK_I_EN   0x00006679U        /*!< Step CACHE_LOCK enable Init value */
/* ---- Check ------------------------------------------------------------------------------------------------------ */
/* ------ Init ----------------------------------------------------------------------------------------------------- */
#define FLOW_STEP_CACHE_LOCK_I_CH   0x0001cef0U        /*!< Step CACHE_LOCK Init check value */

/* -- MPU ---------------------------------------------------------------------------------------------------------- */
#if defined(OEMIROT_MPU_ENABLE)
/* ---- Enable ----------------------------------------------------------------------------------------------------- */
/* ------ Init ----------------------------------------------------------------------------------------------------- */
#define FLOW_STEP_MPU_I_EN_R0       0x00006787U        /*!< Step MPU Region 0 Init enable value */
#define FLOW_STEP_MPU_I_EN_R1       0x0000aab5U        /*!< Step MPU Region 1 Init enable value */
#define FLOW_STEP_MPU_I_EN_R2       0x0000cccdU        /*!< Step MPU Region 2 Init enable value */
#define FLOW_STEP_MPU_I_EN_R3       0x0000d2d3U        /*!< Step MPU Region 3 Init enable value */
#define FLOW_STEP_MPU_I_EN_R4       0x0001cf0eU        /*!< Step MPU Region 4 Init enable value */
#define FLOW_STEP_MPU_I_EN_R5       0x00043d7aU        /*!< Step MPU Region 5 Init enable value */
#define FLOW_STEP_MPU_I_EN_R6       0x0051445dU        /*!< Step MPU Region 6 Init enable value */
#define FLOW_STEP_MPU_I_EN          0x0000e3dcU        /*!< Step MPU Init enable value */
/* ------ App ------------------------------------------------------------------------------------------------------ */
#define FLOW_STEP_MPU_A_EN_R1       0x000ac741U        /*!< Step MPU Region 1 App enable value */
/* ---- Check ------------------------------------------------------------------------------------------------------ */
/* ------ Init ----------------------------------------------------------------------------------------------------- */
#define FLOW_STEP_MPU_I_CH_R0       0x0001d0eeU        /*!< Step MPU Region 0 Init check value */
#define FLOW_STEP_MPU_I_CH_R1       0x0001ffffU        /*!< Step MPU Region 1 Init check value */
#define FLOW_STEP_MPU_I_CH_R2       0x00025dafU        /*!< Step MPU Region 2 Init check value */
#define FLOW_STEP_MPU_I_CH_R3       0x00029cf6U        /*!< Step MPU Region 3 Init check value */
#define FLOW_STEP_MPU_I_CH_R4       0x0003da37U        /*!< Step MPU Region 4 Init check value */
#define FLOW_STEP_MPU_I_CH_R5       0x0001fe01U        /*!< Step MPU Region 5 Init check value */
#define FLOW_STEP_MPU_I_CH_R6       0x0052a4e2U        /*!< Step MPU Region 6 Init check value */
#define FLOW_STEP_MPU_I_CH          0x0002e8fbU        /*!< Step MPU Init check value */
/* ------ App ------------------------------------------------------------------------------------------------------ */
#define FLOW_STEP_MPU_A_CH_R1       0x000b3642U        /*!< Step MPU Region 1 App check value */
#else
/* ---- Enable ----------------------------------------------------------------------------------------------------- */
/* ------ Init ----------------------------------------------------------------------------------------------------- */
#define FLOW_STEP_MPU_I_EN_R0       0x0U
#define FLOW_STEP_MPU_I_EN_R1       0x0U
#define FLOW_STEP_MPU_I_EN_R2       0x0U
#define FLOW_STEP_MPU_I_EN_R3       0x0U
#define FLOW_STEP_MPU_I_EN_R4       0x0U
#define FLOW_STEP_MPU_I_EN_R5       0x0U
#define FLOW_STEP_MPU_I_EN_R6       0x0U
#define FLOW_STEP_MPU_I_EN          0x0U
/* ------ App ------------------------------------------------------------------------------------------------------ */
#define FLOW_STEP_MPU_A_EN_R1       0x0U
/* ---- Check ------------------------------------------------------------------------------------------------------ */
/* ------ Init ----------------------------------------------------------------------------------------------------- */
#define FLOW_STEP_MPU_I_CH_R0       0x0U
#define FLOW_STEP_MPU_I_CH_R1       0x0U
#define FLOW_STEP_MPU_I_CH_R2       0x0U
#define FLOW_STEP_MPU_I_CH_R3       0x0U
#define FLOW_STEP_MPU_I_CH_R4       0x0U
#define FLOW_STEP_MPU_I_CH_R5       0x0U
#define FLOW_STEP_MPU_I_CH_R6       0x0U
#define FLOW_STEP_MPU_I_CH          0x0U
/* ------ App ------------------------------------------------------------------------------------------------------ */
#define FLOW_STEP_MPU_A_CH_R1       0x0U
#endif /* OEMIROT_MPU_ENABLE */

/* -- SAU ---------------------------------------------------------------------------------------------------------- */
/* ---- Enable ----------------------------------------------------------------------------------------------------- */
/* ------ Init ----------------------------------------------------------------------------------------------------- */
#define FLOW_STEP_SAU_I_EN          0x0U
/* ---- Check ------------------------------------------------------------------------------------------------------ */
/* ------ Init ----------------------------------------------------------------------------------------------------- */
#define FLOW_STEP_SAU_I_CH          0x0U

/* -- FLASH -------------------------------------------------------------------------------------------------------- */
/* ---- Enable ----------------------------------------------------------------------------------------------------- */
/* ------ Init ----------------------------------------------------------------------------------------------------- */
#define FLOW_STEP_FLASH_I_EN_B1     0x0003dbc9U        /*!< Step FLASH Init Bank1 enable value */
#define FLOW_STEP_FLASH_I_EN_B2     0x00045afcU        /*!< Step FLASH Init Bank2 enable value */
/* ------ App ------------------------------------------------------------------------------------------------------ */
#define FLOW_STEP_FLASH_A_EN_B1     0x0007eb92U        /*!< Step FLASH Appli Bank1 enable value */
#define FLOW_STEP_FLASH_A_EN_B2     0x0007f58cU        /*!< Step FLASH Appli Bank2 enable value */
/* ---- Check ------------------------------------------------------------------------------------------------------ */
/* ------ Init ----------------------------------------------------------------------------------------------------- */
#define FLOW_STEP_FLASH_I_CH_B1     0x000474dfU        /*!< Step FLASH Init Bank1 check value */
#define FLOW_STEP_FLASH_I_CH_B2     0x0004a7f3U        /*!< Step FLASH Init Bank2 check value */
/* ------ App ------------------------------------------------------------------------------------------------------ */
#define FLOW_STEP_FLASH_A_CH_B1     0x0007f92bU        /*!< Step FLASH Appli Bank1 check value */
#define FLOW_STEP_FLASH_A_CH_B2     0x000a0b8dU        /*!< Step FLASH Appli Bank2 check value */

/* -- FLASH-ECC ---------------------------------------------------------------------------------------------------- */
/* ---- Enable ----------------------------------------------------------------------------------------------------- */
/* ------ Init ----------------------------------------------------------------------------------------------------- */
#define FLOW_STEP_FLASH_ECC_I_EN    0x0032d27aU        /*!< Step FLASH Init ECC enable value */
/* ------ App ------------------------------------------------------------------------------------------------------ */
#define FLOW_STEP_FLASH_ECC_A_EN    0x005abbc2U        /*!< Step FLASH Appli ECC enable value */
/* ---- Check ------------------------------------------------------------------------------------------------------ */
/* ------ Init ----------------------------------------------------------------------------------------------------- */
#define FLOW_STEP_FLASH_ECC_I_CH    0x001726b0U        /*!< Step FLASH Init ECC check value */
/* ------ App ------------------------------------------------------------------------------------------------------ */
#define FLOW_STEP_FLASH_ECC_A_CH    0x0034d3e1U        /*!< Step FLASH Appli ECC check value */

/* -- TAMP --------------------------------------------------------------------------------------------------------- */
#if defined(OEMIROT_TAMPER_ENABLE)
/* ---- Enable ----------------------------------------------------------------------------------------------------- */
/* ------ Init ----------------------------------------------------------------------------------------------------- */
#define FLOW_STEP_TAMP_INT_EN       0x00067f4dU        /*!< Step Tamper internal enable value */

#define FLOW_STEP_TAMP_SEC_EN       0x0U
#define FLOW_STEP_TAMP_CFG_EN       0x0U
#define FLOW_STEP_TAMP_PRIV_EN      0x0006b381U        /*!< Step Tamper privilege enable value */
/* ---- Check ------------------------------------------------------------------------------------------------------ */
/* ------ Init ----------------------------------------------------------------------------------------------------- */
#define FLOW_STEP_TAMP_INT_CH       0x0006d407U        /*!< Step Tamper internal check value */
#define FLOW_STEP_TAMP_SEC_CH       0x0U
#define FLOW_STEP_TAMP_CFG_CH       0x0U
#define FLOW_STEP_TAMP_PRIV_CH      0x00072a07U        /*!< Step Tamper privilege check value */
#endif /* OEMIROT_TAMPER_ENABLE */

/* -- RAMCFG ------------------------------------------------------------------------------------------------------- */
/* ---- Enable ----------------------------------------------------------------------------------------------------- */
/* ------ Init ----------------------------------------------------------------------------------------------------- */
#define FLOW_STEP_RAMCFG_I_EN      0x000a6e24U        /*!< Step RAM Double ECC errors enable value */

/* ---- Check ------------------------------------------------------------------------------------------------------ */
/* ------ Init ----------------------------------------------------------------------------------------------------- */
#define FLOW_STEP_RAMCFG_I_CH      0x000ab163U        /*!< Step RAM Double ECC errors check value */

/* Flow control Stage 1 */
#define FLOW_CTRL_CACHE_LOCK_I_EN   (FLOW_CTRL_INIT_VALUE ^      FLOW_STEP_CACHE_LOCK_I_EN)

/**
  * @brief  SFU_BOOT Flow Control : Control values runtime protections.
  */
/* Flow control Stage 1 */
#define FLOW_CTRL_MPU_I_EN_R0       (FLOW_CTRL_CACHE_LOCK_I_EN ^ FLOW_STEP_MPU_I_EN_R0)
#define FLOW_CTRL_MPU_I_EN_R1       (FLOW_CTRL_MPU_I_EN_R0 ^     FLOW_STEP_MPU_I_EN_R1)
#define FLOW_CTRL_MPU_I_EN_R2       (FLOW_CTRL_MPU_I_EN_R1 ^     FLOW_STEP_MPU_I_EN_R2)
#define FLOW_CTRL_MPU_I_EN_R3       (FLOW_CTRL_MPU_I_EN_R2 ^     FLOW_STEP_MPU_I_EN_R3)
#define FLOW_CTRL_MPU_I_EN_R4       (FLOW_CTRL_MPU_I_EN_R3 ^     FLOW_STEP_MPU_I_EN_R4)
#define FLOW_CTRL_MPU_I_EN_R5       (FLOW_CTRL_MPU_I_EN_R4 ^     FLOW_STEP_MPU_I_EN_R5)
#define FLOW_CTRL_MPU_I_EN_R6       (FLOW_CTRL_MPU_I_EN_R5 ^     FLOW_STEP_MPU_I_EN_R6)
#define FLOW_CTRL_MPU_I_EN          (FLOW_CTRL_MPU_I_EN_R6 ^     FLOW_STEP_MPU_I_EN)

#define FLOW_CTRL_SAU_I_EN          (FLOW_CTRL_MPU_I_EN ^        FLOW_STEP_SAU_I_EN)

#define FLOW_CTRL_FLASH_I_EN_B1     (FLOW_CTRL_SAU_I_EN ^        FLOW_STEP_FLASH_I_EN_B1)
#define FLOW_CTRL_FLASH_I_EN_B2     (FLOW_CTRL_FLASH_I_EN_B1 ^   FLOW_STEP_FLASH_I_EN_B2)

#define FLOW_CTRL_TAMP_INT_EN       (FLOW_CTRL_FLASH_I_EN_B2 ^   FLOW_STEP_TAMP_INT_EN)
#define FLOW_CTRL_TAMP_SEC_EN       (FLOW_CTRL_TAMP_INT_EN ^     FLOW_STEP_TAMP_SEC_EN)
#define FLOW_CTRL_TAMP_PRIV_EN      (FLOW_CTRL_TAMP_SEC_EN ^     FLOW_STEP_TAMP_PRIV_EN)
#define FLOW_CTRL_TAMP_CFG_EN       (FLOW_CTRL_TAMP_PRIV_EN ^    FLOW_STEP_TAMP_CFG_EN)

#define FLOW_CTRL_RAMCFG_I_EN      (FLOW_CTRL_TAMP_CFG_EN ^     FLOW_STEP_RAMCFG_I_EN)

#define FLOW_CTRL_FLASH_ECC_I_EN    (FLOW_CTRL_RAMCFG_I_EN ^    FLOW_STEP_FLASH_ECC_I_EN)

#define FLOW_CTRL_STAGE_1           FLOW_CTRL_FLASH_ECC_I_EN

/* Flow control Stage 2 */
#define FLOW_CTRL_CACHE_LOCK_I_CH   (FLOW_CTRL_STAGE_1 ^         FLOW_STEP_CACHE_LOCK_I_CH)
#define FLOW_CTRL_MPU_I_CH_R0       (FLOW_CTRL_CACHE_LOCK_I_CH ^ FLOW_STEP_MPU_I_CH_R0)
#define FLOW_CTRL_MPU_I_CH_R1       (FLOW_CTRL_MPU_I_CH_R0 ^     FLOW_STEP_MPU_I_CH_R1)
#define FLOW_CTRL_MPU_I_CH_R2       (FLOW_CTRL_MPU_I_CH_R1 ^     FLOW_STEP_MPU_I_CH_R2)
#define FLOW_CTRL_MPU_I_CH_R3       (FLOW_CTRL_MPU_I_CH_R2 ^     FLOW_STEP_MPU_I_CH_R3)
#define FLOW_CTRL_MPU_I_CH_R4       (FLOW_CTRL_MPU_I_CH_R3 ^     FLOW_STEP_MPU_I_CH_R4)
#define FLOW_CTRL_MPU_I_CH_R5       (FLOW_CTRL_MPU_I_CH_R4 ^     FLOW_STEP_MPU_I_CH_R5)
#define FLOW_CTRL_MPU_I_CH_R6       (FLOW_CTRL_MPU_I_CH_R5 ^     FLOW_STEP_MPU_I_CH_R6)
#define FLOW_CTRL_MPU_I_CH          (FLOW_CTRL_MPU_I_CH_R6 ^     FLOW_STEP_MPU_I_CH)

#define FLOW_CTRL_SAU_I_CH          (FLOW_CTRL_MPU_I_CH ^        FLOW_STEP_SAU_I_CH)

#define FLOW_CTRL_FLASH_I_CH_B1     (FLOW_CTRL_SAU_I_CH ^        FLOW_STEP_FLASH_I_CH_B1)
#define FLOW_CTRL_FLASH_I_CH_B2     (FLOW_CTRL_FLASH_I_CH_B1 ^   FLOW_STEP_FLASH_I_CH_B2)

#define FLOW_CTRL_TAMP_INT_CH       (FLOW_CTRL_FLASH_I_CH_B2 ^   FLOW_STEP_TAMP_INT_CH)
#define FLOW_CTRL_TAMP_SEC_CH       (FLOW_CTRL_TAMP_INT_CH ^     FLOW_STEP_TAMP_SEC_CH)
#define FLOW_CTRL_TAMP_PRIV_CH      (FLOW_CTRL_TAMP_SEC_CH ^     FLOW_STEP_TAMP_PRIV_CH)
#define FLOW_CTRL_TAMP_CFG_CH       (FLOW_CTRL_TAMP_PRIV_CH ^    FLOW_STEP_TAMP_CFG_CH)

#define FLOW_CTRL_RAMCFG_I_CH      (FLOW_CTRL_TAMP_CFG_CH ^     FLOW_STEP_RAMCFG_I_CH)

#define FLOW_CTRL_FLASH_ECC_I_CH    (FLOW_CTRL_RAMCFG_I_CH ^    FLOW_STEP_FLASH_ECC_I_CH)

#define FLOW_CTRL_STAGE_2            FLOW_CTRL_FLASH_ECC_I_CH

/* Flow control Stage 3 Appli */
#define FLOW_CTRL_MPU_A_EN_R1       (FLOW_CTRL_STAGE_2 ^         FLOW_STEP_MPU_A_EN_R1)

#define FLOW_CTRL_FLASH_A_EN_B1     (FLOW_CTRL_MPU_A_EN_R1 ^     FLOW_STEP_FLASH_A_EN_B1)
#define FLOW_CTRL_FLASH_A_EN_B2     (FLOW_CTRL_FLASH_A_EN_B1 ^   FLOW_STEP_FLASH_A_EN_B2)

#define FLOW_CTRL_FLASH_ECC_A_EN    (FLOW_CTRL_FLASH_A_EN_B2 ^   FLOW_STEP_FLASH_ECC_A_EN)

#define FLOW_CTRL_STAGE_3_A         FLOW_CTRL_FLASH_ECC_A_EN

/* Flow control Stage 4 Appli */
#define FLOW_CTRL_MPU_A_CH_R1       (FLOW_CTRL_STAGE_3_A ^       FLOW_STEP_MPU_A_CH_R1)

#define FLOW_CTRL_FLASH_A_CH_B1     (FLOW_CTRL_MPU_A_CH_R1 ^     FLOW_STEP_FLASH_A_CH_B1)
#define FLOW_CTRL_FLASH_A_CH_B2     (FLOW_CTRL_FLASH_A_CH_B1 ^   FLOW_STEP_FLASH_A_CH_B2)

#define FLOW_CTRL_FLASH_ECC_A_CH    (FLOW_CTRL_FLASH_A_CH_B2 ^   FLOW_STEP_FLASH_ECC_A_CH)

#define FLOW_CTRL_STAGE_4_A         FLOW_CTRL_FLASH_ECC_A_CH

/* External variables --------------------------------------------------------*/
/**
  *  Flow control protection values
  *  Flow control stage
  */
extern volatile uint32_t uFlowProtectValue;
extern volatile uint32_t uFlowStage;

/* Exported macros -----------------------------------------------------------*/
/**
  * Control with STEP operation :
  * (uFlowValue XOR STEP_VALUE) must be equal to CTRL_VALUE ==> execution stopped if failed !
  */
#if defined(OEMIROT_FLOW_CONTROL)
#define FLOW_CONTROL_STEP(C,B,A) \
  do{ \
    (C) ^= (B);\
    if ((C) != (A))\
    { \
      Error_Handler();\
    } \
  }while(0)
#else
#define FLOW_CONTROL_STEP(C,B,A) ((void)0)
#endif /* OEMIROT_FLOW_CONTROL */

/**
  * Control without STEP operation :
  * uFlowValue must be equal to CTRL_VALUE ==> execution stopped if failed !
  */
#if defined(OEMIROT_FLOW_CONTROL)
#define FLOW_CONTROL_CHECK(B,A) \
  do{ \
    if ((B) != (A))\
    { \
      Error_Handler();\
    } \
  }while(0)
#else
#define FLOW_CONTROL_CHECK(B,A) ((void)0)
#endif /* OEMIROT_FLOW_CONTROL */

/**
  * Control flow initialization
  */
#if defined(OEMIROT_FLOW_CONTROL)
#define FLOW_CONTROL_INIT(B,A) \
  do{ \
    (B) = (A);\
  }while(0)
#else
#define FLOW_CONTROL_INIT(B,A) ((void)0)
#endif /* OEMIROT_FLOW_CONTROL */

/**
  * STEP update only :
  * (uFlowValue XOR STEP_VALUE)
  */
#if defined(OEMIROT_FLOW_CONTROL)
#define FLOW_STEP(B,A) \
  do{ \
    (B) ^= (A);\
  }while(0)
#else
#define FLOW_STEP(B,A) ((void)0)
#endif /* OEMIROT_FLOW_CONTROL */

/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* BOOT_HAL_FLOWCONTROL_H */
