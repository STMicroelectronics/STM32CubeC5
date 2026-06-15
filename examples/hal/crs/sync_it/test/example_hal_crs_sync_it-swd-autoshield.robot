*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_crs_sync_it.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants
Resource            example.resource

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_crs


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.

    Example Step 1
    Example Step 2
    Example Step 3


*** Keywords ***
Example Step 1
    [Documentation]    The init of the CRS instance is triggered by the applicative code. The HSI48 clock is configured to be output on the MCO pin.
    ...    Checks that initialization (app_init) runs without error
    Comment    Nothing to check

Example Step 2
    [Documentation]    Starts the CRS synchronization
    Comment    Nothing to check

Example Step 3
    [Documentation]    Checks the synchronization is finished
    ${MCO_DIVIDER}=    Read Test Variable    MCO_DIVIDER
    ${MCO_DIVIDER}=    Convert To Integer    ${MCO_DIVIDER}

    IF    ${MCO_DIVIDER} == 0
        Comment    MCO output pin is not accessible
    ELSE
        Check Frequency    mco_pin    ${{$HSI48_FREQ / $MCO_DIVIDER}}    precision=0.1    timeout=1000ms
    END

    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    timeout=1s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=5s
