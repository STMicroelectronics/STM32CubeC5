*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.


*** Settings ***
Documentation        This file contains the applicative constants used by hal_spi_full_duplex_two_boards_crc_com_it_controller


*** Variables ***
# The transmitted USART messages are null-terminated strings.
# \x00 is the ASCII code character for the null character '\0'.
${MESSAGE_PING}=    USART Two Boards Communication - Message A\x00
${MESSAGE_PONG}=    USART Two Boards Communication - Message B\x00
