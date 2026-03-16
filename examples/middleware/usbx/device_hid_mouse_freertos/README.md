<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *usbx_device_hid_mouse_freertos*__

**Example version:** 2.0.0

How to configure a USB Human Interface Device "HID" mouse in RTOS mode based on USBX stack.

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the Cube Firmware package.")


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

- __Step 1__: USB PCD initialization

- __Step 2__: Initialization of USBX stack.

- __Step 3__: Initialization of USBX HID Device Mouse Class.

__End of example__: After Step 3, the example is completed and the HID mouse class is ready to interact with the host.

In case of successful operations:

- LED remains turned on
- Global variable `ExecStatus` set to EXEC_STATUS_INIT_OK

You can verify the USBX stack operates as expected:

- Plug the device running the HID mouse example into a USB port on the PC.
- Press the user button on the device to initiate mouse movement and observe the mouse cursor on the PC screen as it moves in rectangular pattern.

In case of failure:

- LED toggle infinitely
- Global variable `ExecStatus` set to EXEC_STATUS_ERROR


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the Cube Firmware package.")

This example uses the following peripheral:

__USB PCD FS__: is configured as indicated below:

  - Full-speed operation
  - Embedded PHY Interface
  - SOF Disabled


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

- Make sure the STM32 board is powered
- Plug the STM32 device into a host PC via the USB connector.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  |  Board connector  |  MCU pin  |  Signal name  |  ARDUINO <br/> connector pin  |
  |:-----------------:|:---------:|:-------------:|:----------------------------: |
  |         -         |    PA5    | MX_STATUS_LED |              -                |

  </details>
  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  | Board connector   | MCU pin | Signal name     | ARDUINO Uno V3 <br/> connector pin|
  | :---:             | :---:   | :---:           | :---:                             |
  | CN7-11            | PA5     | MX_STATUS_LED   | Not exposed                       |

  </details>
  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  Board connector  |  MCU pin  |  Signal name  |  ARDUINO <br/> connector pin  |
  |:-----------------:|:---------:|:-------------:|:----------------------------: |
  |         -         |    PA5    |MX_STATUS_LED  |              -                |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the Cube Firmware package.")

Here are the points of attention for this specific example:

  1. If there is an issue, remember to check that:

    - The STM32 is powered.
    - The STM32 USB connector is connected to a USB port in the host.
  2. Depending on STM32 series, and USB instance used (FS/HS) specific USB configurations must be respected (Endpoint Max Packet Size, Clock)

For details, refer to the reference manual of your MCU.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the Cube Firmware package.")

More information on USB and STM32: [Introduction to USB with STM32](https://wiki.st.com/stm32mcu/wiki/Introduction_to_USB_with_STM32).

More information about the STM32 Cube Drivers can be found in the drivers' user manual of the STM32 series you are using.

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
