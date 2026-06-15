*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_pwr_standby_rtc.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.Console
Library             robot_for_stm32.Autoshield
Library             robot_for_stm32.STLinkPWR
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags
...    ip:hal_pwr
...    profile:printf
...    robot:skip
# Skip because we should be able to power reset the board between each test


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    ${min_current}=    Read Test Variable    LOW_POWER_MODE_MIN_CURRENT
    ${max_current}=    Read Test Variable    LOW_POWER_MODE_MAX_CURRENT
    ${mcu_voltage}=    Read Test Variable    MCU_VOLTAGE
    Set Mcu Voltage    ${mcu_voltage}

    Example Step 1 Start
    Example Step 2
    FOR    ${i}    IN RANGE   0    2
        Example Step 3    ${min_current}    ${max_current}
        Example Step 1 Restart
        Example Step 2
    END


*** Keywords ***
Example Step 1 Start
    [Documentation]    Configure the PWR and RTC
    ...    Check initialization (app_init) runs without error
    Read Console Until    [INFO] Step 1
    Read Console Until    [INFO] First start

Example Step 2
    [Documentation]    Stay in RUN mode during few seconds
    Read Console Until    [INFO] Step 2
    Comment    Verify status LED is activated
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    timeout=5s
    Comment    Wait for status LED switches OFF in less than 3s
    ${inactive_level}=    Read Test Variable    MX_STATUS_LED_INACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${inactive_level}    timeout=3s

Example Step 3
    [Documentation]    Switch to LOW POWER mode
    ...    Check target stays in Low Power mode then measure board current
    [Arguments]    ${min_current}    ${max_current}
    Read Console Until    [INFO] Step 3
    ${inactive_level}=    Read Test Variable    MX_STATUS_LED_INACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${inactive_level}    duration=1s
    Check Mcu Current    ${min_current}    ${max_current}

Example Step 1 Restart
    [Documentation]    Configure the GPIOs - LED and WAKEUP pin
    ...    Check initialization (app_init) runs without error
    Read Console Until    [INFO] Step 1
    Read Console Until    [INFO] Restart
