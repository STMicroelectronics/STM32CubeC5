*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_dma_trigger.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_dma


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3
    Example Step 4
    Example Step 5
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    Configures the DMA in memory to memory mode.
    Comment    transient    ExecStatus, no check possible

Example Step 2
    [Documentation]    Start the DMA transfer on the triggered channel.
    Comment    No Check possible

Example Step 3
    [Documentation]    Trigger the first DMA channel to start multiple small transfers.
    Comment    No Check

Example Step 4
    [Documentation]    Wait for the second DMA channel to complete the full transfer.
    Comment    Check application (app_process) runs without error
    Check Integer Variable    TriggeringTransferComplete    1    timeout=5s
    Check Integer Variable    TriggeredTransferComplete    1    timeout=5s
    Check Integer Variable    TriggeringTransferError    0
    Check Integer Variable    TriggeredTransferError    0

Example Step 5
    [Documentation]    DMA deinitialization.
    Comment    No Check

End Of Example
    [Documentation]    Verify successful end of example
    Comment    Verify status LED is in success mode
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s

    Comment    Verify ExecStatus value is EXEC_STATUS_OK
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
