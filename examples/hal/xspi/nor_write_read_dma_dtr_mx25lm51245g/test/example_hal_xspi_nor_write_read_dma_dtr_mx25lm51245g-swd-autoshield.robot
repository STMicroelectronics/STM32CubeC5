*** Comments ***
Copyright (c) 2025 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_xspi_nor_write_read_dma_dtr_mx25lm51245g.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_xspi


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
    Example Step 7


*** Keywords ***
Example Step 1
    [Documentation]    Initialize the XSPI instance
    ...    Checks that initialization (app_init) runs without error
    Comment    Nothing to be checked here
    Comment    ExecStatus value is then EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after.

Example Step 2
    [Documentation]    Verify that the application successfully configures the external MX25LM51245G NOR memory,
    ...    in Octal DTR (double-transfer rate) mode.
    Comment    Nothing to be checked here

Example Step 3
    [Documentation]    Verify that the application successfully erases the first 4K-byte sector of the memory.
    Comment    Nothing to be checked here

Example Step 4
    [Documentation]    Verify that the application successfully writes the appropriate data,
    ...    to the first 256-byte page of the memory's first sector.
    Comment    Nothing to be checked here

Example Step 5
    [Documentation]    Verify that the application successfully reads back the written data from the memory.
    Comment    Nothing to be checked here

Example Step 6
    [Documentation]    Verify that the read data matches the written one.
    Comment    Nothing to be checked here

Example Step 7
    [Documentation]    De-initialize the XSPI instance
    Comment    Check uninitialization (app_deinit) runs without error
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
    Comment    Check that the LED Success pattern is executed
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    timeout=2s
