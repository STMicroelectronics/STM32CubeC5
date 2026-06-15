*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_rcc_lsi_config.
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
    ...    the scenario when a reset occurs. Because the LSI is part of the backup domain, the LSI state is
    ...    stored and set at its previous state after a reset. The test checks that even if a reset occurs
    ...    after backup domain registers have been modified, the example behavior is still as expected.

    Example Step 1
    Example Step 2-1
    Reset Target
    Example Step 1
    Example Step 2-1
    Example Step 2-2
    Example Step 3


*** Keywords ***
Example Step 1
    [Documentation]    IPs and LSI initialized.
    Comment    Verify ExecStatus value is EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after

Example Step 2-1
    [Documentation]    Check the LSI enable step
    Comment    Check application (app_process) runs without error
    ${mco_divider}=    Read Test Variable    mco_divider
    ${lsi_frequency}=    Read Test Variable    lsi_frequency

    ${mco_divider}=    Convert To Integer    ${mco_divider}
    ${lsi_frequency}=    Convert To Integer    ${lsi_frequency}

    IF    ${mco_divider} == 0
        Comment    MCO output pin is not accessible
        Read Console Until
        ...    [INFO] Step 2: LSI enabled (${lsi_frequency} Hz) to MCO pin
    ELSE
        Read Console Until
        ...    [INFO] Step 2: LSI enabled (${lsi_frequency} Hz) to MCO pin, with a ${mco_divider} divider (MCO freq: ${{int($lsi_frequency/$mco_divider)}} Hz).
    END

Example Step 2-2
    [Documentation]    Check the LSI disable step
    Comment    Check application (app_process) runs without error
    Read Console Until    [INFO] Step 2: LSI disabled.

Example Step 3
    [Documentation]    MCO deinitialized.
    Comment    Check uninitialization (app_deinit) runs without error
    Read Console Until    [INFO] Step 3: LSI status restored and outputted on MCO pin.
    Comment    Verify ExecStatus value is EXEC_STATUS_OK
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=5s
