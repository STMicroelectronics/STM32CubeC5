*** Comments ***
[Documentation]    This file contains the applicative constants used by hal_spi_full_duplex_two_boards_com_it_controller


*** Variables ***
# The transmitted SPI messages are null-terminated strings.
# Sensor request correspond to the channel (0 or 1).
${SENSOR_REQUEST}=    \x00\x00
# Sensor value correspond to 1000mV.
${SENSOR_VALUE_HEXA}=    \x04\xD9
${SENSOR_VALUE_MV}=      1000
