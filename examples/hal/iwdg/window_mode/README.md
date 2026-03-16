<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_iwdg_window_mode*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the IWDG HAL API with the window mode to refresh the watchdog and let it reset the
MCU on purpose.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: Initializes the IWDG by app_init(). Due to the initialization parameters,
the value of the RELOAD corresponds to 1 second.

__Step 2__: The IWDG is refreshed inside the window a certain number of times (PROCESS_LOOP_NUMBER).

__Step 3__: The watchdog is refreshed outside the window to trigger the IWDG reset.

__Step 4__: The device is restarted due to IWDG.

__End of example__: After step 4, the example is completed. You can verify that the example runs properly via the user
LED and the `ExecStatus` variable.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:
```text
[INFO] Step 1: Device initialization COMPLETED.
[INFO] Step 2: IWDG refreshed.
[INFO] Step 2: IWDG refreshed.
[INFO] Step 2: IWDG refreshed.
[INFO] Step 3: IWDG refreshed outside the window.
[INFO] Step 4: Device's restart completed due to IWDG.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

IWDG: configuration with a reload value to have a timeout of 1 second for the watchdog and a window value of 400 milliseconds.
The RELOAD value is calculated as follows: RELOAD = t(s) x f_LSI(Hz) / PRESCALER.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

The LSI frequency used for the RELOAD parameter calculation depends on the board and setup you are using:
<details>
  <summary>On board with STM32U5xx MCUs</summary>

The LSI frequency is equal to 32kHz. The value for the LSI frequency can change if you change the LSI clock divider,
which is by default equal to 1.
During the IWDG initialization, the PRESCALER is set to 32. With this configuration, the RELOAD value is equal to:

> RELOAD = t(s) x f_LSI(Hz) / PRESCALER = 1000.

</details>

### __3.2. Specific board setups__

No specific board setup needed for this example.


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example.

__IWDG initialization__: If the following message is displayed when the basic trace utility is used, you need to check the IWDG initialization.
```text
[ERROR] The IWDG is supposed to reset before reaching this stage due to the Step 3.
```

__Low reload value and the basic trace utility__: If the reload value of the watchdog is set too low whereas the basic
trace utility is used, the example behavior could be impacted due to the CPU cycles taken to generate logs in the process
loop. This value also depends on the board used and the LSI precision of the target. It is recommended to set the reload
value bigger than 60 to handle this problem or to disable the basic trace utility.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

This [application note](https://www.st.com/resource/en/application_note/an5701-stm32cube-mcu-package-examples-for-stm32u5-series-stmicroelectronics.pdf)
explains further this IWDG feature.

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
