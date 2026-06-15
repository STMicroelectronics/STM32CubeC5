*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_opamp_standalone.
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

*** Keywords ***
Example Step 1
    [Documentation]    OPAMP Configuration
    ...    Checks that OPAMP initialization runs without error
    Comment    Nothing to test here

Example Step 2
    [Documentation]    OPAMP Ready
    ...    Checks that OPAMP works as expected
    Comment    The purpose of the test is to demonstrate a use case in standalone mode, for which we have selected comparator mode.
    Comment    If VINP is lower than VINM, the output VOUT should be at 0 V.
    Write Voltage    MX_EXAMPLE_OPAMP_VINP_PIN    1000mV
    Write Pin    MX_EXAMPLE_OPAMP_VINM_PIN    1
    Check Voltage    MX_EXAMPLE_OPAMP_VOUT_PIN     0     0.1

    Comment    If VINP is higher than VINM, the OPAMP output VOUT should be at 3.3 V.
    Write Voltage    MX_EXAMPLE_OPAMP_VINP_PIN    3000mV
    Write Pin    MX_EXAMPLE_OPAMP_VINM_PIN    0
    Check Voltage    MX_EXAMPLE_OPAMP_VOUT_PIN     3.2     3.3

    ${active_level} =    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
