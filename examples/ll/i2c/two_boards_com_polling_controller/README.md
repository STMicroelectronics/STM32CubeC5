<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *ll_i2c_two_boards_com_polling_controller*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://confluence.st.com/display/E01/Cube+Firmware+Examples+Documentation "An offline version is also available in the STM32Cube firmware package.")

How to handle an infinite number of transmit-receive transactions in polling mode between two boards based on the I2C-bus protocol with the LL API.

The example implements the controller's code.

**Note that the terminology Controller/Responder characterizes the role taken by each device in the I2C communication, also known as I2C master and slave in legacy terminology.**


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: Initializes the I2C instance.

__Step 2__: The controller starts the communication by sending a message in blocking mode to the responder. The message is as a null-terminated string and is sent within a specific timeout period.

__Step 3__: The controller expects to receive the message back within the specified timeout period, in blocking mode.

__Step 4__: The controller checks that the sent and received buffers match.
              Returns to __Step2__ indefinitely if no error occurs.

> **_NOTE:_** when an error occurs in the reception or transmission process, the controller restarts the communication by sending again the same message. The error_handler() function is called when the maximum number of attempts is reached and an error status is reported.

The communication status is reported via the status LED and the variable ExecStatus.

__End of example__: If no error occurs, the data is transferred infinitely between the controller and the responder.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

__I2C__: is configured as indicated below:

- The 7-bit addressing mode is selected:
  - The device own address is set to 0x5DU. Note that it is only used if the peripheral switches to the I2C responder mode.
  - The responder address is set to 0x3FU. It can be configured by changing the value of the I2C_RESPONDER_ADDR_7BIT define.

> **_NOTE:_** The SADD[7:1] field of the I2C_CR2 register is written with the 7-bit responder address to be sent.
> SADD[0] is ignored, reserved for the Read/Write bit. So that the 7-bit responder address must be shifted to the left before calling I2C LL APIs.

- The I2C-bus timings are calculated by STM32CubeMX2 in line with the I2C initialization section in the reference manual.
- The selected GPIO pins support the I2C alternate function. They are configured in open drain mode with (internal or external) pull-up.

> **_NOTE:_** Internal pull-up resistors must be configured if the board does not embed external pull-up resistors connected to I2C SCL and SDA lines.
> Note that for a real I2C application, external pull-up resistors must be used to reach better performances.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

The controller board is connected to the responder board through the two I2C lines and a common GND.

<!--
```text
@startuml
@startditaa{doc/example_ll_i2c_two_boards_com_polling_controller-setup.png} -E -S
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

    Note that this setup applies to boards requiring internal pull up resistors on I2C lines.
    For other boards, external pull up resistors are already connected to I2C lines.

@endditaa
@endumldd
 ```
-->

![example_ll_i2c_two_boards_com_polling_controller-setup](doc/example_ll_i2c_two_boards_com_polling_controller-setup.png)

### __3.2. Specific board setups__

The I2C serial clock (SCL) and data (SDA) lines can be observed by connecting an oscilloscope or a logic analyzer to the corresponding board connectors.

This section describes the exact hardware configurations of your project.


<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PB6    |   I2C1_SCL    |      PB6      |
  |    PB7    |   I2C1_SDA    |      PB7      |
  |    PA5    |     GPIO      | MX_STATUS_LED |

  </details>

  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PB6    |   I2C1_SCL    |      PB6      |
  |    PB7    |   I2C1_SDA    |      PB7      |
  |    PA5    |     GPIO      | MX_STATUS_LED |

  </details>

  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PH0    |  RCC_OSC_IN   |  PH0_OSC_IN   |
  |    PH1    |  RCC_OSC_OUT  |  PH1_OSC_OUT  |
  |    PB6    |   I2C1_SCL    |      PB6      |
  |    PB7    |   I2C1_SDA    |      PB7      |
  |    PA5    |     GPIO      | MX_STATUS_LED |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

  1. The example needs to ensure that the controller sends the exact number of bytes expected by the responder.

  2. If there are no I2C signals observed, remember to check these points first:
     - The GND pins of the controller and responder boards are connected.
     - pull-up resistors (internal or external) are applied for the selected I2C pins.

  3. If the I2C signals observed do not comply with the I2C specification, especially at high frequencies, you should try the following tips:
     - Use the oscilloscope instead of the logic analyzer for a better measuring and viewing analog characteristics of the signals SCL and SDA. Check that the grounds of the instrument and the board are well wired.
     - Replace the internal pull-up resistors with external ones. The selected values of the resistors should be compliant with the I2C specification.

  4. Note that the selected I2C timeout period of 3 seconds ensures a stable communication between the two boards and a reliable synchronization in different cases of controller or responder reset. Be sure to adjust its value carefully when needed.

__Error handling__: In LL examples, error handling is controlled by the USE_LL_APP_ERROR constant in the application files to reduce code footprint. This compilation flag is disabled by default. If the example does not behave as expected, enable error handling for debugging by setting USE_LL_APP_ERROR to 1 in ll_example.h.

__Timeout management__: Polling flag instructions can cause the example to enter an infinite loop. To prevent this, a timeout mechanism is implemented. When the timeout is reached, the program exits the loop and reports the error at the application level. This mechanism is controlled by the USE_LL_APP_TIMEOUT compilation flag, which is disabled by default to reduce code footprint. If the example execution appears to be stuck in an infinite loop, enable this mechanism for debugging by setting USE_LL_APP_TIMEOUT to 1 in ll_example.h.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

- You can find the application note AN10216-01 related to the I2C MANUAL on the [i2c-bus.org](https://www.i2c-bus.org/specification/) website if you want to go further on some technical details of the I2C bus (such as external pull-up resistors calculation for example).

- You can refer to the *ll_i2c_two_boards_com_polling_responder* example pack to have a look at the responder's board application.

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
