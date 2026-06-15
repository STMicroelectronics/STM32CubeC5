*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_cordic_sincos_it.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_cordic


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3
    Example Step 4
    Example Step 5
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    The applicative code initializes the CORDIC instance
    Comment    transient    ExecStatus, no check possible

Example Step 2
    [Documentation]    Prepares the input values for CORDIC: angles must be in radians divided by pi
    ...    and presented in q1.31 format.
    Comment    No Check

Example Step 3
    [Documentation]    Starts CORDIC calculation in IT mode.
    Comment    No Check.

Example Step 4
    [Documentation]    Waits for the CORDIC read it transfer completion.
    ...    Then compares the CORDIC results to the reference values.
    Check Integer Variable    CordicCalculationCompleted    1    timeout=1s

Example Step 5
    [Documentation]    Deinitializes the CORDIC instance before leaving the scenario.
    ...    Checks that uninitialization (app_deinit) runs without error
    Comment    No Check

End Of Example
    [Documentation]    Verify successful end of example
    Comment    Verify ExecStatus value is EXEC_STATUS_OK
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}

    Comment    Verify STATUS LED is in success mode
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED_PIN    ${active_level}    duration=1s
