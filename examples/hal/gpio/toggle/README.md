<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_gpio_toggle*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure and drive a GPIO through the HAL API. \
The best illustration is to use a GPIO connected to a led.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: configures the GPIO as a digital output because we are interested only in driving the GPIO HIGH and LOW states.

__Step 2__: infinite loop toggling the GPIO state with a 100ms delay.

__End of example__: the GPIO toggles forever.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

This example demonstrates the following peripheral:

- GPIO

It requires a GPIO pin supporting the following features:

- High Speed frequency
- Output push-pull mode

The GPIO pin is configured as follows:

- Mode set to output push-pull as this is the default GPIO output setting in most cases.
- No pull-up or pull-down required as the push-pull mode is selected.
- Speed set to very high frequency to have a good slew rate.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

<!--
@startuml
@startditaa{doc/generic_hardware_setup.png}
    +-------------------------------------------------------------------------+
    |          /--------------\                                               |
    |          |STM32         |                                               |
    |          |              |                     /-----\                   |
    |          |              |                     |     |                   |
    |          |      Pin x   *---------------------* LED |                   |
    |          |              |                     |     |                   |
    |          |              |                     \-----/                   |
    |          |              |                                               |
    |          |              |                                               |
    |          \--------------/                                               |
    |                                                                         |
    | STM32_Board                                                             |
    +-------------------------------------------------------------------------+
@endditaa
@enduml
-->
![generic_hardware_setup](doc/generic_hardware_setup.png)

> **_NOTE:_**  This is a block diagram and not a schematic (resistors, transistors, exact links are not shown)
 as we focus on the role of the GPIO.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<!-- YOUR BOARDS ADDED HERE BY README GENERATION -->
<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  |  MCU pin  |  Signal name  |  User Label  |
  |:---------:|:-------------:|:------------:|
  |    PH0    |  RCC_OSC_IN   |    OSC_IN    |
  |    PH1    |  RCC_OSC_OUT  |   OSC_OUT    |
  |    PA5    |     GPIO      |      -       |

  </details>
  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label  |
  |:---------:|:-------------:|:------------:|
  |    PH0    |  RCC_OSC_IN   |    OSC_IN    |
  |    PH1    |  RCC_OSC_OUT  |   OSC_OUT    |
  |    PA5    |     GPIO      |      -       |

  </details>
  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  MCU pin  |  Signal name  |  User Label  |
  |:---------:|:-------------:|:------------:|
  |    PH0    |  RCC_OSC_IN   |  PH0_OSC_IN  |
  |    PH1    |  RCC_OSC_OUT  | PH1_OSC_OUT  |
  |    PA5    |     GPIO      |      -       |

  </details>
</details>

## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

- Make sure that the GPIO port is clocked properly.
- If you use the GPIO to blink a led, do not use a too high rate that the eye cannot detect because of persistence of vision.
- If you use a GPIO to drive an external led, consider that a resistor may be needed to protect it.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

You can find pinout, pin description, and alternate functions in the datasheet of your chosen MCU. \
You can find the hardware layout and configuration of a board in its user manual. \
The connectors are described in this document, as well as the status LEDs and the buttons.

Example of documents available for STM32U585xx MCUs mounted on the discovery kit for IoT node board:

- See section 4 of [STM32U585xx](https://www.st.com/resource/en/datasheet/stm32u585ai.pdf).
- See section 7 of [Discovery kit for IoT node with STM32U5 Series](https://www.st.com/resource/en/user_manual/um2839-discovery-kit-for-iot-node-with-stm32u5-series-stmicroelectronics.pdf).

You can also refer to these other examples to go further with GPIOs:

- hal_gpio_toggle_trustzone to see how to deal with it in the secure world
- hal_gpio_exti to see how to manage an EXTI line

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
