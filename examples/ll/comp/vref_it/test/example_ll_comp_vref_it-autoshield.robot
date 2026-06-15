*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_ll_comp_vref_it.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_comp


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3
    Example Step 4
    Example Step 3


*** Keywords ***
Example Step 1
    [Documentation]    Initialize the COMP
    ...    Check initialization (app_init) runs without error
    Comment    Nothing to be tested here

Example Step 2
    [Documentation]    Start the comparator in interrupt mode.
    Comment    Nothing to be tested here

Example Step 3
    [Documentation]    Switch to LOW POWER mode
    Comment    Nothing to be tested here

Example Step 4
    [Documentation]    Apply a voltage level on the COMP input pin.
    ...    The comparator generates an interrupt and system wakes-up from Stop mode.
    ...    The led starts flashing.
    Write Voltage    COMP_PIN    0V
    Check Pin    MX_STATUS_LED    ${LOW}
    ${Vref_internal}=    Read Test Variable    Vref_internal
    ${Tolerance_value}=    Set Variable    ${{${Vref_internal} * 0.1}}
    ${voltage_below_vref}=    Set Variable    ${{${Vref_internal} - ${Tolerance_value}}}
    ${voltage_over_vref}=    Set Variable    ${{${Vref_internal} + ${Tolerance_value}}}

    Write Voltage    COMP_PIN    ${voltage_below_vref}V
    # Check that the LED has not turned ON as the threshold has not been crossed
    Check Pin    MX_STATUS_LED    ${LOW}    timeout=0.300s

    Write Voltage    COMP_PIN    ${voltage_over_vref}V
    # Check that the LED flashes
    Check Pin    MX_STATUS_LED    ${HIGH}    timeout=0.300s
    Check Pin    MX_STATUS_LED    ${LOW}    timeout=1s

    Write Voltage    COMP_PIN    3.300V
    # Check that the LED has not turned ON as the threshold has not been crossed
    Check Pin    MX_STATUS_LED    ${LOW}    timeout=0.300s

    # Add a delay to verify that the MCU has switched to stop mode
    Sleep    100ms

    Write Voltage    COMP_PIN    0V
    # Check that the LED flashes
    Check Pin    MX_STATUS_LED    ${HIGH}    timeout=0.300s
    Check Pin    MX_STATUS_LED    ${LOW}    timeout=1s
