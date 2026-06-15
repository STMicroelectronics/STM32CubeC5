*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_aes_cipher_saes_cbc_sharedkey.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_aes


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3


*** Keywords ***
Example Step 1
    [Documentation]    AES and SAES initialization
    ...    Checks that initialization (app_init) runs without error
    Comment    ExecStatus value is then EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after.

Example Step 2
    [Documentation]    Uses the shared key to encrypt a known plaintext and to verify the the generated ciphertext.
    ...    Then decrypts a known ciphertext and verifies the the generated plaintext.
    ...    2.1: Configures the AES Key with a HW key source and CBC algorithm.
    ...    2.2: Encrypts an application key to be shared with AES peripheral, using SAES HW key.
    ...    2.3: Configures the AES Key with a HW key source and CBC algorithm.
    ...    2.4: Decrypts an application key to be shared with AES peripheral, using SAES HW key.
    ...    2.5: Configures the AES peripheral with the CBC algorithm with encryption method.
    ...    2.6: Encrypts of a known plaintext then verifies the generated ciphertext.
    ...    2.7: Configures the AES peripheral with the CBC algorithm with decryption method.
    ...    2.8: Decrypts of a known ciphertext then verifies the generated plaintext.
    ...    2.9: Clears all data.
    Comment    Nothing to check

Example Step 3
    [Documentation]    Deinitializes the AES/SAES instances before leaving the scenario.
    ...    Checks that uninitialization (app_deinit) runs without error
    ...    Assuming the test scenario lasts under 5s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=5s
