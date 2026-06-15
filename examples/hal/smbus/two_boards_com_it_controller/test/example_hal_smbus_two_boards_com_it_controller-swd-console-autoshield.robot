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
Library             robot_for_stm32.Console
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants
Resource            example.resource

Test Setup          Default Setup    ${TEST_CONFIG_FILE}    do_reset=False
Test Teardown       Default Cleanup

Test Tags           ip:hal_smbus    profile:printf    robot:skip
# SWD: reading ExecStatus while in sleep mode fails.


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    ...    Due to the speed of the communication, the autoshield cannot
    ...    assume several check in parallel. To solve this problem, the test
    ...    doesn't stick to the scenario and some check is done post processing.

    Init Smbus    SMBUS_SCL    SMBUS_SDA    ${TARGET_ADDRESS}    baudrate=${BAUDRATE}    slave_txloop_enable=${true}

    Comment    Initializes messages to be send by the autoshield
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

${TEST_ID} Message transmission failure
    [Documentation]    This test is designed to evaluate the retry mechanism implemented in the controller's code,
    ...    when it fails once to send a message to the responder board (the Autoshield).
    ...    Due to the speed of the communication, the autoshield cannot
    ...    assume several check in parallel. To solve this problem, the test
    ...    doesn't stick to the scenario and some check is done post processing.

    Init Smbus    SMBUS_SCL    SMBUS_SDA    ${TARGET_ADDRESS}    baudrate=${BAUDRATE}    slave_read_enable=${false}

    Comment    Launch the target to start the SMBUS communication
    Reset Target

    Check Initialization

    Read Console Until    [ERROR] Controller - Communication ERROR

    Init Smbus    SMBUS_SCL    SMBUS_SDA    ${TARGET_ADDRESS}    baudrate=${BAUDRATE}    slave_txloop_enable=${true}
    Write Smbus As Target    ${TARGET_ADDRESS}    ${MESSAGE_PING}

    Comment    Let the communication run for a while.
    Sleep    1s

    Comment    Deinitialize the SMBUS instance to free some resources on the autoshield side.
    Deinit Smbus

    Example Step 2-6    ${MESSAGE_PING}
    Check End Of Example

${TEST_ID} Message reception failure
    [Documentation]    This test is designed to evaluate the retry mechanism implemented in the controller's code,
    ...    when it fails once to receive back the message from the Autoshield.
    ...    Due to the speed of the communication, the autoshield cannot
    ...    assume several check in parallel. To solve this problem, the test
    ...    doesn't stick to the scenario and some check is done post processing.

    Init Smbus    SMBUS_SCL    SMBUS_SDA    ${TARGET_ADDRESS}    baudrate=${BAUDRATE}    slave_write_enable=${false}
    Write Smbus As Target    ${TARGET_ADDRESS}    ${MESSAGE_PING}

    Comment    Launch the target to start the SMBUS communication
    Reset Target

    Check Initialization
    Example Step 2-6    ${MESSAGE_PING}
    Read Console Until    [ERROR] Controller - Communication ERROR

    Init Smbus    SMBUS_SCL    SMBUS_SDA    ${TARGET_ADDRESS}    baudrate=${BAUDRATE}    slave_txloop_enable=${true}
    Write Smbus As Target    ${TARGET_ADDRESS}    ${MESSAGE_PING}

    Comment    Let the communication run for a while.
    Sleep    1s

    Comment    Deinitialize the SMBUS instance to free some resources on the autoshield side.
    Deinit Smbus

    Example Step 2-6    ${MESSAGE_PING}
    Check End Of Example

${TEST_ID} Exchanged messages inconsistency
    [Documentation]    This test is designed to evaluate the retry mechanism implemented in the controller's code,
    ...    when it receives back a wrong message from the Autoshield.
    ...    Due to the speed of the communication, the autoshield cannot
    ...    assume several check in parallel. To solve this problem, the test
    ...    doesn't stick to the scenario and some check is done post processing.

    Init Smbus    SMBUS_SCL    SMBUS_SDA    ${TARGET_ADDRESS}    baudrate=${BAUDRATE}
    # push a "wrong" message in the queue, so that the first exchange will be inconsistent.
    # no impact on later exchanges, they will work "as usual"
    Write Smbus As Target    ${TARGET_ADDRESS}    ${MESSAGE_PONG}
    Write Smbus As Target    ${TARGET_ADDRESS}    ${MESSAGE_PING}

    Comment    Launch the target to start the SMBUS communication
    Reset Target

    Comment    Let the communication run for a while.
    Sleep    1s

    Comment    Deinitialize the SMBUS instance to free some resources on the autoshield side.
    Deinit Smbus

    Check Initialization
    Example Step 2-6    ${MESSAGE_PING}

    Read Console Until    [ERROR] Controller - Tx/Rx Buffers DIFFERENT. TRYING AGAIN.
    Example Step 2-6    ${MESSAGE_PING}
    Check End Of Example

${TEST_ID} Message transfer abort
    [Documentation]    This test checks that the message transfer between the two boards is stopped,
    ...    when the maximum number of attempts is reached.
    ...    Due to the speed of the communication, the autoshield cannot
    ...    assume several check in parallel. To solve this problem, the test
    ...    doesn't stick to the scenario and some check is done post processing.

    # No I2C setup needed here
    Comment    Launch the target to start the SMBUS communication
    Reset Target

    Check Initialization
    Repeat Keyword    ${MAX_ATTEMPTS} times
    ...    Read Console Until    [ERROR] Controller - Communication ERROR

    # Checks that the LED runs the error pattern:
    # Signal_freq = 1/2050ms ~0.49Hz; Signal_duty_cycle = 50/2050 ~0.02;
    Check Frequency    MX_STATUS_LED    freq=0.49Hz    duty_cycle=0.02    timeout=5s


*** Keywords ***
Check Initialization
    [Documentation]    Check the initialization step is completed
    Read Console Until    [INFO] Step 1: Device initialization COMPLETED.

Example Step 2-6
    [Documentation]    The controller starts the communication, in interrupt mode, by sending a message to the responder.
    [Arguments]    ${expected_msg}
    Comment    The Autoshield reads the message sent by the controller board
    Check SMBUS As Target    ${TARGET_ADDRESS}    ${expected_msg}    timeout=2s

Check End Of Example
    [Documentation]    Check that the transmission ran without error.
    Read Console Until    [INFO] Controller - Tx/Rx Buffers IDENTICAL.
    Check Pin    MX_STATUS_LED    ${HIGH}    timeout=2s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=2s
