*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_fdcan_two_boards_com_it_responder.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants
Resource            example.resource

Test Setup          Default Setup    ${TEST_CONFIG_FILE}    do_reset=False
Test Teardown       Default Cleanup

Test Tags           ip:hal_fdcan


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


*** Keywords ***
Example Step 1
    [Documentation]    Configures and initializes the FDCAN instance.
    ...    Checks that initialization (app_init) runs without error

    Comment    Checking ExecStatus and the LED status is not currently possible,
    ...    as the example immediately starts FDCAN communications
    Comment    Verify status LED is inactivated.
    ${inactive_level}=    Read Test Variable    MX_STATUS_LED_INACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${inactive_level}    duration=200ms

Example Step 2
    [Documentation]    The responder expects to receive the FDCAN message in interrupt mode with correct ID.
    [Arguments]    ${transmitted_msg}
    Comment    The Autoshield sends the message to the responder board.
    Write Fdcan    ${CONTROLLER_ID}    ${transmitted_msg}

Example Step 3
    [Documentation]    The responder transmits in interrupt mode a FDCAN message with the correct ID.
    [Arguments]    ${expected_msg}
    Comment    The Autoshield reads the message sent by the responder board
    Check Fdcan    ${RESPONDER_ID}    ${expected_msg}    timeout=${EXAMPLE_TIMEOUT}
    Check Fdcan Rxfifo Empty

Example Step 4
    [Documentation]    The responder checks that the sent and received buffers match.
    Comment    Nothing to be tested here.

Example End
    [Documentation]    The final flags must be correctly set.
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    timeout=2s

Send Filtered Messages
    [Documentation]    The responder filter configuration expects to flush these FDCAN message.
    [Arguments]    ${frame_id}
    Comment    The Autoshield sends a burst of FDCAN messages over the FDCAN bus.
    FOR    ${i}    IN RANGE    ${5}
        Write Fdcan    ${${frame_id} + ${i}}    ${FILTERED_MESSAGE}
    END
