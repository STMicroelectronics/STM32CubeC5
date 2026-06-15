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
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:utilities_eeprom_emulation    using:mmi


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2-3


*** Keywords ***
Example Step 1
    Comment    return_status = EXEC_STATUS_INIT_OK is transient and can't be checked

Example Step 2-3
    [Documentation]    Configure and initialize peripherals and eeprom_emulation utility
    Comment    Checks that that the dataCounter variable increments up to 20000
    Check Integer Variable    dataCounter    1000    20000    timeout=30s
    Comment    Checks the final ExecStatus
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=3s
