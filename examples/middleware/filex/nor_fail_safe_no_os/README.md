<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *filex_nor_fail_safe_no_os*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

This application provides an example of FileX stack usage on an external NOR flash.

It demonstrates FileX usage in a **no-OS** context, with **fail-safe / fault-tolerant** operations enabled. The application performs basic file operations on the NOR flash device while exercising a reset sequence driven by a persistent counter stored in the internal Flash.

The example starts by initializing the system, then initializes FileX and the NOR flash driver. On the first application iteration, the FileX media is formatted if no valid filesystem is found. On later reset iterations, the application expects the fail-safe media state to be recovered without reformatting the user media.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory, MPU regions (if applicable), the system clock, and the SysTick.

The application also uses one internal Flash word at `FLASH_USER_ADDR` as a persistent reset counter for the current scenario execution. When the location is erased (`0xFFFFFFFF`), the counter is treated as `0`. During initialization, the counter is incremented and written back to internal Flash before FileX is initialized. Therefore, the first scenario iteration stores the value `1`, then subsequent iterations store `2`, `3`, and so on until the configured reset limit is reached.

The application executes the following __example steps__:

- __Step 1__: Initializes the platform handles, updates the internal Flash reset counter, and initializes FileX.
- __Step 2__: Opens the FileX media on the external NOR flash. If the media cannot be opened during the first iteration (`FLASH_USER_ADDR == 1`), the media is formatted and opened again. On later iterations, the media is not reformatted; the fail-safe state is expected to allow recovery or the example reports the media as corrupted.
- __Step 3__: Starts generic file I/O operations on the target media: create a file, write data, flush the media, read the data back, verify it, and close the media.
- __Step 4__: Handles the end of the reset scenario. If the reset counter limit has been reached, the example de-initializes the used resources.

__Fail-safe reset behavior__: During Step 3, a timer is started in interrupt mode. When the timer expires, its callback checks the current reset counter value. As long as the counter is lower than `NVIC_RESET_COUNT` (defined in [application/example.h](application/example.h), default: 5), the firmware prints a reset message and triggers `NVIC_SystemReset()`. After the reset, the counter value stored at `FLASH_USER_ADDR` is preserved, incremented during the next initialization, and the scenario restarts from Step 1.

This sequence intentionally creates system resets while FileX operations may be in progress. It is used to demonstrate the fault-tolerant media behavior: the first iteration may create the filesystem, while subsequent iterations exercise recovery of the existing media instead of formatting it again.


__End of example__: After Step 4, the example reports its status via the status LED.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

- *W25Q128J*: is configured with the following features and settings:
- Write and erase operations enabled
- Optimized for safe/robust file operations
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


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

