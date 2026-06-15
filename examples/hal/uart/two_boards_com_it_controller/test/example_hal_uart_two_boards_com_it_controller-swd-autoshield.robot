*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_uart_two_boards_com_it_controller.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants
Resource            example.resource

Test Setup          Default Setup    ${TEST_CONFIG_FILE}    do_reset=False
Test Teardown       Default Cleanup

Test Tags           ip:hal_uart

*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to the scenario described
    ...    in the README.
    Init Uart    MX_EXAMPLE_UART_TX_PIN    MX_EXAMPLE_UART_RX_PIN

    Reset Target

    Example Step 1

    FOR    ${i}    IN RANGE    5
        Example Step 2    ${MESSAGE_PING}
        Example Step 3
        Example Step 4    ${MESSAGE_PING}
        Example Step 5
        Example Step 6

        Example Step 2    ${MESSAGE_PONG}
        Example Step 3
        Example Step 4    ${MESSAGE_PONG}
        Example Step 5
        Example Step 6
    END


*** Keywords ***
Example Step 1
    [Documentation]    configures and initializes the UART instance and the NVIC.
    ${inactive_level}=    Read Test Variable    MX_STATUS_LED_INACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${inactive_level}    timeout=1

Example Step 2
    [Documentation]    Checks the transmission of the message by the controller.
    [Arguments]    ${expected_message}
    Check Uart    ${expected_message}    timeout=${RX_TIMEOUT}

Example Step 3
    [Documentation]    waits for one of these UART interrupts: write transfer completed or transfer error.
    Comment    Nothing to check here

Example Step 4
    [Documentation]    Sends the expected message to the controller and checks its reception.
    [Arguments]    ${expected_message}
    Write Uart    ${expected_message}

Example Step 5
    [Documentation]    waits for one of these UART interrupts: read transfer completed or transfer error.
    Comment    Nothing to check here

Example Step 6
    [Documentation]    Checks the preparation of the controller to the next message.
    Comment    The preparation of the controller to the next message is checked in the next round of communication.
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    timeout=2s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=2s
