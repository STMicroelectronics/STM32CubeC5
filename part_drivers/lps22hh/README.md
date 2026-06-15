# __Board Parts: LPS22HH__

## __Summary__

Part LPS22HH provides APIs to drive the LPS22HH component through I²C or SPI.

## __Description__

### Peripherals initialization:

Part LPS22HH assumes that the initialization of all needed peripherals (I2C, SPI, GPIO) is done by the main application.
After the MCU IPs have been initialized, the selected resources can be assigned to LPS22HH objects. This is done in `lps22hh_io_init()`, which the user application must override.
That function is called by `lps22hh_drv_init()`; the `.id` field of the `lps22hh_io_t` structure should be used to discriminate the resources to be assigned.

## __How to use it?__

To use the LPS22HH part API, the application should define the `lps22hh_object_t` object and the `lps22hh_io_init()` function to assign hardware resources to it.
The link between the two is made by `lps22hh_drv_init()`, which should be called after all the MCU peripherals have been initialized.

Once this is successfully done, all the other part APIs can be used freely.

## __Contributing__

STM32 customers and users who want to contribute to this component can follow instructions on the [STMicroelectronics GitHub page]( https://github.com/STMicroelectronics)

## __Keywords__
- STM32
- BSP
- Parts
- LPS22HH

## __License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
