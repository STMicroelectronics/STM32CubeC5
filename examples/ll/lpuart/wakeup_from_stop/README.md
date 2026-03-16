<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *ll_lpuart_wakeup_from_stop*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")


How to use the LPUART to perform an infinite communication in low power mode in responder side using LL APIs.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: Initializes the LPUART instance.

__Step 2__: the responder expects to receive a message as a null-terminated string from the controller board, in interrupt mode. A counter of attempts is reset when initiating the communication loop.

__Step 3__: the MCU will enter low power mode and one or several LPUART interrupts will be able to wake it up.

__Step 4__: the responder sends back the received message in interrupt mode. LPUART communication wakes the MCU up, and the clock is re-configured.

__Step 5__: the core will enter low power mode and one or several LPUART interrupts will be able to wake it up.
            Returns to step 2 indefinitely if no error occurs.


The communication status is reported via the status LED and the variable ExecStatus.

__End of example__: if no error occurs, the data is transferred infinitely between the controller and the responder. If the maximum number of attempts is reached, the data transfer is stopped by reporting an error status.

MCU turns on this LED in RUN mode and turns it off in low power mode.

<details>
<summary> Expand this tab to visualize the sequence chart diagram in case of a data transmission error. </summary>

![uart-wakeup_from_stop_AttemptsMechanism](doc/uart-wakeup_from_stop_AttemptsMechanism.svg)

</details>


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

The example demonstrates the following peripheral:

__UART__:

We select an LPUART with accessible Tx and Rx signals on the board so that we can wire it to the controller board.

The LPUART is configured with the following settings:

- The baud rate is set to 115200.
- The word length is set to 8 bits.
- Stop bits are set to 1 bit.
- Parity is set to NONE.

<!--
@startuml
@startditaa{doc/ASCII_data_frame.png} -E -S

    The UART data frame of the current configuration:

      /--------------------------------------\
      |  /------+-----------------+-------\  |
      |  |  SB  |   8 bits data   |  STB  |  |
      |  \------+-----------------+-------/  |
      \--------------------------------------/

      /---------------\
      | SB:  Start Bit|
      | STB: Stop Bit |
      \=--------------/
@endditaa
@enduml
-->


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This section describes the hardware setup principles that apply to any board.

<!--
@startuml
@startditaa{doc/ASCII_uart_two_boards.png} -E -S
    /-------------------------\                     /-------------------------\
    |          /--------------+                     +--------------\          |
    |          | STM32 USARTi |                     | STM32 USARTi |          |
    |          |              |                     |              |          |
    |          |   LPUARTi_TX *---------------------* LPUARTi_RX   |          |
    |          |              |                     |              |          |
    |          |              |                     |              |          |
    |          |              |                     |              |          |
    |          |   LPUARTi_RX *---------------------* LPUARTi_TX   |          |
    |          |              |                     |              |          |
    |          \--------------+                     +--------------/          |
    |                         |                     |                         |
    |                     GND *---------------------* GND                     |
    |                         |                     |                         |
    |  /------------------\   |                     |  /-----------------\    |
    |  | STM32 Responder  |   |                     |  | STM32 Controller|    |
    |  | Board            |   |                     |  | Board           |    |
    |  \------------------/   |                     |  \-----------------/    |
    \-------------------------/                     \-------------------------/
@endditaa
@enduml
-->

![ASCII_uart_two_boards](doc/ASCII_uart_two_boards.png)

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PB7    |  LPUART1_RX   |      PB7      |
  |    PB6    |  LPUART1_TX   |      PB6      |
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PB0    |     GPIO      |       -       |

  **_NOTE:_**
    - LPUART1 is the UART instance used for the communication between the Nucleo boards.
    - LPUART1 is clocked by HSIK
    - UART transmission interrupts (TX) cannot wake up the MCU from STOP mode. Only UART reception (RX) can do it.

  </details>

  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PB7    |  LPUART1_RX   |      PB7      |
  |    PB6    |  LPUART1_TX   |      PB6      |
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PB0    |     GPIO      |       -       |

  **_NOTE:_**
    - LPUART1 is the UART instance used for the communication between the Nucleo boards.
    - LPUART1 is clocked by HSIK
    - UART transmission interrupts (TX) cannot wake up the MCU from STOP mode. Only UART reception (RX) can do it.

  </details>

  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PH0    |  RCC_OSC_IN   |  PH0_OSC_IN   |
  |    PH1    |  RCC_OSC_OUT  |  PH1_OSC_OUT  |
  |    PB7    |  LPUART1_RX   |      PB7      |
  |    PB6    |  LPUART1_TX   |      PB6      |
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PB0    |     GPIO      |       -       |

  **_NOTE:_**
    - LPUART1 is the UART instance used for the communication between the Nucleo boards.
    - LPUART1 is clocked by HSIK
    - UART transmission interrupts (TX) cannot wake up the MCU from STOP mode. Only UART reception (RX) can do it.

  </details>
</details>

### __3.3. Testing the Example__

This example can be tested:

__3.3.1. Using Another STM32 Board as Controller
You can use one of the following examples to act as the controller that sends messages to wake up the responder:

hal_uart_two_boards_com_polling_controller: The controller side in a polling mode UART communication.
hal_uart_two_boards_com_it_controller: The controller side in an interrupt mode UART communication.
hal_uart_two_boards_com_dma_controller: The controller side in a DMA mode UART communication.
These examples will send UART messages to the responder, which will wake up from STOP mode upon receiving these messages.


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Find below the points of attention for this specific example.

__Communication Buffers__: Make sure that the size, in bytes, of the responder's reception buffer is equal to the size of the controller's transmission buffer.
__System Clock__: When exiting from Stop mode, the system clock must be configured (see the RCC peripheral section in the reference manual of your MCU).

__Error handling__: In LL examples, error handling is controlled by the USE_LL_APP_ERROR constant in the application files to reduce code footprint. This compilation flag is disabled by default. If the example does not behave as expected, enable error handling for debugging by setting USE_LL_APP_ERROR to 1 in ll_example.h.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

You can also refer to these examples to go further with the UART peripheral:

- ll_uart_echo_it: echo example using UART interrupts.
- ll_uart_echo_dma: echo example using UART and DMA transfers.

More information about the STM32Cube Drivers can be found in the drivers' user manual of the STM32 series you are using.

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
