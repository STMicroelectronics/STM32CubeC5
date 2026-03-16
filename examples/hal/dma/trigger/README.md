<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_dma_trigger*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure and use the DMA HAL API to perform DMA triggered transactions.


## __1. Detailed scenario__

`SrcTriggeringBuffer` and `SrcTriggeredBuffer` are declared as 'const' so that the linker places them in flash.

In this example, the CPU and a DMA share two buffers to manage the data: `DstTriggeringBuffer`, `DstTriggeredBuffer`.
On an STM32 device with data cache enabled, it is mandatory to ensure the buffer is never cached, as this scenario does not include data cache maintenance operations.
To do so, we place the buffers in the `.non_cacheable_variables` memory section and apply the appropriate MPU settings during system initialization in `mx_system_init()`.

__Initialization phase__: At the beginning of the `main()` function, the `mx_system_init()` function is called to initialize the peripherals, the flash interface, the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: Configures the DMA in **memory to memory mode**.
               Registers the user callbacks for DMA events: transfer errors and transfer complete.

__Step 2__: Start the DMA transfer on the triggered channel.

__Step 3__: Sequentially starts several triggering DMA transfers,
               each transferring 4 bytes in interrupt mode. After each triggering DMA completes,
               the triggered DMA channel transfers a single word,
               enabled by the **single burst trigger configuration**.
               Repeat this loop (waiting for each triggering DMA completion via interrupts),
               until the entire triggered DMA transfer is completed.

__Step 4__: After all small triggering DMA transfers complete,
               wait for the triggered DMA channel to finish the entire full buffer transfer.
               This wait is implemented using interrupt-driven flags,
               as the triggered DMA channel uses interrupts in this example.
               This step ensures the triggered DMA has completed processing all triggered transfers.

__Step 5__: Deinitialize the DMA instances before exiting the application scenario.

__End of example__: Reports the outcome of the data transfer via the variable **`ExecStatus`**,
and the **status LED** remains turned on in case of success.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

- The **DMA** channels (triggering Channel and triggered Channel) are configured in **memory-to-memory transfer mode**.

- **Both channels** are set with source and destination **address increment enabled**.
  After each word transfer, the DMA automatically increments the source and destination addresses to copy the next word to the subsequent location in the destination buffer.

- The **DMA** transfer is triggered by software for triggering Channel.

- Both the **triggering Channel** and the **triggered Channel** are configured to operate in **interrupt mode (IT)**.

- The **triggering Channel** performs **multiple small transfers** with interrupts on completion.

- The **triggering Channel** transfers **one data word** from a source memory location to a destination memory location. This mode is **single burst transfer mode**. 
  When the triggering transfer completes, the hardware DMA automatically starts the **triggered Channel transfer** of its own **data word**. 
  After the **triggered Channel** finishes its transfer, it sends a signal to the system (interrupt) to indicate the transfer is complete. 
  This sequence repeats until the **entire data block** from the source buffer is transferred to the destination buffer.

- The **data width** for both source and destination is set to **word (32-bit)**, matching the buffer data type to avoid data misalignment.

- **Both channels** have a **low-weight high priority setting** to balance resource allocation.

- The **source** and **destination** buffers are declared as **uint32_t arrays**, ensuring compatibility with the **32-bit word data width** configuration.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

JTAG/SWD probe may be used to check variables.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.
This example can run without external setup.

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |

  </details>

  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |

  </details>

  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |  PH0_OSC_IN   |
  |    PH1    |  RCC_OSC_OUT  |  PH1_OSC_OUT  |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

Take care of __data misalignment__:
Depending on the DMA data width used, source and destination addresses must respect data alignment.
For details, refer to the reference manual of your MCU.

Take care of __cache coherency issue__:
When the data cache memory is enabled, it is generally not in the path of DMA transfer, thus a cache coherency issue might
appear. It might be necessary to tackle cache coherency.
See H7 FAQ:
[DMA-is-not-working-on-STM32H7-devices](https://community.st.com/s/article/FAQ-DMA-is-not-working-on-STM32H7-devices).

Take care of __DMA ports__:
Depending on STM32 series, and DMA instance used (GPDMA/HPDMA/LPDMA) specific DMA ports constraints must be respected.
For details, refer to the reference manual of your MCU. You can also see the application note in the `__5. See Also` section.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
