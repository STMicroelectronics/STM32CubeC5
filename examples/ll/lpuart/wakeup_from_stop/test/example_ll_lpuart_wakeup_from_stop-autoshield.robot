*** Comments ***
Copyright (c) 2025 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.


*** Settings ***
Documentation       Automatic test for example_ll_uart_wakeup_from_stop.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.Autoshield
Library             robot_for_stm32.STM32CubeProgrammer
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

        Example Step 2    ${MESSAGE_PONG}
        Example Step 3
        Example Step 4    ${MESSAGE_PONG}
        Example Step 5
    END


*** Keywords ***
Example Step 1
    [Documentation]    configures and initializes the UART instance and the NVIC.
    Comment    Nothing to check here

Example Step 2
    [Documentation]    The responder expects to receive a message from the controller board, in interrupt mode, within a specific timeout period.
    [Arguments]    ${expected_message}
    Write Uart    ${expected_message}

Example Step 3
    [Documentation]    waits for one of these UART interrupts: read transfer completed or transfer error.
    Comment    Nothing to check here

Example Step 4
    [Documentation]    The responder sends back the received message in interrupt mode.
    [Arguments]    ${expected_message}
    Check Uart    ${expected_message}
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}


Example Step 5
    [Documentation]    waits for one of these UART interrupts: write transfer completed or transfer error.
    Comment    Nothing to check here
