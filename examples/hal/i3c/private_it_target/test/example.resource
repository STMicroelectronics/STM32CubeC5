*** Comments ***
[Documentation]    This file contains the applicative constants used by example_hal_i3c_private_it_target


*** Variables ***
${TARGET_ADDRESS}=          32
${MAX_ATTEMPTS}=            10
${MAX_ATTEMPTSINI}=         2
# The exchanged I3C messages are null-terminated strings.
${MESSAGE_PING}=            I3C Two Boards Communication - Message A\x00
${MESSAGE_PONG}=            I3C Two Boards Communication - Message B\x00
