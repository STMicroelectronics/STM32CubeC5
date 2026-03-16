<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_i2c_two_boards_com_dma_controller*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to handle an infinite number of transmit-receive transactions between two boards based on the I2C bus protocol with the HAL API, using DMA.

The example implements the controller's code.

**Note that the terminology controller/responder characterizes the role taken by each device in the I2C communication, also known as I2C master and slave in legacy terminology.**


## __1. Detailed scenario__

In this example, the CPU and a DMA share a buffer to manage the data: `RxBuffer`.
On an STM32 device with data cache enabled, it is mandatory to ensure that the buffer is never cached, as this scenario does not include data cache maintenance operations.
To do so, we place the buffer in the `.non_cacheable_variables` memory section and apply the appropriate MPU settings during system initialization in `mx_system_init()`.


__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: Configures and initializes the I2C instance, the NVIC, and the DMA channels.

  - Links the transmit and receive DMA handles to the I2C handle.
  - Registers the user callbacks for I2C events: TX/RX transfer completed and transfer error.

__Step 2__: The controller starts the communication, in DMA mode, by sending a message as a null-terminated string to the responder. A counter of attempts is reset when initiating the communication loop.

__Step 3__: Waits for one of these I2C interrupts: write transfer complete or transfer error.

__Step 4__: The controller expects to receive the message back in DMA mode.

__Step 5__: Waits for one of these I2C interrupts: read transfer complete or transfer error.

__Step 6__: The controller checks that the sent and received buffers match.
              Returns to __Step2__ indefinitely if no error occurs.

If the data transmit or receive operation fails or the exchanged buffers are different, the controller restarts the communication by sending again the same message. The error_handler() function is called when the maximum number of attempts is reached.

The communication status is reported via the status LED and the variable ExecStatus.

__End of example__: If no error occurs, the data is transferred infinitely between the controller and the responder. If the maximum number of attempts is reached, the data transfer is stopped and an error status is reported.

If you enable **`USE_TRACE`**, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: Device initialization COMPLETED.
[INFO] Controller - Tx/Rx Buffers IDENTICAL. Transfer COMPLETED of I2C Two Boards Communication - Message A
[INFO] Controller - Tx/Rx Buffers IDENTICAL. Transfer COMPLETED of I2C Two Boards Communication - Message B
[INFO] Controller - Tx/Rx Buffers IDENTICAL. Transfer COMPLETED of I2C Two Boards Communication - Message A
[INFO] Controller - Tx/Rx Buffers IDENTICAL. Transfer COMPLETED of I2C Two Boards Communication - Message B
```


The following **message sequence chart** describes the I2C communication between the controller board and the responder board.

![i2C-two-boards-com-dma_NominalCase](doc/i2C-two-boards-com-dma_NominalCase.svg)

<details>
<summary> Expand this tab to visualize the sequence chart diagram in case of a data transmission error. </summary>

![i2C-two-boards-com-dma_RetryMechanism-TxError](doc/i2C-two-boards-com-dma_RetryMechanism-TxError.svg)

</details>


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

This example demonstrates the following peripherals.

__I2C__: is configured as indicated below:

- The 7-bit addressing mode is selected:
  - The device own address is set to 0x5DU. Note that it is only used if the peripheral switches to the I2C responder mode.
  - The responder address is set to 0x3FU. It can be configured by changing the value of the I2C_RESPONDER_ADDR_7BIT variable.

> **_NOTE:_** The SADD[7:1] field of the I2C_CR2 register is written with the 7-bit responder address to be sent.
> SADD[0] is ignored, reserved for the Read/Write bit. So that the 7-bit responder address must be shifted to the left before calling I2C HAL APIs.

- The I2C IP is configured to run at the maximum supported speed to demonstrate its highest performance.
  See `__I2C maximum speed__` in section [3.2 Specific board setups](#32-specific-board-setups).
- STM32CubeMX2 computes the I2C-bus timings according to the I2C initialization section in the reference manual.
- The event and error interrupts of the I2C instance are configured and enabled in the NVIC.
- The selected GPIO pins support the I2C alternate function. They are configured in open drain mode with internal pull-up activation.

> **_NOTE:_** In this use-case, internal pull-up resistors are used to set up the system easily and demonstrate the HAL API. Note that for a real I2C application, external pull-up resistors must be used to reach better performances.

__DMA__: is used to manage data transfers.

Two DMA channels I2C Tx and I2C Rx are enabled and configured, respectively, as indicated below:
  - The DMA transmit channel is configured in memory to peripheral mode with an incremented source address and a fixed destination address.
    After each byte transfer, the DMA automatically increments the source address to copy the next byte from an SRAM area to the I2C transmit data register.
  - The DMA receive channel is configured in peripheral to memory mode with a fixed source address and an incremented destination address.
    The data is loaded from the I2C receive data register to an SRAM area incrementally.

For each DMA channel (I2C Tx and Rx), the corresponding NVIC line is configured and enabled.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

The controller board is connected to the responder board through the two I2C lines and a common GND.

<!--
@startuml
@startditaa{doc/example_hal_i2c_two_boards_com_dma_controller-setup.png} -E -S
    /-------------------------\                     /-------------------------\
    |    /--------------------+                     +--------------\          |
    |    |STM32 I2Ci          |                     |  STM32 I2Ci  |          |
    |    |                    |                     |              |          |
    |    |      VDD _________ |                     |              |          |
    |    |           |    |   |                     |              |          |
    |    |          +++  +++  |                     |              |          |
    |    |         R| | R| |  |                     |              |          |
    |    |          +++  +++  |                     |              |          |
    |    |           |    |   |                     |              |          |
    |    |I2Ci_SCL---+----*---+---------------------+ I2Ci_SCL     |          |
    |    |           |        |                     |              |          |
    |    |           |   c4BE |                     |              |          |
    |    |           |        |                     |              |          |
    |    |I2Ci_SDA---*--------+---------------------+ I2Ci_SDA     |          |
    |    |               c4BE |                     |       c4BE   |          |
    |    \--------------------+                     +--------------/          |
    |                         |                     |                         |
    |                     GND +---------------------+ GND                     |
    |                         |                     |                         |
    |     STM32 MCU on        |                     |     STM32 MCU on        |
    |     Controller board    |                     |     Responder board     |
    \-------------------------/                     \-------------------------/

    /------------------------------\
    | VDD:  Power supply           |
    | R: Internal pull up resistor |
    \-=----------------------------+

@endditaa
@endumldd
-->

![example_hal_i2c_two_boards_com_dma_controller-setup](doc/example_hal_i2c_two_boards_com_dma_controller-setup.png)

### __3.2. Specific board setups__

The I2C serial clock (SCL) and data (SDA) lines can be observed by connecting an oscilloscope or a logic analyzer to the corresponding board connectors.

This section describes the exact hardware configurations of your project.

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>I2C maximum speed</summary>

  The maximum speed configured for these series is 1 MHz.

  </details>
  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PA2    |   USART2_TX   |      PA2      |
  |    PB6    |   I2C1_SCL    |      PB6      |
  |    PB7    |   I2C1_SDA    |      PB7      |

  </details>
  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PA2    |   USART2_TX   |      PA2      |
  |    PB6    |   I2C1_SCL    |      PB6      |
  |    PB7    |   I2C1_SDA    |      PB7      |

  </details>
  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |  PH0_OSC_IN   |
  |    PH1    |  RCC_OSC_OUT  |  PH1_OSC_OUT  |
  |    PA2    |   USART2_TX   | DBGIN_VCP_TX  |
  |    PB6    |   I2C1_SCL    |      PB6      |
  |    PB7    |   I2C1_SDA    |      PB7      |

  </details>
</details>

## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

  __Communication checking__: The example ensures that the controller sends the exact number of bytes expected by the responder.

  __No signal__: If there are no I2C signals observed, remember to check these points first:
     - The GND pins of the controller and responder boards are connected.
     - The internal pull-up resistors are activated for the selected I2C pins. This configuration is enabled by default.

  __Incorrect signals__: If the I2C signals observed do not comply with the I2C specification, especially at high frequencies, you can try the following tips:
     - Use the oscilloscope instead of the logic analyzer for a better measuring and viewing analog characteristics of the signals SCL and SDA. Check that the grounds of the instrument and the board are well wired.
     - Replace the internal pull-up resistors with external ones. The selected values of the resistors should be compliant with the I2C specification.

  __DMA data__: During the I2C transmission or reception operations, only the data are transferred with DMA. The transmitted responder address cannot be transferred with DMA.

  __DMA scope__: DMA requests are generated only for the data transfer. I2C interrupts manage all remaining events such as the 'transmit/receive end of transfer' or the transfer error.

  __Data alignment__: Take care of data misalignment. Depending on the DMA data width used, source and destination addresses must respect data alignment. For details, refer to the reference manual of your MCU.

  __Cache coherency__: Take care of cache coherency issue. When cache memory is enabled, it is generally not in the path of DMA transfer, thus a cache coherency issue might appear. It might be necessary to tackle cache coherency. See H7 FAQ:
  [DMA-is-not-working-on-STM32H7-devices](https://community.st.com/s/article/FAQ-DMA-is-not-working-on-STM32H7-devices).

  __DMA ports__: Depending on the STM32 series, and DMA instance used (GPDMA/HPDMA/LPDMA) specific DMA ports constraints must be respected.
  For details, refer to the reference manual of your MCU. You can also see the application note in the `__5. See Also` section.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

- Find the application note AN10216-01 related to the I2C on the [i2c-bus.org](https://www.i2c-bus.org/specification/) website. This document goes further on some technical details of the I2C bus (such as external pull-up resistors calculation for example).

- Refer to the *hal_i2c_two_boards_com_dma_responder* example pack to have a look at the responder's board application.

- See the application note [AN5593](https://www.st.com/resource/en/application_note/an5593-how-to-use-the-gpdma-for-stm32-mcus-stmicroelectronics.pdf) to get further explanation about DMA port allocation.

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
