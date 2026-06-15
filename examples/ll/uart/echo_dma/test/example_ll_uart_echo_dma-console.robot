*** Comments ***
Copyright (c) 2025 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_ll_uart_echo_dma.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.Console
Variables           robot_for_stm32.cube_examples_constants
Resource            example.resource

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_uart    


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs correctly
    ...    and respects the restrictions on the input line.

    Comment    Test to check the maximum length of the input line (10 characters by default).
    Standard Echo Test    ${START_MESSAGE}    ${INPUT_CHARACTERS}


*** Keywords ***
Standard Echo Test
    [Documentation]    A standard echo test that writes a line and waits
    ...    to receive it back.
    [Arguments]    ${START_MESSAGE}    ${INPUT_CHARACTERS}
    Read Console Until    ${START_MESSAGE}
    Write Console    ${INPUT_CHARACTERS}
    Read Console Until    ${INPUT_CHARACTERS}
