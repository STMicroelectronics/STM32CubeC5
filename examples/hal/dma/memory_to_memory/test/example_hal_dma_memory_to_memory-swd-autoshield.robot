*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_dma_memory_to_memory.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_dma

*** variables ***

${SRC_BUFFER_SIZE_BYTE}=    128
${NON_CACHEABLE_DEST_BUFFER_SIZE_BYTE}=    128


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3
    Example Step 4
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    Configures the DMA in Memory to Memory mode
    Comment    transient    ExecStatus, no check possible

Example Step 2
    [Documentation]    Starts the DMA transfer.
    Comment    No Check

Example Step 3
    [Documentation]    Waits for one of these DMA interrupts: transfer complete or transfer error.
    Comment    Check application (app_process) runs without error
    Check Integer Variable    TransferComplete    1    timeout=5s
    Check Integer Variable    TransferError    0

    Comment    Compare destination buffer with source buffer

    ${SrcBuffer}=    Read Raw Variable    SrcBuffer    size=${SRC_BUFFER_SIZE_BYTE}
    ${DstBuffer}=    Read Raw Variable    DstBuffer    size=${NON_CACHEABLE_DEST_BUFFER_SIZE_BYTE}
    Should Be Equal    ${DstBuffer}    ${SrcBuffer}

Example Step 4
    [Documentation]    DMA deinitialization.
    Comment    No Check

End Of Example
    [Documentation]    Verify successful end of example
    Comment    Verify status LED is in success mode
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s

    Comment    Verify ExecStatus value is EXEC_STATUS_OK
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
