*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_pwr_sleep.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.Autoshield
Library             robot_for_stm32.STLinkPWR
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_pwr


*** Variables ***
# Margin in percent to apply on the nominal current to define the acceptance range for current measurement
${MARGIN_IN_PERCENT}=       100


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    ${mcu_voltage}=    Read Test Variable    MCU_VOLTAGE
    ${nominal_current}=    Read Test Variable    LOW_POWER_MODE_CURRENT
    ${margin_current}=    Set Variable    ${{${nominal_current} * ${MARGIN_IN_PERCENT} / ${100}}}

    Set Mcu Voltage    ${mcu_voltage}

    FOR    ${i}    IN RANGE    0    2
        Example Step 1
        Example Step 2    ${{${nominal_current} - ${margin_current}}}    ${{${nominal_current} + ${margin_current}}}
    END


*** Keywords ***
Example Step 1
    [Documentation]    Configure the wakeup source and the system to reach the typical consuption
    ...    Check initialization (app_init) runs without error
    Comment    Check button pull resistor configuration
    ${active_button_level}=    Read Test Variable    MX_STATUS_BUTTON_ACTIVE_STATE
    Check Button Configuration    MX_EXAMPLE_WAKEUP_PIN    ${active_button_level}

Example Step 2
    [Documentation]    Check that the system switch to LOW POWER mode, consumption measurement then wakeup
    [Arguments]    ${min_current}    ${max_current}
    Comment    Verify status LED is activated
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s

    Comment    Wait for status LED switches OFF
    ${inactive_level}=    Read Test Variable    MX_STATUS_LED_INACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${inactive_level}    timeout=2s

    Comment    Wait few time to allow low-power current consumption stabilization, then measure current
    Sleep    1s
    Check Mcu Current    ${min_current}    ${max_current}

    Comment    Activate target wakeup pin
    Write Pulse    MX_EXAMPLE_WAKEUP_PIN