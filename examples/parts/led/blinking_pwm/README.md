<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *led_blinking_pwm*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the Cube firmware package.")

This example shows how to use the LED part PWM API.
It illustrates it by blinking an LED through a PWM.


## __1. Detailed scenario__

This scenario demonstrates how to drive an LED connected to a PWM with the LED part API.

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

 __Step 1__: Initializes the LED device using the ``led_pwm_init`` LED part function.

__Step 2__: Turns on/off the LED 10 times, using the ``led_pwm_on`` and ``led_pwm_off`` LED part functions

__Step 3__: Toggles the LED 10 times, using the ``led_pwm_toggle`` LED part function.

__Step 4__: Blinks the LED, indefinitely, using the ``led_pwm_blink`` LED part function.

__End of example__: The LED blinks indefinitely if no error occurs.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the Cube firmware package.")

In accordance with the selected GPIO input pin, the following peripherals shall be configured:

- *GPIO*:
  - The pin must be configured with an alternate function mode to be used as an output timer channel.
  - No pull-up or pull-down resistors are enabled as the timer peripheral actively drives the pin.
  - The pin speed is set to "low frequency" to optimize power consumption and reduce EMI.
- *TIM*:
  - The timer prescaler is set to achieve a counter clock frequency of 512 Hz (autoreload register set to 2249).
  - The timer repetition counter is initialized to 0.
  - The timer channel is set to work as a PWM generator in up-counting mode 1.
  - The output polarity of the timer channel is set to high, matching the LED's active level.
  - The output source of the timer trigger is set to the update event (default).
- *DMA*:
  - The DMA direction is configured as "memory to peripheral."
  - The DMA is configured with a fixed destination address.
  - The DMA is configured with source address increment.
  - The DMA trigger source is the timer capture/compare channel 1 update event.
  - The DMA channel is configured to use a circular linked-list.

**_NOTE:_** the value of the timer prescaler must be copied in the LED PWM structure (usually done through code generation).


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This section describes the hardware setup principles that apply to any board.

### __3.2. Specific board setups__

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label  |
  |:---------:|:-------------:|:------------:|
  |    PH0    |  RCC_OSC_IN   |    OSC_IN    |
  |    PH1    |  RCC_OSC_OUT  |   OSC_OUT    |
  |    PA2    |   USART2_TX   |     PA2      |
  |    PA5    |   TIM2_CH1    |     PA5      |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the Cube firmware package.")


No specific debug tips.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the Cube firmware package.")

This [Application note AN2841](https://www.st.com/content/ccc/resource/technical/document/application_note/64/4d/e3/1d/b3/34/4f/0b/CD00213809.pdf/files/CD00213809.pdf/jcr:content/translations/en.CD00213809.pdf)
gives a detailed example of LED dimming implemented on STM32 microcontroller in combination with the STP24DP05 LED driver.

You can also refer to these other examples:

- hal_gpio_toggle to toggle any GPIO with HAL API (the GPIO can be connected to an LED)
- hal_tim_pwm_ouput shows how to configure the TIM peripheral in PWM (Pulse Width Modulation) mode.
- led_blinking_gpio shows how to drive an LED using the LED GPIO part driver.

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
