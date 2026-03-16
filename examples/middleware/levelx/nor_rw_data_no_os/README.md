<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *levelx_nor_rw_data_no_os*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the LevelX stack in standalone mode.

This application demonstrates read and write operations on NOR flash using the nor flash dma no os driver.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

- __Step 1__: Initializes and opens the NOR flash driver.
- __Step 2__: Writes data to the test sector.
- __Step 3__: Reads data from the test sector.
- __Step 4__: Verifies data integrity.
- __Step 5__: Releases the test sector.
- __Step 6__: Closes the NOR flash driver.
__End of example__: After Step 6, the example is completed. You can check its status via the MX_STATUS_LED.


If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step1: Initializing LevelX NOR flash...
[INFO] LevelX NOR flash initialized successfully
[INFO] Opening NOR flash driver...
[INFO] NOR flash driver opened successfully
[INFO] Step2: Writing data to the test sector...
[INFO] Successfully wrote data to the test sector
[INFO] Step3: Reading data from the test sector...
[INFO] Successfully read data from the test sector
[INFO] Step4: Verifying data integrity...
[INFO] Data integrity check passed
[INFO] Step5: Releasing the test sector...
[INFO] Successfully released the test sector
[INFO] Step6: NOR flash driver closed successfully.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This section describes the hardware setup principles that apply to any board.

<!--
@startuml
@startditaa{doc/w25q128j_generic_hardware_setup.png}
  +-------------------------+                     +-------------------------+
  |          +--------------+                     |                         |
  |          |   STM32 SPIi |                     |                         |
  |          |              |                     |                         |
  |          |          SCLK *---------------------* SCLK                   |
  |          |              |                     |                         |
  |          |          MOSI *---------------------* MOSI                   |
  |          |              |                     |                         |
  |          |         MISO *---------------------* MISO                    |
  |          |              |                     |                         |
  |          |          CS  *---------------------* CS#                     |
  |          |              |                     |                         |
  |          |              |                     |                         |
  |          +--------------+                     |                         |
  |                         |                     |                         |
  |                         |                     |                         |
  | STM32 MCU               |                     |       W25Q128J          |
  +-------------------------+                     +-------------------------+
@endditaa
@enduml
-->

No generic hardware dependencies for this example.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<details>
<summary>On STM32C5 series.</summary>
<details>
  <summary>On board NUCLEO-C562RE.</summary>

  | Arduino pin | MCU pin | Signal name     | User Label    |
  | :---:       | :---:   | :---:           | :---:         |
  | D13         | PA5     | SPI1_CLK        | -             |
  | D12         | PA6     | SPI1_MISO       | -             |
  | D11         | PA7     | SPI1_MOSI       | -             |
  | D9          | PC6     | SPI1_CS         | -             |

  The W25Q128J NOR Flash memory supports up to 133 Mhz except for Read Data operation which supports only up to 50 Mhz.. For this example, the SPI1 clock is set to 48Mhz.

</details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the Cube Firmware package.")


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the Cube Firmware package.")


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
