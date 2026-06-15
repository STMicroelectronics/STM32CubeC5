*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_cortex_get_systick.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             DateTime
Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_cortex    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    Computes the processing duration in milliseconds
    Comment    Compare embedded processing measurement with robot processing measurement
    ${date_start}=    Get Current Date
    Read Console Until    [INFO] Step 1: 34567901 is prime --- Processing duration =    timeout=8s
    ${date_stop}=    Get Current Date
    ${robot_measure}=    Subtract Date From Date    ${date_stop}    ${date_start}
    ${embedded_measure}=    Read Integer Variable    ProcessingDuration
    Check Integer Variable    ProcessingDuration    ${robot_measure * 1000}    precision=0.1

    Comment    Embedded measure should not be null
    Should Not Be Equal    ${embedded_measure}    ${0}

End Of Example
    [Documentation]    Check ExecStatus and Led Status
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
