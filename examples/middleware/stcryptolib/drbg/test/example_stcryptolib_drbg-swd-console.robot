*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_stcryptolib_drbg.
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
    [Documentation]    Configure ICACHE, Basic Trace and initialize Cryptographic library
    ...    Checks that initialization (app_init) runs without error
    Comment    Check initialization (app_init) runs without error
    Read Console Until    [INFO] Step 1: STCryptoLib initialization COMPLETED.

Example Step 2
    [Documentation]    Demonstrates how to use the STCryptoLib to generate random numbers
    ...    based on known input entropy, nonce and personalization.
    ...    2.1: Initializes the DRBG context with entropy, nonce and personalization string parameters.
    ...    2.2: Reseeds the DRBG with reseed parameters.
    ...    2.3: Generates random data.
    ...    2.4: Verifies generated data.
    ...    2.5: Cleans the DRBG context.
    Read Console Until    [INFO] Step 2.1: DRBG context initialization COMPLETED.
    Read Console Until    [INFO] Step 2.2: Reseed the DRBG with reseed parameters COMPLETED.
    Read Console Until    [INFO] Step 2.3: Generate random data COMPLETED.
    Read Console Until    [INFO] Step 2.4: Verify generated data COMPLETED.
    Read Console Until    [INFO] Step 2.5: Clean-up the DRBG context COMPLETED.

Example Step 3
    [Documentation]    End of program.
    ...    Assuming the test scenario lasts under 5s
    Comment    Verify ExecStatus value is still EXEC_STATUS_OK
    Read Console Until    [INFO] Step 3: STCryptoLib deinitialization COMPLETED.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=5s
