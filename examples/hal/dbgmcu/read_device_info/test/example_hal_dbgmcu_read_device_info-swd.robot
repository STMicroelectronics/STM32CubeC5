*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_dbgmcu_read_device_info.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_dbgmcu


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.

    Comment    For this test, there only are checks a-posteriori (no realtime interaction),
    ...    so we wait for the end of example first.
    End Of Example

    Example Step 1
    Example Step 2


*** Keywords ***
Example Step 1
    [Documentation]    retrieves the Device ID and stores it in a global variable.
    ${DEVICE_ID}=    Read Test Variable    DEVICE_ID
    ${DEVICE_ID}=    Convert To Integer    ${DEVICE_ID}
    Check Integer Variable    DeviceID    ${DEVICE_ID}

Example Step 2
    [Documentation]    retrieves the Revision ID and stores it in a global variable.
    Comment    Won't check the actual revision ID, as it may vary, even with the same device model

    # minimum check: nonzero revision ID
    ${rev_id}=    Read Integer Variable    RevisionID
    Should Not Be Equal As Integers    ${rev_id}    0

End Of Example
    [Documentation]    Reports the outcome of the example via the variable **`ExecStatus`**,
    ...    and the **status LED** remains turned on in case of success.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=1s
