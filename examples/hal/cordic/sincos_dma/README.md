<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_cordic_sincos_dma*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the Cube Firmware package.")

How to calculate sines and cosines array in DMA mode, using the CORDIC HAL API.


## __1. Detailed scenario__

In this example, the CPU and a DMA share buffers to manage the data: `Q1_31` and `CalculatedSinCos`.
On an STM32 device with data cache enabled, it is mandatory to ensure that the buffer is never cached, as this scenario does not handle data cache maintenance.
To do so, we place the buffer in the `.non_cacheable_variables` memory section and apply the appropriate MPU settings during system initialization in `mx_system_init()`.


__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: The applicative code initializes the CORDIC instance.

__Step 2__: Prepares the input values for CORDIC: angles must be in radians divided by pi and presented in Q1.31 format.

__Step 3__: Starts CORDIC calculation in DMA mode.

__Step 4__: Waits for the CORDIC read DMA transfer completion.
            Then compares the CORDIC results to the reference values.

__Step 5__: Deinitializes the CORDIC instance before leaving the scenario.

__End of example__: reports the outcome of the data transfer via the variable **`ExecStatus`** and the **status LED** remains turned on in case of success.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

__CORDIC__:

- Function: Sine. It includes sine as a primary result and cosine as a secondary result.
- Argument: 1. The angle is expressed in radians divided by pi.
- Results: 2. Sine and cosine.
- Input and output size: 32b. Thus, format Q1.31 is used.
- Precision: 6. Best precision for sines and cosines.
- Scale: 0. Mandatory for sine and cosine functions.


__DMA__:

Two DMA channels CORDIC write and CORDIC read are used and configured, respectively, as indicated below:

  - The DMA write channel is used to feed CORDIC with input values: angles in radians divided by pi in Q1.31 format.
    It is configured in memory to peripheral mode with an incremented source address and a fixed destination address.
    After each word transfer, the DMA automatically increments the source address.
    Thus, it copies the next input word from memory to the CORDIC argument register.

  - The DMA read channel is used to extract the computed values from the CORDIC to the output buffer.
    It is configured in peripheral to memory mode with a fixed source address and an incremented destination address.
    The data is loaded from the CORDIC result register to memory area incrementally.

For each DMA channel, CORDIC write and read, the corresponding NVIC line is configured and enabled.

__MPU__: is used to ensure that the buffer used for DMA transfer is not cached.
The linker script defines a specific memory section `.non_cacheable_variables` for this buffer.
This memory section is 1 KB in size and is aligned as per the MPU requirements.
The MPU configuration ensures that this 1 KB region is not cached.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

No specific hardware set up is needed for this example.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.
<details>
  <summary>On STM32C5 series.</summary>
  <details>
  <summary>On board NUCLEO-C562RE.</summary>

  | Board connector   | MCU pin | Signal name      | ARDUINO pin |
  | :---:             | :---:   | :---:            | :---:       |
  | CN5-6             | PA5     | MX_STATUS_LED    | D13         |

  </details>
</details>

## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

__Data misalignment__: Depending on the DMA data width used, source and destination addresses must respect data alignment. Refer to the Reference Manual of your MCU.

__Cache coherency__: When cache memory is enabled, it is generally not in the path of DMA transfer, thus cache coherency issues might appear.
See H7 FAQ to tackle cache coherency issues: [DMA-is-not-working-on-STM32H7-devices](https://community.st.com/s/article/FAQ-DMA-is-not-working-on-STM32H7-devices)

__DMA ports selection__: Depending on the STM32 series, and DMA instance used (GPDMA/HPDMA/LPDMA) specific DMA ports constraints must be respected.  For details, refer to the reference manual of your MCU. You can also see the application note in the `__5. See Also` section.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

- [Application Note AN5325](https://www.st.com/resource/en/application_note/an5325-how-to-use-the-cordic-to-perform-mathematical-functions-on-stm32-mcus-stmicroelectronics.pdf): How to use the CORDIC to perform mathematical functions on STM32 MCUs

- You can see the application note [AN5593](https://www.st.com/resource/en/application_note/an5593-how-to-use-the-gpdma-for-stm32-mcus-stmicroelectronics.pdf) to get further explanation about DMA port allocation.

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
