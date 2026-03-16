<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *ll_gpio_exti*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure a GPIO in EXTI line interrupt mode with the LL API.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

In the app_init() function, mx_extix_init() function is called to configure the EXTI Line.

The application executes the following __example steps__:

__Step 1__: configures the GPIO and the corresponding EXTI line.
    - sets the chosen GPIO in the correct input mode.
    - sets the corresponding EXTI line to the given GPIO port and requested trigger edge.

__Step 2__: starts the EXTI line and waits for the interrupt.

__Step 3__: detects an EXTI interrupt.

__Step 4__: stops the EXTI Line, deinitializes the GPIO input, and terminates.

__End of example__:
The example is a one-shot application. It ends once the EXTI line is triggered.
You can verify that the example runs properly via the status LED and the `ExecStatus` variable.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

This example demonstrates the following peripherals:

__GPIO and EXTI__:

It requires a GPIO input:

- With a pin connected to an EXTI line,
- With a way to toggle the GPIO pin voltage from 0V to MCU power supply with a button, jumper, or transistor switch.


The GPIO input pin is configured as follows:

- Mode set at external interrupt mode with rising edge trigger detection.
- Internal pull-up resistors deactivated.
- Associated IRQ activated with a high priority.

> **_NOTE:_** for a given multiplexed EXTI line, only one of the connected GPIO pins of that EXTI multiplexer is active at a time.
In case of multiplexing, GPIO pins of the same order are grouped together and connected to an EXTI line: PA0, PB0 ... connected to the same EXTI line.
This means that you can configure an entire GPIO port as the source for EXTI lines.
> **__NOTE:__** be careful, some lines can share the same interrupt handler. On some platforms, the EXTI9_5_IRQn handler manages the EXTI lines 5 to 9.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

To use this example, you just need to connect a high level voltage on the GPIO input with:

- Either a simple button,
- Or a wire connected to a 3.3V power supply,
- Or an output from another MCU or any external component.

<!--
http://ditaa.sourceforge.net/#usage
plantUML ditaa integration supports PNG only
-->

<!--
@startditaa doc/example_ll_gpio_exti_setup.png
    /------------------\
    |                  |
    |                  *------------ VCC
    |                  |          |
    |       /----------+          :
    |       |   EXTI   |          |
    |       |          |          v
    |       |  Line_xx *------------ GPIO_Input
    |       |  cBLK    |
    |       \----------+
    |                  |
    |                  |
    |     STM32_Board  |
    \------------------/
@endditaa
-->

![example_ll_gpio_exti_setup](doc/example_ll_gpio_exti_setup.png)

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  | Board pin   | MCU pin |    Signal name       | ARDUINO <br/> connector pin |        User Label       |
  |   :---:     |  :---:  |       :---:          |           :---:             |          :---:          |
  |   CN5-6     | PA5     |    MX_STATUS_LED     |         D13 - CN5-6         |     MX_STATUS_LED       |
  |     -       | PC13    |   EXTI_TRIGGER_PIN   |             -               | MX_EXAMPLE_EXTI_TRIGGER |

  </details>
  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  | Board pin   | MCU pin |    Signal name       | ARDUINO <br/> connector pin |        User Label       |
  |   :---:     |  :---:  |       :---:          |           :---:             |          :---:          |
  |   CN5-6     | PA5     |    MX_STATUS_LED     |         D13 - CN5-6         |     MX_STATUS_LED       |
  |     -       | PC13    |   EXTI_TRIGGER_PIN   |             -               | MX_EXAMPLE_EXTI_TRIGGER |

  </details>
  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  | Board pin   | MCU pin |    Signal name       | ARDUINO <br/> connector pin |        User Label       |
  |   :---:     |  :---:  |       :---:          |           :---:             |          :---:          |
  |   CN5-6     | PA5     |    MX_STATUS_LED     |         D13 - CN5-6         |     MX_STATUS_LED       |
  |     -       | PC13    |   EXTI_TRIGGER_PIN   |             -               | MX_EXAMPLE_EXTI_TRIGGER |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

1. Using several GPIOs with EXTI interrupt:

  - The EXTI multiplexer groups pins from different ports to a global EXTI line.
      PA0, PB0,...PH0 are connected to the EXTI0 line.
      PA1, PB1,...PH1 are connected to the EXTI1 line.
      PAn, PBn,...PHn are connected to the EXTIn line.
    If you use several pins on the same EXTI line, you cannot detect from which port happened the interrupt.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

This [application note](https://www.st.com/content/ccc/resource/training/technical/product_training/group0/62/71/4f/2a/5d/25/4c/c7/STM32F7_System_EXTI/files/STM32F7_System_EXTI.pdf/jcr:content/translations/en.STM32F7_System_EXTI.pdf) gives information about the STM32 extended interrupts and events controller.

More information about the LL flow can be found [here](https://wiki.st.com/stm32mcu/wiki/Getting_started_with_EXTI)

The documentation of the drivers of the relevant STM32 series contains more detailed information.

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
