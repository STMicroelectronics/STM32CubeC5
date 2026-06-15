<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *freertos_low_power*__

**Example version:** 2.0.3

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure and use the FreeRTOS API to enable tickless low-power mode in a single-task application, using a binary semaphore for task control and wakeup, and configuring LPTIM (Low-Power Timer) as the wakeup source.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

- __Step 1__: Initializes the system by creating a binary semaphore and a single task (Task_one).
- __Step 2__: Starts the FreeRTOS scheduler.
- __Step 3__: Wait for the binary semaphore in Task_one. Upon receiving the semaphore, toggle the status LED 10 times, notify the background task, and delay for 2 seconds to maximize time spent in tickless low-power mode.
- __Step 4__: Enter tickless low-power (STOP) mode when the system is idle, with the LPTIM peripheral configured as the wakeup source.
- __Step 5__: Wake up the system upon LPTIM interrupt, restore clocks, and give the binary semaphore to resume Task_one

__End of example__: This example is repeated endlessly, allowing the MCU to spend extended periods in low-power mode between task executions.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Starting FreeRTOS Scheduler
[INFO] Task One - Binary Semaphore Taken!
[INFO] Task One - Toggling the status LED 10 times with 500ms delay.
[INFO] Entering STOP mode.


[INFO] WakeUp detection and clocks restored.
[INFO] Task One - Binary Semaphore Taken!
[INFO] Task One - Toggling the status LED 10 times with 500ms delay.
[INFO] Entering STOP mode.

```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

__TIM__:

FreeRTOS exclusively uses the SysTick as its timebase. Thus a timer should be configured to be used as a separate timebase for the HAL.
To do so, the HALCore is configured to use the desired TIM peripheral. The initialization is handled through the HAL within HAL_InitTick() function.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  The `TIM6`, is used as a separate timebase for the HAL.

  | Board pin   | MCU pin | Signal name       | ARDUINO pin |
  | :---:       | :---:   | :---:             | :---:       |
  | CN5-6       | PA5     | MX_STATUS_LED     | D13         |

  </details>
</details>

## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

