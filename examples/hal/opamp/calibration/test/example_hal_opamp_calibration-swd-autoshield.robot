*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_opamp_calibration.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_opamp


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3
    Example Step 4
    Example Step 5
    Example Step 6
    Example Step 7
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    configures the OPAMP peripheral.
    Comment    Verify ExecStatus value is EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after the init.

Example Step 2
    [Documentation]    Retrieve the default trimming values and verify that the trimming mode is set to factory mode.
    Comment    No check possible

Example Step 3
    [Documentation]    Perform self-calibration of the OPAMP.
    Comment    No check possible

Example Step 4
    [Documentation]    Retrieve the trimming values after calibration and confirm that the mode has switched to user mode.
    Comment    No check possible

Example Step 5
    [Documentation]    Modify the OPAMP settings by making a dummy change, resetting the trimming offset to simulate a second calibration.
    Comment    No check possible

Example Step 6
    [Documentation]    Perform a second calibration of the OPAMP.
    Comment    No check possible

Example Step 7
    [Documentation]    Retrieve the trimming values and verify that the trimming mode remains unchanged and is set to user mode.
    Comment    No check possible

End Of Example
    [Documentation]    Check ExecStatus and Led Status
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    timeout=10s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
