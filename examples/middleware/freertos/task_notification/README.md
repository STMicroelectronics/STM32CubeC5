<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *freertos_task_notification*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure and use the FreeRTOS API to synchronize between four threads using task notification and software timers.

We demonstrates the usage of a notification by establishing communication between three threads through it. Additionally, task notification is employed between two threads to signal the toggling of status LED.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

- __Step 1__: creates a software timer and 3 tasks.
- __Step 2__: starts the FreeRTOS scheduler.
_ __Step 3__: 'Main Task' waits until being notified by 'EXTI Task' or/and 'Timer Task', (sequence is repeated 3 times). In the end of the sequence, Green led is toggled.
- __Step 4__: Once the user button is pressed, 'EXTI Task' sends notification to the 'Main task', starts the software timer and prints message to the serial port.
- __Step 5__: 'Timer Task' waits for the timer callback and then sends notification to the Main Task and prints message to the serial port.


__End of example__:

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Starting FreeRTOS Scheduler
[INFO] [Main Task]: Waiting notification from Event Tasks
[INFO] [EXTI Task]: Waiting notification from Push Button
[INFO] [Timer Task]: waiting notification from Software timer

[INFO] [EXTI Task]: Push Button pressed -> Notify Main Task
[INFO] [Main Task]: EXTI Event received
[INFO] [Timer Task]: Getting notification from Software timer
[INFO] [Timer Task]: Timer period reached -> Notify Main Task
[INFO] [Main Task]: Software Timer Event received

[INFO] [EXTI Task]: Push Button pressed -> Notify Main Task
[INFO] [Main Task]: EXTI Event received
[INFO] [Timer Task]: Getting notification from Software timer
[INFO] [Timer Task]: Timer period reached -> Notify Main Task
[INFO] [Main Task]: Software Timer Event received

[INFO] [EXTI Task]: Push Button pressed -> Notify Main Task
[INFO] [Main Task]: EXTI Event received
[INFO] [Timer Task]: Getting notification from Software timer
[INFO] [Timer Task]: Timer period reached -> Notify Main Task
[INFO] [Main Task]: Sequence finished -> Toggle MX_STATUS_LED

```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the Cube Firmware package.")


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

### __3.2. Specific board setups__

Please find the exact hardware configurations of your project below.

<details>
  <summary>On STM32C5 series.</summary>
  <details>
  <summary>On board NUCLEO-C562RE.</summary>

  | Board connector   | MCU pin | Signal name      | ARDUINO pin |
  | :---:             | :---:   | :---:            | :---:       |
  | CN5-6             | PA5     | MX_STATUS_LED    | D13         |
  | CN7-23            | PC13    | EXTI_TRIGGER_PIN | -           |

  </details>
</details>

## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the Cube Firmware package.")


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the Cube Firmware package.")


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
