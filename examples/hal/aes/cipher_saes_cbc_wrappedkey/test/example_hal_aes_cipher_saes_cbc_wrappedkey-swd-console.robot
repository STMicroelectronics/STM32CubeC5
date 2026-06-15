*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_aes_cipher_saes_cbc_wrappedkey.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_aes    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3


*** Keywords ***
Example Step 1
    [Documentation]    SAES initialization and configuration.
    ...    Checks that initialization (app_init) runs without error
    Read Console Until
    ...    [INFO] Step 1: SAES initialization, configuration of the HW key source and CBC algorithm COMPLETED.

Example Step 2
    [Documentation]    Using the a wrapped key to encrypts a known plaintextand verifies the generated ciphertext
    ...    then decrypts a known ciphertext and verifies the generated plaintext.

    Read Console Until    [INFO] Step 2.1: Encryption of an application key with SAES HW key(wrapper key) COMPLETED.
    Read Console Until    [INFO] Step 2.2: Configuration of the HW key source and CBC algorithm COMPLETED.
    Read Console Until    [INFO] Step 2.3: Decryption of an application key with SAES HW key(wrapper key) COMPLETED.
    Read Console Until    [INFO] Step 2.4: Configuration of the AES peripheral to work with CBC mode COMPLETED.
    Read Console Until    [INFO] Step 2.5: Encryption and verification of a known plaintext COMPLETED.
    Read Console Until    [INFO] Step 2.6: Configuration of the AES peripheral to work with CBC mode COMPLETED.
    Read Console Until    [INFO] Step 2.7: Decryption and verification of a known ciphertext COMPLETED.
    Read Console Until    [INFO] Step 2.8: Data clearness COMPLETED.

Example Step 3
    [Documentation]    SAES deinitialization before leaving the scenario.
    Read Console Until    [INFO] Step 3: SAES deinitialization COMPLETED.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
