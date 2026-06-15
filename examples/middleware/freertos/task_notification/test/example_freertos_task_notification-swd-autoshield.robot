*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_freertos_task_notification.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:mw_freertos


*** Variables ***
${SEQUENCE_MAX_VALUE}       3
${TIMEOUT_S}=               1


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


*** Keywords ***
Example Step 1
    [Documentation]    Create a software timer and 3 tasks.
    Comment    ExecStatus value is then EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after.
    ${inactive_level}=    Read Test Variable    MX_STATUS_LED_INACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${inactive_level}
    Comment    Check button pull resistor configuration
    ${active_button_level}=    Read Test Variable    MX_STATUS_BUTTON_ACTIVE_STATE
    Check Button Configuration    MX_EXAMPLE_EXTI_TRIGGER_PIN    ${active_button_level}

Example Step 2
    [Documentation]    Starts the FreeRTOS scheduler.
    Comment    No time to do a check before the next step is executed.

Example Step 3
    ${max_value_plus_one}=    Evaluate    ${SEQUENCE_MAX_VALUE} + 1
    FOR    ${i}    IN RANGE    1    ${max_value_plus_one + 1}
        Comment
        ...    'Main Task' waits until being notified by 'EXTI Task' or/and 'Timer Task', (sequence is repeated 3 times). In the end of the sequence, Green led is toggled.
        Comment    Active EXTI interruption on the board pin
        Write Pulse    MX_EXAMPLE_EXTI_TRIGGER_PIN
    END

Example Step 4
    [Documentation]    Once the user button is pressed, 'EXTI Task' sends notification to the 'Main task', starts the software timer and prints message to the serial port.
    ${max_value_plus_one}=    Evaluate    ${SEQUENCE_MAX_VALUE} + 1
    Check Integer Variable    SequenceValue    ${max_value_plus_one}

Example Step 5
    [Documentation]    'Timer Task' waits for the timer callback and then sends notification to the Main Task and prints message to the serial port.
    Comment    No time to do a check before the next step is executed.

Example Step 6
    [Documentation]    Sequence value reaches the SEQUENCE_MAX_VALUE, Stop EXTI event, print message and Toggle MX_STATUS_LED then delete Main task.
    Sleep    ${{2 * ${TIMEOUT_S}}}s
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=10s
