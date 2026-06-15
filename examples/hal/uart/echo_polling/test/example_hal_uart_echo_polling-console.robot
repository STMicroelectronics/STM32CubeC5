*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_uart_echo_polling.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.Console
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_uart


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs correctly
    ...    and respects the restrictions on the input line.

    Comment    Tests to check the end of line characters.
    Echo Test With Control Character    New line character    \n
    Echo Test With Control Character    Carriage return character    \r
    Echo Test With Control Character    Backspace character    \x08

    Comment    Test to check the maximum length of the input line (26 characters by default).
    Standard Echo Test    abcdefghijklmnopqrstuvwxyz


*** Keywords ***
Standard Echo Test
    [Documentation]    A standard echo test that writes a line and waits
    ...    to receive it back.
    [Arguments]    ${console_input}
    Read Console Until    [input] <<
    Write Console    ${console_input}
    Read Console Until    ${console_input}

Echo Test With Control Character
    [Documentation]    An echo test with a control character to mark the end
    ...    of the input line.
    [Arguments]    ${console_input}    ${control_character}
    Read Console Until    [input] <<
    Write Console    ${console_input}
    Write Console    ${control_character}
    Read Console Until    ${console_input}
