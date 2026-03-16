<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *sequencer_task_prio*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the adv_trace utility to manage task priorities. During the main program execution, the sequencer is initialized and three tasks are registered: MX_TASK_MANAGE_PRIO, MX_TASK_GPIO_TOGGLE_LOW_FREQUENCY and MX_TASK_GPIO_TOGGLE_HIGH_FREQUENCY.

MX_TASK_MANAGE_PRIO: Orders the execution of the MX_TASK_MANAGE_PRIO with low priority, the MX_TASK_GPIO_TOGGLE_LOW_FREQUENCY with medium priority and the MX_TASK_GPIO_TOGGLE_HIGH_FREQUENCY with the highest priority.
<br />MX_TASK_GPIO_TOGGLE_LOW_FREQUENCY: Toggles the LED at a frequency of 0.5 Hz for a duration of 5 seconds.
<br />MX_TASK_GPIO_TOGGLE_HIGH_FREQUENCY: Toggles the LED at a frequency of 4 Hz for a duration of 5 seconds.

The sequencer schedules the execution of these tasks based on their assigned priorities.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.


The application executes the following __example steps__:

__Step 1__: Configure a GPIO pin in output mode, typically a pin connected to an LED.

__Step 2__: Initialize the sequencer and set the "MX_TASK_MANAGE_PRIO" task with the lowest priority for execution.

__Step 3__: Create a task function to order the execution of the three tasks with their respective priorities.

__Step 4__: Create a task function to toggle the LED connected to a GPIO pin at a frequency of 0.5 Hz for 5 seconds.

__Step 5__: Create a task function to toggle the LED connected to a GPIO pin at a frequency of 4 Hz for 5 seconds.

__Step 6__: Enter an infinite loop to run the sequencer by continuously calling SEQ_Run().

__End of example__: The example runs indefinitely, with the GPIO toggling for 5 seconds at a frequency of 0.5 Hz, and for 5 seconds at a frequency of 4 Hz.
<br />During execution a counter LowFreqToggleCounter increments with each toggle at low frequency (0.5Hz), while another counter HighFreqToggleCounter increments with each toggle at high frequency (4Hz). Both counters are reset within the MX_TASK_MANAGE_PRIO task.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")


__Sequencer__: The sequencer is configured with these specific parameters:

  - _priority_: __3__

  - _number of task_: __3__

  - _task_1 name_: __MX_TASK_MANAGE_PRIO__

  - _task_2 name_: __MX_TASK_GPIO_TOGGLE_LOW_FREQUENCY__

  - _task_3 name_: __MX_TASK_GPIO_TOGGLE_HIGH_FREQUENCY__

  - _task_1 function name_: __manage_prio__

  - _task_2 function name_: __gpio_toggle_low_frequency__

  - _task_3 function name_: __gpio_toggle_high_frequency__

__GPIO__: The GPIO pin is configured in output mode:

  - _label_: __MX_EXAMPLE_GPIO__


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

<!--
@startuml
@startditaa{doc/generic_hardware_setup.png}
    +-------------------------------------------------------------------------+
    |          /----------------------\                                       |
    |          |STM32                 |                                       |
    |          |                      |                  /-----\              |
    |          | MX_EXAMPLE_GPIO_PIN  *------------------* LED |              |
    |          |                      |                  \-----/              |
    |          |                      |                                       |
    |          |                      |                                       |
    |          \----------------------/                                       |
    | STM32_Board                                                             |
    +-------------------------------------------------------------------------+
@endditaa
@enduml
-->
![generic_hardware_setup](doc/generic_hardware_setup.png)

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

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

Find below the points of attention for this specific example.

__Topic A__: Make sure that the switch "SEQ_USER_CONFIG" necessary to load sequencer user configuration is present on toolchain preprocessor define.

__Topic B__: make sure that GPIO pin is properly configured and clocked.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

This [application note](https://www.st.com/resource/en/application_note/an5701-stm32cube-mcu-package-examples-for-stm32u5-series-stmicroelectronics.pdf)
explains further the STM32Cube MCU Package examples for STM32U5 MCUs.


You can also refer to these other examples to go further with sequencer:

- sequencer_gpio_toggle_lowpower: demonstrates the use of sequencer to manage tasks and switching to lowpower mode.

- sequencer_gpio_toggle: demonstrates the use of sequencer to handle a GPIO toggle.

- task_pause_resume: shows how the sequencer handles the task pause/resume mechanism.

- task_wait_event: shows how the sequencer handles tasks waiting for event.

The documentation of the drivers of the relevant STM32 series contains more detailed information.

the user manual of the sequencer utility contains more detailed information about using the sequencer.

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
