<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *ll_comp_vref_it*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use a comparator peripheral to compare a voltage level applied on the COMP input pin to the internal voltage reference (VrefInt), in interrupt mode.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: initializes the COMP instance.

__Step 2__: starts the comparator in interrupt mode.

__Step 3__: Switches to stop mode.
            When the comparator input crosses (rising or falling edge) the internal reference voltage VrefInt,
            the comparator generates an interrupt and the system wakes up from Stop mode.

__Step 4__: waits for the wake up from Stop mode (through the EXTI line), then reconfigures the clock.
            Restores the SysTick.

When the system is in Stop mode, the LED is off. Once a rising or falling edge is detected, the system wakes up and the LED blinks once.

__End of example__: This example is repeated endlessly (step 3 to step 4 are executed in a loop).


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

This example demonstrates the following peripherals:

__COMP__:

The comparator has configurable plus and minus inputs used for flexible voltage selection:

- The minus input is connected to the internal reference voltage.
- The plus input can be connected to various sources, such as an external voltage supply.
- The interrupt generation capability of the comparator allows the system to wake up from Stop modes through the EXTI controller.

The EXTI line is configured to trigger an interrupt on both rising and falling edges.

__PWR__:

When calling the function to enter stop mode, we need to specify the wake up parameter:

- HAL_PWR_LOWPOWER_MODE_WFI (wait for interrupt).

In this example, we have used the deepest Stop mode. But, the use case can work with any Stop mode valid in the STM32 series.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This section describes the hardware setup principles that apply to any board.

<!--
```
@startuml
@startditaa{doc/STMicroelectronics.example_ll_comp_vref_it.png}

                /------------------------------------------\
                |                                          |
                |                                          |
                |                                          |
                |            /---------------\             |
                |            |     COMP      |             |
                |            |               |             |
     COMP_Pin---+----------+->COMPx_INP      |Interrupt    |
       Input    |            |               *--+--+->     |
                |VREFINT---+->COMPx_INM      |             |
                |            |               |             |
                |            \---------------/             |
                |                                          |
                |                                          |
                |                                          |
                |                                          |
                |                                          |
                |                                          |
                |               STM32 MCU                  |
                \------------------------------------------/


@endditaa
@enduml
```
-->

![example_hal_comp_vref_it](doc/STMicroelectronics.example_ll_comp_vref_it.png)

### __3.2. Specific board setups__

Find the exact hardware configurations of your project below.

<details>
  <summary>On STM32C5 series.</summary>
  <details>
  <summary>On board NUCLEO-C542RC.</summary>

  | Board pin   | MCU pin | Signal name        |ARDUINO <br/> connector pin|User Label|
  | :---:       | :---:   | :---:              | :---:                    |          |
  |      -      | PB0     | COMP1_INP2         | -                        |PB0       |

  </details>
  <details>
   <summary>On board NUCLEO-C562RE.</summary>

  | Board pin   | MCU pin | Signal name        |ARDUINO <br/> connector pin|User Label|
  | :---:       | :---:   | :---:              | :---:                    |          |
  |      -      | PB0     | COMP1_INP2         | -                        |PB0       |

  </details>
  <details>
  <summary>On board NUCLEO-C5A3ZG.</summary>

  | Board pin   | MCU pin | Signal name        |ARDUINO <br/> connector pin|User Label|
  | :---:       | :---:   | :---:              | :---:                    |          |
  |      -      | PB0     | COMP1_INP2         | -                        |PB0       |

  </details>
  </details>

## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the Cube Firmware package.")

Find below the points of attention for this specific example.

__VrefInt__: For precise information on the internal voltage reference (VrefInt), read the official STMicroelectronics datasheet of your MCU.
             In the datasheet, look for the "Electrical Characteristics" section, where you find details on VrefInt, including its typical value, tolerances, and operating conditions.

__External voltage__: We recommend two possibilities to apply an external voltage on the COMP input pin.

1. Connection to a fixed voltage (3.3 V):
   Connect the GPIO pin directly to a 3.3 V source on the board. For example, a 3.3V pin from the board's power supply.
   This can be done using a jumper wire or jumper.

2. Connection to an external power source:
   Connect an external power source. For example, a laboratory power supply.
   Make sure that the applied voltage is within the acceptable range for the comparator and GPIO pin. For example, 0 V to 3.3 V.

__Exiting Stop mode__: When exiting from stop mode, the system clock must be configured. See the RCC peripheral section in the reference manual of your MCU.

__Wake-up causes__: Any peripheral interrupt enabled in the NVIC can wake up the system from stop mode when the AHB/APB clocks are present, not only EXTI.  This is the reason for switching off the SysTick interrupt before entering the stop mode.

__Spurious wake-up__: When no external signal is applied on the COMP pin, the input is floating and may result in a spurious COMP trigger and system wake up.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

More information about the STM32Cube Drivers can be found in the drivers' user manual of the STM32 series you are using.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
