<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_timebase_tim*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use an STM32 timer instead of the Cortex Systick to reconfigure the HAL tick source.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

> **_NOTE:_** The function `HAL_InitTick()` is defined as weak in the HAL core driver, and overridden in the HAL timebase driver with a timer-based implementation (rather than SysTick-based).

The application executes the following __example steps__:

__Step 1__: Reconfigures the HAL timebase with the desired frequency.

__Step 2__: The LED state toggles with a delay of 500 ms.

__End of example__: If no error occurs, the status LED toggles forever.

> **_NOTE:_** when an error occurs, the `error_handler()` function runs and turns the LED off.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

This example demonstrates the following peripheral:

__TIM__:

At startup, `HAL_Init()` calls the function `HAL_InitTick()` to configure the HAL tick source.

The `HAL_InitTick()` function in the HAL timebase driver overrides the default HAL core driver implementation.
It configures an STM32 timer as the HAL timebase instead of the SysTick (default implementation).

In this example, the timer is reconfigured with a different frequency before running `app_process()`.

This reconfiguration occurs when `HAL_InitTick()` is called again in `app_init()`.
It updates the timer prescaler to obtain a 100 Hz frequency.

<details>
  <summary>Timebase frequency configuration</summary>

__Principle__:

A timebase is generated thanks to the interrupt mode.
To do so, the timer generates update events continuously at the chosen timebase frequency.

__Counting mechanism__:

The timer counter generates an update event each time the counter exceeds the value of the auto-reload register (ARR).
For instance, by setting this ARR to 99, the timer generates an update event every 100 timer clock cycles.

__Timer count frequency__:

The prescaler value (N) ranges from 0 to 65535, and the counter clock frequency (tim_cnt_ck) is divided by N + 1.

A counter counts from 0 to the ARR value, so (ARR + 1) clock cycles are counted.
Depending on the counter clock (tim_cnt_ck) value the timebase period and frequency are:

    timebase period = tim_cnt_ck period * (ARR + 1)
    timebase frequency = tim_cnt_ck frequency / (ARR + 1)

So, you can compute the ARR value to set to obtain the desired timebase frequency with this formula:

    ARR = (tim_cnt_ck frequency / timebase frequency) - 1

__Practical Applications of Numerical Computations__:

<details>
      <summary>Detailed calculation for each timebase frequency with a timer input clock frequency of 10 kHz</summary>

  We explain below the numerical calculation of:

  - ARR value,
  - Period value,
  - HAL tick granularity.

  The timer's input clock frequency is 10 kHz.

  <details>
        <summary>HAL timebase 1 kHz</summary>

        ARR value:
          ARR = (10 kHz / 1 kHz) - 1
          ARR = (10000 / 1000) - 1
          ARR = 10 - 1
          ARR = 9

        Tick Granularity: The HAL Tick interrupt occurs every 1 millisecond.

   </details>
   <details>
        <summary>HAL timebase 100 Hz</summary>

        ARR value:
          ARR = (10 kHz / 100 Hz) - 1
          ARR = (10000 / 100) - 1
          ARR = 100 - 1
          ARR = 99

        Tick Granularity: The HAL Tick interrupt occurs every 10 milliseconds.

   </details>
   <details>
        <summary>HAL timebase 100 Hz</summary>

        ARR value:
          ARR = (10 kHz / 10 Hz) - 1
          ARR = (10000 / 10) - 1
          ARR = 1000 - 1
          ARR = 999

        Tick Granularity: The HAL Tick interrupt occurs every 100 milliseconds.

   </details>
  </details>
</details>


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

No specific hardware setup is needed for this example.

Any STM32 basic timer can be used as a generic timer for timebase generation, and no external signal is provided.

TIM6 and TIM7 are basic timers in many STM32 microcontrollers, so they are good candidates for timebase generation.

### __3.2. Specific board setups__

This section focuses on the clock settings as this is critical to obtain the desired timebase.

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>Common configuration.</summary>

  The AHB clocks prescaler is set to 1:

  - The AHB clock (HCLK) is equal to the system clock (SYSCLK)

  - The timer's input clock (tim_ker_ck) is equal to its dedicated AHB clock (HCLK).

    tim_ker_ck = HCLK = SYSCLK (system clock)

  So: tim_ker_ck = HCLK in Hz

  To get the timer's counter clock frequency (tim_cnt_ck), the timer prescaler is computed as follows:

    timer_prescaler = (HCLK / tim_cnt_ck ) - 1

  Typical clock diagram for STM32C5xx MCU peripherals:
    <!--
@startuml
@startditaa{doc/stm32c5_peripherals_clocks.png}
 +---------+
  | clock   |
  | source  |
  | control |
 +---+-----+
  |
    ++-\
  --+  |
  |  |
  |  |
  --+  |           +---------------+        +--------------+
  |  |  SYSCLCK  |  AHB          |  HCLK  |  APBx        |  PCLKx
  |  +-----------+  PRESC        +----+---+  PRESC       +--------------------------------
  --+  |           |  / 1,2,...512 |    |   | / 1,2,4,8,16 |          To APBx peripherals
  |  |           +---------------+    |   +--------------+
  |  |                                |
  --+  |                                +---------------------------------------------------
  |  |                                                                          To TIMx
    +--/
@endditaa
@enduml
-->
  ![clocks](doc/stm32c5_peripherals_clocks.png)

Numerical values:

  - The HCLK is set to 144 MHz
  - The timer's counter clock is set to 1 MHz

        timer_prescaler = (144 MHz / 1 MHz) - 1 = 143

  </details>

  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |

  In this example, TIM6 is selected as the timebase source.

  </details>

  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |

  In this example, TIM6 is selected as the timebase source.

  </details>

  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |  PH0_OSC_IN   |
  |    PH1    |  RCC_OSC_OUT  |  PH1_OSC_OUT  |

  In this example, TIM6 is selected as the timebase source.

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Find below the points of attention for this specific example.

__HAL tick in milliseconds__: The HAL tick counter stored in the global HAL variable `uwTick` is always expressed in milliseconds. The increment of this variable depends on `uwTickFreq`. The driver increments `uwTick` by the number of milliseconds that have elapsed between two HAL ticks. This is 1 for a 1 ms timebase, 10 for a 10 ms timebase and 100 for a 100 ms timebase.

__Tick System Cost__: The HAL tick granularity affects how accurately time intervals can be measured. A higher granularity allows for more precise timing operations, but at the cost of increased CPU usage and power consumption. The choice of granularity must be based on the specific requirements of the application.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

You can also refer to these other examples demonstrating how to measure time, either with different HAL timebase setups, or by using the Cortex HAL SysTick:

- example_hal_timebase_rtc_wakeup
- example_hal_timebase_rtc_alarm
- example_hal_tim_timebase
- example_hal_cortex_get_systick
- example_hal_cortex_systick_delay

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
