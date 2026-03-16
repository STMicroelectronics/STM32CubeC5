<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *button_async_gpio_control*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the button part API.
This example illustrates the usage of a button by toggling an LED on each button press, using the "register callback" feature.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: initializes the button part and the status LED

__Step 2__: activates the part button interrupt to enable the corresponding NVIC IRQ line.
Registers the user callback for all button events (`BUTTON_EVENT_ANY`, meaning PRESSED and UNPRESSED).

__Step 3__: waits for the user to press the button. On each button event, an event counter is incremented, and the state of the LED is updated to match the state of the button (the LED is on when the button is pressed).

__End of example__: The step 3 is executed in a loop endlessly until an error occurs.
The execution status can be monitored through the global variable ExecStatus.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: Button and LED initialization COMPLETED.
[INFO] Step 2: Press the Button:
[INFO] Step 3: Button event nr. 1: button pressed.
[INFO] Step 3: Button event nr. 2: button released.
[INFO] Step 3: Button event nr. 3: button pressed.
[INFO] Step 3: Button event nr. 4: button released.
[INFO] Step 3: Button event nr. 5: button pressed.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

__GPIO__:

- The pin must be configured as an input with no pull-up or pull-down resistor.
- The pin must have an external interrupt capability.

__EXTI__:

- The corresponding EXTI line interrupt must be set and enabled with both rising and falling edges detection.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This section describes the hardware setup principles that apply to any board.

No generic board setup is needed for this example.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |   PC13    |     GPIO      |   MX_BUTTON   |
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PA2    |   USART2_TX   |      PA2      |

  </details>
  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |   PC13    |     GPIO      |   MX_BUTTON   |
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PA2    |   USART2_TX   |      PA2      |

  </details>
  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |   PC13    |     GPIO      |   MX_BUTTON   |
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |  PH0_OSC_IN   |
  |    PH1    |  RCC_OSC_OUT  |  PH1_OSC_OUT  |
  |    PA2    |   USART2_TX   | DBGIN_VCP_TX  |

  </details>
</details>

## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

__Interrupts multiplexing__: The EXTI groups several pins on the same IRQ line.
If several pins of the same group have external interrupts configured, it is not possible to distinguish them during an EXTI event.

__Debounce and events handling__: When multiple button events occur too rapidly, some may be missed by the example.
This is especially visible when the trace is activated and on boards without hardware debounce.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

The following application note discusses external interrupts, LEDs, and buttons on STM8: [Using the GPIOs and interrupt controller to drive LEDs on STM8 Nucleo-64](https://www.st.com/content/ccc/resource/technical/document/application_note/group0/5b/78/6a/67/4d/af/41/c1/DM00513070/files/DM00513070.pdf/jcr:content/translations/en.DM00513070.pdf).

You can also refer to this other example:

- hal_gpio_exti: How to configure an interrupt on an EXTI line with the HAL API.

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
