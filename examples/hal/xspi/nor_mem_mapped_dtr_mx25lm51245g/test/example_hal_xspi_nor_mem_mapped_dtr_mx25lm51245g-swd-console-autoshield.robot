*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_xspi_nor_mem_mapped_dtr_mx25lm51245g.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_xspi    profile:printf


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
    Example Step 8
    Example Step 9


*** Keywords ***
Example Step 1
    [Documentation]    Initialize the XSPI instance
    ...    Checks that initialization (app_init) runs without error
    Read Console Until    [INFO] Step 1: Device initialization COMPLETED.
    Comment    ExecStatus value is then EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after.

Example Step 2
    [Documentation]    Configure the MX25LM51245G NOR memory in Octal-SPI STR mode
    Read Console Until    [INFO] Step 2: Memory configuration in DTR mode COMPLETED.

Example Step 3
    [Documentation]    Erase the first 4K-byte sector of the MX25LM51245G NOR memory
    Read Console Until    [INFO] Step 3: Memory erasing COMPLETED.

Example Step 4
    [Documentation]    Enables the memory-mapped mode for octal DTR read and write operations
    Read Console Until    [INFO] Step 4: Memory configured for octal read/write in memory-mapped mode.

Example Step 5
    [Documentation]    Configures and start the memory-mapped mode.
    Read Console Until    [INFO] Step 5: Memory-mapped mode STARTED.

Example Step 6
    [Documentation]    Writes the TX buffer to the first page of the MX25LM512G device.
    Read Console Until    [INFO] Step 6: Memory write COMPLETED.

Example Step 7
    [Documentation]    Read back the written TxBuffer from the memory first page and check data correctness
    Read Console Until    [INFO] Step 7: Memory read COMPLETED. Data written and read match.

Example Step 8
    [Documentation]    Stop the memory-mapped mode
    Read Console Until    [INFO] Step 8: Memory-mapped mode STOPPED.

Example Step 9
    [Documentation]    De-initialize the XSPI instance
    Comment    Check uninitialization (app_deinit) runs without error
    Read Console Until    [INFO] Step 9: de-init
    Comment    Check that the LED Success pattern is executed
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    timeout=2s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
