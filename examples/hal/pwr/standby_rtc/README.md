<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_pwr_standby_rtc*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure and use the low-power Standby Mode with the HAL API.


## __1. Detailed scenario__

An STM32 MCU offers different power modes (from high to low energy consumption):
Run, Sleep, Stop modes, Standby, Shutdown.

In Standby mode, the MCU and the clocks are stopped.
The low-power regulator is running and few autonomous peripherals can operate in this mode.

We illustrate this by switching the MCU from the Run to Standby mode and waking the MCU up with an interrupt triggered by RTC.

__Initialization phase__: At the beginning of the `main()` function, the `mx_system_init()` function is called to initialize all the peripherals, the flash interface, the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: starts (or restarts from Standby mode), configures the RTC wakeup time and the status LED output.

__Step 2__: stays in Run mode for a few seconds and activates the status LED.

__Step 3__: switches to Standby mode and deactivates the status LED.
            restarts from Standby mode with a RESET of the MCU (through __Step1__) from an RTC event and continues to __Step2__ if no error occurs.

__End of example__: This example is repeated endlessly if no error occurs (the steps 1 to 3 are executed in a loop).

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:
```text
[INFO] Step 1: Device initialization COMPLETED.
[INFO] First start.
[INFO] Start an RTC event to fire in 5s.
[INFO] Step 2: RUN mode - Switch to STANDBY mode in 2000 milliseconds.
[INFO] Step 3: Enter STANDBY mode - After few seconds, RTC will wakeup the system.
[INFO] Step 1: Device initialization COMPLETED.
[INFO] Restart from STANDBY mode.
[INFO] Start an RTC event to fire in 5s.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

This example demonstrates the following peripheral:

__PWR__:

To check if the application starts for the first time or restarts (from Standby mode), we need to enable the memory
retention for Standby and VBAT mode.

To wake up from Standby mode, we need to configure a RTC wakeup time.

The RTC is initialized using the LSE clock as the source. The asynchronous and synchronous prescalers are set to divide the clock for timekeeping.

RTC time base period = 1 / [LSE / ((asynch_prediv + 1) * (synch_prediv + 1))]

The wakeup timer is configured with a selectable clock divider and a reload value to determinethe wakeup interval.

Wakeup Period = (wakeup_reload_time + 1) * Tick Period

Tick Period depends on the selected wakeup timer clock divider (e.g., RTCCLK/16, 1 second, 2 seconds, etc.).
wakeup_reload_time is the value loaded into the wakeup timer.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This example is designed to run on the boards that support PWR.

### __3.2. Specific board setups__

No specific hardware dependencies for this example.


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

__Status LED__: This example does not follow the standard status LED pattern. A transition from the LED being on to off indicates that the device is entering Standby mode.

__IOs__: In Standby mode, the I/Os are by default in floating state.

__wake-up events__: Only the wake-up line, external RESET, watchdog, and RTC/TAMPER events can wake up the system from Standby mode.

__Memory retention__: When exiting from Standby mode, the application restarts. The SRAMs and register contents are lost except for registers in the Backup domain and Standby circuitry. However, it is possible to enable the memory retention of some specific RAM areas. Refer to the reference manual of the MCU in use.

__BOR__: The BOR is always available in Standby mode.

__wake-up flag__: Before entering Standby mode, we have to clear the wake-up flag previously set.

__SysTick settings__: The example needs to ensure that the SysTick time base is always set to 1 millisecond to have correct HAL operation in Run mode.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

[Application Note 4759](https://www.st.com/resource/en/application_note/an4759-introduction-to-using-the-hardware-realtime-clock-rtc-and-the-tamper-management-unit-tamp-with-stm32-mcus-stmicroelectronics.pdf): Introduction to using the hardware real-time clock (RTC) and the tamper management unit (TAMP) with STM32 MCUs.

[Getting Started with PWR](https://wiki.st.com/stm32mcu/wiki/Getting_started_with_PWR): This article explains low-power modes, and provides code examples.

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
