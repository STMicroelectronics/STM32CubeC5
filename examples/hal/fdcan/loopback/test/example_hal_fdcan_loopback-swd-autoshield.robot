*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_fdcan_loopback.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_fdcan


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.

    Example Step 1
    Example Step 2
    Example Step 3
    Example End


*** Keywords ***
Example Step 1
    [Documentation]    Configures and initializes the FDCAN instance.
    ...    Checks that initialization (app_init) runs without error
    Comment    Checking ExecStatus and the LED status is not currently possible,
    ...    as the example immediately starts FDCAN communications

Example Step 2
    [Documentation]    sends several FDCAN messages.
    Comment    Nothing to be tested here.

Example Step 3
    [Documentation]    receives back and check the FDCAN messages
    Comment    Nothing to be tested here.

Example End
    [Documentation]    The final flags must be correctly set.
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Comment    Wait for 1s max the LED is ON then check it matches the success pattern
    Check Pin    MX_STATUS_LED    ${active_level}    timeout=1s
    Check Pin    MX_STATUS_LED    ${active_level}    duration=250ms
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
