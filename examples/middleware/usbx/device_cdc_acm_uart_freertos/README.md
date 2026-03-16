<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *usbx_device_cdc_acm_uart_freertos*__

**Example version:** 2.0.0

How to configure a USB communication device class "CDC_ACM" in RTOS mode based on the USBX stack.

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the Cube Firmware package.")


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

- __Step 1__: USB PCD initialization.

- __Step 2__: Initialization of USBX stack.

- __Step 3__: Initialization of USBX CDC_ACM device class.

__End of example__: After step 3, the example is completed and the CDC device is ready to interact with the host.

In case of successful operations:

- The LED remains turned on
- Global variable `ExecStatus` set to EXEC_STATUS_INIT_OK

You can verify that the USBX stack operates as expected:

- Plug the device running the CDC ACM example into a USB port on the PC.
- Open two hyperterminals (USB com port and UART com port(USB STLINK VCP)) to send/receive data to/from host from/to device.

In case of failure:

- The LED flashes infinitely.
- The global variable `ExecStatus` is set to EXEC_STATUS_ERROR.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the Cube Firmware package.")

This example uses the following peripherals:

__USB PCD__:

   __USB PCD OTG__:

   __OTG__:


   The USB is configured with the following settings:

   - Full-speed for B-U585I-IOT02A and High-speed for NUCLEO-V873XJ
   - Embedded PHY Interface
   - SOF Disabled

   __DRD__:

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

   __DMA__:

The UART DMA mode is configured with the following settings:

- The DMA is configured in memory to peripheral mode.
- The DMA is configured with source increment and destination fixed.
- The DMA transfer width is set to byte.

__MPU__: is used to ensure that the buffer used for DMA transfer is not cached.
The linker script defines a specific memory section `.non_cacheable_variables` for this buffer.
This memory section is 2 KB in size and is aligned as per the MPU requirements.
The MPU configuration ensures that this 2 KB region is not cached.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

      - Make sure the STM32 board is powered.
      - Plug the STM32 device into a host PC via the USB connector.
      - Check the list of the COM ports to find out the COM port number.
      - Configure the BaudRate to 115200 baud on the terminal.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.



<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  | Board pin  | MCU pin | Signal name      | ARDUINO <br/> connector pin |
  | :---:      | :---:   | :---:            | :---:       |
  | CN5-6      | PA5     | ARDUINO LED LD1  | D13         |

  </details>
  </details>

## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the Cube Firmware package.")

Here are the points of attention for this specific example:

__Terminal display__: If there is an issue, remember to check the serial terminal configuration. Make sure that:

- The COM port number is correct.
- The baud rate is set to 115200.

__USB configuration__:  Depending on the STM32 series, and USB instance used (FS/HS) specific USB configurations must be respected (endpoint max packet size, clock). For details, refer to the reference manual of your MCU.

__Data misalignment__: Depending on the DMA data width used, source and destination addresses must respect data alignment. Refer to the Reference Manual of your MCU.

__Cache coherency__: When cache memory is enabled, it is generally not in the path of DMA transfer, thus cache coherency issues might appear.
See H7 FAQ to tackle cache coherency issues: [DMA-is-not-working-on-STM32H7-devices](https://community.st.com/s/article/FAQ-DMA-is-not-working-on-STM32H7-devices)

__DMA ports selection__: Depending on the STM32 series, and DMA instance used (GPDMA/HPDMA/LPDMA) specific DMA ports constraints must be respected.  For details, refer to the reference manual of your MCU. You can also see the application note in the `__5. See Also` section.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the Cube Firmware package.")

This [wiki](https://wiki.st.com/stm32mcu/wiki/Introduction_to_USBX) introduction to USBX.

More information about the STM32Cube Drivers can be found in the drivers' user manual of the STM32 series you are using.

For instance, for the STM32U5 series: [User Manual](https://www.st.com/resource/en/user_manual/dm00813340-.pdf).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
