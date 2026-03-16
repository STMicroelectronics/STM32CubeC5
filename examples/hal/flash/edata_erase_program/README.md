<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_flash_edata_erase_program*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the HAL FLASH API to erase and program EDATA pages in the flash memory.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, MPU regions, and the system clock.

The application executes the following __example steps__:

__Step 1__: Initializes the flash instance and unlocks the access to the flash control register.

__Step 2__: Programs the flash EDATA area. Checks that the data is written in the desired location.

__Step 3__: Erases the flash EDATA area. Tries to read back the erased data.

__Step 4__: Manage the ECC error generated when reading back erased EDATA memory.

__Step 5__: Back from the NMI handler, compare the read data with the expected data.

__Step 6__: FLASH peripheral deinitialization.

__End of example__: After step 6, the example is completed. You can verify that the example runs properly via the status LED and the `ExecStatus` variable.

If an abnormal NMI error is detected during the example flow, the code remains stuck in the while(1) loop inside the NMI_Handler() function.

You can also check the contents of the memory after programming it.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO]  Step 1: Device initialization COMPLETED within EDATA area is ENABLED.
[ERROR] Step 1: Device initialization ERROR: Flash EDATA area is still DISABLED.
[INFO]  Step 2: EDATA area programming COMPLETED.
[INFO]  Step 5: EDATA area is ERASED.
[INFO]  Step 6: de-init.
...
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

__Option Bytes__

The flash EDATA area must be enabled through option bytes before using it.

[See section 3.1: Generic Setup](#31-generic-setup).

__EDATA area and instruction cache__

The flash EDATA area is configured as data memory.
When instruction cache is enabled in the application, 16/32-bit data access to the EDATA area is not allowed.

[See section 3.2: Specific board setups](#32-specific-board-setups) to manage this situation when applicable.

__EDATA management__

For dual bank devices, it is recommended to use pages on the other bank for the flash EDATA area. Thus, the application benefits from the RWW capability of the dual-bank arrangement.

The flash EDATA area is protected by ECC.
Operations on the flash EDATA area must obey the flash programming sequence as described in the reference manual of the STM32 device.

[See section 3.2: Specific board setups](#32-specific-board-setups) for further details.

__HAL configuration file__

The compilator switch USE_HAL_FLASH_OB_EDATA MUST be set to 1 inside the stm32c5xx_hal_conf.h file.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

Before running the example, ensure that the EDATA memory is activated through option bytes. Follow these steps:

- Connect the device to *STM32CubeProgrammer*.
- Go to *Option bytes*.
- Select *User Configuration*.
- Check *EDATA_EN* option.
- Go to *Erase and Programming*.
- Perform a *Full chip erase*.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<details>
  <summary>On STM32C5 series.</summary>
  <details>
    <summary>EDATA area and ICACHE</summary>

  By disabling instruction access to the EDATA area, the ICACHE does not manage this area.
      This ensures proper data access.
      The MPU region is set up as follows:

      Base Address: FLASH_EDATA_BASE (e.g., 0x09000000),
      Limit Address: FLASH_EDATA_BASE + (2 * FLASH_EDATA_BANK_SIZE) - 1,
      Size: 2 * FLASH_EDATA_BANK_SIZE,
      Access Permission: Full access (HAL_CORTEX_MPU_REGION_ALL_RW),
      Execution: Disabled

  </details>

  <details>
    <summary>EDATA management</summary>

  When the EDATA_EN option bit is set, the EDATA memory row is 137 bits long: 128-bit + 9-bit ECC.
      When erasing an EDATA area, all bits are set to 1.
      This implies that the ECC bits are also set to 1. This is not a valid ECC code.
      Therefore, when reading back erased EDATA memory, an ECC error is generated.

  </details>

  <details>
    <summary>On board NUCLEO-C542RC.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PA2    |   USART2_TX   |      PA2      |

  </details>

  <details>
    <summary>On board NUCLEO-C562RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |    OSC_IN     |
  |    PH1    |  RCC_OSC_OUT  |    OSC_OUT    |
  |    PA2    |   USART2_TX   |      PA2      |

  </details>

  <details>
    <summary>On board NUCLEO-C5A3ZG.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PA5    |     GPIO      | MX_STATUS_LED |
  |    PH0    |  RCC_OSC_IN   |  PH0_OSC_IN   |
  |    PH1    |  RCC_OSC_OUT  |  PH1_OSC_OUT  |
  |    PA2    |   USART2_TX   | DBGIN_VCP_TX  |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

__EDATA activation:__ Check the *EDATA* option bytes using *STM32CubeProgrammer*.

__Erasing and Programming:__ In case of issues, perform a *Full chip erase* before running the example again.

__ECC errors__: When reading back erased EDATA memory, an ECC error is generated. This is expected behavior as explained in [section 3.2: Specific board setups](#32-specific-board-setups).

__EDATA access with ICACHE enabled__: When instruction cache is enabled in the application, 16/32-bit data access to the EDATA area is not allowed. Ensure that the MPU region is configured to disable instruction access to the EDATA area as described in [section 3.2: Specific board setups](#32-specific-board-setups).


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
