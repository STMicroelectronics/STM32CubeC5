*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_ll_rcc_clock_switch_max_default.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants
Resource            example.resource

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_rcc


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3


*** Keywords ***
Example Step 1
    [Documentation]    At startup, SYSCLK is configured to the maximum clock source frequency.
    ...    The SYSCLK signal is provided on the MCO pin.
    ...    Checks that initialization (app_init) runs without error
    Log    MCO is on mco_pin pin on target
    Comment    ExecStatus value is then EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after.

Example Step 2
    [Documentation]    Switches the clock source from Max to Default.
    Comment    Check sysclock frequency is equal to default
    ${sysclk}=    Read Integer Variable    DefaultSysclkFrequency
    ${defaultsysclk}=    Read Test Variable    sysclk_default_frequency
    ${defaultsysclk}=    Convert To Integer    ${defaultsysclk}
    Should Be Equal    ${sysclk}    ${defaultsysclk}

Example Step 3
    [Documentation]    Switches the clock source from Default to Max.
    Comment    Check sysclock frequency is equal to max
    ${sysclk}=    Read Integer Variable    MaxSysclkFrequency
    ${maxsysclk}=    Read Test Variable    sysclk_max_frequency
    ${maxsysclk}=    Convert To Integer    ${maxsysclk}
    Should Be Equal    ${sysclk}    ${maxsysclk}
    Comment    Verify ExecStatus value is EXEC_STATUS_OK
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=5s
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
