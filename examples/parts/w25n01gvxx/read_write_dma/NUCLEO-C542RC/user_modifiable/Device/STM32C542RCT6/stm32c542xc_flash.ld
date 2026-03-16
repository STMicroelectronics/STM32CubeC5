/**
******************************************************************************
* @file    stm32c542xc_flash.ld
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

HEAP_SIZE = 0x200;
STACK_SIZE = 0x400;


MEMORY
{
  ROM (rx) : org = 0x8000000, len = 0x40000
  RAM (xrw) : org = 0x20000000, len = 0x10000
}

SECTIONS
{
  .vectors :
  {
    . = ALIGN(8);
    KEEP(*(.vectors));
    . = ALIGN(8);
  } > ROM

  .text :
  {
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

  /* Non-cacheable TX buffer: 4 KB at start of RAM */
  .bss.non_cacheable_tx_buffer (NOLOAD) :
  {
    . = ALIGN(32);  /* 32-byte alignment for DMA */
    __non_cacheable_tx_buffer_start__ = .;
    *(.bss.non_cacheable_tx_buffer)
    . = __non_cacheable_tx_buffer_start__ + 0x1000;  /* 4 kB */
    __non_cacheable_tx_buffer_end__ = .;
    FILL(0xAA);
  } > RAM

  /* Non-cacheable RX buffer: 4 KB after TX buffer */
  .bss.non_cacheable_rx_buffer (NOLOAD) :
  {
    . = ALIGN(32);  /* 32-byte alignment for DMA */
    __non_cacheable_rx_buffer_start__ = .;
    *(.bss.non_cacheable_rx_buffer)
    . = __non_cacheable_rx_buffer_start__ + 0x1000;  /* 4 kB */
    __non_cacheable_rx_buffer_end__ = .;
    FILL(0xAA);
  } > RAM

  /* Non-cacheable area to store part obj: 256 Bytes after RX buffer */
  .bss.non_cacheable_part_obj (NOLOAD) :
  {
    . = ALIGN(32);  /* 32-byte alignment for DMA */
    __non_cacheable_part_obj_start__ = .;
    *(.bss.non_cacheable_part_obj)
    . = __non_cacheable_part_obj_start__ + 0x100;  /* 256 Bytes */
    __non_cacheable_part_obj_end__ = .;
    FILL(0xAA);
  } > RAM

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
