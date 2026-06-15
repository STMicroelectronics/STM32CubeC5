*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_eeprom_emulation_read_write.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:utilities_eeprom_emulation    profile:printf    using:mmi


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2-3


*** Keywords ***
Example Step 1
    [Documentation]    eeprom emulation is initialized
    Comment    Check initialization (app_init) runs without error
    Read Console Until    [INFO] Step 1: EEPROM INITIALIZATION COMPLETED.

Example Step 2-3
    [Documentation]    write data in eeprom
    Comment    Check application (app_process) runs without error
    Comment    Verify ExecStatus value is EXEC_STATUS_OK
    Read Console Until    [INFO] Step 3: READ and WRITTEN VARIABLES ARE EQUAL
