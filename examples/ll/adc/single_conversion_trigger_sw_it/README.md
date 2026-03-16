<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *ll_adc_single_conversion_trigger_sw_it*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the ADC to convert a single channel at each software request.
The conversion is performed using the interrupt programming model. This example is based on the ADC LL API.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: The ADC is configured to convert a single channel triggered by software.

__Step 2__: The ADC autocalibration is performed to ensure accurate measurements. The calibration is a prerequisite
to any ADC operation. It removes the systematic errors that may vary from chip to chip.

__Step 3__: The ADC conversion is started in interrupt mode.

__Step 4__: Waits for End Of Conversion (EOC): conversion complete callback. Retrieves the ADC value in millivolt for GPIO conversions.

__End of example__: The ADC conversion is repeated every second endlessly (step 3 and step 4 are running in a loop)
as long as there is no error. The example status is reported via the variable **`ExecStatus`**, and the status LED
remains turned on in case of success.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

__ADC__: is configured with the following parameters:

- Single channel conversion.
- Software trigger.
- Start with interrupt mode.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

JTAG/SWD probe may be used to check variables.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.
This example can run without external setup: in this case, ADC measures a floating voltage level and
conversion data are not deterministic.
To control voltage converted by ADC, apply a voltage level in range [0; VDDA] on pin ADCx_INy mentioned in table below.

<details>
  <summary>On STM32C5 series.</summary>
  <details>
  <summary>On board NUCLEO-C542RC.</summary>

  | Board pin   | MCU pin | Signal name          | ARDUINO <br> connector pin |User Label|
  | :---:       | :---:   | :---:                | :---:                      |          |
  | CN8-3       | PA4     | ADC1_INP4            | A2                         |PA4       |
  | CN5_6       | PA5     | LED                  | D13                        |PA5       |

  The example uses ADC1 on channel ADC1_IN4.
  
  </details>
  <details>
  <summary>On board NUCLEO-C562RE.</summary>

  | Board pin   | MCU pin | Signal name          | ARDUINO <br> connector pin |User Label|
  | :---:       | :---:   | :---:                | :---:                      |          |
  | CN8-3       | PA4     | ADC1_INP4            | A2                         |PA4       |
  | CN5_6       | PA5     | LED                  | D13                        |PA5       |
 
   The example uses ADC1 on channel ADC1_IN4.
  
  </details>
  <details>
  <summary>On board NUCLEO-C5A3ZG.</summary>

  | Board pin   | MCU pin | Signal name          | ARDUINO <br> connector pin |User Label|
  | :---:       | :---:   | :---:                | :---:                      |          |
  | CN8-3       | PA4     | ADC1_INP4            | A2                         |PA4       |
  | CN5_6       | PA5     | LED                  | D13                        |PA5       |

  The example uses ADC1 on channel ADC1_IN4.
  
  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

__Warning__: When debugging the ADC, any read to Data Register (DR) clears the End of Conversion flag (EOC).\
For instance, debugging step by step with any register inspection view, might break the LL/example behavior.

The __ADC conversion precision__ is highly dependent on the sampling time, which should be adjusted according to the input signal
  characteristics.

***Sampling time***:
      Before starting a conversion, the ADC must establish a direct connection between the voltage source under
      measurement and the embedded sampling capacitor of the ADC. This sampling time must be long enough for the input
      voltage source to charge the embedded capacitor to the input voltage level.

__Error handling__: In LL examples, error handling is controlled by the USE_LL_APP_ERROR constant in the application files to reduce code footprint. This compilation flag is disabled by default. If the example does not behave as expected, enable error handling for debugging by setting USE_LL_APP_ERROR to 1 in ll_example.h.

__Timeout management__: Polling flag instructions can cause the example to enter an infinite loop. To prevent this, a timeout mechanism is implemented. When the timeout is reached, the program exits the loop and reports the error at the application level. This mechanism is controlled by the USE_LL_APP_TIMEOUT compilation flag, which is disabled by default to reduce code footprint. If the example execution appears to be stuck in an infinite loop, enable this mechanism for debugging by setting USE_LL_APP_TIMEOUT to 1 in ll_example.h.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

- [Application Note AN2834](https://www.st.com/content/ccc/resource/technical/document/application_note/group0/3f/4c/a4/82/bd/63/4e/92/CD00211314/files/CD00211314.pdf/jcr:content/translations/en.CD00211314.pdf): How to get the best ADC accuracy in STM32 microcontrollers

- [Application Note AN3116](https://www.st.com/content/ccc/resource/technical/document/application_note/c4/63/a9/f4/ae/f2/48/5d/CD00258017.pdf/files/CD00258017.pdf/jcr:content/translations/en.CD00258017.pdf): STM32's ADC modes and their applications

- [Application Note AN4195](https://www.st.com/resource/en/application_note/an4195-stm32f30x-adc-modes-and-application-stmicroelectronics.pdf): STM32F30x ADC modes and application

- [Application Note AN5346](https://www.st.com/content/ccc/resource/technical/document/application_note/group1/02/ba/86/7e/6c/d7/4e/08/DM00625282/files/DM00625282.pdf/jcr:content/translations/en.DM00625282.pdf): STM32G4 ADC use tips and recommendations

- [Application Note AN2668](https://www.st.com/content/ccc/resource/technical/document/application_note/c5/24/7d/f6/98/7f/4c/f3/CD00177113.pdf/files/CD00177113.pdf/jcr:content/translations/en.CD00177113.pdf): Improving STM32F1 Series, STM32F3 Series and STM32Lx Series
 ADC resolution by oversampling

- [Application Note AN4073](https://www.st.com/content/ccc/resource/technical/document/application_note/a0/71/3e/e4/8f/b6/40/e6/DM00050879.pdf/files/DM00050879.pdf/jcr:content/translations/en.DM00050879.pdf): How to improve ADC accuracy when using STM32F2xx and STM32F4xx microcontrollers

- [Application Note AN4629](https://www.st.com/content/ccc/resource/technical/document/application_note/33/e1/e4/5c/aa/67/4c/74/DM00150423.pdf/files/DM00150423.pdf/jcr:content/translations/en.DM00150423.pdf): ADC hardware oversampling for microcontrollers of the STM32 L0 and L4 series


- [Application Note AN5354](https://www.st.com/content/ccc/resource/technical/document/application_note/group1/11/72/be/05/cd/94/44/5b/DM00628458/files/DM00628458.pdf/jcr:content/translations/en.DM00628458.pdf): Getting started with the STM32H7 Series MCU 16-bit ADC

- [Training](https://www.st.com/content/ccc/resource/training/technical/product_training/1e/0f/65/34/7e/b0/4e/ca/STM32L4_Analog_ADC.pdf/files/STM32L4_Analog_ADC.pdf/jcr:content/translations/en.STM32L4_Analog_ADC.pdf): STM32L4-Analog-ADC(ADC)

- [Training](https://www.st.com/content/ccc/resource/training/technical/product_training/group0/0b/e4/af/01/4a/92/44/dc/STM32F7_Analog_ADC/files/STM32F7_Analog_ADC.pdf/jcr:content/translations/en.STM32F7_Analog_ADC.pdf): STM32F7_Analog_ADC

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
