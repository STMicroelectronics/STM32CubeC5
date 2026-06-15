*** Comments ***
[Documentation]    This file contains the applicative constants used by hal_spi_full_duplex_two_boards_com_it_controller


*** Variables ***
# The transmitted SPI messages are null-terminated strings.
# \x00 is the ASCII code character for the null character '\0'.
${MESSAGE_PING}=    SPI Full Duplex Two Boards Communication - Message A\x00
${MESSAGE_PONG}=    SPI Full Duplex Two Boards Communication - Message B\x00
