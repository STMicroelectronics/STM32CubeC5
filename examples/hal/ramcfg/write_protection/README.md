<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_ramcfg_write_protection*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the Cube Firmware package.")

How to configure and use the RAMCFG HAL API to demonstrate the RAMCFG write protection monitoring in DMA mode.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.


The application executes the following __example steps__:

- __Step 1__: configures and initializes the RAMCFG instance and the DMA channel in memory to memory mode.
Registers the user callbacks for DMA events: transfer errors and transfer complete.
- __Step 2__: erases the given SRAM memory.
- __Step 3__: enables write protection for the first half of SRAM memory.
- __Step 4__: fills SRAM pages with data.
- __Step 5__: checks the content of protected and unprotected SRAM pages.
- __Step 6__: deinitializes the DMA instance before leaving the scenario.
__End of example__: after step 6, the example is completed. You can check its status via the variable ExecStatus and the **status LED** remains turned on in case of success.


If you enable **`USE_TRACE`**, you can follow these steps, in the nominal case of execution, in the terminal logs:

```text
[INFO] Step 1: Device initialization COMPLETED.
[INFO] Step 2: Memory erasing COMPLETED.
[INFO] Step 3: Write protection enabled for the first half of the pages.
[INFO] Step 4: Memory filled with data.
[INFO] Step 5: Protected and unprotected SRAM checked successfully.
[INFO] Step 6: de-init.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

This example demonstrates the following peripheral:

__RAMCFG__:
The RAMCFG instance configured supports the write protection feature.

__DMA__:

- The *DMA* is configured in memory to memory mode.
- The *DMA* is configured with source and destination increment:
  After each word transfer, the DMA automatically increments source and destination to copy the next word to the next address in the internal SRAM.
- The *DMA* is triggered by software.
- Source buffer is declared as uint32_t arrays, it matches the DMA configuration to work with word data width. This avoids data misalignment.
- The *DMA* is configured with no offset applied to the source and destination addresses after each burst transfer.
- The *DMA* is configured to decrement the source address by 128 bytes and increment the destination address after each block transfer, with no offset applied to the destination address after each block transfer.
- The *DMA* channel is set up with direct transfer repeated block configuration.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

### __3.2. Specific board setups__

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PA2    |   USART2_TX   |      PA2      |

  </details>

  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PA2    |   USART2_TX   |      PA2      |

  </details>

  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |  PH0_OSC_IN   |
  |    PH1    |  RCC_OSC_OUT  |  PH1_OSC_OUT  |
  |    PA2    |   USART2_TX   | DBGIN_VCP_TX  |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

You can also refer to these other examples demonstrating how to secure access violation SRAM or with different HAL RAMCFG setups:

- example_hal_rif_internal_sram_protection
- example_hal_ramcfg_ecc_error_generation

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
