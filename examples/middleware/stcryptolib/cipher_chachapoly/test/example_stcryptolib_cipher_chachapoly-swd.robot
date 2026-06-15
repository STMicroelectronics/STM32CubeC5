*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_stcryptolib_cipher_chachapoly.
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
    [Documentation]    Initialize CMOX
    ...    Checks that initialization (app_init) runs without error
    Comment    Check initialization (app_init) runs without error

    Comment    ExecStatus value is then EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after.

Example Step 2
    [Documentation]    Demonstrates encryption and decryption of a known ciphertext using
    ...    the Chacha20-Poly1305 algorithm in the single API call method.
    ...    Step 2.1: Enecrypts and verifies directly a plainText and a tag using the single call method.
    ...    Step 2.2: Decrypts and verifies directly a ciphertext and a tag using the single call method.
    Comment    Check application (app_process) runs without error

    Comment    Nothing to do here

Example Step 3
    [Documentation]    Demonstrates encryption and decryption of a known ciphertext using
    ...    the Chacha20-Poly1305 algorithm in the multiple API calls method.
    ...    Step 3.1: Sets the encryption context for Chacha20-Poly1305.
    ...    Step 3.2: Enecrypts and verifies directly a plainText and a tag using the multiple API calls method.
    ...    Step 3.3: Sets the decryption context for Chacha20-Poly1305.
    ...    Step 3.4: Decrypts and verifies directly a ciphertext and a tag using the multiple API calls method.
    Comment    Check application (app_process) runs without error

Example Step 4
    [Documentation]    Deinitializes STM32 Cryptographic library before leaving the scenario.
    ...    Assuming the test scenario lasts under 5s
    Comment    Verify ExecStatus value is still EXEC_STATUS_OK

    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=5s
