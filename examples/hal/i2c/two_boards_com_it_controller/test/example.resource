*** Comments ***
[Documentation]    This file contains the applicative constants used by hal_i2c_two_boards_com_it_controller


*** Variables ***
${TARGET_ADDRESS}=      3F
${MAX_ATTEMPTS}=        10
${BAUDRATE}=            1Mbds
# The exchanged I2C messages are null-terminated strings.
${MESSAGE_PING}=        I2C Two Boards Communication - Message A\x00
${MESSAGE_PONG}=        I2C Two Boards Communication - Message B\x00
