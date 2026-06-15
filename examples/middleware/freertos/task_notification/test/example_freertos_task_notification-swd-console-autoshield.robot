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
Library             robot_for_stm32.Console
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:mw_freertos    profile:printf


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
    Read Console Until    [INFO] Starting FreeRTOS Scheduler

Example Step 3
    Comment
    ...    'Main Task' waits until being notified by 'EXTI Task' or/and 'Timer Task', (sequence is repeated 3 times). In the end of the sequence, Green led is toggled.
    Read Console Until    [INFO] [Main Task]: Waiting notification from Event Tasks
    Read Console Until    [INFO] [EXTI Task]: Waiting notification from Push Button
    Read Console Until    [INFO] [Timer Task]: Waiting notification from Software timer
    Comment    Active EXTI interruption on the board pin
    ${max_value_plus_one}=    Evaluate    ${SEQUENCE_MAX_VALUE}
    FOR    ${i}    IN RANGE    1    ${max_value_plus_one}
        Write Pulse    MX_EXAMPLE_EXTI_TRIGGER_PIN
        Read Console Until    [INFO] [EXTI Task]: Push Button pressed -> Notify Main Task
        Read Console Until    [INFO] [Main Task]: EXTI Event received
        Read Console Until    [INFO] [Timer Task]: Getting notification from Software timer
        Read Console Until    [INFO] [Timer Task]: Timer period reached -> Notify Main Task
        Read Console Until    [INFO] [Main Task]: Software Timer Event received
        Sleep    ${{2 * ${TIMEOUT_S}}}s
    END

Example Step 4
    [Documentation]    Once the user button is pressed, 'EXTI Task' sends notification to the 'Main task', starts the software timer and prints message to the serial port.
    Write Pulse    MX_EXAMPLE_EXTI_TRIGGER_PIN
    Read Console Until    [INFO] [EXTI Task]: Push Button pressed -> Notify Main Task
    Read Console Until    [INFO] [Main Task]: EXTI Event received
    Read Console Until    [INFO] [Timer Task]: Getting notification from Software timer
    Read Console Until    [INFO] [Timer Task]: Timer period reached -> Notify Main Task

Example Step 5
    [Documentation]    'Timer Task' waits for the timer callback and then sends notification to the Main Task and prints message to the serial port.
    Comment    No time to do a check before the next step is executed.

Example Step 6
    [Documentation]    Sequence value reaches the SEQUENCE_MAX_VALUE, Stop EXTI event, print message and Toggle MX_STATUS_LED then delete Main task.
    Read Console Until    [INFO] [Main Task]: Sequence finished -> Toggle MX_STATUS_LED
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=10s
