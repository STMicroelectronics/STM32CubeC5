*** Comments ***
[Documentation]    This file contains the applicative constants used by hal_uart_two_boards_com_polling_controller


*** Variables ***
# UART transmit & receive timeout period in seconds.
${RX_TIMEOUT}=      1
# The exchanged UART messages are null-terminated strings.
${MESSAGE_PING}=    UART Two Boards Communication - Message A\x00
${MESSAGE_PONG}=    UART Two Boards Communication - Message B\x00
