<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_cortex_systick_delay*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use a millisecond precision delay based on the default 1ms SysTick configuration thanks to the HAL API.
This delay is demonstrated thanks to a GPIO toggling.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.
It includes the Cortex SysTick default HAL configuration: 1ms.

The application executes the following __example steps__:

__Step 1__: Inserts a 50ms delay and toggles the GPIO pin.
              Step 1 is executed in a loop.

__End of example__: Reports the outcome of the example via the variable **`ExecStatus`**,\
and the **status LED** toggles every 50ms.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

- The *GPIO* pin is connected to the LED.
- The Cortex *SysTick* timer is configured by default to generate an interrupt at 1kHz (every 1ms).
  - The configuration is done using HAL_InitTick() located in HAL_Init().
  - The HAL_InitTick() function configures:
       - The SysTick reload register according to the frequency provided as a parameter.
       - Configure the SysTick IRQ priority to the lowest value.
       - Reset the SysTick counter register.
       - Configure the SysTick counter clock source.
       - Enable the SysTick interrupt.
       - Start the SysTick counter.
  - The SysTick is automatically reconfigured to remain at 1ms, when the system clock is reconfigured thanks to the HAL API.

Note: Caution, this 1 msec timebase must not be changed, as it is used by HAL drivers.
But it is possible to change the HAL timebase source, see dedicated examples:

- example_hal_timebase_timer
- example_hal_timebase_rtc_alarm
- example_hal_timebase_wkup_alarm


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

JTAG/SWD probe may be used to check variables and buffers.

### __3.2. Specific board setups__

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |

  </details>
  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |

  </details>
  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |  PH0_OSC_IN   |
  |    PH1    |  RCC_OSC_OUT  |  PH1_OSC_OUT  |

  </details>
</details>

## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

- Care must be taken when using HAL_Delay(), this function provides an accurate delay (in milliseconds)
  based on a variable incremented in the SysTick ISR. As HAL_Delay() is called from a
  peripheral ISR process, the SysTick interrupt must have higher priority (numerically lower)
  than the peripheral interrupt. Otherwise, the caller ISR process is blocked.
  To change the SysTick interrupt priority, you can use the HAL_CORTEX_NVIC_SetPriority() function.

- The application needs to ensure that the HAL timebase is always set to 1 millisecond
  to have correct HAL operation.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

You can also refer to these other examples demonstrating how to change the HAL timebase:

- example_hal_timebase_timer
- example_hal_timebase_rtc_alarm
- example_hal_timebase_wkup_alarm

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
