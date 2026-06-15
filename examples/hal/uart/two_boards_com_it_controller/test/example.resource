*** Comments ***
[Documentation]    This file contains the applicative constants used by hal_uart_two_boards_com_it_controller


*** Variables ***
${RX_TIMEOUT}=          2
# The exchanged UART messages are null-terminated strings.
${MESSAGE_PING}=        UART Two Boards Communication - Message A\x00
${MESSAGE_PONG}=        UART Two Boards Communication - Message B\x00
${WRONG_MESSAGE} =      UART Two Boards Communication - Message C\x00
