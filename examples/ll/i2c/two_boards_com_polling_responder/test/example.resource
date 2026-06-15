*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.


*** Settings ***
Documentation        This file contains the applicative constants used by example_ll_i2c_two_boards_com_polling_responder


*** Variables ***
${CONTROLLER_ADDRESS}=      5D
${TARGET_ADDRESS}=          3F
${MAX_ATTEMPTS}=            10
${BAUDRATE}=                1Mbds
# The exchanged I2C messages are null-terminated strings.
# \x00 is the ASCII code character for the null character '\0'.
${MESSAGE_PING}=            I2C Two Boards Communication - Message A\x00
${MESSAGE_PONG}=            I2C Two Boards Communication - Message B\x00
