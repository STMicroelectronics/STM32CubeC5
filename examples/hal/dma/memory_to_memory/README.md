<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_dma_memory_to_memory*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use a DMA to transfer a word data buffer from flash memory to RAM through the HAL API.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: Configures the DMA in memory to memory mode.
               Registers the user callbacks for DMA events: transfer errors and transfer complete.

__Step 2__: Starts the DMA transfer.

__Step 3__: Waits for one of these DMA interrupts: transfer complete or transfer error.

__Step 4__: Deinitializes the DMA.

__End of example__: Reports the outcome of the data transfer via the variable **`ExecStatus`**,
and the **status LED** remains turned on in case of success.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

- The *DMA* is configured in memory to memory mode.
- The *DMA* is configured with source and destination increment:
  After each word transfer, the DMA automatically increments source and destination to copy the next word to the
  next location in the destination buffer.

- The *DMA* is triggered by software.
- Source and destination buffers are declared as uint32_t arrays, it matches the DMA configuration to work with word
  data width. This avoids data misalignment.

The `SrcBuffer` variable is declared as const to ensure that the integrated development environment places it in the flash memory section during preprocessing.
The `DstBuffer` variable is a buffer shared between the CPU and the DMA.

When the data cache is enabled, inconsistent data values can occur if the CPU and DMA access the same resources. This issue arises because the CPU accesses cached data, while the DMA accesses data directly from memory. As a result, discrepancies between the data accessed by the CPU and DMA can occur. To address this issue, place shared variables in a non cacheable area. Therefore, the `DstBuffer` variable is located in a non cacheable area called `.bss.non_cacheable_area`, and non cacheable attributes are applied to it using the memory protection unit (MPU) during system initialization in `mx_system_init()`, before starting the application scenario.

According to the previous description, three types of alignment must be considered in these kind of examples:

- MPU region alignment (when data cache available):
For Cortex M33, each MPU region must have a size that is a power of two and a base address aligned to that size. In practice, if a region is 2^n bytes, its start address must be a multiple of 2^n.
In this example, the non cacheable region is 1 KB, so the base address is a multiple of 1 KB, as recommended by ARM. This is considered in the linker files of supported integrated development environments.
The base address and size of the MPU region and the `.bss.non_cacheable_area` must be identical.

- DMA access alignment:
For DMA, the buffer address and its size should be aligned to the DMA transfer width, for example, 4-byte alignment for 32-bit transfers, so the controller can generate efficient, valid bus accesses.  This is considered in the declaration of the `DstBuffer` variable in the example.c.
Misaligned buffers may force the DMA to split transfers into extra bus cycles or, on some implementations, may not be supported.

- Cache alignment (when available):
For cache alignment, shared buffers should be aligned to the cache line size and sized as multiples of that line size.
This ensures that clean and invalidate operations apply only to the buffer itself and do not affect other data that might share the same cache line.

To prevent loss of addressable memory, the linker aligns sections to the memory protection unit (MPU) region size, which is 1 KB. The direct memory access (DMA) alignment requirement is only 8 bytes. This constraint ensures that no data is lost or truncated from a DMA perspective.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

JTAG/SWD probe may be used to check variables and buffers.

### __3.2. Specific board setups__

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>Data cacheability</summary>

  For C5 devices, there is no data cache (DCACHE). Therefore, there is no memory protection unit (MPU) configuration. However, the linker files retain the non cacheable section named `RAM_NON_CACHEABLE` to maintain code portability across series. This section is also kept as a reference for educational purposes.

  </details>
  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |

  </details>
</details>

## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

Take care of __DMA data misalignment__:
Depending on the DMA data width used, source and destination addresses must respect data alignment.
For details, refer to the reference manual of your MCU.

Take care of __Cache data misalignment__:
Depending on the cache line size used.
For details, refer to the reference manual of your MCU.

Take care of __MPU region misalignment__:
Depending on the cortex in use.
For details, refer to ARM documentation of your cortex.

In case you cannot use the non cacheble area memory, Take care of __cache coherency issue__:
When the data cache memory is enabled, it is generally not in the path of DMA transfer, thus a cache coherency issue might
appear. It might be necessary to tackle cache coherency.
See H7 FAQ:
[DMA-is-not-working-on-STM32H7-devices](https://community.st.com/s/article/FAQ-DMA-is-not-working-on-STM32H7-devices).

Take care of __DMA ports__:
Depending on STM32 series, and DMA instance used (GPDMA/HPDMA/LPDMA) specific DMA ports constraints must be respected.
For details, refer to the reference manual of your MCU. You can also see the application note in the `__5. See Also` section.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

- [Application Note AN2548](https://www.st.com/resource/en/application_note/cd00160362-using-the-stm32f0f1f3gxlx-series-dma-controller-stmicroelectronics.pdf): Using the STM32F0/F1/F3/Gx/Lx Series DMA controller
- [Application Note AN4031](https://www.st.com/resource/en/application_note/dm00046011-using-the-stm32f2-stm32f4-and-stm32f7-series-dma-controller-stmicroelectronics.pdf): Using the STM32F2, STM32F4 and STM32F7 Series DMA controller
- [Application Note AN5593](https://www.st.com/resource/en/application_note/an5593-how-to-use-the-gpdma-for-stm32u5-series-microcontrollers-stmicroelectronics.pdf): How to use the GPDMA for STM32U5 Series microcontrollers

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
