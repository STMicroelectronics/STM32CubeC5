<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *filex_nor_rw_file_no_os*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

This application provides an example of FileX and LevelX stack usage, running in standalone mode. It demonstrates how to create a Fat File system  in polling mode from on the NOR flash using FileX a long side LevelX.

The application is designed to execute file operations on the W25Q128J NOR flash device, the code provides all required software code for properly managing it.

The application's main calls the app_filex_init() will start by formatting the NOR Flash using FileX services. The resulting file system is a FAT32 compatible, with 512 bytes per sector

Upon successful opening of the flash media, FileX continue with creating a file called "STM32.TXT" into the root directory, then write into it some dummy data. Then file is re-opened in read only mode and content is checked.

Through all the steps, FileX/LevelX services are called to print the flash size available before and after the example file is written into the flash.

The number of occupied sectors is also shown.

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

- __Step 1__: configures and initializes the System File.
- __Step 2__: open the external memory.
- __Step 3__: create file called STM32.TXT in the root directory to the external memory.
- __Step 4__: writes data in the file created in external memory
- __Step 5__: Read data from the file into read_buffer.
-__step 6__: Deinitializes the SPI instance before leaving the scenario.
__End of example__: After Step 6, the example is completed. You can verify that the example runs properly via the status LED and the `ExecStatus` variable.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the Cube firmware package.")

- *W25Q128J*: is configured with the following features and settings:
- Write and erase operations enabled
- Configured for SPI operations
- Selected GPIO pins support the SPI alternate function. They are configured in push-pull mode with no pull-up or pull-down.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This section describes the hardware setup principles that apply to any board.

```
  +-------------------------+                     +-------------------------+
  |          +--------------+                     |                         |
  |          | STM32 SPIi   |                     |                         |
  |          |              |                     |                         |
  |          |         SCLK *---------------------* SCLK                    |
  |          |              |                     |                         |
  |          |         MOSI *---------------------* MOSI                    |
  |          |              |                     |                         |
  |          |         MISO *---------------------* MISO                    |
  |          |              |                     |                         |
  |          |           CS *---------------------* CS#                     |
  |          |              |                     |                         |
  |          |              |                     |                         |
  |          +--------------+                     |                         |
  |                         |                     |                         |
  | STM32 MCU               |                     |       W25Q128J          |
  +-------------------------+                     +-------------------------+
```

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<details>
<summary>On STM32C5 series.</summary>
<details>
<summary>On board NUCLEO-C562RE.</summary>

  On the NUCLEO-C562RE board, the external W25Q128J memory is connected via the SPI interface.

  |  MCU pin  |  Signal name  |  User Label  |
  |:---------:|:-------------:|:------------:|
  |    PH0    |  RCC_OSC_IN   |    OSC_IN    |
  |    PH1    |  RCC_OSC_OUT  |   OSC_OUT    |
  |    PA2    |   USART2_TX   |     PA2      |
  |    PA5    |   SPI1_SCK    |     PA5      |
  |    PA6    |   SPI1_MISO   |     PA6      |
  |    PA7    |   SPI1_MOSI   |     PA7      |
  |    PC6    |     GPIO      |      -       |

  The W25Q128J NOR Flash memory supports up to 133 Mhz except for Read Data operation which supports only up to 50 Mhz. For this example, the SPI1 clock is set to 2Mhz.

</details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

You can also refer to this example:

More information about the STM32Cube Drivers can be found in the drivers' user manual of the STM32 series you are using.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
