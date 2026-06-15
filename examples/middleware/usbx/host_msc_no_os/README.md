<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *usbx_host_msc_no_os*__

**Example version:** 2.0.0

This example demonstrates a USB Host Mass Storage Class "MSC" able to enumerate and communicate with a USB storage device in bare metal mode based on USBX stack.

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the Cube Firmware package.")


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock and the systick

The application executes the following __example steps__:

- __Step 1__: USB HCD initialization.
- __Step 2__: Initialization of USBX stack.
- __Step 3__: Initialization of USBX MSC Host Class.

__End of example__: After Step 3, the example is completed.


In case of successful operations:

- Global variable `ExecStatus` set to EXEC_STATUS_OK

- Plug the USB MSC device into the STM32 board through a 'Type C to A-Female' cable to the USB connector used by the example.
  - Connect the ST-Link cable to the PC USB port to display data on the HyperTerminal.

  A virtual COM port will then appear in the HyperTerminal:
  - Hyperterminal configuration
    - Data Length = 8 Bits
    - One Stop Bit
    - No parity
    - BaudRate = 115200 baud
    - Flow control: None

In case of failure:

- LED toggles infinitely
- Global variable `ExecStatus` set to EXEC_STATUS_ERROR


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the Cube Firmware package.")


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This section describes the hardware setup principles that apply to any board.

### __3.2. Specific board setups__

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  | Board connector   | MCU pin | Signal name     | ARDUINO Uno V3 <br/> connector pin |
  | :---:             | :---:   | :---:           | :---:                             |
  | CN5-6             | PA5     | MX_STATUS_LED   | Not exposed                       |

  </details>
  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  Board connector  |  MCU pin  |  Signal name  |  ARDUINO <br/> connector pin  |  User Label  |
  |:-----------------:|:---------:|:-------------:|:----------------------------:|:------------:|
  |         -         |    PA5    |     GPIO      |              -               |      -       |
  |         -         |    PH0    |  RCC_OSC_IN   |              -               |  PH0_OSC_IN  |
  |         -         |    PH1    |  RCC_OSC_OUT  |              -               | PH1_OSC_OUT  |

  </details>
  </details>

## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

  1. If there is an issue, remember to check that:
     - External 5v injection for Vbus
     - COM port number is correct
     - Baudrate is set to 115200

  2. Depending on STM32 series, and USB instance used (FS/HS) specific USB configurations must be respected (Endpoint Max Packet Size, Clock)
     For details, refer to the reference manual of your MCU.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")


This [wiki](https://wiki.st.com/stm32mcu/wiki/Introduction_to_USBX) introduction to USBX.

More information about the STM32 Cube Drivers can be found in the drivers' user manual of the STM32 series you are using.

For instance, for the STM32C5: [User Manual](https://www.st.com).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
