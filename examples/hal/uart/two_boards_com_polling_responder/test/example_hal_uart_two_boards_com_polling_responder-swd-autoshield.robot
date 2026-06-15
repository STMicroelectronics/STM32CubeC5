*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_uart_two_boards_com_polling_responder.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants
Resource            example.resource

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
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
        Example Step 3    ${MESSAGE_PING}

        Example Step 2    ${MESSAGE_PONG}
        Example Step 3    ${MESSAGE_PONG}
    END

${TEST_ID} Message transfer abort
    [Documentation]    This test checks that the message transfer between the two boards is stopped,
    ...    when the maximum number of attempts is reached.

    Reset Target

    Example Step 1

    Comment    No action whatsoever on the UART bus. After timing out MAX_ATTEMPTS times,
    ...    the example will run its error handler.

    ${total_duration}=    Evaluate    ${MAX_ATTEMPTS} * ${EXAMPLE_TIMEOUT}

    Check Integer Variable    ExecStatus    ${EXEC_STATUS_ERROR}    timeout=${total_duration}

    # Checks that the LED runs the error pattern:
    # Signal_freq = 1/2050ms ~0.49Hz; Signal_duty_cycle = 50/2050 ~0.02;
    Check Frequency    MX_STATUS_LED    freq=0.49Hz    duty_cycle=0.02


*** Keywords ***
Example Step 1
    [Documentation]    The application configures and initializes the UART instance.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_INIT_OK}    timeout=2s
    ${inactive_level}=    Read Test Variable    MX_STATUS_LED_INACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${LOW}

Example Step 2
    [Documentation]    The responder expects to receive a message from the controller board, in blocking mode, within a specific timeout period.
    [Arguments]    ${expected_message}
    Write Uart    ${expected_message}

Example Step 3
    [Documentation]    The responder sends back the received message within the specified timeout period, in blocking mode.
    [Arguments]    ${expected_message}
    Check Uart    ${expected_message}    timeout=1s
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    timeout=2s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
