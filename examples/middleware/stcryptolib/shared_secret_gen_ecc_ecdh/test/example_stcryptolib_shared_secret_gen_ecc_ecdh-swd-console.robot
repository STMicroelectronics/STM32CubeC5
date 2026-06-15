*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_stcryptolib_shared_secret_gen_ecc_ecdh.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:mw_stcryptolib    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3


*** Keywords ***
Example Step 1
    [Documentation]    CMOX Initialization
    ...    Checks that initialization (app_init) runs without error
    Read Console Until    [INFO] Step 1: STM32 Cryptographic library initialization COMPLETED.

Example Step 2
    [Documentation]    Demonstrates how to establish and to verify the exchange of
    ...    shared secret using ECC algorithm configuration and operation in a single call method.
    ...    2.1: Establishes the shared secret    using known remote public and known local Key.
    ...    2.2: Compares the generated secret X coordinate with the expected one.
    Read Console Until
    ...    [INFO] Step 2.1: Establish the shared secret using known remote public and known local Key COMPLETED.
    Read Console Until    [INFO] Step 2.2: Compare the generated secret X coordinate with the expected one COMPLETED.

Example Step 3
    [Documentation]    Deinitializes STM32 Cryptographic library before leaving the scenario.
    ...    Checks that uninitialization (app_deinit) runs without error
    Read Console Until    [INFO] Step 3: STM32 Cryptographic library deinitialization COMPLETED.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
