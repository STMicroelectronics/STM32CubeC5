*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_w25n01gvxx_read_write.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:parts_w25n01gvxx    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3
    Example Step 4
    Example Step 5


*** Keywords ***
Example Step 1
    [Documentation]    Initializes the w25n01gvxx part.
    Comment    Can't check ExecStatus = EXEC_STATUS_INIT_OK: this value is transient.
    Read Console Until    [INFO] Step 1: Device initialization COMPLETED.

Example Step 2
    [Documentation]    Erases the block in which we want to write
    Read Console Until    [INFO] Step 2: Block(s) erased.

Example Step 3
    [Documentation]    Writes a data buffer to the flash memory.
    Read Console Until    [INFO] Step 3: Data buffer written to the flash memory.

Example Step 4
    [Documentation]    Reads the same data buffer from the flash memory, through one data transfer, in continuous mode, and checks that it matches the buffer written in step 3.
    Read Console Until
    ...    [INFO] Step 4: Data read from the flash memory in continuous mode and matching the written value.

Example Step 5
    [Documentation]    Deinitializes the w25n01gvxx before leaving the scenario.
    Read Console Until    [INFO] Step 5: Device de-initialization.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=5s
