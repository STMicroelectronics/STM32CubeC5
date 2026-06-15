*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_ll_opamp_programmable_gain_amplifier.
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

    ${opamp_gain} =    Read Test Variable    OPAMP_GAIN
    ${voltages_list} =    Read Test Variable    INPUT_VOLTAGES
    Example Step 1
    Example Step 2
    FOR    ${input_voltage}    IN    @{{eval($voltages_list)}}
        ${min_voltage} =    Set Variable    ${${input_voltage} * ${opamp_gain} * 0.75}
        ${max_voltage} =    Set Variable    ${${input_voltage} * ${opamp_gain} * 1.33}
        Example Step 3    ${input_voltage}    ${min_voltage}    ${max_voltage}
        Example Step 4    ${min_voltage}    ${max_voltage}
    END


*** Keywords ***
Example Step 1
    [Documentation]    ADC Configuration
    ...    Checks that ADC initialization runs without error
    Comment    Nothing to test here

Example Step 2
    [Documentation]    OPAMP Configuration
    ...    Checks that OPAMP initialization runs without error
    Comment    Nothing to test here

Example Step 3
    [Documentation]    OPAMP Ready
    ...    Checks that OPAMP works as expected
    [Arguments]    ${input_voltage}    ${min_voltage}    ${max_voltage}
    Write Voltage    MX_EXAMPLE_OPAMP_VINP_PIN    ${input_voltage}mV
    Check Voltage
    ...    MX_EXAMPLE_OPAMP_VOUT_PIN
    ...    bound_low=${min_voltage}mV
    ...    bound_high=${max_voltage}mV
    ...    duration=1s

Example Step 4
    [Documentation]    Output voltage measurement
    ...    Checks voltage measurement and example status
    [Arguments]    ${min_voltage}    ${max_voltage}
    Check Integer Variable    AmplifiedVoltageValue    ${min_voltage}    ${max_voltage}    timeout=2s
    ${active_level} =    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
