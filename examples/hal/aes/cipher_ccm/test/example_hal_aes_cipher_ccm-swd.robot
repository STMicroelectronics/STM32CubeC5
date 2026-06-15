*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_aes_cipher_ccm.
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
    [Documentation]    Initializes and configures the AES instance.
    ...    Checks that initialization (app_init) runs without error
    Comment    ExecStatus=EXEC_STATUS_INIT_OK is transient and can't be checked

Example Step 2
    [Documentation]    Encrypts a known plaintext and verifies the generated encrypted ciphertext,
    ...    then decrypts a known ciphertext and verifies the generated decrypted plaintext.
    ...    Step 2.1: Encrypts a known plaintext and verifies the generated ciphertext
    ...    Step 2.2: Configures the AES peripheral with the CCM algorithm for decryption process.
    ...    Step 2.3: Decrypts a known ciphertext then verifies the generated plaintext.
    ...    Step 2.4: Clears all data.
    Comment    Nothing to test

Example Step 3
    [Documentation]    AES deinitialization COMPLETED.
    ...    Checks that uninitialization (app_deinit) runs without error
    ...    Assuming the test scenario lasts under 5s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=5s
