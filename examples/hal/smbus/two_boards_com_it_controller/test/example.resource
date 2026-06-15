*** Comments ***
[Documentation]    This file contains the applicative constants used by hal_smbus_two_boards_com_it_controller


*** Variables ***
${TARGET_ADDRESS}=      3F
${MAX_ATTEMPTS}=        10
${BAUDRATE}=            1Mbds
# The exchanged SMBUS messages are null-terminated strings.
${MESSAGE_PING}=        SMBUS Two Boards Communication - Message A\x00
${MESSAGE_PONG}=        SMBUS Two Boards Communication - Message B\x00
