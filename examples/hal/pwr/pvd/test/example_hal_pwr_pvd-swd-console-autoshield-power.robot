*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_pwr_pvd.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Library             robot_for_stm32.Autoshield
Library             robot_for_stm32.STLinkPWR
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_pwr    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    ${normal_voltage}=    Read Test Variable    MCU_NOMIMAL_VOLTAGE
    ${low_voltage}=    Read Test Variable    MCU_LOW_VOLTAGE
    Set Mcu Voltage    ${normal_voltage}
    Example Step 1
    Example Step 2    ${low_voltage}
    Example Step 3
    Example Step 4
    Set Mcu Voltage    ${normal_voltage}


*** Keywords ***
Example Step 1
    [Documentation]    Configure the PVD and its interrupt
    ...    Check initialization (app_init) runs without error
    Read Console Until    [INFO] Step 1
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_INIT_OK}
    ${inactive_level}=    Read Test Variable    MX_STATUS_LED_INACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${inactive_level}    timeout=1s

Example Step 2
    [Documentation]    Waiting for the PVD interrupt
    [Arguments]    ${voltage}
    Read Console Until    [INFO] Step 2
    Comment    Decrease MCU power supply
    Set Mcu Voltage    ${voltage}

Example Step 3
    [Documentation]    Detect PVD interrupt
    Comment    Wait for PVD detection
    Read Console Until    [INFO] Step 3
    Check Integer Variable    PvdDetected    1

Example Step 4
    [Documentation]    Unconfigure the PVD, associated EXTI and leave the example
    ...    Check that uninitialization (app_deinit) runs without error
    Read Console Until    [INFO] Step 4
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
