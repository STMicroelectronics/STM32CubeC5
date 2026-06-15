<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *open_bootloader_demo*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

This application provides an example of open Bootloader usage through USART, I2C, SPI interfaces.

**Note that Icache is generated but remains disabled because enabling it can causes issues with read/write OpenBL functionality. If cache support is required, it should be enabled together with the proper MPU configuration for each memory zone.**


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or RAM), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: OpenBootloader Initialization.

    - configures and initializes the USART, I2C, SPI instances and configures memories FLASH, RAM, OTP, Options bytes.

__Step 2__: OpenBootloader interface detection.

    - waits for incoming communication on one of the supported protocol through cubeprogrammeur tool

__Step 3__: Command detection in the interface used.

    - waits for commands sent by the host in mx_openbl_process() until receive dedicated command

__End of example__: If no error occurs, The data is transferred between Host Cube programmer and open bootloader.

In case of successful operations:

- Global variable `ExecStatus` set to EXEC_STATUS_OK

In case of failure:

- Global variable `ExecStatus` set to EXEC_STATUS_ERROR


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

### __2.1. IPs configuration__

__USART__:

Connect the ST-Link cable to the PC USB port and launch STM32cubeprogrammer tool:

 - A virtual COM port will then appear in the STM32Cubeprogrammer:
 - configuration
    - Data Length = 8 Bits
    - One Stop Bit
    - No parity
    - BaudRate = 115200 baud
    - Flow control: None
 - select COM and press connect and connection is established

External st-lik V3 can be used as a probe to communicate with STM32Cubeprogrammer by connecting RX to USART_RX and TX to USART TX pins.

__I2C__:

Use St-link -V3 external and connect SDA and CLK and launch STM32cubeprogrammer:

 - select STlink
 - select I2C and put Address in hexadecimal format 0x36
 - connect and connection is established

__SPI__:

Use St-link -V3 external and connect SCK, MISO and MOSI and launch STM32cubeprogrammer:

 - select STlink
 - select SPI
 - connect and connection is established


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This section describes the hardware setup principles that apply to any board using VCP port.

    +-------------------------+                     +-------------------------+
    |          +--------------+                     +--------------+          |
    |          | STM32 MCU    |                     |   STM32Cube  |          |
    |          | USARTi       |                     |   Programmer |          |
    |          |              |                     |              |          |
    |          |  ST-Link     *----------------------->*  COMxx    |          |
    |          |  VCP_Port    |                     |              |          |
    |          |              |                     |              |          |
    |          |              |                     |              |          |
    |          |              |                     |              |          |
    |          +--------------+                     +--------------+          |
    |                         |                     |                         |
    |                         |                     |                         |
    |      STM32 target board |                     | PC via port USB         |
    +-------------------------+                     +-------------------------+

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project using probe.

__USART__:

    +-------------------------+                     +-------------------------+
    |          +--------------+                     +--------------+          |
    |          | STM32 MCU    |                     |  STLINK-V3   |          |
    |          | USARTi       |                     |  Probe       |          |
    |          |              |                     |              |          |
    |          |   USARTi_TX *-------------------->* TX            |          |
    |          |              |                     |              |          |
    |          |              |                     |              |          |
    |          |              |                     |              |          |
    |          |   USARTi_RX +<--------------------* RX            |          |
    |          |              |                     |              |          |
    |          +--------------+                     +--------------+          |
    |                         |                     |                         |
    |                      GND *---------------------* GND                    |
    |                         |                     |                         |
    |      STM32 target board |                     |       STLINK-V3 probe   |
    +-------------------------+                     +-------------------------+
                                                    |
                                                    |
                                      +-------------+-------------+
                                      |      PC / CubeProgrammer  |
                                      |   connected to STLINK-V3  |
                                      +---------------------------+


<details>
  <summary>On STM32C5 series.</summary>
    <summary>On board NUCLEO-C542RC.</summary>
    <summary>On board NUCLEO-C562RE.</summary>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  | Board connector | MCU pin | Signal name | ARDUINO <br/> connector pin | User Label |
  | :-------------: | :-----: | :---------: | :-------------------------: | :--------: |
  |      CN7        |   PA3   |  USART_RX   |         11                  |    PA3     |
  |      CN7        |   PA2   |  USART_TX   |          9                  |    PA2     |

</details>
__I2C__:

    +-------------------------+                     +-------------------------+
    |          +--------------+                     +--------------+          |
    |          | STM32 MCU    |                     |  STLINK-V3   |          |
    |          | I2Ci         |                     |  Probe       |          |
    |          |              |                     |              |          |
    |          |   I2Ci_SCL  *-------------------->* SCL           |          |
    |          |              |                     |              |          |
    |          |              |                     |              |          |
    |          |              |                     |              |          |
    |          |   I2Ci_SDA  *-------------------->* SDA           |          |
    |          |              |                     |              |          |
    |          +--------------+                     +--------------+          |
    |                         |                     |                         |
    |                      GND *---------------------* GND                    |
    |                         |                     |                         |
    |      STM32 target board |                     |       STLINK-V3 probe   |
    +-------------------------+                     +-------------------------+
                                                    |
                                                    |
                                      +-------------+-------------+
                                      |      PC / CubeProgrammer  |
                                      |   connected to STLINK-V3  |
                                      +---------------------------+


<details>
  <summary>On STM32C5 series.</summary>
    <summary>On board NUCLEO-C542RC.</summary>
    <summary>On board NUCLEO-C562RE.</summary>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  | Board connector | MCU pin | Signal name | ARDUINO <br/> connector pin | User Label |
  | :-------------: | :-----: | :---------: | :-------------------------: | :--------: |
  |      CN5        |   PB6   |  I2C1_SCL   |         D15                 |     SCL    |
  |      CN5        |   PB7   |  I2C1_SDA   |         D14                 |     SDA    |

</details>
__SPI__:

    +-------------------------+                     +-------------------------+
    |          +--------------+                     +--------------+          |
    |          | STM32 MCU    |                     |  STLINK-V3   |          |
    |          | SPIi         |                     |  Probe       |          |
    |          |              |                     |              |          |
    |          |   SPIi_SCK  *-------------------->* SCK           |          |
    |          |              |                     |              |          |
    |          |   SPIi_MISO *-------------------->* MISO          |          |
    |          |              |                     |              |          |
    |          |   SPIi_MOSI *-------------------->* MOSI          |          |
    |          |              |                     |              |          |
    |          +--------------+                     +--------------+          |
    |                         |                     |                         |
    |                      GND *---------------------* GND                    |
    |                         |                     |                         |
    |      STM32 target board |                     |       STLINK-V3 probe   |
    +-------------------------+                     +-------------------------+
                                                    |
                                                    |
                                      +-------------+-------------+
                                      |      PC / CubeProgrammer  |
                                      |   connected to STLINK-V3  |
                                      +---------------------------+


<details>
  <summary>On STM32C5 series.</summary>
    <summary>On board NUCLEO-C542RC.</summary>
    <summary>On board NUCLEO-C562RE.</summary>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  | Board connector | MCU pin | Signal name | ARDUINO <br/> connector pin | User Label |
  | :-------------: | :-----: | :---------: | :-------------------------: | :--------: |
  |     CN10        |   PB13  |  SPI2_SCK   |          30                 |     SCK    |
  |     CN10        |   PB14  |  SPI2_MISO  |          28                 |     MISO   |
  |     CN10        |   PB15  |  SPI2_MOSI  |          26                 |     MOSI   |

</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32 bootloader: [documentation](https://www.st.com/resource/en/application_note/an2606-introduction-to-system-memory-boot-mode-on-stm32-mcus-stmicroelectronics.pdf).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
