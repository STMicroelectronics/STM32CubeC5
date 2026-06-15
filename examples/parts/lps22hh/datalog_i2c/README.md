<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *lps22hh_datalog_i2c*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the Cube firmware package.")

How to use part LPS22HH API.
It illustrates it by getting the value of the temperature (in celsius) and of pressure (in hPascal) and displaying them on a terminal.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: Initializes and enables temperature feature of LPS22HH

__Step 2__: Gets the values of the temperature (in celsius) and of pressure (in hPascal) and display them on a terminal

__End of example__: It is an endless example that loops infinitely on step 2.

If you enable **`USE_TRACE`**, you can follow these steps, in the nominal case of execution, in the terminal logs:

```text
[INFO] Step 1: LPS22HH sensor init completed
[INFO] Step 1: Enabling TEMP and PRESS feature completed
[INFO] Step 2: TEMP: 31 degC
[INFO] Step 2: PRESS: 1081 hPa
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the Cube firmware package.")

This example demonstrates the following driver:

- Part lps22hh.c/.h
- Part lps22hh/interfaces/i2c/lps22hh_io.c/.h


In this example, the LPS22HH component is configured through the I2C IO operations defined under the folder interfaces/i2c.
Once the I2C is initialized, the LPS22HH can be initialized too through the call of lps22hh_drv_init() API.
After this step, the MEMS sensor will be ready to provide the values of the temperature and pressure.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This section describes the hardware setup principles that apply to any board.

### __3.2. Specific board setups__

<details>
<summary>On STM32C5 series.</summary>
  <summary>On board NUCLEO-C562RE.</summary>

  | Board connector | MCU pin | Signal name | ARDUINO <br> connector pin |
  | :-------------: | :-----: | :---------: | :------------------------: |
  |      CN5-10     |   PB6   |  I2C1_SCL   |  ARDUINO CONNECTOR - D15   |
  |      CN5-9      |   PB7   |  I2C1_SDA   |  ARDUINO CONNECTOR - D14   |

  - I2C ADDw = 0x5D (7-bit address).

</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the Cube firmware package.")


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the Cube firmware package.")

- [Application Note AN5227](https://www.st.com/resource/en/application_note/an5227-lps22hh-digital-pressure-sensor-hardware-guidelines-for-system-integration-stmicroelectronics.pdf): LPS22HH digital pressure sensor: hardware guidelines for system integration

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
