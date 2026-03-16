<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *ll_wwdg_reset*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the WWDG LL API to refresh the watchdog and let it reset the MCU on purpose.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: Initializes the WWDG by app_init(). Due to the initialization parameters,
the value of the WWDG_MAX_TIME_VALUE corresponds to 209 milliseconds and WWDG_MIN_TIME_VALUE corresponds to 78 milliseconds.
Both values are chosen arbitrarily and the WWDG_MIN_TIME_VALUE should be less or equal than the WWDG_MAX_TIME_VALUE.

__Step 2__: The WWDG is refreshed inside the window a certain number of times (PROCESS_LOOP_NUMBER).

__Step 3__: The watchdog is refreshed outside the window to trigger the WWDG reset.

__Step 4__: The device is restarted due to WWDG.

__End of example__: After step 4, the example is completed. You can verify that the example runs properly via the user
LED and the `ExecStatus` variable.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")
> To ensure that the WWDG operates correctly, the application must reload the counter value within the specified window. The reload operation should occur when the counter value is between the window register value and 0x3F, and the value written to the WWDG_CR register must be between 0xFF and 0xC0.
> The WWDG_TIMEOUT value is calculated as follows: `WWDG_TIMEOUT = tPCLK x 4096 x 2^WDGTB[2:0] x (T[5:0] + 1)`


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

The frequency used for the WWDG_TIMEOUT parameter calculation depends on the board and setup you are using:
<details>
  <summary>On STM32U5 series.</summary>

The APB frequency is equal to 160 MHz. The value for the frequency can change if you change the clock divider, which is by default equal to 1.
During the WWDG initialization, the WWDG_INTERNAL_DIVIDER is set to 4096, `WDGTB[2:0]` is set to 7 and `T[5:0]` is set to 63.
Below is the calculation of the WWDG_TIMEOUT based on this configuration:

> WWDG_TIMEOUT = (1 / 160000000) * 4096 * 2^7 * (63 + 1) = 209.72 ms.
> After the WWDG is refreshed, it will expire after approximately 209.71 milliseconds and generate a reset if the counter is not reloaded within this time frame.


</details>

### __3.2. Specific board setups__

No specific board setup needed for this example.


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Attention: The value of WWDG_MAX_TIME_VALUE must be less than WWDG_ALLOWED_MAX_TIME, which depends on the time unit and frequency and calculated on the driver file.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

This [application note](https://www.st.com/resource/en/application_note/an5701-stm32cube-mcu-package-examples-for-stm32u5-series-stmicroelectronics.pdf)
explains further this WWDG feature.

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
