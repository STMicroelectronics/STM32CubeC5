*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_fdcan_two_boards_com_it_controller.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants
Resource            example.resource

Test Setup          Default Setup    ${TEST_CONFIG_FILE}    do_reset=False
Test Teardown       Default Cleanup

Test Tags           ip:hal_fdcan    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.

    Init Fdcan    MX_EXAMPLE_FDCAN_TX_PIN    MX_EXAMPLE_FDCAN_RX_PIN    ${NOMINAL_SPEED}    ${DATA_SPEED}
    Reset Target
    Example Step 1
    FOR    ${i}    IN RANGE    ${MAX_ATTEMPTS}
        Example Step 2    ${MESSAGE_PING}
        Example Step 3    ${MESSAGE_PING}
        Example Step 4

        Example Step 2    ${MESSAGE_PONG}
        Example Step 3    ${MESSAGE_PONG}
        Example Step 4
    END
    Example End

${TEST_ID} Frame filter
    [Documentation]    This test checks that the FDCAN filter set in application works as expected.
    ...    Autoshield will send messages that must not perturb the scenario described in the README.

    Init Fdcan    MX_EXAMPLE_FDCAN_TX_PIN    MX_EXAMPLE_FDCAN_RX_PIN    ${NOMINAL_SPEED}    ${DATA_SPEED}
    Reset Target

    FOR    ${i}    IN RANGE    ${MAX_ATTEMPTS}
        Send Filtered Messages    ${${FILTERED_ID} + ${i} * 10}
        Example Step 2    ${MESSAGE_PING}
        Example Step 3    ${MESSAGE_PING}
        Example Step 4

        Send Filtered Messages    ${${FILTERED_ID} + ${i} * 10}
        Example Step 2    ${MESSAGE_PONG}
        Example Step 3    ${MESSAGE_PONG}
        Example Step 4
    END
    Example End

${TEST_ID} Retry mechanisms
    [Documentation]    This test checks that the retry mechanism set in application works as expected.
    ...    Autoshield will not automatically reply messages to check the scenario described in the README.

    Init Fdcan    MX_EXAMPLE_FDCAN_TX_PIN    MX_EXAMPLE_FDCAN_RX_PIN    ${NOMINAL_SPEED}    ${DATA_SPEED}
    Reset Target

    Comment    Send wrong message without reaching MAX_ATTEMPS and get a PASS status
    FOR    ${i}    IN RANGE    ${RETRY_ATTEMPTS}
        Example Step 2    ${MESSAGE_PING}
        # Generate a wrong message reception and use retry mechanisms from target as controller
        Write Fdcan
        ...    ${RESPONDER_ID}
        ...    ${FILTERED_MESSAGE}
    END
    Example Step 2    ${MESSAGE_PING}
    Example Step 3    ${MESSAGE_PING}
    Example Step 4

    Comment    Send wrong message while reaching MAX_ATTEMPS and get a FAIL status
    FOR    ${i}    IN RANGE    ${MAX_ATTEMPTS}
        Example Step 2    ${MESSAGE_PONG}
        # Generate a wrong message reception and use retry mechanisms from target as controller
        Write Fdcan
        ...    ${RESPONDER_ID}
        ...    ${FILTERED_MESSAGE}
        Read Console Until    [ERROR] Controller - Tx/Rx Buffers DIFFERENT
    END
    Check Failed Status


*** Keywords ***
Example Step 1
    [Documentation]    Configures and initializes the FDCAN instance.
    ...    Checks that initialization (app_init) runs without error
    Read Console Until    [INFO] Step 1: Device initialization COMPLETED.
    Comment    Checking ExecStatus and the LED status is not currently possible,
    ...    as the example immediately starts FDCAN communications

Example Step 2
    [Documentation]    The controller transmits in interrupt mode a FDCAN message with the correct ID.
    [Arguments]    ${expected_msg}
    Comment    The Autoshield reads the message sent by the controller board
    Check Fdcan    ${CONTROLLER_ID}    ${expected_msg}    timeout=${EXAMPLE_TIMEOUT}
    Check Fdcan Rxfifo Empty

Example Step 3
    [Documentation]    The controller expects to receive the FDCAN message back in interrupt mode with correct ID.
    [Arguments]    ${received_msg}
    Comment    The Autoshield sends back the message to the controller board.
    Write Fdcan    ${RESPONDER_ID}    ${received_msg}

Example Step 4
    [Documentation]    The controller checks that the sent and received buffers match.
    Read Console Until    [INFO] Controller - Tx/Rx Buffers IDENTICAL. Transfer COMPLETED of ${MESSAGE_LENGTH} bytes.

Example End
    [Documentation]    The final flags must be correctly set.
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    timeout=2s

Send Filtered Messages
    [Documentation]    The controller filter configuration expects to flush these FDCAN message.
    [Arguments]    ${frame_id}
    Comment    The Autoshield sends a burst of FDCAN messages over the FDCAN bus.
    FOR    ${i}    IN RANGE    ${5}
        Write Fdcan    ${${frame_id} + ${i}}    ${FILTERED_MESSAGE}
    END

Check Failed Status
    [Documentation]    The example fails with associated flags.
    ${inactive_level}=    Read Test Variable    MX_STATUS_LED_INACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${inactive_level}    timeout=200ms
