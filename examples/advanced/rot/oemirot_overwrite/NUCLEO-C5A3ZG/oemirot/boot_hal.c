/**
  ******************************************************************************
  * @file    boot_hal.c
  * @brief   This file contains  mcuboot hardware specific implementation
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

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "mx_system.h"
#include "boot_hal_cfg.h"
#include "boot_hal.h"
#include "boot_hal_hash_ref.h"
#include "boot_hal_imagevalid.h"
#include "boot_hal_flowcontrol.h"
#include "mcuboot_config.h"
#include "low_level_security.h"
#include "cmsis.h"
#include "low_level_flash.h"
#include "region_defs.h"
#include "low_level_rng.h"
#include "boot_hal_utils.h"

#include "basic_stdio_core.h"
#include "bootutil/boot_record.h"
#include "bootutil_priv.h"
#include "mx_basic_stdio_app.h"
#include "mx_hal_def.h"

#if defined(MCUBOOT_DOUBLE_SIGN_VERIF)
#if (MCUBOOT_IMAGE_NUMBER == 1)
#define OEMIROT_BOOT_IMAGE_NUMBER 1
#elif (MCUBOOT_IMAGE_NUMBER == 2)
#define OEMIROT_BOOT_IMAGE_NUMBER 2
#elif (MCUBOOT_IMAGE_NUMBER == 3)
#define OEMIROT_BOOT_IMAGE_NUMBER 3
#elif (MCUBOOT_IMAGE_NUMBER == 4)
#define OEMIROT_BOOT_IMAGE_NUMBER 4
#else
#error "MCUBOOT_IMAGE_NUMBER not supported"
#endif /* (MCUBOOT_IMAGE_NUMBER == 1)  */

/* Global variables to memorize images validation status */
uint8_t ImageValidStatus[OEMIROT_BOOT_IMAGE_NUMBER] = { INIT_STRUCT(OEMIROT_BOOT_IMAGE_NUMBER, IMAGE_INVALID) };

uint8_t ImageValidEnable = 0U;
uint8_t ImageValidIndex = 0U;
#endif /* MCUBOOT_DOUBLE_SIGN_VERIF */

#define OEMIROT_APPLI_REGION_TO_X ((uint32_t) HAL_CORTEX_MPU_REGION_1)

#if defined(OEMIROT_FLOW_CONTROL)
/* Global variable for Flow Control state */
volatile uint32_t uFlowProtectValue = FLOW_CTRL_INIT_VALUE;
#endif /* OEMIROT_FLOW_CONTROL */
volatile uint32_t uFlowStage = FLOW_STAGE_CFG;

/* External References -------------------------------------------------------*/
extern void SystemInit(void);
extern __NO_RETURN void __PROGRAM_START(void);
extern uint32_t __STACK_LIMIT;

/* Private function prototypes -----------------------------------------------*/
__NO_RETURN void Reset_Handler(void);
void Default_IRQHandler_Hook(void);
void HardFault_Handler(void);
static void boot_jump_to_EXITHDPLIB(uint32_t boot_jump_addr1, uint32_t boot_jump_addr2,
                                    uint32_t vector_table, uint32_t mpu_region);

/**
  * @brief  This function is the Reset Handler called on controller reset.
  * @param  None
  * @retval None
  */
__NO_RETURN void Reset_Handler(void)
{
  __set_MSPLIM((uint32_t)(&__STACK_LIMIT));

  /* Early startup */
  SECURITY_ApplyRamCfgEarlyStartup();
  SECURITY_ApplyTamperEarlyStartup();

  /* Configure DWT to enable cycles counter */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk | CoreDebug_DEMCR_MON_EN_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

  SystemInit();         /* CMSIS System Initialization */

  __PROGRAM_START();    /* Enter PreMain (C library entry point) */
}

/**
  * @brief  This function is the default IRQ handler hook
  *         when the IRQ line is not used by the application.
  * @param  None
  * @retval None
  */
void Default_IRQHandler_Hook(void)
{
  Error_Handler();
}

/**
  * @brief  Hard Fault Handler.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  Error_Handler();
}

/**
  * @brief  Jumping to next image using BL service, after erasing SRAM twice.
  * @param  boot_jump_addr1 boot_jump_to_EXITHDPLIB address in order to execute twice SRAM erasing
  * @param  boot_jump_addr2 BL service address
  * @param  vector table vector table of the next image
  * @param  mpu_region MPU region to be enabled
  */
__attribute__((naked)) void boot_jump_to_EXITHDPLIB(uint32_t boot_jump_addr1, uint32_t boot_jump_addr2,
                                                    uint32_t vector_table, uint32_t mpu_region)
{
  __ASM volatile(
#if !defined(__ICCARM__)
    ".syntax unified                 \n"
#endif /* !__ICCARM__ */
    "mov     r7, r0                  \n"
    "mov     r8, r1                  \n"
    "mov     r9, r2                  \n"
    "mov     r10, r3                 \n"
    "bl      boot_clear_ram_area     \n" /* Clear RAM before jump */
    "movs    r0, #0                  \n" /* Clear registers: R0-R12, */
    "mov     r1, r0                  \n" /* except R7/R8/R9/R10 */
    "mov     r2, r0                  \n"
    "mov     r3, r0                  \n"
    "mov     r4, r0                  \n"
    "mov     r5, r0                  \n"
    "mov     r6, r0                  \n"
    "mov     r11, r0                 \n"
    "mov     r12, r0                 \n"
    "mov     lr,  r0                 \n"
    "mov     r0, r8                  \n"
    "mov     r1, r9                  \n"
    "mov     r2, r10                 \n"
    "mov     r8, r3                  \n"
    "mov     r9, r3                  \n"
    "mov     r10, r3                 \n"
    "bx      r7                      \n" /* Jump to RSS service */
  );
}

#if defined(OEMIROT_FAST_WAKE_UP)
/**
  * @brief This function checks if system wakes-up from low-power mode:
  *        Stand-by mode and shut down mode (depending on HW capability).
  * @note
  * @retval FIH_SUCCESS in case of wake-up
  */
fih_ret boot_platform_wakeup(void)
{
  fih_ret fih_rc = FIH_FAILURE;

  /* Check Stand-By Flag */
  if (HAL_PWR_GetPreviousSystemPowerMode() == HAL_PWR_SYSTEM_STANDBY_MODE)
  {
    fih_rc = FIH_SUCCESS;
  }

  FIH_RET(fih_rc);
}

/**
  * @brief This function returns the configured secure application vector table.
  * @param vt  pointer to secure application vector table descriptor
  * @retval FIH_SUCCESS in case of vt has well been set
  */
fih_ret boot_platform_vector_table(struct boot_arm_vector_table **vt)
{
  fih_ret fih_rc = FIH_FAILURE;

  *vt = (struct boot_arm_vector_table *)(CODE_START);
  fih_rc = FIH_SUCCESS;

  FIH_RET(fih_rc);
}
#endif /* OEMIROT_FAST_WAKE_UP */

/**
  * @brief This function manage the jump to secure application.
  * @note
  * @retval void
  */
void boot_platform_quit(struct boot_arm_vector_table *vector)
{
  struct boot_arm_vector_table *vt = NULL;
  uint32_t exithdplib_jump_hdp_lvl2 = 0U;
#if defined(MCUBOOT_DOUBLE_SIGN_VERIF)
  uint32_t image_index;

  (void)fih_delay();
#if defined(OEMIROT_FAST_WAKE_UP)
  fih_ret fih_rc = FIH_FAILURE;

  /* Check if wake-up from low-power to bypass images revalidation */
  FIH_CALL(boot_platform_wakeup, fih_rc);
  if (FIH_NOT_EQ(fih_rc, FIH_SUCCESS))
#endif /* OEMIROT_FAST_WAKE_UP */
  {
    /* Check again if images have been validated, to resist to basic hw attacks */
    for (image_index = 0U; image_index < MCUBOOT_IMAGE_NUMBER; image_index++)
    {
      if (ImageValidStatus[image_index] != IMAGE_VALID)
      {
        ROT_LOG_ERR("Error while double controlling images validation");
        Error_Handler();
      }
    }
  }
#endif /* MCUBOOT_DOUBLE_SIGN_VERIF */

  /* Init RSS jump function descriptor */
  exithdplib_jump_hdp_lvl2 = (uintptr_t)(EXITHDPLIB_PFUNC->JumpHDPLvl2);

#if defined(MCUBOOT_USE_HASH_REF)
  /* Store new hash references in flash for next boot */
  if (boot_hash_ref_store() != 0)
  {
    ROT_LOG_ERR("Error while storing hash references");
    Error_Handler();
  }
#endif /* MCUBOOT_USE_HASH_REF */

  /* Check Flow control state */
  FLOW_CONTROL_CHECK(uFlowProtectValue, FLOW_CTRL_STAGE_2);
  uFlowStage = FLOW_STAGE_CFG;

  RNG_DeInit();

  /* Invalidate ICache before jumping to application */
  hal_icache_handle_t *p_icache = mx_rot_icache_gethandle();
  (void)HAL_ICACHE_Invalidate(p_icache);

  /* Update run time protections for application execution */
  SECURITY_UpdateRunTimeProtections();

  vt = (struct boot_arm_vector_table *)vector;

  /* Check Flow control state */
  FLOW_CONTROL_CHECK(uFlowProtectValue, FLOW_CTRL_STAGE_3_A);
  uFlowStage = FLOW_STAGE_CHK;

  /* Double the update of run time protections, to resist to basic hardware attacks */
  SECURITY_UpdateRunTimeProtections();

  /* Check Flow control for dynamic protections */
  FLOW_CONTROL_CHECK(uFlowProtectValue, FLOW_CTRL_STAGE_4_A);
  /*  change stack limit  */
  __set_MSPLIM(0);

  boot_jump_to_EXITHDPLIB((uintptr_t)&boot_jump_to_EXITHDPLIB, exithdplib_jump_hdp_lvl2, (uint32_t) vt,
                          OEMIROT_APPLI_REGION_TO_X);

  /* Avoid compiler to pop registers after having changed MSP */
#if !defined(__ICCARM__)
  __builtin_unreachable();
#else
  for (;;) {}
#endif /* defined(__ICCARM__) */
}

/**
  * @brief This function is called to clear all RAM area before jumping in
  * in Secure application .
  * @note
  * @retval void
  */
void boot_clear_ram_area(void)
{
  __IO uint32_t *pt = (uint32_t *)ROT_DATA_START;
  uint32_t index;

  for (index = 0; index < (ROT_DATA_SIZE / 4U); index++)
  {
    pt[index] = 0;
  }
}

/* exported variables --------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Platform init.
  * @param  None
  * @retval status
  */
int32_t boot_platform_init(void)
{
  /**
    * System Init: this code placed in targets folder initializes your system.
    * It calls the initialization (and sets the initial configuration) of the peripherals.
    * You can use STM32CubeMX to generate and call this code or not in this project.
    * It also contains the HAL initialization and the initial clock configuration.
    */
  if (mx_system_init() != SYSTEM_OK)
  {
    Error_Handler();
  }

#if defined(MCUBOOT_HAVE_LOGGING)
  /* Initialize basic_stdio separately, but after system init. */
  (void)mx_basic_stdio_init();
#endif /* MCUBOOT_HAVE_LOGGING */

  ROT_LOG_INF("Starting OEMiRoT");

  /* Display the system core clock frequency */
  ROT_LOG_INF("SystemCoreClock: %u Hz", SystemCoreClock);

  /* Start HW randomization */
  RNG_Init();

  (void)fih_delay_init();

  if (FLASH_DEV_NAME.Initialize(NULL) != ARM_DRIVER_OK)
  {
    ROT_LOG_ERR("Error while initializing Flash Interface");
    Error_Handler();
  }

  /* Apply Run time Protection */
  SECURITY_ApplyRunTimeProtections();
  /* Check static protections */
  SECURITY_CheckStaticProtections();

  /* Check Flow control state */
  FLOW_CONTROL_CHECK(uFlowProtectValue, FLOW_CTRL_STAGE_1);
  uFlowStage = FLOW_STAGE_CHK;
  /* Double protections apply / check to resist to basic fault injections */
  /* Apply Run time Protection */
  (void)fih_delay();
  SECURITY_ApplyRunTimeProtections();
  /* Check static protections */
  SECURITY_CheckStaticProtections();

#if defined(MCUBOOT_USE_HASH_REF)
  /* Load all images hash references (for mcuboot) */
  if (boot_hash_ref_load() != 0)
  {
    ROT_LOG_ERR("Error while loading Hash references from FLash");
    Error_Handler();
  }
#endif /* MCUBOOT_USE_HASH_REF */

  return 0;
}

/**
  * @brief  This function is executed in case of error occurrence.
  *         This function does not return.
  * @param  None
  * @retval None
  */
#if defined(OEMIROT_STOP_ON_ERROR)
/* Place code in a specific section */
#if defined(__ICCARM__)
#pragma default_function_attributes = @ ".ROT_Error_Code"
#else
__attribute__((section(".ROT_Error_Code")))
#endif /* __ICCARM__ */
__NO_RETURN void Error_Handler(void)
{
  while (1);
}
#else /* OEMIROT_STOP_ON_ERROR */
/* Place code in a specific section */
#if defined(__ICCARM__)
#pragma default_function_attributes = @ ".ROT_Error_Code"
#else
__attribute__((section(".ROT_Error_Code")))
#endif /* __ICCARM__ */
__NO_RETURN void Error_Handler(void)
{
  /* It is customizable */
  NVIC_SystemReset();
#if !defined(__ICCARM__)
  /* Avoid bx lr instruction (for fault injection) */
  __builtin_unreachable();
#endif /* defined(__ICCARM__) */
}
#endif /* OEMIROT_STOP_ON_ERROR */

/* Stop placing data in specified section */
#if defined(__ICCARM__)
#pragma default_function_attributes =
#endif /* __ICCARM__ */

#if defined(__ARMCC_VERSION)
/* reimplement the function to reach Error Handler */
void __aeabi_assert(const char *expr, const char *file, int line)
{
#if defined(OEMIROT_DEV_MODE)
  ROT_LOG_INF("assertion \" %s \" failed: file %s %d\n", expr, file, line);
#endif /* OEMIROT_DEV_MODE */
  Error_Handler();
}
#endif  /*  __ARMCC_VERSION */
#if defined(USE_FULL_ASSERT)

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  Error_Handler();
}
#endif /* USE_FULL_ASSERT */

#if defined(__ARMCC_VERSION)
/* Reimplement the exit function to invoke the Error Handler.
 * This is necessary because the __ARMCC_VERSION toolchain uses the microlib,
 * which does not provide a default implementation of the exit function.
 * By redirecting exit calls to Error_Handler(), we ensure the system enters
 * a safe error state in case of unexpected termination.
 */
void exit(int status)
{
  Error_Handler();
}
#endif /* __ARMCC_VERSION */
