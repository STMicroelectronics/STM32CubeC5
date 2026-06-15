*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_dma_linked_list.
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
    Example Step 6
    Example Step 7
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    Fill the DMA linkied list nodes with specifying the source address, the destination address and the data size in bytes for each node
    Comment    No check possible

Example Step 2
    [Documentation]    Initialize the DMA linked list Queue object and build it according to nodes filled in Step 1
    Comment    No check possible

Example Step 3
    [Documentation]    Initialize the DMA channel in linked list mode and register transfer error and transfer complete user callbacks
    Comment    No check possible

Example Step 4
    [Documentation]    Starts the DMA linked list Queue execution in interrupt mode
    Comment    No check possible

Example Step 5
    [Documentation]    Enters sleep mode and waits for the DMA interrupt: transfer complete or transfer error
    Comment    Check application (app_process) runs without error
    Check Integer Variable    TransferComplete    1    timeout=1s
    Check Integer Variable    TransferError    0

Example Step 6
    [Documentation]    Checks DMA transfers for each DMA linked list node within the executed Queue
    Comment    No check as buffers content check is done within application

Example Step 7
    [Documentation]    Deinitializes the DMA channel instance and deinitilize the DMA linked list Queue
    Comment    No check possible

End Of Example
    [Documentation]    Verify successful end of example
    Comment    Verify status LED is in success mode
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s

    Comment    Verify ExecStatus value is EXEC_STATUS_OK
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
