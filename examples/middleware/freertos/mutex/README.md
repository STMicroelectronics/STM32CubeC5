<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *freertos_mutex*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure and use the FreeRTOS API to synchronize two threads using a mutex.


## __1. Detailed scenario__

While semaphores are used to notify threads, mutexes are used to protect critical sections, shared resources from
concurrent accesses.

We demonstrate mutex usage, by making two threads accessing the same LED and toggling it at different paces.

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: Creates a mutex and two tasks with the same priority.

__Step 2__: Starts the FreeRTOS scheduler.

__Step 3__: 'App Task One' takes the mutex, toggles the status LED 10 times at a 500ms pace then releases the mutex.

__Step 4__: 'App Task Two' takes the mutex, toggles the status LED 10 times at a 100ms pace then releases the mutex.
Returns to __Step 3__ indefinitely if no errors occur.

__End of example__: This example is repeated endlessly (step 3 and step 4 are executed in a loop).

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Starting FreeRTOS Scheduler
[INFO] [Task One]: Mutex Taken!
[INFO] [Task One]: Toggling the status LED 10 times (500 ms)
[INFO] [Task One]: Mutex Given!
[INFO] [Task Two]: Mutex Taken!
[INFO] [Task Two]: Toggling the status LED 10 times (100 ms)
[INFO] [Task Two]: Mutex Given!
------------
[INFO] [Task One]: Mutex Taken!
[INFO] [Task One]: Toggling the status LED 10 times (500 ms)
[INFO] [Task One]: Mutex Given!
[INFO] [Task Two]: Mutex Taken!
[INFO] [Task Two]: Toggling the status LED 10 times (100 ms)
[INFO] [Task Two]: Mutex Given!
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<details>
  <summary>On STM32C5 series.</summary>
  <details>
  <summary>On board NUCLEO-C562RE.</summary>

  | Board connector   | MCU pin | Signal name      | ARDUINO pin |
  | :---:             | :---:   | :---:            | :---:       |
  | CN5-6             | PA5     | MX_STATUS_LED    | D13         |

  </details>
</details>

## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

- [FreeRTOS usage of semaphores as mutex API](https://www.freertos.org/CreateMutex.html) to understand the FreeRTOS semaphore API when used as a mutex.
- [FreeRTOS Task Notifications](https://www.freertos.org/Documentation/02-Kernel/04-API-references/05-Direct-to-task-notifications/00-RTOS-task-notifications
) to understand the example status management.


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
