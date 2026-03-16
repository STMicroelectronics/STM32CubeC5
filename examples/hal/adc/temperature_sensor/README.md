<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_adc_temperature_sensor*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

This example demonstrates how to use the ADC to perform a single conversion on the internal temperature sensor and calculate the temperature in degrees Celsius.
It is based on the ADC HAL API.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: initializes and configures the ADC instance.

__Step 2__: performs the ADC activation and self calibration to improve measurements accuracy. The calibration is a prerequisite to any ADC operation. It removes the systematic errors that may vary from chip to chip.

__Step 3__: starts the ADC conversion of the sensor output voltage in polling mode.

__Step 4__: retrieves the ADC raw value, and computes the corresponding temperature.

__End of example__: The ADC conversion is repeated every second endlessly (step 3 and 4 is running in a loop).
The example status is reported via the variable **`ExecStatus`**, and the status LED
remains turned on in case of success.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```test
[INFO] Step 1: ADC initialization COMPLETED.
[INFO] Step 2: ADC start and calibration COMPLETED.
[INFO] Step 3: ADC start conversion COMPLETED.
[INFO] Step 4: ADC conversion DONE.  Raw ADC value temperature = 0x320   Temperature value = 29 degree Celsius  
[INFO] Step 3: ADC start conversion COMPLETED.
[INFO] Step 4: ADC conversion DONE.  Raw ADC value temperature = 0x323   Temperature value = 30 degree Celsius
...
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

__ADC__: is configured with the following parameters:

- Single channel conversion.
- Single conversion mode.
- ADC resolution 12 bit.
- Software trigger.
- ADC clock rate and sampling clock cycle are configured to satisfy the temperature sensor characteristics, related to the ADC sampling time when reading the temperature, as described in the STM32 MCU datasheet.
- Dedicated ADC input channel connected to the internal temperature sensor is selected.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

JTAG/SWD probe may be used to check variables.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.
This example run without external setup.

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PA2    |   USART2_TX   |      PA2      |

  ADC1_IN12 is the internal ADC channel connected to the internal temperature sensor of the device.
    <details>
      <summary>Temperature sensor characteristics</summary>

   - Minimum value of ADC sampling time when reading the temperature is 13&micro;s.
  - Junction temperature range: &minus;40 to +140 &deg;C

    </details>
  </details>

  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PA2    |   USART2_TX   |      PA2      |

  ADC1_IN12 is the internal ADC channel connected to the internal temperature sensor of the device.
    <details>
      <summary>Temperature sensor characteristics</summary>

   - Minimum value of ADC sampling time when reading the temperature is 13&micro;s.
  - Junction temperature range: &minus;40 to +140 &deg;C

    </details>
  </details>

  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |  PH0_OSC_IN   |
  |    PH1    |  RCC_OSC_OUT  |  PH1_OSC_OUT  |
  |    PA2    |   USART2_TX   | DBGIN_VCP_TX  |

  ADC1_IN12 is the internal ADC channel connected to the internal temperature sensor of the device.
    <details>
      <summary>Temperature sensor characteristics</summary>

   - Minimum value of ADC sampling time when reading the temperature is 13&micro;s.
  - Junction temperature range: &minus;40 to +140 &deg;C

    </details>
  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

The __ADC conversion precision__ is highly dependent on the sampling time, which should be adjusted according to the input signal characteristics.

__Sampling time__:
Pay attention to the ADC's clock rate and the sampling clock cycle count for the temperature sensor's channel. It needs to satisfy at least the minimum sampling time, as described in the STM32 MCU datasheet, to get reliable results.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

- [Application Note AN5036](https://www.st.com/resource/en/application_note/an5036-guidelines-for-thermal-management-on-stm32-applications-stmicroelectronics.pdf): Guidelines for thermal management on STM32 applications

- [Application Note AN3116](https://www.st.com/content/ccc/resource/technical/document/application_note/c4/63/a9/f4/ae/f2/48/5d/CD00258017.pdf/files/CD00258017.pdf/jcr:content/translations/en.CD00258017.pdf): STM32's ADC modes and their applications

- [Application Note AN5346](https://www.st.com/content/ccc/resource/technical/document/application_note/group1/02/ba/86/7e/6c/d7/4e/08/DM00625282/files/DM00625282.pdf/jcr:content/translations/en.DM00625282.pdf): STM32G4 ADC use tips and recommendations

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
