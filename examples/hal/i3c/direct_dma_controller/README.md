<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_i3c_direct_dma_controller*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/read/read_toc.html "An offline version is also available in the STM32Cube firmware package.")


How to handle transmit-receive transactions between two boards using the I3C bus protocol in direct mode with the HAL API, utilizing DMA mode.

The example implements the controller's code in I3C direct mode, using DMA.

**Note that the terminology Controller/Target characterizes the role taken by each device in the I3C communication, corresponding respectively to the I3C master and I3C slave in legacy terminology.**


## __1. Detailed scenario__

In this example, the MCU and the DMA share a buffer to manage the data: `RxBuffer`.
On an STM32 device with data cache enabled, it is mandatory to ensure the buffer is never cached, as this scenario does not include data cache maintenance operations.
To do so, we place the buffer in the `.non_cacheable_variables` memory section and apply the appropriate MCU settings during system initialization in `mx_system_init()`.

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MCU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: Initialization of the I3C peripheral and configuration of the transfer context and registers all user callbacks for controller events.

__Step 2__: Initiates and manages the Dynamic Address Assignment (DAA) process for the controller. The DAA process is executed only once per power-up in app_process.

__Step 3__: The controller initiates communication in direct DMA mode by alternately sending DirectWriteCCC to the target and receiving CCC in RxBuffer.

__Step 4__: Waits for one of these I3C interrupts: write transfer complete or transfer error.

__Step 5__: The controller checks that the sent and received buffers match.

The communication status is reported via the status LED and the variable ExecStatus.

__End of example__: If no error occurs, the data is transferred infinitely between the controller and the target. If the maximum number of attempts is reached, the data transfer is stopped and an error status is reported.

If you enable **`USE_TRACE`**, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: Device initialization COMPLETED.
[INFO] Step 2: DAA process and transfer context configuration COMPLETED.
[INFO] Controller - GETMWL = <data> , GETMRL = <data> , GETPID = <data> , GETBCR = <data> , GETDCR = <data> , GETSTATUS = <data>.
[INFO] Step 3: Data transfer and reception completed.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

This example demonstrates the following peripherals.


__I3C__: is configured as indicated below:

- The target address is set to 0x32U. It can be configured by changing the value of the DEVICE_TARGET_ADDR variable.

- The bus usage, including the I3C bus and its duty cycle timings, is calculated by STM32CubeMX2 in accordance with the I3C initialization section of the reference manual.
  - The bus is configured as an `I3C pure bus`, dedicated to private I3C-to-I3C communication between controller and target devices.

- The I3C bus is configured to run at the maximum supported speed to demonstrate its highest performance.
  See `__I3C maximum speed__` in section [3.2 Specific board setups](#32-specific-board-setups).
- The event and error interrupts of the I3C instance are configured and enabled in the NVIC.

__DMA__: is used to manage data transfers.

Two DMA channels (I3C Tx and I3C Rx) are enabled and configured:

- The DMA transmit channel is configured in memory-to-peripheral mode, with an incrementing source address and a fixed destination address.
  After each byte transfer, the DMA reads the next byte from SRAM and writes it to the I3C transmit data register.
- The DMA receive channel is configured in peripheral-to-memory mode, with a fixed source address and an incrementing destination address.
  The DMA reads bytes from the I3C receive data register and stores them sequentially in SRAM.

For each DMA channel (I3C Tx and I3C Rx), the corresponding NVIC interrupt line is configured and enabled.

To test this example with the target, you can use the corresponding *hal_i3c_direct_dma_target* example pack.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

- The controller board is connected to the target board through the two I3C lines and a common GND.

<!--
@startuml
@startditaa{doc/example_hal_i3c_direct_dma_controller-setup.png} -E -S
    /-------------------------\                     /-------------------------\
    |    /--------------------+                     +--------------\          |
    |    |STM32 I3Ci          |                     |  STM32 I3Ci  |          |
    |    |                    |                     |              |          |
    |    |                    |                     |              |          |
    |    |                    |                     |              |          |
    |    |                    |                     |              |          |
    |    |                    |                     |              |          |
    |    |                    |                     |              |          |
    |    |                    |                     |              |          |
    |    |I3Ci_SCL------------+---------------------+ I3Ci_SCL     |          |
    |    |                    |                     |              |          |
    |    |                    |                     |              |          |
    |    |                    |                     |              |          |
    |    |I3Ci_SDA------------+---------------------+ I3Ci_SDA     |          |
    |    |               c4BE |                     |       c4BE   |          |
    |    \--------------------+                     +--------------/          |
    |                         |                     |                         |
    |                     GND +---------------------+ GND                     |
    |                         |                     |                         |
    |     STM32 MCU on        |                     |     STM32 MCU on        |
    |     Controller board    |                     |     target board        |
    \-------------------------/                     \-------------------------/

@endditaa
@endumldd
-->


![example_hal_i3c_direct_dma_controller-setup](doc/example_hal_i3c_direct_dma_controller-setup.png)

### __3.2. Specific board setups__

The I3C serial clock (SCL) and data (SDA) lines can be observed by connecting an oscilloscope or a logic analyzer to the corresponding board connectors.

This section describes the exact hardware configurations of your project.

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>I3C maximum speed</summary>

  The maximum speed configured for these series is 12,5MHz.

  </details>

  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PA2    |   USART2_TX   |      PA2      |
  |    PB6    |   I3C1_SCL    |      PB6      |
  |    PB7    |   I3C1_SDA    |      PB7      |

  </details>

  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PA2    |   USART2_TX   |      PA2      |
  |    PB6    |   I3C1_SCL    |      PB6      |
  |    PB7    |   I3C1_SDA    |      PB7      |

  </details>

  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |  PH0_OSC_IN   |
  |    PH1    |  RCC_OSC_OUT  |  PH1_OSC_OUT  |
  |    PA2    |   USART2_TX   | DBGIN_VCP_TX  |
  |    PB6    |   I3C1_SCL    |      PB6      |
  |    PB7    |   I3C1_SDA    |      PB7      |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

  1. __Dynamic Address Assignment (DAA)__: DAA is started by the controller using the `ENTDAA` command, one of the I3C Common Command Codes (CCC), standard broadcast commands used to manage devices on the bus. The controller enumerates each target, which responds with its identification payload (PID, BCR, DCR). The controller then assigns a unique dynamic address to each target, which is used for all further direct transfers.
  In this example, DAA completion and address verification are handled via interrupts (see Step 2).

  2. If there are no I3C signals observed, remember to check these points first:
     - The GND pins of the controller and target boards are connected.
     - Use the shortest possible wires between the boards to improve signal integrity.

  3. Initialization order recommendation: initialize the controller I3C peripheral first, then initialize the target I3C peripheral to avoid errors during the first DAA.
      - Warning message (Target and Controller side): "I3C target must be initialized after I3C controller to avoid error during the first DAA".
      - Practical tip when using two boards: ensure the target firmware is flashed and ready, then reset/restart the controller so its I3C initialization runs first.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

- You can find the application note AN5879 related to the I3C MANUAL on the [AN5879](https://www.st.com/resource/en/application_note/an5879-introduction-to-i3c-for-stm32-mcus-stmicroelectronics.pdf) website if you want to go further on some technical details of the I3C bus

- You can refer to the *hal_i3c_direct_dma_target* example pack to have a look at the target's board application.

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
