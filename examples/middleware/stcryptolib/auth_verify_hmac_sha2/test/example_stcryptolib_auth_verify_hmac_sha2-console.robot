*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_stcryptolib_auth_verify_hmac_sha2.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
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
    [Documentation]    Initialize CMOX
    ...    Checks that initialization (app_init) runs without error
    Comment    Check initialization (app_init) runs without error
    Read Console Until    [INFO] Step 1: STM32 Cryptographic library initialization COMPLETED.
    Comment    ExecStatus value is then EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after.

Example Step 2
    [Documentation]    Generates and verifies a message authentication code (MAC) of a plaintext
    ...    using the HMAC-SHA256 algorithm in two ways.
    Read Console Until    [INFO] Step 2.1: MAC generation and verification using single API call method COMPLETED.
    Read Console Until    [INFO] Step 2.2: MAC verification using single API call method COMPLETED.
    Read Console Until    [INFO] Step 2.3: MAC generation and verification using multiple API calls method COMPLETED.
    Read Console Until    [INFO] Step 2.4: MAC verification using multiple API calls method COMPLETED.

Example Step 3
    [Documentation]    Deinitializes STM32 Cryptographic library before leaving the scenario.
    ...    Assuming the test scenario lasts under 5s
    Comment    Verify ExecStatus value is still EXEC_STATUS_OK
    Read Console Until    [INFO] Step 3: STM32 Cryptographic library deinitialization COMPLETED.
