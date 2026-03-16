<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo/>

# __Example: *usbx_host_hub_msc_hid_freertos*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube Firmware package.")

How to configure a USB host hub application supporting human interface device (HID) and mass storage (MSC) class in RTOS mode using the USBX stack


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

- __Step 1__: USB HCD initialization

- __Step 2__: Initialization of USBX stack.

- __Step3__: Initialization of USBX HUB MSC HID Host.

__End of example__: After Step 3, the example is completed and the Hub MSC HID host is ready to interact with the device.

In case of successful operations:

- LED remains turned on
- Global variable `ExecStatus` set to EXEC_STATUS_INIT_OK

You can verify the USBX stack operates as expected:

- Plug The USB HUB device into the STM32 board through 'Type C  to A-Female' cable to the connector:
    - Use USB in full speed (FS)
    - Connect ST-Link cable to the PC USB port to display data on the HyperTerminal.

    A virtual COM port will then appear in the HyperTerminal:
     - Hyperterminal configuration
       - Data Length = 8 Bits
       - One Stop Bit
       - No parity
       - BaudRate = 115200 baud
       - Flow control: None

- After enumeration phase, a message will indicate that the device is ready for use and start connecting other devices through the hub.

- In case of a mouse:

   - (x,y) mouse position
   - Wheel position
   - Pressed mouse buttons

- In case of a keyboard:

   - Pressed key

- In case of a usb flash disk, the host proceed to file operations :

  - Create a "Test.txt" file.
  - Write  a small text in the created file.
  - Read the written text and check data integrity
  - Close the File


In case of failure:

- The status LED flashes infinitely
- Global variable `ExecStatus` set to EXEC_STATUS_ERROR


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the Cube Firmware package.")

This example uses the following peripheral(s):

__USB HCD__:

The USB is configured with the following settings:

- Full-speed
- Embedded PHY Interface
- SOF Disabled

__UART__:

The UART is configured with the following settings:

- The baud rate is set to 115200.
- The word length is set to 8 bits.
- Stop bits are set to 1 bit.
- Parity is set to NONE.


## __3. Hardware environment and setup__

      - Make sure the STM32 board is powered.
      - External 5v injection for Vbus.
      - Configure the BaudRate to 115200 baud on the terminal.

### __3.1. Generic Setup__

      Not Applicable.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  | Board connector   | MCU pin | Signal name     | ARDUINO Uno V3 <br> connector pin |
  | :---:             | :---:   | :---:           | :---:                             |
  | CN5-6             | PA5     | MX_STATUS_LED   | Not exposed                       |

  </details>
  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  Board connector  |  MCU pin  |  Signal name  |  ARDUINO <br/> connector pin |
  |:-----------------:|:---------:|:-------------:|:----------------------------:|
  |         -         |    PA5    |     GPIO      |              -               |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the Cube Firmware package.")


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the Cube Firmware package.")


More information about the STM32 Cube Drivers can be found in the drivers' user manual of the STM32 series you are using.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
