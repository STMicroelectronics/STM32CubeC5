<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *freertos_task_privilege*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure and use the MPU with FreeRTOS to control memory/peripheral access for tasks.
It implements simple recovery mechanisms from prohibited memory accesses.


## __1. Detailed scenario__

We demonstrate Memory Protection usage, by making two unprivileged threads accessing the shared memory with different access types.
When an illegal access is detected, a Memory Management fault is triggered, the fault information is sent to a privileged supervisor task, and the unprivileged tasks are suspended.

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: Creates a background task, a Main task with privileged access, a Supervisor task with privileged access, two unprivileged tasks with the same priority, and a queue used to transfer fault information.

__Step 2__: Starts the FreeRTOS scheduler.

__Step 3__: `MainTask` monitors the state of the unprivileged tasks and toggles the status LED, while `SupervisorTask` waits for fault information received through the queue.

__Step 4__: `Task1` performs a few legal reads from shared memory, then deliberately tries to perform an illegal write to the same shared memory.

__Step 5__: `Task2` toggles the status LED and performs legal reads from the shared memory.

__Step 6__: The `MemManage_Handler` and `MemManage_Recover()` routines capture the fault address, send the fault information to `SupervisorTask`, and trigger a context switch.

__Step 7__: `SupervisorTask` reports the Memory Management fault and suspends both unprivileged tasks. `MainTask` then keeps reporting that both tasks are suspended.

__End of example__: The scheduler continues to run. the privileged monitoring tasks keep executing, while the two unprivileged tasks remain suspended after the fault is handled.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Starting FreeRTOS Scheduler
[INFO] Main Privileged Task is running
[INFO] Task unprivileged Task1 is in state:  Ready
[INFO] Task unprivileged Task2 is in state:  Ready
[INFO] Main Privileged Task is running
[INFO] Task unprivileged Task1 is in state:  Ready
[INFO] Task unprivileged Task2 is in state:  Ready
[INFO] Main Privileged Task is running
[INFO] Task unprivileged Task1 is in state:  Ready
[INFO] Task unprivileged Task2 is in state:  Ready
[INFO] Main Privileged Task is running
[INFO] Task unprivileged Task1 is in state:  Ready
[INFO] Task unprivileged Task2 is in state:  Ready
[INFO] Supervisor Privileged Task is running
------------------------------
[INFO] MemManage fault occurred
.[DATA] Address: 0x2000925F
[INFO] Unprivileged Tasks were suspended.
------------------------------
[INFO] Main Privileged Task is running
[INFO] Task unprivileged Task1 is in state:  Suspended
[ALERT] User must investigate the memory fault cause
[INFO] Task unprivileged Task2 is in state:  Suspended
[ALERT] User must investigate the memory fault cause
[INFO] Main Privileged Task is running
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


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
