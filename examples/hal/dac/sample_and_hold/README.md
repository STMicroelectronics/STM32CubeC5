<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_dac_sample_and_hold*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the Cube firmware package.")

How to use the sample and hold feature of the DAC to perform a simple conversion in low power mode.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: The DAC is initialized and the sample and hold mode is enabled. The DAC outputs a voltage.

__Step 2__: The device enters the STOP mode. The DAC continues to output the signal thanks to the
sample and hold feature. DAC performs conversion even in low power mode like STOP mode.

__End of example__: The device stays in low power mode and the DAC maintains the output voltage using the sample and hold
feature.
The example status is reported via the variable **`ExecStatus`**, and the status led remains turned on in case of success.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the Cube firmware package.")

__DAC__: The configuration of the peripheral configures a channel of the DAC with specific parameters:
  - No trigger for conversion
  - 8-bits DAC data are used with right alignment
  - Output buffer disabled
  - Clock source: the DAC clock source must be configured with LSI or LSE (when present) to set the dac_hold_ck that clocks the sample and hold registers.
  - Delays for sample and hold mode are equal (according to the reference manual) to:
    - Sampling_time   = (Sample_reg + 1) / dac_hold_ck_freq
    - Holding_time    = Hold_reg / dac_hold_ck_freq
    - Refresh_time    = Refresh_reg x dac_hold_ck_freq

    So, registers must be set like:
    - Sample_reg  = (Sampling_time * dac_hold_ck_freq) - 1
    - Hold_reg    = Holding_time * dac_hold_ck_freq
    - Refresh_reg = Refresh_time * dac_hold_ck_freq

    These delays depend on the load at the output of the DAC and the buffer state of the DAC, according to:
    - Sampling_time   = 3 us + (10 * R_buff_off * C_sh)
    - Refresh_time    = 3 us + (R_buff_off * C_sh) * ln(2 * N_LSB)
    - Holding_time    = D_v * C_sh / i_leak
    with:
      - *R_buff_off*: the DAC impedance when the output buffer is disabled
      - *C_sh": the capacitance wired at the output of the DAC and chosen by the user
      - *N_LSB*: the number of LSB that dropout during the hold phase
      - *D_v*: the dropping voltage linked with the LSB, VDDA, and DAC resolution by:
        > D_v = VDDA / (2^DAC_resolution - 1) * N_LSB
      - *i_leak*: the current leakage

    *Note*: These formulas do not consider an external resistor load.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

The hardware setup of clocks and timings applies to any board with the same clock tree.
<details>

  <summary>On board with STM32C5xx MCUs</summary>

  <summary>Common configuration</summary>

  The autonomous mode feature of the DAC needs to be enabled.

  Configuration of the timer counter clock:
  - The DAC clock when the sample and hold feature is used (dac_hold_ck) is set to the LSI clock.
  - The LSI clock is set to 32 kHz since the LSI prescaler is set to 1.

  DAC supply voltage:
  - The DAC uses VDDA as the reference voltage (Vref)
  - The analogic supply voltage is VDDA = 3.3 V

  - For a hypothetical external load wired at the output of the DAC, composed of a capacitor (no resistor load):
    > C_sh = 68 nF

    According to the datasheet, the worst case of:
    - The internal impedance of the DAC is:
      > R_buff_off = 17.8 kOhms

    - The worst case for IO leakage is:
      > i_leak = 200 nA

    To get a dropout voltage corresponding to 10 LSB and that the DAC is supplied by VDDA = 3.3 V:
    > N_LSB = 5
    > D_v = 3.3 V / (2^8 - 1) * 5 = 0.065 V

    Delays calculations are:
    > Sampling_time   = 3 us + (10 * 17.8 kOhms * 68 nF)
    > Refresh_time    = 3 us + (17.8 kOhms * 68 nF) * ln(2 * 5)
    > Holding_time    = 0.065 * 68 nF / 200 nA

    > Sampling_time   = 12.1 ms
    > Refresh_time    = 2.8 ms
    > Holding_time    = 22.1 ms


  - Register values correspond to:
  > Sample_reg    = (Sampling_time * dac_hold_ck_freq) - 1  = (12.1 ms * 32 kHz) - 1 = 386
  > Refresh_reg   = Refresh_time * dac_hold_ck_freq         = 2.8 ms * 32 kHz     = 89
  > Hold_reg      = Holding_time * dac_hold_ck_freq         = 22.1 ms * 32 kz       = 707
  > (rounded to the nearest integer)


</details>

<details>
  <summary>On board with STM32H7Pxx MCUs</summary>

  <summary>Common configuration</summary>

  The autonomous mode feature of the DAC needs to be enabled.

  Configuration of the timer counter clock:
  - The DAC clock when the sample and hold feature is used (dac_hold_ck) is set to the LSI clock.
  - The LSI clock is set to 32 kHz since the LSI prescaler is set to 1.

  DAC supply voltage:
  - The DAC uses VDDA as the reference voltage (Vref)
  - The analogic supply voltage is VDDA = 3.3 V

  - For a hypothetical external load wired at the output of the DAC, composed of a capacitor (no resistor load):
    > C_sh = 68 nF

    According to the datasheet, the worst case of:
    - The internal impedance of the DAC is:
      > R_buff_off = 17.5 kOhms

    - The worst case for IO leakage is:
      > i_leak = 150 nA

    To get a dropout voltage corresponding to 10 LSB and that the DAC is supplied by VDDA = 3.3 V:
    > N_LSB = 5
    > D_v = 3.3 V / (2^8 - 1) * 5 = 0.065 V

    Delays calculations are:
    > Sampling_time   = 3 us + (10 * 17.5 kOhms * 68 nF)
    > Refresh_time    = 3 us + (17.5 kOhms * 68 nF) * ln(2 * 5)
    > Holding_time    = 0.065 * 68 nF / 150 nA

    > Sampling_time   = 11.9 ms
    > Refresh_time    = 2.7 ms
    > Holding_time    = 29.3 ms


  - Register values correspond to:
  > Sample_reg    = (Sampling_time * dac_hold_ck_freq) - 1  = (11.9 ms * 32 kHz) - 1 = 379
  > Refresh_reg   = Refresh_time * dac_hold_ck_freq         = 2.7 ms * 32 kHz     = 87
  > Hold_reg      = Holding_time * dac_hold_ck_freq         = 29.3 ms * 32 kz       = 938
  > (rounded to the nearest integer)


</details>

### __3.2. Specific board setups__

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PA4    |   DAC1_OUT1   |      PA4      |

  </details>
</details>

## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the Cube firmware package.")

Make sure that the LSI is enabled and set as the DAC clock source.

The sample and hold mode allows the MCU to charge a capacitor during the sample timing, discharge during the hold
timing, and reload the capacitor's charge during the refresh timing. These timings values need to be adapted to the
external load. The fluctuation of the output voltage depends on these delays and the load.

When exiting from STOP mode, the clock may need to be reconfigured. See STOP mode examples.

<details>
<summary>On STM32U5 series.</summary>

Make sure that the autonomous mode is enabled to be sure the RCC enabled the low-power clock to perform conversion while in STOP0 mode.

</details>


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the Cube firmware package.")

- [Application Note AN4566](https://www.st.com/content/ccc/resource/technical/document/application_note/6f/35/61/e9/8a/28/48/8c/DM00129215.pdf/files/DM00129215.pdf/jcr:content/translations/en.DM00129215.pdf): How to improve DAC performance in STM32 microcontrollers

- [Application Note AN3126](https://www.st.com/content/ccc/resource/technical/document/application_note/05/fb/41/91/39/02/4d/1e/CD00259245.pdf/files/CD00259245.pdf/jcr:content/translations/en.CD00259245.pdf): How to use DAC for audio and waveform generation in STM32 products.

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
