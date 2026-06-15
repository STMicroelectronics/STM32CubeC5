*** Comments ***
[Documentation]    This file contains the applicative constants used by hal_i3c_private_it_controller


*** Variables ***
${TARGET_ADDRESS}=      32

# The exchanged I3C messages are null-terminated strings.
${MESSAGE_PING}=        I3C Two Boards Communication - Message A\x00
${MESSAGE_PONG}=        I3C Two Boards Communication - Message B\x00
