<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *example_oemirot_overwrite*__

**Example version:** 2.0.0

How to implement an __OEMiRoT__ boot stage in dual-slot __overwrite__ mode, relying on software cryptography.

OEMiRoT stands for *OEM Immutable Root of Trust*. It provides two services:

- __Secure Boot__: Verifies the integrity and authenticity of the application firmware before execution.
- __Secure Firmware Update__: Verifies and installs new encrypted firmware images.

Dual slot overwrite mode requires two flash areas: one for execution, one for downloading new firmware. After verification, the new image permanently overwrites the previous one.

During operation, OEMiRoT verifies static hardware protections and sets dynamic hardware protections.

The core function of OEMiRoT relies on the MCUboot middleware.
The cryptography functions rely on the STFCF middleware, using the STCryptoLib software and the HASH peripheral.


## __1. Overview__

This example contains two projects:

- **OEMiRoT**: Handles secure boot and secure firmware update.
- **User application**: The main firmware image to be protected and updated.

Download of new firmware image is performed via the application menu and YModem transfer in the terminal emulator.

Supporting Python scripts include:

- `provisioning.py`: Guides the user through configuration, build and programming steps.
- `project_appli.py`: Automates prebuild/postbuild steps.
- `project_oemirot.py`: Automates prebuild steps.


## __2. Requirements__

- Python 3.10 or later
- STM32CubeProgrammer (with STM32TrustedPackageCreator)
- Tera Term (or another terminal emulator supporting YModem)
- Required Python modules:

  ```bash
  pip install -r requirements.txt
  ```

## __3. Quickstart__

Follow [How to start with OEMiRoT on STM32C5](https://wiki.st.com/stm32mcu/wiki/Security:How_to_start_with_OEMiRoT_on_STM32C5)

## __4. Known limitations__

- Only IAR Embedded Workbench for ARM (EWARM) project supported.

## __5. See Also__

More information about OEMiRoT can be found in [OEMiRoT for STM32C5](https://wiki.st.com/stm32mcu/wiki/Security:OEMiRoT_for_STM32C5)

## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
