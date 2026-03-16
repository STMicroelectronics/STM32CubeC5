<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *freertos_message_queue*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure and use the FreeRTOS API to synchronize between four threads using message queue and task notification.

We demonstrates the usage of a message queue by establishing communication between three threads through it. Additionally, task notification is employed between two threads to signal the toggling of MX_STATUS_LED.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

- __Step 1__: creates a queue and 4 tasks.
- __Step 2__: starts the FreeRTOS scheduler.
- __Step 3__: 'Producer Task' sends message (incrementing value "ProducerValue") to the queue.
- __Step 4__: 'Consumer Task' gets message from the queue, checks if it's the correct message (ProducerValue == ConsumerValue) then prints message to the serial port.
- __Step 5__: 'Gen Task' waits for the message on the queue to reach a specific "GenerationValue" then sends a notification to Receive Task.
- __Step 6__: 'Receive Task' waits until being notified, then toggles the green led.



__End of example__:

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Starting FreeRTOS Scheduler
[INFO] [Producer Task]: Sending ProducerValue msg to the queue

[INFO] [Consumer Task]: Getting the msg from queue & checking if it's the correct msg
[INFO] [Consumer Task]: Correct msg verified

[INFO] [Receive Task]: Wait until being notified

[INFO] [Producer Task]: Sending ProducerValue msg to the queue

[INFO] [Gen Task]: Getting the msg from queue & checking if the GEN_VALUE is reached
[INFO] [Gen Task]: GenerationValue reached -> Notify Receive Task

[INFO] [Receive Task]: Notification received -> Toggle MX_STATUS_LEDc
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
