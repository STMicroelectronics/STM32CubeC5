*** Comments ***
Copyright (c) 2025 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_rcc_clock_switch_max_default.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_rcc    profile:printf


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
    ${sysclk}=    Read Integer Variable    MaxSysclkFrequency
    ${sysclk}=       Evaluate    ${sysclk} // 1000
    ${mco_divider}=    Read Test Variable    mco_divider
    ${mcosysclk}=       Evaluate    ${sysclk} // ${mco_divider}
    Read Console Until    [INFO] Step 1: Output SYSCLK (${sysclk}kHz) to MCO pin, with a ${mco_divider} divider (MCO freq: ${mcosysclk}kHz).
    Comment    Verify ExecStatus value is EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after

Example Step 2
    [Documentation]    Switches the clock source from Max to Default.
    Comment    Check sysclock frequency is equal to default
    ${sysclk}=    Read Integer Variable    DefaultSysclkFrequency
    ${sysclk}=       Evaluate    ${sysclk} // 1000
    ${mco_divider}=    Read Test Variable    mco_divider
    ${mcosysclk}=       Evaluate    ${sysclk} // ${mco_divider}
    Read Console Until    [INFO] Step 2: Default settings are used to generate SYSCLK at ${sysclk}kHz, (MCO freq: ${mcosysclk}kHz).

Example Step 3
    [Documentation]    Switches the clock source from Default to Max.
    Comment    Check sysclock frequency is equal to max
    ${sysclk}=    Read Integer Variable    MaxSysclkFrequency
    ${sysclk}=       Evaluate    ${sysclk} // 1000
    ${mco_divider}=    Read Test Variable    mco_divider
    ${mcosysclk}=       Evaluate    ${sysclk} // ${mco_divider}
    Read Console Until    [INFO] Step 3: Max settings are used to generate SYSCLK at ${sysclk}kHz, (MCO freq: ${mcosysclk}kHz).
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
