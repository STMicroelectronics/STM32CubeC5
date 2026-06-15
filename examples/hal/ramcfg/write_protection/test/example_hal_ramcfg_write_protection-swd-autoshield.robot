*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_ramcfg_write_protection.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_ramcfg


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3
    Example Step 4
    Example Step 5
    Example Step 6


*** Keywords ***
Example Step 1
    [Documentation]    Initialize the RAMCFG instance
    ...    Checks that initialization (app_init) runs without error
    Comment    ExecStatus value is then EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after.

Example Step 2
    [Documentation]    Verify that the application successfully erases the given SRAM.
    Comment    Nothing to check here.

Example Step 3
    [Documentation]    Verify that the application successfully enables the write protection.
    Comment    Nothing to check here.

Example Step 4
    [Documentation]    Verify that the application fills all pages of given SRAM.
    Comment    Nothing to check here.

Example Step 5
    [Documentation]    Verify that the application successfully checks protected and unprotected SRAM pages content.
    Comment    Nothing to check here.

Example Step 6
    [Documentation]    De-initialize the DMA instance
    Comment    Check uninitialization (app_deinit) runs without error
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
    Comment    Check that the LED Success pattern is executed
    Check Pin    MX_STATUS_LED    ${HIGH}    timeout=2s
