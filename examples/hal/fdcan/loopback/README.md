<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_fdcan_loopback*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to handle transmit and receive transactions based on the FDCAN-bus protocol with the HAL API, with a single board in loopback and polling mode.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: configures and initializes the FDCAN instance.

__Step 2__: sends several FDCAN messages.

__Step 3__: receives back and check the FDCAN messages (ID, DLC, and data fields).

__End of example__: The example ends after step 4. You can verify that it runs properly via the status LED and the `ExecStatus` variable.

If you enable **`USE_TRACE`**, you can follow these steps, in the nominal case of execution, in the terminal logs:
```text
[INFO] Step 1: Device initialization COMPLETED.
[INFO] Step 2: Messages transmitted.
[INFO] Step 3: Received messages are correct.
[INFO] Tx/Rx Buffers IDENTICAL.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

This example demonstrates the **FDCAN peripheral** configured as indicated below:

- The FDCAN configuration is set to external loopback mode.
- The FDCAN IP is configured to run at a 500kbds speed for nominal phase (sampling point at 75%) and 2Mbds for data phase (sampling point at 80%).
- The FDCAN-bus timings are directly calculated by STM32CubeMX2 by referring to the FDCAN initialization section in the reference manual.
- The selected GPIO pins support the FDCAN alternate function. They are configured in push pull mode.
- The FDCAN IP is configured to use only two filters in classical mode: standard ID and extended ID to match only one ID.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

The FDCAN bus works with two dedicated lines (CAN_H and CAN_L) at specific voltage level.
SO a CAN/FDCAN transceiver is needed to interface FDCAN_TX and FDCAN_RX pins to a real FDCAN bus.

This example can work without a transceiver.

This section describes the hardware setup principles that apply to any board.

<!--
@startditaa{doc/example_hal_fdcan_loopback-setup.png} -E -S
    /----------------------------\
    |                            |
    |          /-----------------+
    |          | STM32 FDCAN     |
    |          |                 |
    |          |  FDCAN_TX --+---*
    |          |             |   |
    |          |  FDCAN_RX --+   |
    |          |   c4BE          |
    |          \-----------------+
    |                            |
    |         STM32 MCU          |
    \----------------------------/
@endditaa
-->

![example_hal_fdcan_loopback-setup](doc/example_hal_fdcan_loopback-setup.png)

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

The FDCAN signal can be observed by connecting an oscilloscope or a logic analyzer to the corresponding board connectors.

On several ST boards, an FDCAN transceiver is already present.

<!-- YOUR BOARDS ADDED HERE BY README GENERATION -->
<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PA2    |   USART2_TX   |      PA2      |
  |    PB9    |   FDCAN1_TX   |      PB9      |
  |    PB8    |   FDCAN1_RX   |      PB8      |

  </details>

  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PA2    |   USART2_TX   |      PA2      |
  |    PB9    |   FDCAN1_TX   |      PB9      |
  |    PB8    |   FDCAN1_RX   |      PB8      |

  </details>

  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |  PH0_OSC_IN   |
  |    PH1    |  RCC_OSC_OUT  |  PH1_OSC_OUT  |
  |    PA2    |   USART2_TX   | DBGIN_VCP_TX  |
  |    PB9    |   FDCAN1_TX   |      PB9      |
  |    PB8    |   FDCAN1_RX   |      PB8      |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

The filters configuration must be done before starting the FDCAN peripheral.

As FDCAN frame length can be variable, you must prepare an Rx buffer with the maximum expected size (64 for FDCAN - 8 for CAN). You can get the size of any received frame thanks to its hal_fdcan_rx_fifo_header_t element.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

This [application note](https://www.st.com/resource/en/application_note/an5348-introduction-to-fdcan-peripherals-for-stm32-product-classes-stmicroelectronics.pdf)
explains in details CAN IP features and its applications.

This [application note](https://www.st.com/resource/en/product_training/STM32G4-Peripheral-Flexible_Datarate_Controller_Area_Network_FDCAN.pdf)
shows FDCAN additional features towards classical CAN protocol.

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
