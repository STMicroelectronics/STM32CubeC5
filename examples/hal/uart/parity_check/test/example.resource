[Documentation]        This file contains the applicative constants used by hal_uart_parity_check

*** Variables ***
${START_MESSAGE}=        \nUART ParityCheck via IT\n\rEnter 10 chars:\n
${INPUT_CHARACTERS} =        0123456789
${ERROR_MESSAGE}=        \nParity error detected. Parity set to even.\nResend 10 chars.\n
${END_MESSAGE}=        0123456789\nNo Parity error detected, end of example\n