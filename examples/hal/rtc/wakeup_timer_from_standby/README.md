<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_rtc_wakeup_timer_from_standby*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure the RTC Wake-Up Timer to wake the MCU from Standby and indicate the wake-up timing using the RTC calendar and backup (tamper) registers.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, non-volatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__:

 - __Step 1.1__: Detects whether the device resumed from Standby or this is the first start.
 - __Step 1.2__: On first start, initializes the RTC peripheral once.
 - __Step 1.3__: Starts the RTC Wake-Up Timer with interrupt enabled.

__Step 2__: Handles the Standby phase.

 - __Step 2.1__: Enters Standby mode, the RTC generates a wake-up interrupt after 3 seconds.
 - __Step 2.2__: On restart from Standby, re-enables the RTC APB clock, updates the wake-up counter in TAMP backup register,
                 then prepares to re-enter Standby.

__Step 3__: Evaluates wake cycles and elapsed RTC time.

 - if the wake-up counter equals 5, reads the RTC time and confirms that 15 seconds have elapsed; if matched, the example concludes successfully.
 - if fewer than five wake-ups have occurred, returns to __Step 2__.
 - if the sequence extends beyond five wake-ups or the elapsed seconds differ, the example reports an error.

__Step 4__: Deinitializes the RTC peripheral.

__End of example__: The example loops through wake cycles until the expected condition or an error occurs.

You can verify that the example runs properly via the status LED and the `ExecStatus` variable.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1.1: First start.
[INFO] Step 1.2: Device initialization COMPLETED.
[INFO] Step 1.3: Enable RTC wakeup interrupt.
[INFO] Step 2.1: Enter STANDBY mode - After 3s RTC will generate a wakeup interrupt.
[INFO] Step 2.2: Restart from STANDBY mode.
...
[INFO] Step 3: Target wakeup count reached.
[INFO] Step 4: RTC deinitialization COMPLETED.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

### __2.1. IPs configuration__

### RTC (Real-Time Clock)

Configuration overview - purpose:

- Configure the RTC calendar and Wake-Up Timer (WUT) to generate a periodic wake event and verify uptime across Standby using the RTC seconds and backup registers.
- Focus is on the RTC peripheral, its clock source, the Wake-Up Timer feature and the backup domain (tamper/backup registers). Power/control of Standby and the PWR backup access are auxiliary.

Configuration parameters:

- Wake-Up Time: 3 s.
- RTC clock source: LSE (preferred for accuracy; LSI optional with recalibration).
- Interrupt/EXTI: enable the RTC wakeup interrupt.

Backup registers are not a user-facing configurable parameter here. They are used internally to persist the wake-up counter and test state across Standby cycles (5 cycles * 3 s = 15 s target).


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

No hardware dependencies for this example.

### __3.2. Specific board setups__

This section describes the exact hardware configuration of your project.

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |   PC14    | RCC_OSC32_IN  |   OSC32_IN    |
  |   PC15    | RCC_OSC32_OUT |   OSC32_OUT   |
  |    PA2    |   USART2_TX   |      PA2      |

  </details>

  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |   PC14    | RCC_OSC32_IN  |   OSC32_IN    |
  |   PC15    | RCC_OSC32_OUT |   OSC32_OUT   |
  |    PA2    |   USART2_TX   |      PA2      |

  </details>

  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  MCU pin  |  Signal name  |   User Label   |
  |:---------:|:-------------:|:--------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED  |
  |    PH0    |  RCC_OSC_IN   |   PH0_OSC_IN   |
  |    PH1    |  RCC_OSC_OUT  |  PH1_OSC_OUT   |
  |   PC14    | RCC_OSC32_IN  | PC14_OSC32_IN  |
  |   PC15    | RCC_OSC32_OUT | PC15_OSC32_OUT |
  |    PA2    |   USART2_TX   |  DBGIN_VCP_TX  |

  - Wake-Up Time: 3 s.

  The formula: T_wakeup = (WUT + 1) / f_wakeup_clock.

  WUT (wakeup_reload_time) = 12,288; f_wakeup_clock = 32,768 / 8 = 4,096 Hz

  T_wakeup = (12,288 + 1) / 4096 ~= 3 s.

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Find below the points of attention for this specific example.

__RTC kernel clock__: This example expects a stable RTC kernel clock (prefer LSE @ 32.768 kHz) for accurate wake timing. Other RCC clock sources can be selected, but some RTC functions may not be available in certain low-power modes or in VBAT when the selected clock is not LSE. If the calendar or wake-up behavior is unexpected, verify RCC and the backup-domain configuration (PWR backup access, LSE enable, polarity, etc.).

__LSI clock__: If LSI is used instead of LSE, its frequency tolerance is typically lower and can vary with device and temperature. Recompute RTC prescalers and the Wake-Up Timer (WUT) reload value to guarantee a 1 Hz calendar update and to obtain the expected 3 s wake period used by this example. When precision matters, measure or calibrate the LSI before deriving timing values.

WUT sizing note for this example:

- The example configures the WUT to generate an event every 3 s and counts five wake cycles (5 * 3 s = 15 s). Ensure the prescaler + WUT reload combination yields a 3 s period with the chosen RTC clock source.

- If wake timing is off, inspect: clock source selection, prescalers, WUT reload, and any RTC calibration registers.

__HAL API counting base__: The HAL RTC/time APIs use mixed bases - use the correct ranges when filling RTC time structures to avoid unexpected behavior:

- seconds and minutes: base-60 (0-59)
- hours: 0-12 or 0-23 depending on mode
- subseconds: reported as up to 0-999 (milliseconds) or microseconds depending on the API

__Backup registers__:

Ensure that backup domain write access is enabled before first use, and that no unintended backup domain reset (such as RTC domain reset, tamper event, or clock loss) clears their contents.

If the counter fails to increment after wake-up, verify that:

- Backup access is enabled,
- RTC APB clock is re-enabled on resume,
- The correct register index is used.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

- [Application Note AN4759](https://www.st.com/resource/en/application_note/an4759-using-the-hardware-realtime-clock-rtc-and-the-tamper-management-unit-tamp-with-stm32-microcontrollers-stmicroelectronics.pdf): Using the hardware real-time clock (RTC) and the tamper management unit
(TAMP) with STM32 microcontrollers

- [Application Note AN3371](https://www.st.com/resource/en/application_note/an3371-using-the-hardware-realtime-clock-rtc-in-stm32-f0-f2-f3-f4-and-l1-series-of-mcus-stmicroelectronics.pdf): Using the hardware real-time clock (RTC) in STM32F0, F2, F3, F4 and L1 series of MCUs

- Refer to the hal_rtc_calibration example for RTC calibration configuration details.

More information about the STM32Cube Drivers can be found in the drivers' user manual of the STM32 series you are using.


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
