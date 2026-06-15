*** Comments ***
[Documentation]    This file contains the applicative constants used by hal_uart_two_boards_com_polling_responder


*** Variables ***
${MAX_ATTEMPTS}=        10
${EXAMPLE_TIMEOUT}=     1.1    # example timeout (s) between failed attempts (+0.1s for a safety margin)
# The exchanged UART messages are null-terminated strings.
${MESSAGE_PING}=        UART Two Boards Communication - Message A\x00
${MESSAGE_PONG}=        UART Two Boards Communication - Message B\x00
