*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_xspi_nor_toolbox_m2_mx25lm51245g.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
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
    Example Step 10


*** Keywords ***
Example Step 1
    [Documentation]    Configure and initialize the XSPI instance.
    ...    Checks that initialization (app_init) runs without error.
    Read Console Until    [INFO] Step 1: Device initialization COMPLETED.
    Comment    ExecStatus value is then EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after.

Example Step 2
    [Documentation]    Read the JEDEC ID to check that the MX25LM51245G is the expected
    ...    device and correctly connected.
    Read Console Until    [INFO] Step 2: JEDEC ID check: OK (MX25LM51245G detected).

Example Step 3
    [Documentation]    Read the memory SFDP Table.
    Read Console Until    [INFO] Step 3: SFDP table read COMPLETED.

Example Step 4
    [Documentation]    Configure the MX25LM51245G NOR memory in Octal-SPI DTR
    ...    (double-transfer rate) mode.
    Read Console Until    [INFO] Step 4: Memory configuration in DTR mode COMPLETED.

Example Step 5
    [Documentation]    Erase the first 4K-byte sector of the MX25LM51245G NOR memory,
    ...    in automatic polling mode, by executing the Sector Erase command.
    Read Console Until    [INFO] Step 5: Memory erasing COMPLETED.

Example Step 6
    [Documentation]    Configure the memory-mapped mode and start it.
    ...    The MX25LM51245G device is seen as an internal memory.
    Read Console Until    [INFO] Step 6: Memory-mapped mode STARTED.

Example Step 7
    [Documentation]    writes the TX buffer to the first page of the MX25LM512G device.
    Read Console Until    [INFO] Step 7: Page program operation COMPLETED.

Example Step 8
    [Documentation]    Read back the written TxBuffer from the memory first page
    ...    and check data correctness.
    Read Console Until    [INFO] Step 8: Memory read COMPLETED. Data written and read match.

Example Step 9
    [Documentation]    Stop the memory-mapped mode.
    Read Console Until    [INFO] Step 9: Memory-mapped mode STOPPED.

Example Step 10
    [Documentation]    De-initialize the XSPI instance.
    Comment    Check uninitialization (app_deinit) runs without error.
    Read Console Until    [INFO] Step 10: de-init
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}