/**
******************************************************************************
* @file    stm32c562xe_flash.ld
* @brief   Linker File
******************************************************************************
* @attention
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

/* Entry Point */
ENTRY(Reset_Handler)

HEAP_SIZE = 0x8000;
STACK_SIZE = 0x8000;


MEMORY
{
  ROM (rx) : org = 0x8000000, LENGTH = 512K
  RAM (xrw) : org = 0x20000000, LENGTH = 128K
}

/* Initial 32K Flash is used to store kernel functions and
 * initial 32K RAM is used to store kernel data. */
__privileged_functions_region_size__  = 32K;
__privileged_data_region_size__       = 32K;

__FLASH_segment_start__               = ORIGIN( ROM );
__FLASH_segment_end__                 = __FLASH_segment_start__ + LENGTH( ROM );

__SRAM_segment_start__                = ORIGIN( RAM );
__SRAM_segment_end__                  = __SRAM_segment_start__ + LENGTH( RAM );

__privileged_functions_start__        = __FLASH_segment_start__;
__privileged_functions_end__          = __FLASH_segment_start__ + __privileged_functions_region_size__ - 1;

__privileged_sram_start__             = __SRAM_segment_start__;
__privileged_sram_end__               = __SRAM_segment_start__ + __privileged_data_region_size__ - 1;

__unprivileged_flash_start__          = 0x08008F00;
__unprivileged_flash_end__            = __FLASH_segment_end__;

SECTIONS
{
  /* The startup code and FreeRTOS kernel code are placed into privileged
   * flash. */
  .privileged_functions :
  {
    . = ALIGN(8);
    KEEP(*(.vectors))
    KEEP(*(.vector))
    . = ALIGN(8);
    *(privileged_functions)
    . = ALIGN(8);
    FILL(0xDEAD);
    . = __privileged_functions_region_size__;
  } > ROM

  .text :
  {
    . = ALIGN(8);
    __syscalls_flash_start__ = .;
    *(freertos_system_calls)
    __syscalls_flash_end__ = . - 1;
    . = ALIGN(8);
    *(.text);
    *(.text*);
    *(.glue_7);
    *(.glue_7t);
    *(.eh_frame);
    KEEP (*(.init));
    KEEP (*(.fini));
    . = ALIGN(8);
    _etext = .;
  } > ROM

  .rodata :
  {
    . = ALIGN(8);
    *(.rodata);
    *(.rodata*);
    . = ALIGN(8);
  } > ROM

  .ARM.extab (READONLY) :
  {
    . = ALIGN(8);
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(8);
  } > ROM

  .ARM (READONLY) :
  {
    . = ALIGN(8);
    __exidx_start = .;
    *(.ARM.exidx*);
    __exidx_end = .;
    . = ALIGN(8);
  } > ROM

  .preinit_array (READONLY) :
  {
    . = ALIGN(8);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*));
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(8);
  } > ROM

  .init_array (READONLY) :
  {
    . = ALIGN(8);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)));
    KEEP (*(.init_array*));
    PROVIDE_HIDDEN (__init_array_end = .);
    . = ALIGN(8);
  } > ROM

  .fini_array (READONLY) :
  {
    . = ALIGN(8);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)));
    KEEP (*(.fini_array*));
    PROVIDE_HIDDEN (__fini_array_end = .);
    . = ALIGN(8);
  } > ROM

  .copy.table (READONLY) :
  {
    . = ALIGN(8);
    __copy_table_start__ = .;
    LONG(LOADADDR(.privileged_data));
    LONG (ADDR(.privileged_data));
    LONG (SIZEOF(.privileged_data) / 4);
    LONG(LOADADDR(.data));
    LONG (ADDR(.data));
    LONG (SIZEOF(.data) / 4);
    __copy_table_end__ = .;
  } > ROM

  .zero.table (READONLY) :
  {
    . = ALIGN(8);
    __zero_table_start__ = .;
    LONG (ADDR(.bss));
    LONG (SIZEOF(.bss) / 4);
    __zero_table_end__ = .;
  } > ROM

  .privileged_data :
  {
    . = ALIGN(8);
    _sdata = .;
    *(privileged_data)
    . = ALIGN(8);
    FILL(0xDEAD);
    . = __privileged_data_region_size__;
  } > RAM AT> ROM

  .data :
  {
    . = ALIGN(8);
    _sidata = LOADADDR(.data);
    __data_start__ = .;
    _sdata = .;
    *(.data);
    *(.data*);
    . = ALIGN(8);
    _edata = .;
  } > RAM AT> ROM

  .bss :
  {
    . = ALIGN(8);
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss);
    *(.bss*);
    *(COMMON);
    . = ALIGN(8);
    _ebss = .;
    __bss_end__ = _ebss;
  } > RAM

  .heap (NOLOAD) :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE (end = .);
    _heap_start = .;
    . += HEAP_SIZE;
    . = ALIGN(8);
    _heap_end = .;
    __HeapLimit = .;
  } > RAM

  .stack (NOLOAD) :
  {
    . = ALIGN(8);
    __StackLimit = .;
    . += STACK_SIZE;
    . = ALIGN(8);
    __StackTop = .;
    _estack = .;
    __stack = .;
  } > RAM

  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 :
  {
    *(.ARM.attributes)
  }
}
