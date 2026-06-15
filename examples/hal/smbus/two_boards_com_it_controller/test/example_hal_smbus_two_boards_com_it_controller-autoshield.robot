*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_smbus_two_boards_com_it_controller.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants
Resource            example.resource

Test Setup          Default Setup    ${TEST_CONFIG_FILE}    do_reset=False
Test Teardown       Default Cleanup

Test Tags           ip:hal_smbus


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    ...    Due to the speed of the communication, the autoshield cannot
    ...    assume several check in parallel. To solve this problem, the test
    ...    doesn't stick to the scenario and some check is done post processing.

    Init Smbus    SMBUS_SCL    SMBUS_SDA    ${TARGET_ADDRESS}    baudrate=${BAUDRATE}    slave_txloop_enable=${true}

    Comment    Initialiazes messages to be send by the autoshield
    Write Smbus As Target    ${TARGET_ADDRESS}    ${MESSAGE_PING}
    Write Smbus As Target    ${TARGET_ADDRESS}    ${MESSAGE_PONG}

    Comment    Launch the target to start the SMBUS communication
    Reset Target

    Comment    Let the communication run for a while.
    Sleep    1s

    Comment    Deinitialize the SMBUS instance to free some resources on the autoshield side.
    Deinit Smbus

    Comment    Before the retry mechanism leads to an error state, check the example ends correctly.
    Check End Of Example

    Comment    Check the consistency of the exchanged messages. As this check may take more time than the retry
    ...    mechanism, this check is done after the 'Check End Of Example' check to avoid any error.
    FOR    ${i}    IN RANGE    5
        Example Step 2-6    ${MESSAGE_PING}
        Example Step 2-6    ${MESSAGE_PONG}
    END


*** Keywords ***
Example Step 2-6
    [Documentation]    Check that the exchanged messages was the expected one.
    [Arguments]    ${expected_msg}
    Comment    The Autoshield reads the message sent by the controller board
    Check SMBUS As Target    ${TARGET_ADDRESS}    ${expected_msg}    timeout=2s

Check End Of Example
    [Documentation]    Check that the transmission ran without error.
    Check Pin    MX_STATUS_LED    ${HIGH}    timeout=2s
