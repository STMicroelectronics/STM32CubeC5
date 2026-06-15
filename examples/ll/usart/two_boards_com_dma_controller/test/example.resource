*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.


*** Settings ***
Documentation       This file contains the applicative constants used by example_ll_usart_two_boards_com_dma_controller

*** Variables ***
${MESSAGE_WHEN_READING}=  \x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00
# The exchanged USART messages are null-terminated strings.
${MESSAGE_PING}=        USART Two Boards Communication - Message A\x00
${MESSAGE_PONG}=        USART Two Boards Communication - Message B\x00

