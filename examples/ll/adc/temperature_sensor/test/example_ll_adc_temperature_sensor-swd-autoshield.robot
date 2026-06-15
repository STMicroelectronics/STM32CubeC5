*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_ll_adc_temperature_sensor.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_adc


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3
    Example Step 4


*** Keywords ***
Example Step 1
    [Documentation]    initializes and configures the ADC instance, the NVIC, and the DMA channel
    Comment    Verify ExecStatus value is EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after

Example Step 2
    [Documentation]    performs the ADC activation and self calibration
    Comment    Nothing to check.

Example Step 3
    [Documentation]    starts the ADC conversion of the sensor output voltage in polling mode.
    Comment    Nothing to check.

Example Step 4
    [Documentation]    Retrieves the ADC temperature sensor data in DegC.
    ${Temp_DegreeCelsius_below}=    Read Test Variable    Temp_DegreeCelsius_below
    ${Temp_DegreeCelsius_over}=    Read Test Variable    Temp_DegreeCelsius_over
    Check Integer Variable    Temperature_DegC    ${Temp_DegreeCelsius_below}    ${Temp_DegreeCelsius_over}    timeout=5s
    Comment    Verify ExecStatus value is EXEC_STATUS_OK
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    timeout=5s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
