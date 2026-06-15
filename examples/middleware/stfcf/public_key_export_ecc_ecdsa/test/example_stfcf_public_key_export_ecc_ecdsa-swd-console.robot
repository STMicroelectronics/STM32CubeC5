*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_public_key_export_ecc_ecdsa.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:mw_stfcf    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3


*** Keywords ***
Example Step 1
    [Documentation]    Initializes the RNG instance and configure the key.
    Comment    Check initialization (app_init) runs without error
    Read Console Until    [INFO] Step 1.1: RNG initialization COMPLETED.
    Read Console Until    [INFO] Step 1.2: key configuration COMPLETED.
    Comment    Verify ExecStatus value is EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after

Example Step 2
    [Documentation]    ECDSA Public Key Export
    Comment    Check application (app_process) runs without error
    Read Console Until    [INFO] Step 2.1: Export of the public key from the wrapped private key COMPLETED.
    Read Console Until    [INFO] Step 2.2: Verifies computed public key data size COMPLETED.

Example Step 3
    [Documentation]    Deinitializes the RNG instance and destroy the PSA key before leaving the scenario.
    Comment    Check uninitialization (app_deinit) runs without error
    Read Console Until    [INFO] Step 3.1: Cleanup COMPLETED.
    Read Console Until    [INFO] Step 3.2: RNG deinitialization COMPLETED.
    Comment    Verify ExecStatus value is EXEC_STATUS_OK
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
