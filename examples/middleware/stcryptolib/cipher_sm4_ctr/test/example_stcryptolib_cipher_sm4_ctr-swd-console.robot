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
    Example Step 4


*** Keywords ***
Example Step 1
    [Documentation]    Configure ICACHE, Basic Trace and initialize STM32 Cryptographic library
    ...    Checks that initialization (app_init) runs without error
    Comment    Check initialization (app_init) runs without error
    Read Console Until    [INFO] Step 1.1: STM32 Cryptographic library initialization COMPLETED.

Example Step 2
    [Documentation]    Encryption and decryption of a known plainText with SINGLE CALL USAGE
    Read Console Until    [INFO] Step 2.1: Encryption of a known plainText COMPLETED.
    Read Console Until    [INFO] Step 2.2: Decryption of a known ciphertext COMPLETED.

Example Step 3
    [Documentation]    Encryption and decryption of a known plainText with MULTIPLE CALL USAG.
    Read Console Until    [INFO] Step 3.1: Construct, initialize and setup needed data into encryption cipher context.
    Read Console Until    [INFO] Step 3.2: Encryption of plainText in multiple steps COMPLETED.
    Read Console Until    [INFO] Step 3.3: Clean-up encryption cipher context COMPLETED.
    Read Console Until    [INFO] Step 3.4: Construct, initialize and setup needed data into decryption cipher context.
    Read Console Until    [INFO] Step 3.5: Decryption of plainText in multiple steps COMPLETED.
    Read Console Until    [INFO] Step 3.6: Clean-up decryption cipher context COMPLETED.

Example Step 4
    [Documentation]    End of program.
    Read Console Until    [INFO] Step 4.1: Finalize STM32 Cryptographic library COMPLETED.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
