*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_lps22hh_datalog_i2c.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             Collections
Library             struct.py
Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:parts_lps22hh    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2


*** Keywords ***
Example Step 1
    [Documentation]    Initializes and enables temp and press features of LPS22HH
    Comment    Check initialization (app_init) runs without error
    Comment    Check if data on console are printed correctly
    Read Console Until    [INFO] Step 1: LPS22HH sensor init completed
    Read Console Until    [INFO] Step 1: Enabling TEMP and PRESS features completed

Example Step 2
    [Documentation]    Get the values of the temp (in Celsius) and press (in hPa) and display them on a terminal
    Comment    Check if data on console are acquired and printed correctly
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=2s
    ${tmp}=    Read Raw Variable    gTempData
    ${tmp}=    struct.Unpack    ${LITTLE_ENDIAN} ${FLOAT_FMT}    ${tmp}
    ${tmp}=    Get From List    ${tmp}    0
    ${tmp}=    Convert To Integer    ${tmp}
    Should Be True    ${tmp} > -10 and ${tmp} < 50
    ${press}=    Read Raw Variable    gPressData
    ${press}=    struct.Unpack    ${LITTLE_ENDIAN} ${FLOAT_FMT}    ${press}
    ${press}=    Get From List    ${press}    0
    ${press}=    Convert To Integer    ${press}
    Should Be True    ${press} >= 500 and ${press} <= 2000
    Read Console Until    [INFO] Step 2: TEMP: ${tmp} degC
    Read Console Until    [INFO] Step 2: PRESS: ${press} hPa
