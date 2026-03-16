<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_adc_multimode_simultaneous_dma_multi_buffers*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use two ADC instances (master and slave) in dual ADC mode (regular simultaneous mode) with the HAL API to perform conversions triggered by a timer. The converted data is transferred using DMA to separate buffers for each ADC.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: The ADC master and ADC slave are configured to convert a single channel, using DMA transfer and timer trigger. Both ADC
instances are configured in dual ADC mode (regular simultaneous mode).

__Step 2__: The ADC autocalibration is performed on both ADC instances to ensure accurate measurements. The calibration is a
prerequisite to any ADC operation. It removes the systematic errors that may vary from chip to chip.

__Step 3__:  Configures one data buffer for each ADC instance. Start the ADC conversion in multimode DMA mode, multibuffers mode. A timer is configured in time base to generate TRGO events.

__Step 4__: Waits for the interrupt service routine to be called indicating that the data buffer is filled. Then stops the
ADC, DMA, and Timer.

__Step 5__: Computes the corresponding voltage for both ADCs and prints the ADC raw value and the computed voltage.

__End of example__: The ADC conversion is repeated every second endlessly (step 3 to step 5 are running in a loop).
The example status is reported via the variable **`ExecStatus`**, and the status LED
remains turned on in case of success.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```test
[INFO] Step 1: ADC initialization COMPLETED.
[INFO] Step 2: ADC start and calibration COMPLETED.
[INFO] Step 3: ADC start conversion COMPLETED.
[INFO] Step 4: DMA transfer COMPLETED for both ADCs.
[INFO] Step 5: Print voltage buffer for both ADCs.
[INFO] ADC_Master[0] = 3300mV (raw value: 0xfff), ADC_Slave[0] = 3295mV (raw value: 0xff9)
[INFO] ADC_Master[1] = 3300mV (raw value: 0xfff), ADC_Slave[1] = 3296mV (raw value: 0xffb)
[INFO] ADC_Master[2] = 3300mV (raw value: 0xfff), ADC_Slave[2] = 3290mV (raw value: 0xff3)
[INFO] Step 3: ADC conversion STARTED.
[INFO] Step 4: DMA transfer COMPLETED for both ADCs.
[INFO] Step 5: Print voltage buffer for both ADCs.
[INFO] ADC_Master[0] = 733mV (raw value: 0x38e), ADC_Slave[0] = 730mV (raw value: 0x38b)
[INFO] ADC_Master[1] = 740mV (raw value: 0x397), ADC_Slave[1] = 738mV (raw value: 0x394)
[INFO] ADC_Master[2] = 722mV (raw value: 0x380), ADC_Slave[2] = 716mV (raw value: 0x379)
...
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

__ADC__: 2 ADCs are configured with the following parameters:

- Single channel conversion.
- Trigger Timer.
- Dual ADC mode (regular simultaneous mode).
- DMA transfer (One buffer per ADC).
The two chosen ADCs must share the same ADC common instance.


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
  <summary>On board NUCLEO-C562RE.</summary>

  | Board connector | MCU pin | Signal name | ARDUINO <br/> connector pin |
  | :-------------: | :-----: | :---------: | :------------------------: |
  | CN8_3           | PA4     | ADC1_IN4    | A2                         |
  | CN8_5           | PC5     | ADC2_IN5    | A4                         |
  | CN5_6           | PA5     | LED         | D13                        |

</details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

__Warning__: When debugging the ADC, any read to Data Register (DR) clears the End of Conversion flag (EOC).\
For instance, debugging step by step with any register inspection view, might break the HAL/example behavior.

The __ADC conversion precision__ is highly dependent on the sampling time, which should be adjusted according to the input signal characteristics.

__Sampling time__:
      Before starting a conversion, the ADC must establish a direct connection between the voltage source under
      measurement and the embedded sampling capacitor of the ADC. This sampling time must be long enough for the input
      voltage source to charge the embedded capacitor to the input voltage level.

__Warning__: The analog voltage reference, VREF+, may be different from VDD. In such a case, it may be necessary to adjust
    its definition:\
    ```text
      #define VREFPLUS_APPLI    3300UL
    ```

__Linked ADC instances__:

  - The ADC instances must share the same ADC common instance. This ensures synchronized operation and proper coordination between the ADC peripherals.
  - Although the ADCs operate in dual mode, each ADC instance uses its own dedicated DMA channel.
  - Two separate flags, ``NewDataAdcPrimary`` and ``NewDataAdcSecondary``, are used to indicate the completion of conversion for each ADC instance. These flags should be set almost simultaneously (within a few CPU cycles).
  - Maintaining separate flags for each ADC instance is beneficial for debugging purposes: these flags help identify which ADC or DMA channel is causing the problem.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

- [Application Note AN2834](https://www.st.com/content/ccc/resource/technical/document/application_note/group0/3f/4c/a4/82/bd/63/4e/92/CD00211314/files/CD00211314.pdf/jcr:content/translations/en.CD00211314.pdf): How to get the best ADC accuracy in STM32 microcontrollers

- [Application Note AN3116](https://www.st.com/content/ccc/resource/technical/document/application_note/c4/63/a9/f4/ae/f2/48/5d/CD00258017.pdf/files/CD00258017.pdf/jcr:content/translations/en.CD00258017.pdf): STM32's ADC modes and their applications

- [Application Note AN4195](https://www.st.com/resource/en/application_note/an4195-stm32f30x-adc-modes-and-application-stmicroelectronics.pdf): STM32F30x ADC modes and application

- [Application Note AN5346](https://www.st.com/content/ccc/resource/technical/document/application_note/group1/02/ba/86/7e/6c/d7/4e/08/DM00625282/files/DM00625282.pdf/jcr:content/translations/en.DM00625282.pdf): STM32G4 ADC use tips and recommendations

- [Application Note AN2668](https://www.st.com/content/ccc/resource/technical/document/application_note/c5/24/7d/f6/98/7f/4c/f3/CD00177113.pdf/files/CD00177113.pdf/jcr:content/translations/en.CD00177113.pdf): Improving STM32F1 series, STM32F3 series and STM32Lx series
 ADC resolution by oversampling

- [Application Note AN4073](https://www.st.com/content/ccc/resource/technical/document/application_note/a0/71/3e/e4/8f/b6/40/e6/DM00050879.pdf/files/DM00050879.pdf/jcr:content/translations/en.DM00050879.pdf): How to improve ADC accuracy when using STM32F2xx and STM32F4xx microcontrollers

- [Application Note AN4629](https://www.st.com/content/ccc/resource/technical/document/application_note/33/e1/e4/5c/aa/67/4c/74/DM00150423.pdf/files/DM00150423.pdf/jcr:content/translations/en.DM00150423.pdf): ADC hardware oversampling for microcontrollers of the STM32L0 and L4 series


- [Application Note AN5354](https://www.st.com/content/ccc/resource/technical/document/application_note/group1/11/72/be/05/cd/94/44/5b/DM00628458/files/DM00628458.pdf/jcr:content/translations/en.DM00628458.pdf): Getting started with the STM32H7 series MCU 16-bit ADC

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
