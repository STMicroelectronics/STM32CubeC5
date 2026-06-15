*** Comments ***
[Documentation]    This file contains the applicative constants used by hal_i2c_two_boards_com_dma_responder


*** Variables ***
${CONTROLLER_ADDRESS}=      5D
${TARGET_ADDRESS}=          3F
${MAX_ATTEMPTS}=            10
${BAUDRATE}=                1Mbds
# The exchanged I2C messages are null-terminated strings.
# \x00 is the ASCII code character for the null character '\0'.
${MESSAGE_PING}=            I2C Two Boards Communication - Message A\x00
${MESSAGE_PONG}=            I2C Two Boards Communication - Message B\x00
