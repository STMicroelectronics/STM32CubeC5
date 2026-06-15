*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_stcryptolib_sign_verify_ecc_ecdsa.
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
    [Documentation]    CMOX Initialization and RNG instance initialization
    ...    Checks that initialization (app_init) runs without error
    Read Console Until    [INFO] Step 1.1: STM32 Cryptographic library initialization COMPLETED.
    Read Console Until    [INFO] Step 1.2: RNG initialization COMPLETED.

Example Step 2
    [Documentation]    Demonstrates how to use the STM32 Cryptographic Library to sign
    ...    and to verify a message    with a known random and true random
    Read Console Until    [INFO] Step 2.1: Compute and verify the digest message COMPLETED.
    Read Console Until    [INFO] Step 2.2: compute and verify the known message digest using a known random COMPLETED.
    Read Console Until    [INFO] Step 2.3: Compute and verify the signature using a true random COMPLETED.

Example Step 3
    [Documentation]    Deinitializes STM32 Cryptographic library and the RNG instance before leaving the scenario.
    ...    Checks that uninitialization (app_deinit) runs without error
    Read Console Until    [INFO] Step 3.1: STM32 Cryptographic library deinitialization COMPLETED.
    Read Console Until    [INFO] Step 3.2: RNG deinitialization COMPLETED.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
