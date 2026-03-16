<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *levelx_nand_multi_access_freertos*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/read/read_toc.html "An offline version is also available in the STM32Cube firmware package.")

This application provides an example of LevelX stack usage with NAND flash and FreeRTOS.

It demonstrates LevelX's concurrent NAND flash access capabilities. The application is designed to execute read, write, and erase operations on a NAND flash device, showcasing concurrent access from multiple FreeRTOS tasks. All required software code for handling NAND flash I/O operations is provided.

The application starts by initializing the LevelX system and creating tasks to handle concurrent NAND flash operations. At this stage, all LevelX resources are created, the NAND flash driver is initialized, and the application creates 3 tasks with the same priority:

- __main_task__: Initializes and manages the NAND flash driver lifecycle.
- __app_task_one__: Performs write, read, and data integrity checks on a NAND flash sector.
- __app_task_two__: Performs write, read, and data integrity checks on a NAND flash sector.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as NAND flash memory), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

- __Step 1__: Prepare concurrent execution context. Creates an event and 3 tasks with the same priority.
- __Step 2__: Initialize LevelX NOR flash.
- __Step 3__: Start the FreeRTOS scheduler.
- __Step 4__: Format and open LevelX NOR flash.
- __Step 5__: Each task performs 5 iterations of read, write, and data integrity check on random NOR flash sectors, logging each successful operation.
- __Step 6__: Close the NOR flash driver.


If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: Prepare concurrent execution context
[INFO] Step 2: Initialize LevelX NAND flash
[INFO] Step 3: Starting FreeRTOS Scheduler
[INFO] Step 4: Format and open LevelX NAND flash
[INFO] Step 5, Task 1, Iteration 1: Read, write, and data integrity check completed successfully for sector 675
[INFO] Step 5, Task 2, Iteration 1: Read, write, and data integrity check completed successfully for sector 38
[INFO] Step 5, Task 1, Iteration 2: Read, write, and data integrity check completed successfully for sector 895
[INFO] Step 5, Task 2, Iteration 2: Read, write, and data integrity check completed successfully for sector 28
[INFO] Step 5, Task 1, Iteration 3: Read, write, and data integrity check completed successfully for sector 577
[INFO] Step 5, Task 2, Iteration 3: Read, write, and data integrity check completed successfully for sector 722
[INFO] Step 5, Task 1, Iteration 4: Read, write, and data integrity check completed successfully for sector 352
[INFO] Step 5, Task 2, Iteration 4: Read, write, and data integrity check completed successfully for sector 497
[INFO] Step 5, Task 1, Iteration 5: Read, write, and data integrity check completed successfully for sector 125
[INFO] Step 5, Task 2, Iteration 5: Read, write, and data integrity check completed successfully for sector 964
[INFO] Step 5: All concurrent read, write, and data integrity checks completed successfully for both tasks
[INFO] Step 6: Close the NAND flash driver
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

- *NAND*: Configured with the following features and settings:
  - Write, read, and erase operations enabled
  - Optimized for concurrent access
  - Configured for high-speed operations
  - Selected GPIO pins support the NAND alternate function. They are configured in push-pull mode with no pull-up or pull-down.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This section describes the hardware setup principles that apply to any board.

<!--
@startuml
@startditaa{doc/w25n01gvxx_generic_hardware_setup.png}
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
  | STM32 MCU               |                     |       W25N01GVXX        |
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
  | D10         | PC9     | SPI1_CS         | -             |

  The W25N01GVXX NAND Flash supports up to 104Mhz. For this example, the SPI1 clock is set to 48Mhz.

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
