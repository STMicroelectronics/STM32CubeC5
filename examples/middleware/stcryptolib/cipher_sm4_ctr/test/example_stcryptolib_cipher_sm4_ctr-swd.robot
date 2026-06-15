*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_stcryptolib_cipher_sm4_ctr.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:mw_stcryptolib


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3
    Example Step 4


*** Keywords ***
Example Step 1
    [Documentation]    Configure ICACHE, Basic Trace and initialize STM32 Cryptographic library
    ...    Checks that initialization (app_init) runs without error
    Comment    Check initialization (app_init) runs without error

    Comment    ExecStatus value is then EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after.

Example Step 2
    [Documentation]    Encryption and decryption of a known plainText with SINGLE CALL USAGE
    ...
    Comment    Check application (app_process) runs without error

    Comment    Nothing to do here

Example Step 3
    [Documentation]    Encryption and decryption of a known plainText with MULTIPLE CALL USAG.

    Comment    Nothing to do here

Example Step 4
    [Documentation]    Deinitializes STM32 Cryptographic library before leaving the scenario.
    ...    Assuming the test scenario lasts under 5s
    Comment    Verify ExecStatus value is still EXEC_STATUS_OK

    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=5s
